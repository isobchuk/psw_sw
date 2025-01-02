#pragma once

#include "FatFSIntegration.hpp"
#include "IntegrationUsb.hpp"
#include "lcd.hpp"
#include "log.hpp"
#include "mcu_clock.hpp"
#include "mcu_dma.hpp"
#include "mcu_external_interrupts.hpp"
#include "mcu_gpio.hpp"
#include "mcu_interrupts.hpp"
#include "mcu_spim.hpp"
#include "mcu_systick.hpp"
#include "mcu_tim_base.hpp"
#include "mcu_usb_otg_device.hpp"
#include "mx25.hpp"
#include "system_time.hpp"

extern "C" {
#include "SEGGER_RTT.h"
}

namespace integration {

namespace mcu = stm32f217;

class CIntegration final {

  enum class ERotation : uint32_t { None, Clockwise, Counterclockwise, Enter };

  /*!<-------------------------GPIO PIN MAP--------------------------->!*/
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
      // User Led
      /*{EPinFunction::Led1,      mcu::drivers::gpio::Port::PB,   mcu::drivers::gpio::Pin::Pin_0,   mcu::drivers::gpio::Mode::Output},
      {EPinFunction::Led2,      mcu::drivers::gpio::Port::PB,   mcu::drivers::gpio::Pin::Pin_7,   mcu::drivers::gpio::Mode::Output},
      {EPinFunction::Led3,      mcu::drivers::gpio::Port::PB,   mcu::drivers::gpio::Pin::Pin_14,  mcu::drivers::gpio::Mode::Output},*/

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
  static constexpr integration::mcu::drivers::gpio::CGpioPinMap<_PinTable> _PinMap{};

  // DMA Table
  enum class EDmaFunctions : unsigned { FlashRX, FlashTX };
  static constexpr mcu::drivers::dma::SDmaChannelConfiguration<EDmaFunctions> _DmaTable[] = {
      // Flash Memory Dma (SPI1)
      // RX
      {EDmaFunctions::FlashRX, mcu::drivers::dma::EPort::Dma2, mcu::drivers::dma::EStream::Stream2, mcu::drivers::dma::EChannel::Channel3,
       mcu::drivers::dma::EPriority::VeryHigh, mcu::drivers::dma::EMemoryIncrementMode::Enabled},
      // TX
      {EDmaFunctions::FlashTX, mcu::drivers::dma::EPort::Dma2, mcu::drivers::dma::EStream::Stream3, mcu::drivers::dma::EChannel::Channel3,
       mcu::drivers::dma::EPriority::VeryHigh, mcu::drivers::dma::EMemoryIncrementMode::Enabled,
       mcu::drivers::dma::EDataTransferDirection::Memory2Peripheral}

  };
  static constexpr mcu::drivers::dma::CDma<_DmaTable> _DmaController{};

  // Clock system
  static constexpr auto sc_QuartzClock = 8'000'000UL;
  static constexpr auto sc_SystemClock = 120'000'000UL;
  static constexpr integration::mcu::drivers::clock::SystemClock<integration::mcu::drivers::clock::HSE<sc_QuartzClock>, sc_SystemClock> _Clock{};

  // System timer (SysTick)
  using SystemTick = uint32_t;
  static constexpr integration::mcu::drivers::system_timer::SystemTimer<sc_SystemClock, integration::mcu::drivers::system_timer::Unit::mS, SystemTick>
      _SystemTick{};

  // System time module (delays, timeout, current time since launch)
  static constexpr system_time::SystemTime _SystemTime{_SystemTick};
  using Timeout = decltype(_SystemTime)::TimeoutTimer;

  // Debug tracing
  class CRttOutput final {
  public:
    inline consteval CRttOutput() = default;

    inline void Init() const { SEGGER_RTT_Init(); }
    template <const std::size_t N> inline void puts(const char (&buf)[N]) const { SEGGER_RTT_WriteString(0, buf); }
    inline unsigned tick() const { return _SystemTime.Time(); }
  };
  static constexpr CRttOutput _TerminalOutput{};
  static constexpr iso::log::Log debug{_TerminalOutput, iso::log::log_lvl<iso::log::Trace::All, iso::log::Highlight::Enabled>,
                                       iso::format::string<"INTEGRATION">};

  // Interrupts
  static constexpr mcu::drivers::interrupt::InterruptController _InterruptController{};

  // Lcd 2004
  class ILcd2004 final {

