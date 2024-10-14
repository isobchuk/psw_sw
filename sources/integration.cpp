#include "integration.hpp"

namespace integration {

CIntegration::CIntegration() {
  using namespace iso::meta_type;
  _InterruptController.GlobalEnable();
  //_InterruptController[iso::meta_type::const_v<mcu::drivers::interrupt::Number::TIM7>].Enable();
  _Clock.Init();
  _PinMap.Init();
  _DmaController.Init();
  _SystemTick.Init();
  _TerminalOutput.Init();
  _Spi.Init();

  const bool result = _Flash.Connect();
  if (result) {
    _Flash.GetStatus();
    _Flash.WriteEnable();
    _Flash.GetStatus();
  }

  _PinMap[const_v<EPinFunction::UsbEnable>].Write(const_v<false>);
  _InterruptController[const_v<mcu::drivers::interrupt::Number::OTG_FS>].Enable();
  usbDevice.Init();

  _Lcd2004.Init();
  _Lcd2004.Address(uint8_t(0U));
  _Lcd2004.Clear();
  _Lcd2004.String("Test string!");

  debug.message(iso::format::string<PROJECT_NAME>);
  debug.message(iso::format::string<CONFIGURATION>);
  debug.message(iso::format::string<GIT_BRANCH>);
  debug.message(iso::format::string<GIT_COMMIT>);
}

void CIntegration::operator()() {
  static Timeout timeoutLed;

  usbDevice();

  /* _SystemTime.Start(timeoutLed);
  if (_SystemTime.Check(timeoutLed, 1000U)) {
    debug.trace(iso::format::string<"Just a tick">);
  }
  */
}

} // namespace integration

void SysTick_Handler(void) { integration::CIntegration::InterruptSystemTick(); }
void OTG_FS_IRQHandler(void) { integration::CIntegration::InterruptUsb(); }