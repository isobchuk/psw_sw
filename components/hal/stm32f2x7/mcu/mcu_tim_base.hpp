#pragma once

#include "hal_tim_base.hpp"
#include "meta_types.hpp"
#include "reg_rcc.hpp"
#include "reg_tim_base.hpp"

namespace stm32f217::drivers::tim::basic {

enum class ETimer : cpp_register::RegisterAddress { Tim6 = 0x40001000UL, Tim7 = 0x40001400UL };

template <iso::meta_type::const_value_of_type<ETimer> Timer, iso::meta_type::const_value_of_type<uint32_t> BusFrequency,
          iso::meta_type::const_value_of_type<bool> Interrupt = iso::meta_type::const_t<false>>
class CTimer final {
  static constexpr registers::tim::basic::TIM_T<static_cast<cpp_register::RegisterAddress>(Timer::value)> *TIM{};
  static constexpr auto _BusFrequency = BusFrequency::value;
  static constexpr auto _InterruptEnabled = Interrupt::value;

  static volatile bool _UpdateEvent;

public:
  consteval CTimer(const Timer, const BusFrequency, const Interrupt = iso::meta_type::const_v<false>)
  requires ::hal::tim::basic::timer_basic<CTimer>
  {}

  void Init() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::tim::basic;

    if constexpr (Timer::value == ETimer::Tim6) {
      RCC->APB1ENR |= RCC_APB1ENR::TIM6EN;
    } else if (Timer::value == ETimer::Tim7) {
      RCC->APB1ENR |= RCC_APB1ENR::TIM7EN;
    } else {
      static_assert(((Timer::value != ETimer::Tim6) || (Timer::value != ETimer::Tim7)), "Driver timer basic, timer should be one of allowable!");
    }
  }

  template <iso::meta_type::const_value_of_type<uint32_t> Value,
            iso::meta_type::const_value_of_type<bool> OnePulseMode = iso::meta_type::const_t<false>>
  void Launch(const Value, const OnePulseMode = iso::meta_type::const_v<false>) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::tim::basic;

    constexpr auto prescaler = reg_v<(((Value::value < 1000) ? _BusFrequency / 1'000'000UL : _BusFrequency / 1'000UL) - 1)>;
    constexpr auto reload = reg_v<(((Value::value < 1000) ? Value::value : Value::value / 1'000UL) - 1)>;

    _UpdateEvent = false;

    TIM->PSC = TIM_PSC::PSC(prescaler);
    TIM->ARR = TIM_ARR::ARR(reload);
    TIM->CNT = TIM_CNT::CNT(constants::ZERO);
    if constexpr (_InterruptEnabled) {
      TIM->DIER |= TIM_DIER::UIE;
    }
    TIM->EGR = TIM_EGR::UG;
    void(TIM->SR & TIM_SR::UIF);
    TIM->SR &= TIM_SR::UIF;
    TIM->CR1 |= TIM_CR1::OPM(reg_v<((OnePulseMode::value) ? 1U : 0U)>) | TIM_CR1::CEN;
  }

  bool UpdateEvent() const {
    if constexpr (_InterruptEnabled) {
      return _UpdateEvent;
    } else {
      using namespace stm32f217::registers::tim::basic;
      return (TIM->SR & TIM_SR::UIF);
    }
  }

  inline static void InterruptHandler() {
    using namespace stm32f217::registers::tim::basic;
    TIM->SR &= TIM_SR::UIF;
    _UpdateEvent = true;
  }
};

template <iso::meta_type::const_value_of_type<ETimer> Timer, iso::meta_type::const_value_of_type<uint32_t> BusFrequency,
          iso::meta_type::const_value_of_type<bool> Interrupt>
volatile bool CTimer<Timer, BusFrequency, Interrupt>::_UpdateEvent;
} // namespace stm32f217::drivers::tim::basic