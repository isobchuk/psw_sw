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
  enum EScroll : unsigned { Resource, Login, Password, Num };

  using TLength = uint8_t;
  static constexpr uint32_t _Magic = 0x9A7B5C3DUL;
  static constexpr auto _LCD_STRING_LENGTH = 20UL;
  using TFrame = char[3][_LCD_STRING_LENGTH];

  const char (&_PssBuffer)[N];
  const std::size_t _PssLength;
  const bool _IsPss;
  const std::size_t _Number;
  TFrame _Frame;
  static uint8_t _ScrollCounter[EScroll::Num];

  [[nodiscard]] inline bool CheckMagic(const char (&buf)[sizeof(uint32_t)]) const {
    const uint32_t magic = ((static_cast<uint32_t>(buf[3]) << 24) & 0xFF000000UL) | ((static_cast<uint32_t>(buf[2]) << 16) & 0x00FF0000UL) |
                           ((static_cast<uint32_t>(buf[1]) << 8) & 0x0000FF00UL) | ((static_cast<uint32_t>(buf[0]) << 0) & 0x000000FFUL);
    return (_Magic == magic);
  }

  [[nodiscard]] inline std::size_t GetCellsNumbers(const char (&buf)[sizeof(uint32_t)]) const {
    const std::convertible_to<std::size_t> auto number =
        ((static_cast<uint32_t>(buf[3]) << 24) & 0xFF000000UL) | ((static_cast<uint32_t>(buf[2]) << 16) & 0x00FF0000UL) |
        ((static_cast<uint32_t>(buf[1]) << 8) & 0x0000FF00UL) | ((static_cast<uint32_t>(buf[0]) << 0) & 0x000000FFUL);

    return number;
  }

  [[nodiscard]] inline uint8_t Offset(const EScroll type, const uint8_t cellLength) {
    const uint8_t offset = cellLength - _LCD_STRING_LENGTH - (cellLength - _LCD_STRING_LENGTH - _ScrollCounter[type]);
    _ScrollCounter[type]++;
    if(_ScrollCounter[type] > cellLength - _LCD_STRING_LENGTH) {
      _ScrollCounter[type] = 0;
    }
    return offset;
  }

public:
  inline CPasswordStorage(const char (&buf)[N], const std::size_t len = N)
      : _PssBuffer(buf), _PssLength(len), _IsPss(CheckMagic(reinterpret_cast<const char (&)[sizeof(uint32_t)]>(_PssBuffer[0]))),
        _Number(_IsPss ? GetCellsNumbers(reinterpret_cast<const char (&)[sizeof(uint32_t)]>(_PssBuffer[4])) : 0U), _Frame{} {}

  inline bool IsValid() const { return _IsPss; }
  inline std::size_t GetNumber() const { return _Number; }

  const TFrame &GetFrame(const uint16_t cell, const bool scroll = false) {
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
        const std::convertible_to<std::size_t> auto offsetResource = (scroll && cellSize > _LCD_STRING_LENGTH) ? Offset(EScroll::Resource, cellSize) : 0U;
        for (std::size_t j = 0; j < std::size_t(cellSize); j++) {
          _Frame[0][j] = _PssBuffer[counter + offsetResource];
          counter++;
        }
      }

      if (static_cast<char>(EType::Login) == _PssBuffer[counter++]) {
        const std::convertible_to<std::size_t> auto cellSize = _PssBuffer[counter++];
        const std::convertible_to<std::size_t> auto offsetLogin = (scroll && cellSize > _LCD_STRING_LENGTH) ? Offset(EScroll::Login, cellSize) : 0U;
        for (std::size_t j = 0; j < std::size_t(cellSize); j++) {
          _Frame[1][j] = _PssBuffer[counter + offsetLogin];
          counter++;
        }
      }

      if (static_cast<char>(EType::Password) == _PssBuffer[counter++]) {
        const std::convertible_to<std::size_t> auto cellSize = _PssBuffer[counter++];
        const std::convertible_to<std::size_t> auto offsetPassword = (scroll && cellSize > _LCD_STRING_LENGTH) ? Offset(EScroll::Password, cellSize) : 0U;
        for (std::size_t j = 0; j < std::size_t(cellSize); j++) {
          _Frame[2][j] = _PssBuffer[counter + offsetPassword];
          counter++;
        }
      }
    }

    return _Frame;
  }
};

template<const std::size_t N>
uint8_t CPasswordStorage<N>::_ScrollCounter[CPasswordStorage<N>::EScroll::Num];
} // namespace integration::pss
