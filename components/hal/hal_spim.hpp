#pragma once

#include "meta_types.hpp"
#include <concepts>
#include <cstdint>

namespace hal::spim {

template <const unsigned numTx, iso::meta_type::const_value_of_type<unsigned> NumRx = iso::meta_type::const_t<0U>> struct STransmitReceiveBuffer {
  uint8_t _Tx[numTx + NumRx::value];
  uint8_t _Rx[NumRx::value];

  inline STransmitReceiveBuffer(const uint8_t (&tx)[numTx], const NumRx = iso::meta_type::const_v<0U>) {
    for (unsigned i = 0; i < sizeof(tx); i++) {
      _Tx[i] = tx[i];
    }
    for (unsigned i = sizeof(tx); i < sizeof(_Tx); i++) {
      _Tx[i] = 0xFFU;
    }
  }
};

template <typename SpiM>
concept spi_master = requires(const SpiM spi) {
  { spi.Init() } -> std::same_as<void>;
  {
    spi.TransmitReceive(*new STransmitReceiveBuffer({uint8_t{}, uint8_t{}, uint8_t{}}, iso::meta_type::const_t<unsigned{}>{}))
  } -> std::same_as<bool>;
  typename SpiM::TSpiM;
};

} // namespace hal::spim