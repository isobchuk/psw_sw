#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace kdbx {

struct SHeader {
  struct SFormatVersion {
    uint16_t _HighWord;
    uint16_t _LowWord;

    constexpr SFormatVersion(const uint16_t h, const uint16_t l) : _HighWord(h), _LowWord(l) {}
  };

  uint32_t _Signature[2];
  SFormatVersion _FormatVersion;

  constexpr SHeader(const uint32_t s1, const uint32_t s2, const uint16_t h, const uint16_t l) : _Signature{s1, s2}, _FormatVersion(h, l) {}

  inline bool operator==(const SHeader &other) const {
    // Considering only signatures and high word of version number (as recommended by KDBX File Format Specification)
    return ((_Signature[0] != other._Signature[0])                         ? false
            : (_Signature[1] != other._Signature[1])                       ? false
            : (_FormatVersion._HighWord != other._FormatVersion._HighWord) ? false
                                                                           : true);
  }
};

enum class EHeaderField : uint8_t {
  EndOfHeader,
  EncryptionAlgorithm = 2,
  CompressionAlgorithm,
  MasterSaltSeed,
  EncryptionIVNonce = 7,
  KdfParameters = 11,
  PublicCustomData = 12
};

template <const EHeaderField field> struct THeaderField {
  THeaderField() = delete;
};

template <> struct THeaderField<EHeaderField::EncryptionAlgorithm> {
  using Type = uint8_t[16];
};

struct SHeaderFields {
  THeaderField<EHeaderField::EncryptionAlgorithm>::Type _EncryptionAlgorithm;
};

class CKdbx final {

  // Header constants, here to be visible
  static constexpr auto _Header = SHeader(0x9AA2D903, 0xB54BFB67, 4, 1);
  static constexpr auto _Error = std::size_t(-1);

  // Fields of header
  SHeaderFields _HeaderFields;

  template <const std::size_t N> std::size_t ParseHeader(const uint8_t (&buffer)[N]) const {
    std::size_t counter = 0UL;
    using TSign1 = decltype(_Header._Signature[0]);
    const TSign1 signature1 = static_cast<TSign1>(buffer[counter + 3]) << 24 | static_cast<TSign1>(buffer[counter + 2]) << 16 |
                              static_cast<TSign1>(buffer[counter + 1]) << 8 | static_cast<TSign1>(buffer[counter]);
    counter += sizeof(TSign1);

    using TSign2 = decltype(_Header._Signature[1]);
    const TSign2 signature2 = static_cast<TSign2>(buffer[counter + 3]) << 24 | static_cast<TSign2>(buffer[counter + 2]) << 16 |
                              static_cast<TSign2>(buffer[counter + 1]) << 8 | static_cast<TSign2>(buffer[counter]);
    counter += sizeof(TSign2);

    using TVerL = decltype(_Header._FormatVersion._LowWord);
    const TVerL versionLow = static_cast<TVerL>(buffer[counter + 1]) << 8 | static_cast<TVerL>(buffer[counter]);
    counter += sizeof(TVerL);

    using TVerH = decltype(_Header._FormatVersion._HighWord);
    const TVerH versionHigh = static_cast<TVerH>(buffer[counter + 1]) << 8 | static_cast<TVerH>(buffer[counter]);
    counter += sizeof(TVerH);

    const SHeader headerReal(signature1, signature2, versionHigh, versionLow);

    return (_Header == headerReal) ? counter : _Error;
  }

  template <const std::size_t N> std::size_t ParseHeaderFields(const uint8_t (&buffer)[N]) const {
    using enum EHeaderField;
    std::size_t counter = 0UL;
    bool endOfHeader = false;

    while (!endOfHeader) {
      const EHeaderField name = buffer[counter];
      counter += sizeof(name);

      using TLength = uint32_t;
      const TLength length = static_cast<TLength>(buffer[counter + 3]) << 24 | static_cast<TLength>(buffer[counter + 2]) << 16 |
                             static_cast<TLength>(buffer[counter + 1]) << 8 | static_cast<TLength>(buffer[counter]);
      counter += sizeof(length);

      switch (name) {
      case EncryptionAlgorithm:
        if (sizeof(_HeaderFields._EncryptionAlgorithm) != length) {
          return _Error;
        }
        std::memcpy(_HeaderFields._EncryptionAlgorithm, &buffer[counter], _HeaderFields._EncryptionAlgorithm);
        counter += sizeof(_HeaderFields._EncryptionAlgorithm);
        break;
      }
    }

    return counter;
  }

public:
  template <const std::size_t N> std::size_t Parse(const uint8_t (&buffer)[N]) const {
    auto length = ParseHeader(buffer);

    if (_Error != length) {
    }

    return length;
  }
};
} // namespace kdbx