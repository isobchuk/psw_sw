#pragma once

#include "register.hpp"

namespace stm32f217::registers::tim::basic {

template <const cpp_register::RegisterAddress address> struct TIM_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint16_t, struct CR1> CR1{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::RW, uint16_t, struct CR2> CR2{};
  static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::RW, uint16_t, struct DIER> DIER{};
  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RC_W0, uint16_t, struct SR> SR{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::W, uint16_t, struct EGR> EGR{};
  static constexpr cpp_register::Register<address + 0x24, cpp_register::AccessMode::RW, uint16_t, struct CNT> CNT{};
  static constexpr cpp_register::Register<address + 0x28, cpp_register::AccessMode::RW, uint16_t, struct PSC> PSC{};
  static constexpr cpp_register::Register<address + 0x2C, cpp_register::AccessMode::RW, uint16_t, struct ARR> ARR{};
};

inline constexpr TIM_T<0x40001000> const *TIM6{};
inline constexpr TIM_T<0x40001400> const *TIM7{};

struct TIM_CR1 {
  static constexpr cpp_register::Field<decltype(TIM6->CR1), (1UL << 7), cpp_register::AccessMode::RW, 1> ARPE{};
  static constexpr cpp_register::Field<decltype(TIM6->CR1), (1UL << 3), cpp_register::AccessMode::RW, 1> OPM{};
  static constexpr cpp_register::Field<decltype(TIM6->CR1), (1UL << 2), cpp_register::AccessMode::RW, 1> URS{};
  static constexpr cpp_register::Field<decltype(TIM6->CR1), (1UL << 1), cpp_register::AccessMode::RW, 1> UDIS{};
  static constexpr cpp_register::Field<decltype(TIM6->CR1), (1UL << 0), cpp_register::AccessMode::RW, 1> CEN{};
};

struct TIM_CR2 {
  static constexpr cpp_register::Field<decltype(TIM6->CR2), (1UL << 4), cpp_register::AccessMode::RW, 3> MMS{};
};

struct TIM_DIER {
  static constexpr cpp_register::Field<decltype(TIM6->DIER), (1UL << 8), cpp_register::AccessMode::RW, 1> UDE{};
  static constexpr cpp_register::Field<decltype(TIM6->DIER), (1UL << 0), cpp_register::AccessMode::RW, 1> UIE{};
};

struct TIM_SR {
  static constexpr cpp_register::Field<decltype(TIM6->SR), (1UL << 0), cpp_register::AccessMode::RC_W0, 1> UIF{};
};

struct TIM_EGR {
  static constexpr cpp_register::Field<decltype(TIM6->EGR), (1UL << 0), cpp_register::AccessMode::W, 1> UG{};
};

struct TIM_CNT {
  static constexpr cpp_register::Field<decltype(TIM6->CNT), (1UL << 0), cpp_register::AccessMode::RW, 16> CNT{};
};

struct TIM_PSC {
  static constexpr cpp_register::Field<decltype(TIM6->PSC), (1UL << 0), cpp_register::AccessMode::RW, 16> PSC{};
};

struct TIM_ARR {
  static constexpr cpp_register::Field<decltype(TIM6->ARR), (1UL << 0), cpp_register::AccessMode::RW, 16> ARR{};
};

} // namespace stm32f217::registers::tim::basic
