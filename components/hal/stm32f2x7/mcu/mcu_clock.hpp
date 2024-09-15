#include "hal_clock.hpp"
#include "meta_types.hpp"
#include "reg_flash.hpp"
#include "reg_rcc.hpp"

namespace stm32f217::drivers::clock {
enum class SourceClockType : uint32_t { HSI = 0b00, HSE = 0b01, PLL = 0b10, MASK = 0b11 };

template <const SourceClockType sourceClockType, const uint32_t frequency>
concept source_clock_frequency =
    ((SourceClockType::HSE == sourceClockType) ? ((4'000'000UL <= frequency) && (frequency <= 26'000'000UL)) : (16'000'000UL == frequency));

template <const uint32_t frequency>
concept system_frequency = (frequency <= 120'000'000UL);

template <const SourceClockType sourceClockType, const uint32_t frequency = 16'000'000UL>
requires source_clock_frequency<sourceClockType, frequency>
struct SourceClock {
  static constexpr auto sc_SourceClockType = sourceClockType;
  static constexpr auto sc_Frequency = frequency;
  struct SourceClockT;
};

using HSI = SourceClock<SourceClockType::HSI>;
template <const uint32_t frequency> using HSE = SourceClock<SourceClockType::HSE, frequency>;

template <typename T>
concept source_clock = requires(T) {
  T::sc_SourceClockType;
  T::sc_Frequency;
  typename T::SourceClockT;
};

// Order is important, see BusFrequency
enum class EBus : unsigned { AHB, APB1, APB2, APB1Tim, Num };

template <source_clock SourceClock, const uint32_t systemFreq = SourceClock::sc_Frequency>
requires system_frequency<systemFreq>
class SystemClock final {
  static constexpr auto scSystemFrequency = systemFreq;
  static constexpr auto scSourceFrequency = SourceClock::sc_Frequency;
  static constexpr auto scSourceType = SourceClock::sc_SourceClockType;

  // 120'000'000 is a max clock
  static constexpr auto scFreqAHB = scSystemFrequency;

  // 30'000'000 is a max clock, possible prescalers are 1, 2, 4
  static constexpr auto scPreAPB1 = (scSystemFrequency <= 30'000'000UL) ? 1UL : (scSystemFrequency <= 60'000'000UL) ? 2UL : 4UL;
  static constexpr auto scFreqAPB1 = scSystemFrequency / scPreAPB1;
  static constexpr auto scFreqAPBTimers = (1UL == scPreAPB1) ? scFreqAPB1 : 2 * scFreqAPB1;

  // 60'000'000 is a max clock, possible prescalers are 1, 2
  static constexpr auto scPreAPB2 = (scSystemFrequency <= 60'000'000UL) ? 1UL : 2UL;
  static constexpr auto scFreqAPB2 = scSystemFrequency / scPreAPB2;

public:
  inline consteval SystemClock() { static_assert(::hal::clock::system_clock<SystemClock>, "The class should implement a whole concept interface!"); }

  inline void Init(void) const {
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::flash;
    using namespace cpp_register;
    using enum SourceClockType;

    // If HSE is a source - enable it
    if constexpr (SourceClockType::HSE == scSourceType) {
      RCC->CR |= RCC_CR::HSEON;
      while (!(RCC->CR & RCC_CR::HSERDY)) {
      }
    }

    // Check if PLL is needed
    constexpr bool isPllNeeded = (scSourceFrequency != scSystemFrequency);
    if constexpr (isPllNeeded) {
      // Calculate the divider and the multiplier
      constexpr auto VCOInput = 1'000'000UL;
      constexpr auto M = scSourceFrequency / VCOInput;
      constexpr auto N = []() {
        for (uint32_t n = 192; n <= 432; n += 2) {
          if (0 == ((n * 1'000'000UL) % scSystemFrequency)) {
            return n;
          }
        }
        return 0UL;
      }();
      constexpr auto P = (N * 1'000'000UL) / scSystemFrequency;

      static_assert((0 != N), "Clock: PLL factor N was not found!");
      static_assert((0 == P % 2) && (P <= 8) && (0 < P), "Clock: PLL factor P was not found!");

      // 48 MHz
      constexpr auto Q = (N * 1'000'000UL) / 48'000'000UL;
      static_assert((0 == N % Q), "Clock: PLL factor Q was not found!");

      // Set the latency if needed
      FLASH->ACR |= FLASH_ACR::LATENCY(reg_v<(scSystemFrequency - 1) / 30'000'000UL>) | FLASH_ACR::PRFTEN;

      // Set clock configuration according to the source, multiplier and divider
      RCC->PLLCFGR &= RCC_PLLCFGR::PLLM(reg_v<0b111111UL>) | RCC_PLLCFGR::PLLN(reg_v<0b111111111UL>) | RCC_PLLCFGR::PLLQ(reg_v<0b1111UL>);
      RCC->PLLCFGR |= RCC_PLLCFGR::PLLSRC(reg_v<scSourceType>) | RCC_PLLCFGR::PLLN(reg_v<N>) | RCC_PLLCFGR::PLLM(reg_v<M>) |
                      RCC_PLLCFGR::PLLP(reg_v<(P / 2) - 1>) | RCC_PLLCFGR::PLLQ(reg_v<Q>);

      // Buses prescalers
      constexpr auto prescalerTable = [](const unsigned long prescaler) -> unsigned long {
        unsigned long result;
        if (2UL == prescaler) {
          result = 0b100UL;
        } else if (4UL == prescaler) {
          result = 0b101UL;
        } else {
          result = 0UL;
        }
        return result;
      };

      RCC->CFGR |= RCC_CFGR::PPRE[reg_v<1UL, 0>](reg_v<prescalerTable(scPreAPB1)>) | RCC_CFGR::PPRE[reg_v<1UL, 1>](reg_v<prescalerTable(scPreAPB2)>);

      // Enable PLL
      RCC->CR |= RCC_CR::PLLON;
      while (!(RCC->CR & RCC_CR::PLLRDY)) {
      }

      // Set the PLL source
      RCC->CFGR |= RCC_CFGR::SW(reg_v<SourceClockType::PLL>);
      while (RCC_CFGR::SWS(reg_v<SourceClockType::PLL>) != (RCC->CFGR & RCC_CFGR::SWS(reg_v<MASK>))) {
      }
    }
  }

  [[nodiscard]] inline consteval unsigned long Frequency(void) const { return scSystemFrequency; }

  [[nodiscard]] inline consteval unsigned long BusFrequency(const EBus bus) const {
    constexpr unsigned long busTable[] = {scFreqAHB, scFreqAPB1, scFreqAPB2, scFreqAPBTimers};
    static_assert((sizeof(busTable) / sizeof(busTable[0])) == static_cast<std::underlying_type_t<EBus>>(EBus::Num),
                  "Array should represent all busses");
    return busTable[static_cast<std::underlying_type_t<decltype(bus)>>(bus)];
  }
};

} // namespace stm32f217::drivers::clock