    static constexpr mcu::drivers::tim::basic::CTimer _Timer{iso::meta_type::const_v<mcu::drivers::tim::basic::ETimer::Tim7>,
                                                             iso::meta_type::const_v<_Clock.BusFrequency(stm32f217::drivers::clock::EBus::APB1Tim)>};
    static constexpr mcu::drivers::gpio::CGpioBus<_PinMap[iso::meta_type::const_v<EPinFunction::LcdDB4>], 4> _DataBus{};

  public:
    consteval ILcd2004() = default;

    inline void Init(void) const { _Timer.Init(); }

    inline consteval auto Enable() const -> decltype(_PinMap[iso::meta_type::const_v<EPinFunction::LcdE>]) { return {}; }
    inline consteval auto ReadWrite() const -> decltype(_PinMap[iso::meta_type::const_v<EPinFunction::LcdRW>]) { return {}; }
    inline consteval auto InstructionData() const -> decltype(_PinMap[iso::meta_type::const_v<EPinFunction::LcdRS>]) { return {}; }

    template <iso::meta_type::const_value_of_type<uint8_t> Value> inline void Write(const Value) const {
      using namespace iso::meta_type;
      _DataBus.Write(const_v<static_cast<unsigned>(Value::value)>);
    }
    inline void Write(const uint8_t value) const {
      using namespace iso::meta_type;
      _DataBus.Write(value);
    }
    inline unsigned Read() const { return _DataBus.Read(); }
    inline void ToInput() const { _DataBus.ToInput(); }
    inline void ToOutput() const { _DataBus.ToOutput(); }

    template <iso::meta_type::const_value_of_type<unsigned long> Value> inline void Delay(const Value) const {
      using namespace iso::meta_type;
      _Timer.Launch(const_v<Value::value>, const_v<true>);
      while (!_Timer.UpdateEvent()) {
      }
    }
  };

  static constexpr iso::lcd::CLcd<ILcd2004> _Lcd2004{};

  // Spi
  static constexpr mcu::drivers::spi::master::CSpiMasterDma _Spi{
      _DmaController[iso::meta_type::const_v<EDmaFunctions::FlashTX>], _DmaController[iso::meta_type::const_v<EDmaFunctions::FlashRX>],
      _PinMap[iso::meta_type::const_v<EPinFunction::FlashCS>],
      iso::meta_type::const_v<mcu::drivers::spi::master::SSpiMasterConfiguration{mcu::drivers::spi::master::EPort::Spi1}>};
  static constexpr flash::mx25::CMX25 _Flash{_Spi, _PinMap[iso::meta_type::const_v<EPinFunction::FlashWP>]};

  // USB
  static constexpr auto integrationUsb = integration::usb::device::CUsbIntegration(_SystemTime, _Flash);
  static constexpr auto usbDevice = iso::usb::UsbDevice(integrationUsb);

  // FatFs
  static constexpr fatfs::CFatFs fileSystem{};

  // Encoder
  static constexpr mcu::drivers::exti::CExtInterrupt<_PinMap[iso::meta_type::const_v<EPinFunction::EncA>], mcu::drivers::exti::ETrigger::Falling>
      _EncoderA{};
  static constexpr mcu::drivers::exti::CExtInterrupt<_PinMap[iso::meta_type::const_v<EPinFunction::EncB>], mcu::drivers::exti::ETrigger::Falling>
      _EncoderB{};
  static constexpr mcu::drivers::exti::CExtInterrupt<_PinMap[iso::meta_type::const_v<EPinFunction::EncButton>], mcu::drivers::exti::ETrigger::Falling>
      _EncoderButton{};

  // Real variables
  static volatile ERotation _Rotation;

public:
  CIntegration();
  void operator()();

  static consteval auto GetFlash() { return _Flash; }

  // Interrupt
  inline static void InterruptSystemTick() { _SystemTick.InterruptHandler(); }
  inline static void InterruptUsb() { integrationUsb.InterruptHandler(); }

  inline static void InterruptExti0() {
    _EncoderA.InterruptHandler();
    if (_PinMap[iso::meta_type::const_v<EPinFunction::EncB>].Read()) {
      _Rotation = ERotation::Clockwise;
    }
  }
  inline static void InterruptExti1() {
    _EncoderB.InterruptHandler();
    if (_PinMap[iso::meta_type::const_v<EPinFunction::EncA>].Read()) {
      _Rotation = ERotation::Counterclockwise;
    }
  }

  inline static void InterruptExti2() {
    _EncoderButton.InterruptHandler();
    _Rotation = ERotation::Enter;
  }
};

} // namespace integration
