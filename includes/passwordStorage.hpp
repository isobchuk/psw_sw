/**
 * @file passwordStorage.hpp
 * @author Ivan Sobchuk
 * @brief The file with decryption for *.pss files for embedded systems
 * @version 0.1
 * @date 2025-04-13
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>

namespace integration::pss {

template <const std::size_t N> class CPasswordStorage final {
  enum class EType : uint8_t { Record = 0xA0U, Resource = 0xAAU, Login = 0xABU, Password = 0xACU };

  using TLength = uint8_t;
  static constexpr uint32_t _Magic = 0x9A7B5C3DUL;
  using TFrame = char[3][20];

  const char (&_PssBuffer)[N];
  const std::size_t _PssLength;
  const bool _IsPss;
  const std::size_t _Number;
  TFrame _Frame;

  inline bool CheckMagic(const char (&buf)[sizeof(uint32_t)]) const {
    const uint32_t magic = ((static_cast<uint32_t>(buf[3]) << 24) & 0xFF000000UL) | ((static_cast<uint32_t>(buf[2]) << 16) & 0x00FF0000UL) |
                           ((static_cast<uint32_t>(buf[1]) << 8) & 0x0000FF00UL) | ((static_cast<uint32_t>(buf[0]) << 0) & 0x000000FFUL);
    return (_Magic == magic);
  }

  inline std::size_t GetCellsNumbers(const char (&buf)[sizeof(uint32_t)]) const {
    const std::convertible_to<std::size_t> auto number =
        ((static_cast<uint32_t>(buf[3]) << 24) & 0xFF000000UL) | ((static_cast<uint32_t>(buf[2]) << 16) & 0x00FF0000UL) |
        ((static_cast<uint32_t>(buf[1]) << 8) & 0x0000FF00UL) | ((static_cast<uint32_t>(buf[0]) << 0) & 0x000000FFUL);

    return number;
  }

public:
  inline CPasswordStorage(const char (&buf)[N], const std::size_t len = N)
      : _PssBuffer(buf), _PssLength(len), _IsPss(CheckMagic(reinterpret_cast<const char (&)[sizeof(uint32_t)]>(_PssBuffer[0]))),
        _Number(_IsPss ? GetCellsNumbers(reinterpret_cast<const char (&)[sizeof(uint32_t)]>(_PssBuffer[4])) : 0U), _Frame{} {}

  inline bool IsValid() const { return _IsPss; }
  inline bool GetNumber() const { return _Number; }

  const TFrame &GetFrame(const uint16_t cell) {
    for (std::size_t j = 0; j < std::size_t(20U); j++) {
      _Frame[0][j] = 0U;
      _Frame[1][j] = 0U;
      _Frame[2][j] = 0U;
    }

    if (cell <= _Number) {
      std::size_t counter = sizeof(uint32_t) + sizeof(uint32_t);
      std::size_t number = 0U;

      while (number != cell) {
        while (static_cast<char>(EType::Record) != _PssBuffer[counter]) {
          counter++;
        }
        number++;
        counter++;
      }

      [[maybe_unused]] const auto length = _PssBuffer[counter++];

      if (static_cast<char>(EType::Resource) == _PssBuffer[counter++]) {
        const std::convertible_to<std::size_t> auto cellSize = _PssBuffer[counter++];
        for (std::size_t j = 0; j < std::size_t(cellSize); j++) {
          _Frame[0][j] = _PssBuffer[counter++];
        }
      }

      if (static_cast<char>(EType::Login) == _PssBuffer[counter++]) {
        const std::convertible_to<std::size_t> auto cellSize = _PssBuffer[counter++];
        for (std::size_t j = 0; j < std::size_t(cellSize); j++) {
          _Frame[1][j] = _PssBuffer[counter++];
        }
      }

      if (static_cast<char>(EType::Password) == _PssBuffer[counter++]) {
        const std::convertible_to<std::size_t> auto cellSize = _PssBuffer[counter++];
        for (std::size_t j = 0; j < std::size_t(cellSize); j++) {
          _Frame[2][j] = _PssBuffer[counter++];
        }
      }
    }

    return _Frame;
  }
};
} // namespace integration::pss
