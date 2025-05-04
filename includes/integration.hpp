#pragma once

#include "FatFSIntegration.hpp"
#include "IntegrationUsb.hpp"
#include "encoder.hpp"
#include "lcd.hpp"
#include "lcd2004.hpp"
#include "led.hpp"
#include "log.hpp"
#include "mx25.hpp"
#include "rtt.hpp"


namespace integration {

class CIntegration final {
  // Led
  static constexpr hardware::led::Led _Led{hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::Led>]};

  // LCD display
  static constexpr iso::lcd::CLcd<ILcd2004> _Lcd2004{};

  // RTT Output (not used for now)
  static constexpr CRttOutput _TerminalOutput{};
  static constexpr iso::log::Log debug{_TerminalOutput, iso::log::log_lvl<iso::log::Trace::All, iso::log::Highlight::Enabled>,
                                       iso::format::string<"INTEGRATION">};

  // Flash
  static constexpr flash::mx25::CMX25 _Flash{hardware::_Spi, hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::FlashWP>]};

  // USB
  static constexpr auto integrationUsb = integration::usb::device::CUsbIntegration(hardware::_SystemTime, _Flash);
  static constexpr auto usbDevice = iso::usb::UsbDevice(integrationUsb);

  // Encoder
  static constexpr integration::encoder::CEncoder _Encoder{};

  // FatFs
  char fileBuffer[4096]; // expected max size
  char buffer[sizeof(fileBuffer)];
  std::size_t len;
  fatfs::CFatFs fileSystem;

  // Tasks
  void HeartBeat() const;
  void Application();
  void Usb() const;

public:
  CIntegration();
  void operator()();

  static consteval auto GetFlash() { return _Flash; }

  // Interrupt
  inline static void InterruptSystemTick() { hardware::_SystemTick.InterruptHandler(); }
  inline static void InterruptUsb() { integrationUsb.InterruptHandler(); }
  inline static void InterruptExti0() { _Encoder.InterruptExti0(); }
  inline static void InterruptExti1() { _Encoder.InterruptExti1(); }
  inline static void InterruptExti2() { _Encoder.InterruptExti2(); }
};

} // namespace integration
