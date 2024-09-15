#pragma once

#include "hal_gpio.hpp"
#include "hal_spim.hpp"
#include "mcu_dma.hpp"
#include "meta_types.hpp"
#include "reg_rcc.hpp"
#include "reg_spi.hpp"

namespace stm32f217::drivers::spi::master {

enum class EPort : cpp_register::RegisterAddress { Spi1 = 0x4001'3000UL, Spi2 = 0x4000'3C00UL, Spi3 = 0x4000'3800UL };

enum class ESpeedDivider : uint32_t { Divider2, Divider4, Divider8, Divider16, Divider32, Divider64, Divider128, Divider256 };
enum class EClockPhase : uint32_t { Disabled, Enabled };
enum class EClockPolarity : uint32_t { Disabled, Enabled };

struct SSpiMasterConfiguration {
  const EPort _Port;
  const ESpeedDivider _SpeedDivider;
  const EClockPhase _ClockPhase;
  const EClockPolarity _ClockPolarity;

  template <typename... Args>
  requires iso::meta_type::var_pack::is_types_unique_v<Args...> &&
               iso::meta_type::var_pack::is_type_list<ESpeedDivider, EClockPhase, EClockPolarity>::contains_v<Args...>
  consteval SSpiMasterConfiguration(const EPort port, const Args... args)
      : _Port(port), _SpeedDivider(iso::meta_type::var_pack::type<ESpeedDivider>::get(args...)),
        _ClockPhase(iso::meta_type::var_pack::type<EClockPhase>::get(args...)),
        _ClockPolarity(iso::meta_type::var_pack::type<EClockPolarity>::get(args...)) {}
};

template <dma::dma_stream Tx, dma::dma_stream Rx, ::hal::gpio::gpio_out ChipSelect,
          iso::meta_type::const_value_of_type<SSpiMasterConfiguration> Configuration>
class CSpiMasterDma final {
  static constexpr auto _Configuration = Configuration::value;
  static constexpr registers::spi::SPI_T<static_cast<cpp_register::RegisterAddress>(_Configuration._Port)> *SPI{};
  const Tx _DmaTx;
  const Rx _DmaRx;
  const ChipSelect _ChipSelect;

public:
  struct TSpiM;
  consteval CSpiMasterDma(const Tx dmaTx, const Rx dmaRx, const ChipSelect chipSelect, const Configuration)
  requires ::hal::spim::spi_master<CSpiMasterDma>
      : _DmaTx(dmaTx), _DmaRx(dmaRx), _ChipSelect(chipSelect) {}

  inline void Init() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::spi;
    using namespace stm32f217::drivers::dma;
    using namespace iso::meta_type;

    if constexpr (EPort::Spi1 == _Configuration._Port) {
      RCC->APB2ENR |= RCC_APB2ENR::SPI1EN;
    } else if (EPort::Spi2 == _Configuration._Port) {
      RCC->APB1ENR |= RCC_APB1ENR::SPI2EN;
    } else if (EPort::Spi3 == _Configuration._Port) {
      RCC->APB1ENR |= RCC_APB1ENR::SPI3EN;
    } else {
      static_assert(((EPort::Spi1 == _Configuration._Port) || (EPort::Spi2 == _Configuration._Port) || (EPort::Spi3 == _Configuration._Port)),
                    "Driver SPI: No such port in the chip!");
    }

    constexpr std::size_t registerAddress = &(SPI->DR);
    _DmaTx.Init();
    _DmaTx.Address(const_v<EAddressRegion::Peripheral>, const_v<registerAddress>);
    _DmaRx.Init();
    _DmaRx.Address(const_v<EAddressRegion::Peripheral>, const_v<registerAddress>);

    _ChipSelect.Write(const_v<true>);

    SPI->CR2 |= (SPI_CR2::TXDMAEN | SPI_CR2::RXDMAEN);
    SPI->CR1 |= SPI_CR1::SSM | SPI_CR1::SSI | SPI_CR1::MSTR | SPI_CR1::SPE;
  }

  template <const unsigned numTx, iso::meta_type::const_value_of_type<unsigned> NumRx>
  inline bool TransmitReceive(::hal::spim::STransmitReceiveBuffer<numTx, NumRx> &buffer) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::spi;
    using namespace stm32f217::drivers::dma;
    using namespace iso::meta_type;

    _ChipSelect.Write(const_v<false>);

    uint8_t bufferRx[sizeof(buffer._Tx)] = {};

    _DmaRx.Address(const_v<EAddressRegion::Memory0>, bufferRx);
    _DmaRx.Number(const_v<uint32_t(sizeof(bufferRx))>);
    _DmaRx.Enable();

    _DmaTx.Address(const_v<EAddressRegion::Memory0>, buffer._Tx);
    _DmaTx.Number(const_v<uint32_t(sizeof(buffer._Tx))>);
    _DmaTx.Enable();

    while (!_DmaRx.InterruptStatus(const_v<EInterruptReason::TransferComplete>)) {
    }
    _DmaRx.InterruptClear(const_v<EInterruptReason::TransferComplete>);
    while (!_DmaTx.InterruptStatus(const_v<EInterruptReason::TransferComplete>)) {
    }
    _DmaTx.InterruptClear(const_v<EInterruptReason::TransferComplete>);
    _DmaRx.Disable();
    _DmaTx.Disable();

    _ChipSelect.Write(const_v<true>);

    for (unsigned i = 0; i < sizeof(buffer._Rx); i++) {
      buffer._Rx[i] = bufferRx[i + (sizeof(buffer._Tx) - sizeof(buffer._Rx))];
    }

    return true;
  }
};

} // namespace stm32f217::drivers::spi::master