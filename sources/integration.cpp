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
  _EncoderA.Init();
  _EncoderB.Init();
  _EncoderButton.Init();
  _InterruptController[const_v<mcu::drivers::interrupt::Number::EXTI0>].Enable();
  _InterruptController[const_v<mcu::drivers::interrupt::Number::EXTI1>].Enable();
  _InterruptController[const_v<mcu::drivers::interrupt::Number::EXTI2>].Enable();

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
  _Lcd2004.Clear();

  //_Lcd2004.Address(uint8_t(0U));
  //_Lcd2004.String("Test string!");

  /* if (fatfs::EError::NoError == fileSystem.Init()) {
     debug.message(iso::format::string<PROJECT_NAME>);
     debug.message(iso::format::string<CONFIGURATION>);
     debug.message(iso::format::string<GIT_BRANCH>);
     debug.message(iso::format::string<GIT_COMMIT>);
   }*/
}

void CIntegration::operator()() {

  enum class EState { Launch, LogIn, Checking };
  static EState state = EState::Launch;
  [[maybe_unused]] static char pinCode[8] = {};
  if (state == EState::Launch) {
    _Lcd2004.SetCursor(0, 6);
    _Lcd2004.String("PIN CODE");
    _Lcd2004.SetCursor(1, 6);
    _Lcd2004.String("0");
    state = EState::LogIn;
  } else if (state == EState::LogIn) {
    static uint8_t digit;
    static uint8_t row;
    if (ERotation::None != _Rotation) {
      if (ERotation::Clockwise == _Rotation) {
        if (digit < 9) {
          digit++;
        }
      } else if (ERotation::Counterclockwise == _Rotation) {
        if (digit > 0) {
          digit--;
        }
      } else if (ERotation::Enter == _Rotation) {
        if (row < (sizeof(pinCode) - 1)) {
          pinCode[row++] = char(digit + 0x30);
          digit = 0;
        } else {
          state = EState::Checking;
        }
      }
      _Lcd2004.SetCursor(1, row + 6);
      char symbol[2] = {char(digit + 0x30), 0};
      _Lcd2004.String(symbol);
      _Rotation = ERotation::None;
    } else if (state == EState::Checking) {
    }
  }

  usbDevice();
  // static Timeout timeoutLed;
  /*if (ERotation::None != _Rotation) {
    static std::unsigned_integral auto counter = 0UL;

    if (ERotation::Clockwise == _Rotation) {
      if (counter < 3) {
        counter++;
      }
    } else if (ERotation::Counterclockwise == _Rotation) {
      if (counter > 0) {
        counter--;
      }
    }

    _Lcd2004.Clear();
    _Lcd2004.SetCursor(static_cast<uint8_t>(counter), 0);
    _Lcd2004.String("Test string!");
    _Rotation = ERotation::None;
  }

   _SystemTime.Start(timeoutLed);
  if (_SystemTime.Check(timeoutLed, 1000U)) {
    debug.trace(iso::format::string<"Just a tick">);
  }
  */
}

volatile CIntegration::ERotation CIntegration::_Rotation;

} // namespace integration

void SysTick_Handler(void) { integration::CIntegration::InterruptSystemTick(); }
void OTG_FS_IRQHandler(void) { integration::CIntegration::InterruptUsb(); }
void EXTI0_IRQHandler(void) { integration::CIntegration::InterruptExti0(); }
void EXTI1_IRQHandler(void) { integration::CIntegration::InterruptExti1(); }
void EXTI2_IRQHandler(void) { integration::CIntegration::InterruptExti2(); }
