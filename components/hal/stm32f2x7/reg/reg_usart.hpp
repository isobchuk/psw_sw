#pragma once

#include "register.hpp"

namespace stm32f217::registers::usart {

template <const cpp_register::RegisterAddress address> struct USART_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint16_t, struct SR> SR{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::RW, uint16_t, struct DR> DR{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::RW, uint16_t, struct BRR> BRR{};
  static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::RW, uint16_t, struct CR1> CR1{};
  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint16_t, struct CR2> CR2{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::RW, uint16_t, struct CR3> CR3{};
  static constexpr cpp_register::Register<address + 0x18, cpp_register::AccessMode::RW, uint16_t, struct GTPR> GTPR{};
};

inline constexpr USART_T<0x40011400> const *USART6{};
inline constexpr USART_T<0x40011000> const *USART1{};
inline constexpr USART_T<0x40004400> const *USART2{};
inline constexpr USART_T<0x40004800> const *USART3{};
inline constexpr USART_T<0x40004C00> const *UART4{};
inline constexpr USART_T<0x40005000> const *UART5{};

struct USART_SR {
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 9), cpp_register::AccessMode::RC_W0, 1> CTS{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 8), cpp_register::AccessMode::RC_W0, 1> LBD{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 7), cpp_register::AccessMode::R, 1> TXE{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 6), cpp_register::AccessMode::RC_W0, 1> TC{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 5), cpp_register::AccessMode::RC_W0, 1> RXNE{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 4), cpp_register::AccessMode::R, 1> IDLE{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 3), cpp_register::AccessMode::R, 1> ORE{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 2), cpp_register::AccessMode::R, 1> NF{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 1), cpp_register::AccessMode::R, 1> FE{};
  static constexpr cpp_register::Field<decltype(USART6->SR), (1UL << 0), cpp_register::AccessMode::R, 1> PE{};
};

struct USART_DR {
  static constexpr cpp_register::Field<decltype(USART6->DR), (1UL << 0), cpp_register::AccessMode::RW, 9> DR{};
};

struct USART_BRR {
  static constexpr cpp_register::Field<decltype(USART6->BRR), (1UL << 4), cpp_register::AccessMode::RW, 12> DIV_Mantissa{};
  static constexpr cpp_register::Field<decltype(USART6->BRR), (1UL << 0), cpp_register::AccessMode::RW, 4> DIV_Fraction{};
};

struct USART_CR1 {
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 15), cpp_register::AccessMode::RW, 1> OVER{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 13), cpp_register::AccessMode::RW, 1> UE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 12), cpp_register::AccessMode::RW, 1> M{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 11), cpp_register::AccessMode::RW, 1> WAKE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 10), cpp_register::AccessMode::RW, 1> PCE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 9), cpp_register::AccessMode::RW, 1> PS{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 8), cpp_register::AccessMode::RW, 1> PEIE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 7), cpp_register::AccessMode::RW, 1> TXEIE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 6), cpp_register::AccessMode::RW, 1> TCIE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 5), cpp_register::AccessMode::RW, 1> RXNEIE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 4), cpp_register::AccessMode::RW, 1> IDLEIE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 3), cpp_register::AccessMode::RW, 1> TE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 2), cpp_register::AccessMode::RW, 1> RE{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 1), cpp_register::AccessMode::RW, 1> RWU{};
  static constexpr cpp_register::Field<decltype(USART6->CR1), (1UL << 0), cpp_register::AccessMode::RW, 1> SBK{};
};

struct USART_CR2 {
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 14), cpp_register::AccessMode::RW, 1> LINEN{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 12), cpp_register::AccessMode::RW, 2> STOP{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 11), cpp_register::AccessMode::RW, 1> CLKEN{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 10), cpp_register::AccessMode::RW, 1> CPOL{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 9), cpp_register::AccessMode::RW, 1> CPHA{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 8), cpp_register::AccessMode::RW, 1> LBCL{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 6), cpp_register::AccessMode::RW, 1> LBDIE{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 5), cpp_register::AccessMode::RW, 1> LBDL{};
  static constexpr cpp_register::Field<decltype(USART6->CR2), (1UL << 0), cpp_register::AccessMode::RW, 4> ADD{};
};

struct USART_CR3 {
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 11), cpp_register::AccessMode::RW, 1> ONEBIT{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 10), cpp_register::AccessMode::RW, 1> CTSIE{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 9), cpp_register::AccessMode::RW, 1> CTSE{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 8), cpp_register::AccessMode::RW, 1> RTSE{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 7), cpp_register::AccessMode::RW, 1> DMAT{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 6), cpp_register::AccessMode::RW, 1> DMAR{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 5), cpp_register::AccessMode::RW, 1> SCEN{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 4), cpp_register::AccessMode::RW, 1> NACK{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 3), cpp_register::AccessMode::RW, 1> HDSEL{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 2), cpp_register::AccessMode::RW, 1> IRLP{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 1), cpp_register::AccessMode::RW, 1> IREN{};
  static constexpr cpp_register::Field<decltype(USART6->CR3), (1UL << 0), cpp_register::AccessMode::RW, 1> EIE{};
};

struct USART_GTPR {
  static constexpr cpp_register::Field<decltype(USART6->GTPR), (1UL << 8), cpp_register::AccessMode::RW, 8> GT{};
  static constexpr cpp_register::Field<decltype(USART6->GTPR), (1UL << 0), cpp_register::AccessMode::RW, 8> PSC{};
};

} // namespace stm32f217::registers::usart
