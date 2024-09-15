#pragma once

#include "register.hpp"

namespace stm32f217::registers::spi {

template <const cpp_register::RegisterAddress address> struct SPI_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint16_t, struct CR1> CR1{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::RW, uint16_t, struct CR2> CR2{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::RC_W0, uint16_t, struct SR> SR{};
  static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::RW, uint16_t, struct DR> DR{};
  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint16_t, struct CRCPR> CRCPR{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::R, uint16_t, struct RXCRCR> RXCRCR{};
  static constexpr cpp_register::Register<address + 0x18, cpp_register::AccessMode::R, uint16_t, struct TXCRCR> TXCRCR{};
  static constexpr cpp_register::Register<address + 0x1C, cpp_register::AccessMode::RW, uint16_t, struct I2SCFGR> I2SCFGR{};
  static constexpr cpp_register::Register<address + 0x20, cpp_register::AccessMode::RW, uint16_t, struct I2SPR> I2SPR{};
};

inline constexpr SPI_T<0x40013000> const *SPI1{};
inline constexpr SPI_T<0x40003C00> const *SPI3{};
inline constexpr SPI_T<0x40003800> const *SPI2{};

struct SPI_CR1 {
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 15), cpp_register::AccessMode::RW, 1> BIDIMODE{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 14), cpp_register::AccessMode::RW, 1> BIDIOE{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 13), cpp_register::AccessMode::RW, 1> CRCEN{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 12), cpp_register::AccessMode::RW, 1> CRCNEXT{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 11), cpp_register::AccessMode::RW, 1> DFF{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 10), cpp_register::AccessMode::RW, 1> RXONLY{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 9), cpp_register::AccessMode::RW, 1> SSM{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 8), cpp_register::AccessMode::RW, 1> SSI{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 7), cpp_register::AccessMode::RW, 1> LSBFIRST{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 6), cpp_register::AccessMode::RW, 1> SPE{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 3), cpp_register::AccessMode::RW, 3> BR{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 2), cpp_register::AccessMode::RW, 1> MSTR{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 1), cpp_register::AccessMode::RW, 1> CPOL{};
  static constexpr cpp_register::Field<decltype(SPI1->CR1), (1UL << 0), cpp_register::AccessMode::RW, 1> CPHA{};
};

struct SPI_CR2 {
  static constexpr cpp_register::Field<decltype(SPI1->CR2), (1UL << 7), cpp_register::AccessMode::RW, 1> TXEIE{};
  static constexpr cpp_register::Field<decltype(SPI1->CR2), (1UL << 6), cpp_register::AccessMode::RW, 1> RXNEIE{};
  static constexpr cpp_register::Field<decltype(SPI1->CR2), (1UL << 5), cpp_register::AccessMode::RW, 1> ERRIE{};
  static constexpr cpp_register::Field<decltype(SPI1->CR2), (1UL << 4), cpp_register::AccessMode::RW, 1> FRF{};
  static constexpr cpp_register::Field<decltype(SPI1->CR2), (1UL << 2), cpp_register::AccessMode::RW, 1> SSOE{};
  static constexpr cpp_register::Field<decltype(SPI1->CR2), (1UL << 1), cpp_register::AccessMode::RW, 1> TXDMAEN{};
  static constexpr cpp_register::Field<decltype(SPI1->CR2), (1UL << 0), cpp_register::AccessMode::RW, 1> RXDMAEN{};
};

struct SPI_SR {
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 8), cpp_register::AccessMode::R, 1> TIFRFE{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 7), cpp_register::AccessMode::R, 1> BSY{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 6), cpp_register::AccessMode::R, 1> OVR{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 5), cpp_register::AccessMode::R, 1> MODF{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 4), cpp_register::AccessMode::RC_W0, 1> CRCERR{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 3), cpp_register::AccessMode::R, 1> UDR{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 2), cpp_register::AccessMode::R, 1> CHSIDE{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 1), cpp_register::AccessMode::R, 1> TXE{};
  static constexpr cpp_register::Field<decltype(SPI1->SR), (1UL << 0), cpp_register::AccessMode::R, 1> RXNE{};
};

struct SPI_DR {
  static constexpr cpp_register::Field<decltype(SPI1->DR), (1UL << 0), cpp_register::AccessMode::RW, 16> DR{};
};

struct SPI_CRCPR {
  static constexpr cpp_register::Field<decltype(SPI1->CRCPR), (1UL << 0), cpp_register::AccessMode::RW, 16> CRCPOLY{};
};

struct SPI_RXCRCR {
  static constexpr cpp_register::Field<decltype(SPI1->RXCRCR), (1UL << 0), cpp_register::AccessMode::R, 16> RxCRC{};
};

struct SPI_TXCRCR {
  static constexpr cpp_register::Field<decltype(SPI1->TXCRCR), (1UL << 0), cpp_register::AccessMode::R, 16> TxCRC{};
};

struct SPI_I2SCFGR {
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 11), cpp_register::AccessMode::RW, 1> I2SMOD{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 10), cpp_register::AccessMode::RW, 1> I2SE{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 8), cpp_register::AccessMode::RW, 2> I2SCFG{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 7), cpp_register::AccessMode::RW, 1> PCMSYNC{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 4), cpp_register::AccessMode::RW, 2> I2SSTD{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 3), cpp_register::AccessMode::RW, 1> CKPOL{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 1), cpp_register::AccessMode::RW, 2> DATLEN{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SCFGR), (1UL << 0), cpp_register::AccessMode::RW, 1> CHLEN{};
};

struct SPI_I2SPR {
  static constexpr cpp_register::Field<decltype(SPI1->I2SPR), (1UL << 9), cpp_register::AccessMode::RW, 1> MCKOE{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SPR), (1UL << 8), cpp_register::AccessMode::RW, 1> ODD{};
  static constexpr cpp_register::Field<decltype(SPI1->I2SPR), (1UL << 0), cpp_register::AccessMode::RW, 8> I2SDIV{};
};

} // namespace stm32f217::registers::spi
