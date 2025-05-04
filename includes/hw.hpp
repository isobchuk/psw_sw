#pragma once

#include "mcu_clock.hpp"
#include "mcu_cryp.hpp"
#include "mcu_dma.hpp"
#include "mcu_external_interrupts.hpp"
#include "mcu_gpio.hpp"
#include "mcu_interrupts.hpp"
#include "mcu_spim.hpp"
#include "mcu_systick.hpp"
#include "mcu_tim_base.hpp"
#include "mcu_usb_otg_device.hpp"
#include "system_time.hpp"

namespace hardware {

namespace mcu = stm32f217;

/*!<===========================================GPIO PIN MAP================================================>!*/
enum class EPinFunction : unsigned {
  FlashWP,
  FlashHold,
  FlashCS,
  FlashSCK,
  FlashMISO,
  FlashMOSI,

  UsbEnable,
  UsbVBus,
  UsbDM,
  UsbDP,

  Led,

  EncA,
  EncB,
  EncButton,

  DebugRx,
  DebugTx,

  LcdDB4,
  LcdDB5,
  LcdDB7,
  LcdDB6,
  LcdRS,
  LcdRW,
  LcdE,

  Num
};

// clang-format off
  static constexpr mcu::drivers::gpio::PinOut<EPinFunction> _PinTable[] = {
      // SPI Flash
      {EPinFunction::FlashWP,   mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_2,   mcu::drivers::gpio::Mode::Output},
      {EPinFunction::FlashHold, mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_3,   mcu::drivers::gpio::Mode::Output},
      {EPinFunction::FlashCS,   mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_4,   mcu::drivers::gpio::Mode::Output},
      {EPinFunction::FlashSCK,  mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_5,   mcu::drivers::gpio::Mode::Alternate,  mcu::drivers::gpio::Pull::Up, mcu::drivers::gpio::Alternative::AF5},
      {EPinFunction::FlashMISO, mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_6,   mcu::drivers::gpio::Mode::Alternate,  mcu::drivers::gpio::Pull::Up, mcu::drivers::gpio::Alternative::AF5},
      {EPinFunction::FlashMOSI, mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_7,   mcu::drivers::gpio::Mode::Alternate,  mcu::drivers::gpio::Pull::Up, mcu::drivers::gpio::Alternative::AF5},

      // USB 
      {EPinFunction::UsbEnable, mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_8,   mcu::drivers::gpio::Mode::Output},
      {EPinFunction::UsbVBus,   mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_9,   mcu::drivers::gpio::Mode::Input},
      {EPinFunction::UsbDM,     mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_11,  mcu::drivers::gpio::Mode::Alternate,  mcu::drivers::gpio::Speed::VeryHigh, mcu::drivers::gpio::Alternative::AF10},
      {EPinFunction::UsbDP,     mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_12,  mcu::drivers::gpio::Mode::Alternate,  mcu::drivers::gpio::Speed::VeryHigh, mcu::drivers::gpio::Alternative::AF10},

      // Led
      {EPinFunction::Led,       mcu::drivers::gpio::Port::PA,   mcu::drivers::gpio::Pin::Pin_15,  mcu::drivers::gpio::Mode::Output},

      // Encoder
      {EPinFunction::EncA,      mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_0,   mcu::drivers::gpio::Mode::Input},
      {EPinFunction::EncB,      mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_1,   mcu::drivers::gpio::Mode::Input},
      {EPinFunction::EncButton, mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_2,   mcu::drivers::gpio::Mode::Input},

      // Debug UART
      {EPinFunction::DebugTx,   mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_5,   mcu::drivers::gpio::Mode::Alternate,  mcu::drivers::gpio::Speed::VeryHigh, mcu::drivers::gpio::Alternative::AF7},
      {EPinFunction::DebugRx,   mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_6,   mcu::drivers::gpio::Mode::Alternate,  mcu::drivers::gpio::Speed::VeryHigh, mcu::drivers::gpio::Alternative::AF7},
      
      // LCD 2004
      {EPinFunction::LcdDB4,    mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_8,   mcu::drivers::gpio::Mode::Output,     mcu::drivers::gpio::Speed::VeryHigh },
      {EPinFunction::LcdDB5,    mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_9,   mcu::drivers::gpio::Mode::Output,     mcu::drivers::gpio::Speed::VeryHigh },
      {EPinFunction::LcdDB6,    mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_10,  mcu::drivers::gpio::Mode::Output,     mcu::drivers::gpio::Speed::VeryHigh },
      {EPinFunction::LcdDB7,    mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_11,  mcu::drivers::gpio::Mode::Output,     mcu::drivers::gpio::Speed::VeryHigh },
      {EPinFunction::LcdRS,     mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_13,  mcu::drivers::gpio::Mode::Output,     mcu::drivers::gpio::Speed::VeryHigh },
      {EPinFunction::LcdRW,     mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_14,  mcu::drivers::gpio::Mode::Output,     mcu::drivers::gpio::Speed::VeryHigh },
      {EPinFunction::LcdE,      mcu::drivers::gpio::Port::PD,   mcu::drivers::gpio::Pin::Pin_15,  mcu::drivers::gpio::Mode::Output,     mcu::drivers::gpio::Speed::VeryHigh }
  };
// clang-format on

static_assert((static_cast<unsigned>(EPinFunction::Num) == (sizeof(_PinTable) / sizeof(_PinTable[0]))), "All pin function should be used!");
inline constexpr mcu::drivers::gpio::CGpioPinMap<_PinTable> _PinMap{};
/*!<=======================================================================================================>!*/

/*!<======================================Clocking and SYST TICK===========================================>!*/

// Clock system
inline constexpr auto sc_QuartzClock = 8'000'000UL;
inline constexpr auto sc_SystemClock = 120'000'000UL;
inline constexpr mcu::drivers::clock::SystemClock<mcu::drivers::clock::HSE<sc_QuartzClock>, sc_SystemClock> _Clock{};

// System timer (SysTick)
using SystemTick = uint32_t;
inline constexpr mcu::drivers::system_timer::SystemTimer<sc_SystemClock, mcu::drivers::system_timer::Unit::mS, SystemTick> _SystemTick{};

// System time module (delays, timeout, current time since launch)
inline constexpr system_time::SystemTime _SystemTime{_SystemTick};
using Timeout = decltype(_SystemTime)::TimeoutTimer;
/*!<=======================================================================================================>!*/

/*!<============================================DMA TABLE==================================================>!*/
enum class EDmaFunctions : unsigned { FlashRX, FlashTX };
inline constexpr mcu::drivers::dma::SDmaChannelConfiguration<EDmaFunctions> _DmaTable[] = {
    // Flash Memory Dma (SPI1)
    // RX
    {EDmaFunctions::FlashRX, mcu::drivers::dma::EPort::Dma2, mcu::drivers::dma::EStream::Stream2, mcu::drivers::dma::EChannel::Channel3,
     mcu::drivers::dma::EPriority::VeryHigh, mcu::drivers::dma::EMemoryIncrementMode::Enabled},
    // TX
    {EDmaFunctions::FlashTX, mcu::drivers::dma::EPort::Dma2, mcu::drivers::dma::EStream::Stream3, mcu::drivers::dma::EChannel::Channel3,
     mcu::drivers::dma::EPriority::VeryHigh, mcu::drivers::dma::EMemoryIncrementMode::Enabled,
     mcu::drivers::dma::EDataTransferDirection::Memory2Peripheral}

};
inline constexpr mcu::drivers::dma::CDma<_DmaTable> _DmaController{};
/*!<=======================================================================================================>!*/

/*!<===========================================INTERRUPTS==================================================>!*/
inline constexpr mcu::drivers::interrupt::InterruptController _InterruptController{};
/*!<=======================================================================================================>!*/

/*!<==============================================SPI======================================================>!*/
inline constexpr mcu::drivers::spi::master::CSpiMasterDma _Spi{
    _DmaController[iso::meta_type::const_v<EDmaFunctions::FlashTX>], _DmaController[iso::meta_type::const_v<EDmaFunctions::FlashRX>],
    _PinMap[iso::meta_type::const_v<EPinFunction::FlashCS>],
    iso::meta_type::const_v<mcu::drivers::spi::master::SSpiMasterConfiguration{mcu::drivers::spi::master::EPort::Spi1}>};
/*!<=======================================================================================================>!*/

/*!<==========================================CRYPTO AES (NOT USED)=========================================>!*/
// inline constexpr mcu::drivers::cryp::aes::CCrypto _Crypto{iso::meta_type::const_v<mcu::drivers::cryp::aes::EMode::ECB>,
// iso::meta_type::const_v<mcu::drivers::cryp::aes::EKeySize::B256>};
/*!<=======================================================================================================>!*/

/*!<==============================================INITIALIZE===============================================>!*/
inline void Initialize() {
  using namespace iso::meta_type;

  _InterruptController.GlobalEnable();
  //_InterruptController[iso::meta_type::const_v<mcu::drivers::interrupt::Number::TIM7>].Enable();
  _Clock.Init();
  _PinMap.Init();
  _DmaController.Init();
  _SystemTick.Init();
  _Spi.Init();

  //_Crypto.Init();

  _PinMap[const_v<EPinFunction::UsbEnable>].Write(const_v<false>);
  _InterruptController[const_v<mcu::drivers::interrupt::Number::OTG_FS>].Enable();
}

}; // namespace hardware