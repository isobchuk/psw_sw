#pragma once

#include "hal_gpio.hpp"
#include "mcu_gpio.hpp"
#include "meta_types.hpp"
#include "reg_exti.hpp"
#include "reg_rcc.hpp"
#include "reg_syscfg.hpp"
#include <type_traits>

namespace stm32f217::drivers::exti {

enum class EPort : uint32_t { PA, PB, PC, PD, PE, PF, PG, PH };
enum class ETrigger : uint32_t { Rising, Falling };

/**
 * @brief Just a draft class for external interrupt handler
 *
 * @tparam pin
 */
template <const ::hal::gpio::is_gpio auto pin, const ETrigger trigger> class CExtInterrupt {
  static constexpr auto _Pin = pin;
  static constexpr auto _Trigger = trigger;

public:
  consteval CExtInterrupt() = default;

  void Init(void) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::syscfg;
    using namespace stm32f217::registers::exti;

    // Enable clocking to choose port
    RCC->APB2ENR |= RCC_APB2ENR::SYSCFGEN;

    // Choose port in syscfg
    constexpr auto REGISTER = reg_v<static_cast<std::underlying_type_t<stm32f217::drivers::gpio::Pin>>(_Pin.Pin) / 4>;
    constexpr auto FIELD = reg_v<1UL, (static_cast<std::underlying_type_t<stm32f217::drivers::gpio::Pin>>(_Pin.Pin)) % 4>;

    constexpr auto PORT = []() -> EPort {
      using namespace stm32f217::drivers::gpio;

      EPort port{};
      switch (_Pin.Port) {
      case Port::PA:
        port = EPort::PA;
        break;
      case Port::PB:
        port = EPort::PB;
        break;
      case Port::PC:
        port = EPort::PC;
        break;
      case Port::PD:
        port = EPort::PD;
        break;
      case Port::PE:
        port = EPort::PE;
        break;
      case Port::PF:
        port = EPort::PF;
        break;
      case Port::PG:
        port = EPort::PG;
        break;
      case Port::PH:
        port = EPort::PH;
        break;
      default:
        break;
      }
      return port;
    }();
    SYSCFG->EXTICR[REGISTER] |= SYSCFG_EXTICR::EXTI[FIELD](reg_v<PORT>);
    // Enable interrupt
    constexpr auto PIN = reg_v<1UL, static_cast<std::underlying_type_t<stm32f217::drivers::gpio::Pin>>(_Pin.Pin)>;
    EXTI->IMR |= EXTI_IMR::MR[PIN];

    // Set trigger
    if constexpr (ETrigger::Rising == _Trigger) {
      EXTI->RTSR |= EXTI_RTSR::TR[PIN];
    } else {
      EXTI->FTSR |= EXTI_FTSR::TR[PIN];
    }
  }

  bool InterruptHandler() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::exti;
    constexpr auto PIN = reg_v<(1UL << static_cast<std::underlying_type_t<stm32f217::drivers::gpio::Pin>>(_Pin.Pin))>;
    EXTI->PR |= EXTI_PR::PR[PIN];

    return false;
  }
};

} // namespace stm32f217::drivers::exti