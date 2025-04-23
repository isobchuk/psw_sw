/**
 * @file cryptoIntegration.hpp
 * @author Ivan Sobchuk
 * @brief Just a temporary replacement for hw CRYP module
 * @version 0.1
 * @date 2025-04-12
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstddef>
#include <cstdint>

extern "C" {
#include "aes.h"
#include "sha256.h"
}

namespace integration::crypto {

class CCryptoIntegration final {
  static constexpr auto PORTION_SIZE = 16U;
  static constexpr auto ROUNDS = 1UL;

  WORD2 key_schedule[60];
  BYTE key[32];

public:
  CCryptoIntegration(const char (&pin)[8]) : key_schedule{} {
    // Calculate key
    SHA256_CTX ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, reinterpret_cast<const unsigned char *>(&pin[0]), sizeof(pin));
    sha256_final(&ctx, key);

    aes_key_setup(key, key_schedule, 256);
  }

  template <const std::size_t N> inline void Encrypt(const char (&input)[N], char (&output)[N], const std::size_t len) {
    for (unsigned i = 0; i < ROUNDS; i++) {
      for (unsigned j = 0; j < (len / PORTION_SIZE) + 1; j++) {
        aes_encrypt(reinterpret_cast<const unsigned char *>(&input[j * PORTION_SIZE]), reinterpret_cast<unsigned char *>(&output[j * PORTION_SIZE]),
                    key_schedule, 256);
      }

    for (unsigned j = 0U; j < len; j += sizeof(uint32_t)) {
        *reinterpret_cast<uint32_t *>(&output[j]) = static_cast<uint32_t>(output[j + 0]) << 24U |
                                                    static_cast<uint32_t>(output[j + 1]) << 16U |
                                                    static_cast<uint32_t>(output[j + 2]) << 8U | static_cast<uint32_t>(output[j + 3]);
      }
    }
  }

  template <const std::size_t N> inline void Decrypt(const char (&input)[N], char (&output)[N], const std::size_t len) {
    for (unsigned i = 0; i < ROUNDS; i++) {
      for (unsigned j = 0; j < (len / PORTION_SIZE) + 1; j++) {
        aes_decrypt(reinterpret_cast<const unsigned char *>(&input[j * PORTION_SIZE]), reinterpret_cast<unsigned char *>(&output[j * PORTION_SIZE]),
                    key_schedule, 256);
      }

      /*for (unsigned j = 0U; j < len; j += sizeof(uint32_t)) {
        *reinterpret_cast<uint32_t *>(&output[j]) = static_cast<uint32_t>(output[j + 0]) << 24U |
                                                    static_cast<uint32_t>(output[j + 1]) << 16U |
                                                    static_cast<uint32_t>(output[j + 2]) << 8U | static_cast<uint32_t>(output[j + 3]);
      }*/
    }
  }
};

}; // namespace integration::crypto