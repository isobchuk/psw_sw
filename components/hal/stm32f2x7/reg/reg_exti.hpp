#pragma once

#include "register.hpp"

namespace stm32f217::registers::exti {

template <const cpp_register::RegisterAddress address> struct EXTI_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint32_t, struct IMR> IMR{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::RW, uint32_t, struct EMR> EMR{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::RW, uint32_t, struct RTSR> RTSR{};
  static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::RW, uint32_t, struct FTSR> FTSR{};
  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint32_t, struct SWIER> SWIER{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::RC_W1, uint32_t, struct PR> PR{};
};

inline constexpr EXTI_T<0x40013C00> const *EXTI{};

struct EXTI_IMR {
  static constexpr cpp_register::Field<decltype(EXTI->IMR), (1UL << 0), cpp_register::AccessMode::RW, 1, 22> MR{};
};

struct EXTI_EMR {
  static constexpr cpp_register::Field<decltype(EXTI->EMR), (1UL << 0), cpp_register::AccessMode::RW, 1, 22> MR{};
};

struct EXTI_RTSR {
  static constexpr cpp_register::Field<decltype(EXTI->RTSR), (1UL << 0), cpp_register::AccessMode::RW, 1, 22> TR{};
};

struct EXTI_FTSR {
  static constexpr cpp_register::Field<decltype(EXTI->FTSR), (1UL << 0), cpp_register::AccessMode::RW, 1, 22> TR{};
};

struct EXTI_SWIER {
  static constexpr cpp_register::Field<decltype(EXTI->SWIER), (1UL << 0), cpp_register::AccessMode::RW, 1, 22> SWIER{};
};

struct EXTI_PR {
  static constexpr cpp_register::Field<decltype(EXTI->PR), (1UL << 0), cpp_register::AccessMode::RC_W1, 1, 22> PR{};
};

} // namespace stm32f217::registers::exti
