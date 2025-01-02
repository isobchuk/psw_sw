#pragma once

#include "register.hpp"

namespace stm32f217::registers::syscfg {

template <const cpp_register::RegisterAddress address> struct SYSCFG_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint32_t, struct MEMRM> MEMRM{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::RW, uint32_t, struct PMC> PMC{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::RW, uint32_t, struct EXTICR, 4> EXTICR{};
  static constexpr cpp_register::Register<address + 0x20, cpp_register::AccessMode::RW, uint32_t, struct CMPCR> CMPCR{};
};

inline constexpr SYSCFG_T<0x40013800> const *SYSCFG{};

struct SYSCFG_MEMRM {
  static constexpr cpp_register::Field<decltype(SYSCFG->MEMRM), (1UL << 0), cpp_register::AccessMode::RW, 2> MEM_MODE{};
};

struct SYSCFG_PMC {
  static constexpr cpp_register::Field<decltype(SYSCFG->PMC), (1UL << 23), cpp_register::AccessMode::RW, 1> MII_RMII_SEL{};
};

struct SYSCFG_EXTICR {
  static constexpr cpp_register::Field<decltype(SYSCFG->EXTICR), (1UL << 0), cpp_register::AccessMode::RW, 4, 4> EXTI{};
};

struct SYSCFG_CMPCR {
  static constexpr cpp_register::Field<decltype(SYSCFG->CMPCR), (1UL << 7), cpp_register::AccessMode::R, 1> READY{};
  static constexpr cpp_register::Field<decltype(SYSCFG->CMPCR), (1UL << 0), cpp_register::AccessMode::RW, 1> CMP_PD{};
};

} // namespace stm32f217::registers::syscfg
