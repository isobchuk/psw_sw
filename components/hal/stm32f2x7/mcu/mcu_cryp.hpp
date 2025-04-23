#pragma once

#include "meta_types.hpp"
#include "reg_cryp.hpp"
#include "reg_rcc.hpp"

namespace stm32f217::drivers::cryp {

enum class EAlgorithm : uint32_t { TDES = 0b000U, DES = 0b010U, AES = 0b100U };
enum class EDirection : uint32_t { Encrypt, Decrypt };

// AES
namespace aes {
enum class EMode : uint32_t { ECB = 0b00U, CBC = 0b01U, CTR = 0b10U };
enum class EKeySize : uint32_t { B128 = 128U, B192 = 192U, B256 = 256U };

template <iso::meta_type::const_value_of_type<EMode> Mode, iso::meta_type::const_value_of_type<EKeySize> KeySize> class CCrypto final {
  static constexpr auto _ALGORITHM = EAlgorithm::AES;
  static constexpr auto _MODE = Mode::value;
  static constexpr auto _KEY_SIZE = KeySize::value;

  static constexpr auto _KEY_SIZE_BYTES = static_cast<std::underlying_type_t<typename KeySize::type>>(KeySize::value) / 8;

  template <const EKeySize keySize>
  static constexpr bool _IS_KEY_SIZE_VALID = ((_KEY_SIZE_BYTES * 8) == static_cast<std::underlying_type_t<decltype(keySize)>>(keySize));

  static constexpr auto _AES_BLOCK_SIZE = 16U;
  template <const std::size_t size> static constexpr bool _IS_SIZE_AES_BLOCK_SIZED = (0 == size % _AES_BLOCK_SIZE);

  // For now do not use DMA or interrupts
  // len was checked outside
  // Naive realization
  template <const std::size_t N>
  requires _IS_SIZE_AES_BLOCK_SIZED<N>
  inline void Process(const char (&input)[N], char (&output)[N], const std::size_t len) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    CRYP->CR |= CRYP_CR::CRYPEN;

    for (std::size_t i = 0U; i < len; i += _AES_BLOCK_SIZE) {

      // Wait for input FIFO empty (to write block)
      while (!(CRYP->SR & CRYP_SR::IFEM)) {
      }

      // Fill Data In
      CRYP->DIN = *reinterpret_cast<uint32_t *>(input[i + sizeof(uint32_t) * 0]);
      CRYP->DIN = *reinterpret_cast<uint32_t *>(input[i + sizeof(uint32_t) * 1]);
      CRYP->DIN = *reinterpret_cast<uint32_t *>(input[i + sizeof(uint32_t) * 2]);
      CRYP->DIN = *reinterpret_cast<uint32_t *>(input[i + sizeof(uint32_t) * 3]);

      // Wait for data processing
      while (CRYP->SR & CRYP_SR::BUSY) {
      }

      // Wait until output is ready
      while (!(CRYP->SR & CRYP_SR::OFNE)) {
      }

      // Get Data Out
      *reinterpret_cast<uint32_t *>(output[i + sizeof(uint32_t) * 0]) = *(CRYP->DOUT);
      *reinterpret_cast<uint32_t *>(output[i + sizeof(uint32_t) * 1]) = *(CRYP->DOUT);
      *reinterpret_cast<uint32_t *>(output[i + sizeof(uint32_t) * 2]) = *(CRYP->DOUT);
      *reinterpret_cast<uint32_t *>(output[i + sizeof(uint32_t) * 3]) = *(CRYP->DOUT);
    }

    CRYP->CR &= CRYP_CR::CRYPEN;
  }

public:
  consteval CCrypto(const Mode, const KeySize) {}

  inline void Init() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::cryp;

    // Enable clocking
    RCC->AHB2ENR |= RCC_AHB2ENR::CRYPEN;

    // Choose Algorithm
    using Algorithm = uint32_t;
    constexpr Algorithm ALGORITHM = static_cast<Algorithm>(_ALGORITHM) | static_cast<Algorithm>(_MODE);
    CRYP->CR |= CRYP_CR::ALGOMODE(reg_v<ALGORITHM>);

    // Select data type
    // Not needed as we have the standard option
    // CRYP->CR |= CRYP_CR::DATATYPE(reg_v<0U>);

    // Select Key Size
    using enum EKeySize;
    constexpr auto KEY_SIZE = (B256 == _KEY_SIZE) ? 0b10U : (B192 == _KEY_SIZE) ? 0b01U : 0b00U;
    if constexpr (0b00 != KEY_SIZE) {
      CRYP->CR |= CRYP_CR::KEYSIZE(reg_v<KEY_SIZE>);
    }
  }

