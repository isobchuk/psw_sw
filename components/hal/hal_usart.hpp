#pragma once

#include <concepts>
#include <cstdint>

namespace hal::usart {
template <const uint32_t br>
inline constexpr auto is_br_standard = (4800UL == br) || (9600UL == br) || (38400UL == br) || (57600UL == br) || (115200UL == br) ||
                                       (230400UL == br) || (460800UL == br) || (921600UL == br);

template <typename TUsart>
concept usart = requires(const TUsart usart) {
  { usart.Init() } -> std::same_as<void>;
};
} // namespace hal::usart