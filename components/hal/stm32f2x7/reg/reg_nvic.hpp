#pragma once

#include "register.hpp"

namespace stm32f217::registers::nvic {

template <const cpp_register::RegisterAddress address> struct NVIC_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RS, uint32_t, struct ISER, 8> ISER{};
  static constexpr cpp_register::Register<address + 0x80, cpp_register::AccessMode::RS, uint32_t, struct ICER, 8> ICER{};
  static constexpr cpp_register::Register<address + 0x100, cpp_register::AccessMode::RS, uint32_t, struct ISPR, 8> ISPR{};
  static constexpr cpp_register::Register<address + 0x180, cpp_register::AccessMode::RS, uint32_t, struct ICPR, 8> ICPR{};
  static constexpr cpp_register::Register<address + 0x200, cpp_register::AccessMode::R, uint32_t, struct IABR, 8> IABR{};
  static constexpr cpp_register::Register<address + 0x300, cpp_register::AccessMode::RW, uint32_t, struct IPR, 8U> IPR{};
};

inline constexpr NVIC_T<0xE000E100> const *NVIC{};

struct NVIC_ISER {
  static constexpr cpp_register::Field<decltype(NVIC->ISER), (1UL << 0), cpp_register::AccessMode::RS, 32> SETENA{};
};

struct NVIC_ICER {
  static constexpr cpp_register::Field<decltype(NVIC->ICER), (1UL << 0), cpp_register::AccessMode::RS, 32> CLRENA{};
};

struct NVIC_ISPR {
  static constexpr cpp_register::Field<decltype(NVIC->ISPR), (1UL << 0), cpp_register::AccessMode::RS, 32> SETPEND{};
};

struct NVIC_ICPR {
  static constexpr cpp_register::Field<decltype(NVIC->ICPR), (1UL << 0), cpp_register::AccessMode::RS, 32> CLRPEND{};
};

struct NVIC_IABR {
  static constexpr cpp_register::Field<decltype(NVIC->IABR), (1UL << 0), cpp_register::AccessMode::R, 32> ACTIVE{};
};

struct NVIC_IPR {
  static constexpr cpp_register::Field<decltype(NVIC->IPR), (1UL << 0), cpp_register::AccessMode::RW, 8, 4> PRI_N{};
};

} // namespace stm32f217::registers::nvic
