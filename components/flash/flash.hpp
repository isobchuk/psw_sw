#pragma once

#include <concepts>
#include <cstdint>
#include <meta_types.hpp>

namespace hardware::flash {

template <typename T>
concept flash_memory_spi = requires(const T flash_spi, uint8_t (&arr)[16]) {
  { flash_spi.Connect() } -> std::same_as<bool>;
  { flash_spi.WriteEnable() } -> std::same_as<bool>;
  { flash_spi.GetStatus() } -> std::same_as<uint8_t>;
  { flash_spi.SectorErase(size_t{}) } -> std::same_as<bool>;
  { flash_spi.SectorErase(iso::meta_type::const_v<size_t{}>) } -> std::same_as<bool>;
  { flash_spi.ChipErase() } -> std::same_as<bool>;
  { flash_spi.PageProgram(size_t{}, arr) } -> std::same_as<bool>;
  { flash_spi.PageProgram(iso::meta_type::const_v<size_t{}>, arr) } -> std::same_as<bool>;
  { flash_spi.PageRead(size_t{}, arr) } -> std::same_as<bool>;
  { flash_spi.PageRead(iso::meta_type::const_v<size_t{}>, arr) } -> std::same_as<bool>;
};

} // namespace hardware::flash