  // For now do not use DMA or interrupts
  // Length of buffers checked during compile time
  template <const std::size_t N>
  requires _IS_SIZE_AES_BLOCK_SIZED<N>
  inline bool Encrypt(const char (&input)[N], char (&output)[N]) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Set the direction
    CRYP->CR &= CRYP_CR::ALGODIR;

    // Process buffer
    Process(input, output, N);

    return true;
  }

  // For now do not use DMA or interrupts
  // Length of buffers checked dynamically
  template <const std::size_t N> inline bool Encrypt(const char (&input)[N], char (&output)[N], const std::size_t len) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Check len
    if (0U != len % _AES_BLOCK_SIZE) {
      return false;
    }

    // Set the direction
    CRYP->CR &= CRYP_CR::ALGODIR;

    // Process buffer
    Process(input, output, len);

    return true;
  }

  // For now do not use DMA or interrupts
  // Length of buffers checked during compile time
  template <const std::size_t N>
  requires _IS_SIZE_AES_BLOCK_SIZED<N>
  inline bool Decrypt(const char (&input)[N], char (&output)[N]) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Set the direction
    CRYP->CR |= CRYP_CR::ALGODIR;

    // Process buffer
    Process(input, output, N);

    return true;
  }

  // For now do not use DMA or interrupts
  // Length of buffers checked dynamically
  template <const std::size_t N> inline bool Decrypt(const char (&input)[N], char (&output)[N], const std::size_t len) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Check len
    if (0U != len % _AES_BLOCK_SIZE) {
      return false;
    }

    // Set the direction
    CRYP->CR |= CRYP_CR::ALGODIR;

    // Process buffer
    Process(input, output, len);

    return true;
  }

  inline void KeySetup(const char (&key)[_KEY_SIZE_BYTES]) const
  requires _IS_KEY_SIZE_VALID<EKeySize::B256>
  {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Key register type
    using KeyRegister = uint32_t;

    // clang-format off
    // Fill the key
    CRYP->KR[reg_v<0U>] = (static_cast<KeyRegister>(key[0]) << 24)  | (static_cast<KeyRegister>(key[1]) << 16)  |
                          (static_cast<KeyRegister>(key[2]) << 8)   | (static_cast<KeyRegister>(key[3]) << 0);
    CRYP->KR[reg_v<1U>] = (static_cast<KeyRegister>(key[4]) << 24)  | (static_cast<KeyRegister>(key[5]) << 16)  |
                          (static_cast<KeyRegister>(key[6]) << 8)   | (static_cast<KeyRegister>(key[7]) << 0);
    CRYP->KR[reg_v<2U>] = (static_cast<KeyRegister>(key[8]) << 24)  | (static_cast<KeyRegister>(key[9]) << 16)  |
                          (static_cast<KeyRegister>(key[10]) << 8)  | (static_cast<KeyRegister>(key[11]) << 0);
    CRYP->KR[reg_v<3U>] = (static_cast<KeyRegister>(key[12]) << 24) | (static_cast<KeyRegister>(key[13]) << 16) |
                          (static_cast<KeyRegister>(key[14]) << 8)  | (static_cast<KeyRegister>(key[15]) << 0);
    CRYP->KR[reg_v<4U>] = (static_cast<KeyRegister>(key[16]) << 24) | (static_cast<KeyRegister>(key[17]) << 16) |
                          (static_cast<KeyRegister>(key[18]) << 8)  | (static_cast<KeyRegister>(key[19]) << 0);
    CRYP->KR[reg_v<5U>] = (static_cast<KeyRegister>(key[20]) << 24) | (static_cast<KeyRegister>(key[21]) << 16) |
                          (static_cast<KeyRegister>(key[22]) << 8)  | (static_cast<KeyRegister>(key[23]) << 0);
    CRYP->KR[reg_v<6U>] = (static_cast<KeyRegister>(key[24]) << 24) | (static_cast<KeyRegister>(key[25]) << 16) |
                          (static_cast<KeyRegister>(key[26]) << 8)  | (static_cast<KeyRegister>(key[27]) << 0);
    CRYP->KR[reg_v<7U>] = (static_cast<KeyRegister>(key[28]) << 24) | (static_cast<KeyRegister>(key[29]) << 16) |
                          (static_cast<KeyRegister>(key[30]) << 8)  | (static_cast<KeyRegister>(key[31]) << 0);
    // clang-format on
  }

  inline void KeySetup(const char (&key)[_KEY_SIZE_BYTES]) const
  requires _IS_KEY_SIZE_VALID<EKeySize::B192>
  {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Key register type
    using KeyRegister = uint32_t;

    // clang-format off
    // Fill the key
    CRYP->KR[reg_v<2U>] = (static_cast<KeyRegister>(key[0]) << 24)  | (static_cast<KeyRegister>(key[1]) << 16)  |
                          (static_cast<KeyRegister>(key[2]) << 8)   | (static_cast<KeyRegister>(key[3]) << 0);
    CRYP->KR[reg_v<3U>] = (static_cast<KeyRegister>(key[4]) << 24)  | (static_cast<KeyRegister>(key[5]) << 16)  |
                          (static_cast<KeyRegister>(key[6]) << 8)   | (static_cast<KeyRegister>(key[7]) << 0);
    CRYP->KR[reg_v<4U>] = (static_cast<KeyRegister>(key[8]) << 24)  | (static_cast<KeyRegister>(key[9]) << 16)  |
                          (static_cast<KeyRegister>(key[10]) << 8)  | (static_cast<KeyRegister>(key[11]) << 0);
    CRYP->KR[reg_v<5U>] = (static_cast<KeyRegister>(key[12]) << 24) | (static_cast<KeyRegister>(key[13]) << 16) |
                          (static_cast<KeyRegister>(key[14]) << 8)  | (static_cast<KeyRegister>(key[15]) << 0);
    CRYP->KR[reg_v<6U>] = (static_cast<KeyRegister>(key[16]) << 24) | (static_cast<KeyRegister>(key[17]) << 16) |
                          (static_cast<KeyRegister>(key[18]) << 8)  | (static_cast<KeyRegister>(key[19]) << 0);
    CRYP->KR[reg_v<7U>] = (static_cast<KeyRegister>(key[20]) << 24) | (static_cast<KeyRegister>(key[21]) << 16) |
                          (static_cast<KeyRegister>(key[22]) << 8)  | (static_cast<KeyRegister>(key[23]) << 0);
    // clang-format on
  }

  inline void KeySetup(const char (&key)[_KEY_SIZE_BYTES]) const
  requires _IS_KEY_SIZE_VALID<EKeySize::B128>
  {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Key register type
    using KeyRegister = uint32_t;

    // clang-format off
    // Fill the key
    CRYP->KR[reg_v<4U>] = (static_cast<KeyRegister>(key[0]) << 24)  | (static_cast<KeyRegister>(key[1]) << 16)  |
                          (static_cast<KeyRegister>(key[2]) << 8)   | (static_cast<KeyRegister>(key[3]) << 0);
    CRYP->KR[reg_v<5U>] = (static_cast<KeyRegister>(key[4]) << 24)  | (static_cast<KeyRegister>(key[5]) << 16)  |
                          (static_cast<KeyRegister>(key[6]) << 8)   | (static_cast<KeyRegister>(key[7]) << 0);
    CRYP->KR[reg_v<6U>] = (static_cast<KeyRegister>(key[8]) << 24)  | (static_cast<KeyRegister>(key[9]) << 16)  |
                          (static_cast<KeyRegister>(key[10]) << 8)  | (static_cast<KeyRegister>(key[11]) << 0);
    CRYP->KR[reg_v<7U>] = (static_cast<KeyRegister>(key[12]) << 24) | (static_cast<KeyRegister>(key[13]) << 16) |
                          (static_cast<KeyRegister>(key[14]) << 8)  | (static_cast<KeyRegister>(key[15]) << 0);
    // clang-format on
  }

  inline void InitializationVectorsSetup(const char (&vector)[16]) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::cryp;

    // Key register type
    using InitVectorsRegister = uint32_t;

    // clang-format off
    // Fill the key
    CRYP->IVR[reg_v<0U>] = (static_cast<InitVectorsRegister>(vector[0]) << 24)  | (static_cast<InitVectorsRegister>(vector[1]) << 16)  |
                           (static_cast<InitVectorsRegister>(vector[2]) << 8)   | (static_cast<InitVectorsRegister>(vector[3]) << 0);
    CRYP->IVR[reg_v<1U>] = (static_cast<InitVectorsRegister>(vector[4]) << 24)  | (static_cast<InitVectorsRegister>(vector[5]) << 16)  |
                           (static_cast<InitVectorsRegister>(vector[6]) << 8)   | (static_cast<InitVectorsRegister>(vector[7]) << 0);
    CRYP->IVR[reg_v<2U>] = (static_cast<InitVectorsRegister>(vector[8]) << 24)  | (static_cast<InitVectorsRegister>(vector[9]) << 16)  |
                           (static_cast<InitVectorsRegister>(vector[10]) << 8)  | (static_cast<InitVectorsRegister>(vector[11]) << 0);
    CRYP->IVR[reg_v<3U>] = (static_cast<InitVectorsRegister>(vector[12]) << 24) | (static_cast<InitVectorsRegister>(vector[13]) << 16) |
                           (static_cast<InitVectorsRegister>(vector[14]) << 8)  | (static_cast<InitVectorsRegister>(vector[15]) << 0);
    // clang-format on
  }
};
} // namespace aes

} // namespace stm32f217::drivers::cryp