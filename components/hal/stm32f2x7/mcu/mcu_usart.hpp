#pragma once

#include "fifo.hpp"
#include "hal_usart.hpp"
#include "meta_types.hpp"
#include "reg_rcc.hpp"
#include "reg_usart.hpp"

namespace stm32f217::drivers::usart {

enum class EPort : cpp_register::RegisterAddress {
  Usart1 = 0x4001'1000UL,
  Usart2 = 0x4000'4400UL,
  Usart3 = 0x4000'4800UL,
  Uart4 = 0x4000'4C00UL,
  Uart5 = 0x4000'5000UL,
  Usart6 = 0x4001'1400UL
};

template <iso::meta_type::const_value_of_type<EPort> Port> class CUart final {
  static constexpr registers::usart::USART_T<static_cast<cpp_register::RegisterAddress>(Port::value)> *USART{};

  using BufferT = iso::buffer::CFifo<char, 64>;
  static volatile BufferT bufferRx;

public:
  consteval CUart(const Port) {}

  template <iso::meta_type::const_value_of_type<uint32_t> Clock, iso::meta_type::const_value_of_type<uint32_t> BaudRate>
  requires ::hal::usart::is_br_standard<BaudRate::value>
  inline void Init(const Clock, const BaudRate) const {
    using enum EPort;
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::usart;

    // Clock corresponding USART
    if constexpr (Usart1 == Port::value) {
      RCC->APB2ENR |= RCC_APB2ENR::USART1EN;
    } else if (Usart2 == Port::value) {
      RCC->APB1ENR |= RCC_APB1ENR::USART2EN;
    } else if (Usart3 == Port::value) {
      RCC->APB1ENR |= RCC_APB1ENR::USART3EN;
    } else if (Uart4 == Port::value) {
      RCC->APB1ENR |= RCC_APB1ENR::UART4EN;
    } else if (Uart5 == Port::value) {
      RCC->APB1ENR |= RCC_APB1ENR::UART5EN;
    } else if (Usart6 == Port::value) {
      RCC->APB2ENR |= RCC_APB2ENR::USART6EN;
    }

    // Set baud-rate
    constexpr auto _BaudRate = (Clock::value + (BaudRate::value >> 1)) / BaudRate::value;
    USART->BRR = USART_BRR::DIV_Mantissa(reg_v<(_BaudRate >> 4)>) | USART_BRR::DIV_Fraction(reg_v<_BaudRate & 0xFU>);
    USART->CR1 |= USART_CR1::RE | USART_CR1::TE | USART_CR1::UE | USART_CR1::RXNEIE;
  }

  template <const std::size_t N> inline void Transmit(const char (&buf)[N], const std::size_t size = sizeof(buf)) const {
    using namespace stm32f217::registers::usart;

    const auto length = (size > sizeof(buf)) ? sizeof(buf) : size;

    for (std::size_t i = 0; i < length; i++) {
      while (!(USART->SR & USART_SR::TXE)) {
      }
      USART->DR = buf[i];
    }
  }

  inline void Transmit(const char b) const {
    using namespace stm32f217::registers::usart;

    while (!(USART->SR & USART_SR::TXE)) {
    }
    USART->DR = b;
  }

  template <const std::size_t N> inline std::size_t Receive(char (&buf)[N], const std::size_t size = sizeof(buf)) const {
    const auto length = (size > sizeof(buf)) ? sizeof(buf) : size;
    std::size_t i = 0;

    for (; i < length; i++) {
      const auto byte = bufferRx.pull();

      if (byte) {
        buf[i] = *byte;
      } else {
        break;
      }
    }
    return i;
  }

  inline bool Receive(char &b) const {
    bool result = false;
    const auto byte = bufferRx.pull();

    if (byte) {
      b = *byte;
      result = true;
    }

    return result;
  }

  void InterruptHandler() const {
    using namespace stm32f217::registers::usart;
    if (USART->SR & USART_SR::RXNE) {
      USART->SR &= USART_SR::RXNE;
      bufferRx.push(static_cast<char>(*(USART->DR)));
    }
  }
};

template <iso::meta_type::const_value_of_type<EPort> Port> volatile CUart<Port>::BufferT CUart<Port>::bufferRx;

} // namespace stm32f217::drivers::usart