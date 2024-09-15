#pragma once

#include "register.hpp"

namespace stm32f217::registers::flash {

template <const cpp_register::RegisterAddress address> struct FLASH_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint32_t, struct ACR> ACR{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::W, uint32_t, struct KEYR> KEYR{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::W, uint32_t, struct OPTKEYR> OPTKEYR{};
  static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::RW, uint32_t, struct SR> SR{};
  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint32_t, struct CR> CR{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::RW, uint32_t, struct OPTCR> OPTCR{};
};

inline constexpr FLASH_T<0x40023C00> const *FLASH{};

struct FLASH_ACR {
  static constexpr cpp_register::Field<decltype(FLASH->ACR), (1UL << 0), cpp_register::AccessMode::RW, 3> LATENCY{};
  static constexpr cpp_register::Field<decltype(FLASH->ACR), (1UL << 8), cpp_register::AccessMode::RW, 1> PRFTEN{};
  static constexpr cpp_register::Field<decltype(FLASH->ACR), (1UL << 9), cpp_register::AccessMode::RW, 1> ICEN{};
  static constexpr cpp_register::Field<decltype(FLASH->ACR), (1UL << 10), cpp_register::AccessMode::RW, 1> DCEN{};
  static constexpr cpp_register::Field<decltype(FLASH->ACR), (1UL << 11), cpp_register::AccessMode::W, 1> ICRST{};
  static constexpr cpp_register::Field<decltype(FLASH->ACR), (1UL << 12), cpp_register::AccessMode::RW, 1> DCRST{};
};

struct FLASH_KEYR {
  static constexpr cpp_register::Field<decltype(FLASH->KEYR), (1UL << 0), cpp_register::AccessMode::W, 32> KEY{};
};

struct FLASH_OPTKEYR {
  static constexpr cpp_register::Field<decltype(FLASH->OPTKEYR), (1UL << 0), cpp_register::AccessMode::W, 32> OPTKEY{};
};

struct FLASH_SR {
  static constexpr cpp_register::Field<decltype(FLASH->SR), (1UL << 0), cpp_register::AccessMode::RW, 1> EOP{};
  static constexpr cpp_register::Field<decltype(FLASH->SR), (1UL << 1), cpp_register::AccessMode::RW, 1> OPERR{};
  static constexpr cpp_register::Field<decltype(FLASH->SR), (1UL << 4), cpp_register::AccessMode::RW, 1> WRPERR{};
  static constexpr cpp_register::Field<decltype(FLASH->SR), (1UL << 5), cpp_register::AccessMode::RW, 1> PGAERR{};
  static constexpr cpp_register::Field<decltype(FLASH->SR), (1UL << 6), cpp_register::AccessMode::RW, 1> PGPERR{};
  static constexpr cpp_register::Field<decltype(FLASH->SR), (1UL << 7), cpp_register::AccessMode::RW, 1> PGSERR{};
  static constexpr cpp_register::Field<decltype(FLASH->SR), (1UL << 16), cpp_register::AccessMode::RW, 1> BSY{};
};

struct FLASH_CR {
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 0), cpp_register::AccessMode::RW, 1> PG{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 1), cpp_register::AccessMode::RW, 1> SER{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 2), cpp_register::AccessMode::RW, 1> MER{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 3), cpp_register::AccessMode::RW, 4> SNB{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 8), cpp_register::AccessMode::RW, 2> PSIZE{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 16), cpp_register::AccessMode::RW, 1> STRT{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 24), cpp_register::AccessMode::RW, 1> EOPIE{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 25), cpp_register::AccessMode::RW, 1> ERRIE{};
  static constexpr cpp_register::Field<decltype(FLASH->CR), (1UL << 31), cpp_register::AccessMode::RW, 1> LOCK{};
};

struct FLASH_OPTCR {
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 0), cpp_register::AccessMode::RW, 1> OPTLOCK{};
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 1), cpp_register::AccessMode::RW, 1> OPTSTRT{};
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 2), cpp_register::AccessMode::RW, 2> BOR_LEV{};
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 5), cpp_register::AccessMode::RW, 1> WDG_SW{};
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 6), cpp_register::AccessMode::RW, 1> nRST_STOP{};
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 7), cpp_register::AccessMode::RW, 1> nRST_STDBY{};
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 8), cpp_register::AccessMode::RW, 8> RDP{};
  static constexpr cpp_register::Field<decltype(FLASH->OPTCR), (1UL << 16), cpp_register::AccessMode::RW, 12> nWRP{};
};

} // namespace stm32f217::registers::flash
