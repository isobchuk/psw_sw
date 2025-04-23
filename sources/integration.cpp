#include "integration.hpp"
#include "cryptoIntegration.hpp"
#include "passwordStorage.hpp"

namespace integration {

enum class EState { Launch, LogIn, Checking, FillBase, ShowRecords };

//#define ISO_DEBUG

#ifdef ISO_DEBUG
static EState state = EState::Checking;
static char pinCode[8] = {'1', '8', '0', '2', '2', '0', '1', '2'};
#else
static EState state = EState::Launch;
static char pinCode[8] = {};
#endif

static uint16_t record;
static bool firstFrame;

CIntegration::CIntegration() : fileBuffer{}, buffer{}, len(), fileSystem() {
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

  //_Crypto.Init();

  if (integration::fatfs::EError::NoError == fileSystem.Init()) {
    const auto bytes = fileSystem.Read(fileBuffer, iso::meta_type::const_v<1024U>);
    if (std::size_t(-1) == bytes) {
      // Error handling
    }
    len = bytes + (bytes % 16);
  }

#ifdef ISO_DEBUG
  state = EState::Checking;
  pinCode[0] = '1';
  pinCode[1] = '8';
  pinCode[2] = '0';
  pinCode[3] = '2';
  pinCode[4] = '2';
  pinCode[5] = '0';
  pinCode[6] = '1';
  pinCode[7] = '2';
#endif

record = 1U;
firstFrame = true;
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

  constexpr auto PERIOD_HEART_BEAT = 1000U;
  static Timeout timeoutHeartBeat;

  _SystemTime.Start(timeoutHeartBeat);
  if (_SystemTime.Check(timeoutHeartBeat, PERIOD_HEART_BEAT)) {
    HeartBeat();
  }

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
          pinCode[row] = char(digit + 0x30);
          digit = 0;
          state = EState::Checking;
        }
      }
      _Lcd2004.SetCursor(1, row + 6);
      char symbol[2] = {char(digit + 0x30), 0};
      _Lcd2004.String(symbol);
      _Rotation = ERotation::None;
    }
  } else if (state == EState::Checking) {
    integration::crypto::CCryptoIntegration _Crypto(pinCode);
    _Crypto.Decrypt(fileBuffer, buffer, len);

    //_Crypto.KeySetup(sha256);
    //_Crypto.Decrypt(fileBuffer, buffer, len);
    state = EState::FillBase;
  } else if (state == EState::FillBase) {
    static integration::pss::CPasswordStorage pss(buffer, len);
    if (pss.IsValid()) {
      if (ERotation::Clockwise == _Rotation) {
        if (record <= pss.GetNumber() + 1) {
          record++;
        }
      } else if (ERotation::Counterclockwise == _Rotation) {
        if (record > 1U) {
          record--;
        }
      }

      if (_Rotation != ERotation::None || firstFrame) {
        _Rotation = ERotation::None;
        firstFrame = false;
        const auto &frame = pss.GetFrame(record);
        
        _Lcd2004.Clear();
        _Lcd2004.SetCursor(0, 0);
        _Lcd2004.String(frame[0]);
        _Lcd2004.SetCursor(1, 0);
        _Lcd2004.String(frame[1]);
        _Lcd2004.SetCursor(2, 0);
        _Lcd2004.String(frame[2]);

      }
    } else {
      state = EState::Launch;
    }
  }

  usbDevice();
}

volatile CIntegration::ERotation CIntegration::_Rotation;

} // namespace integration

void SysTick_Handler(void) { integration::CIntegration::InterruptSystemTick(); }
void OTG_FS_IRQHandler(void) { integration::CIntegration::InterruptUsb(); }
void EXTI0_IRQHandler(void) { integration::CIntegration::InterruptExti0(); }
void EXTI1_IRQHandler(void) { integration::CIntegration::InterruptExti1(); }
void EXTI2_IRQHandler(void) { integration::CIntegration::InterruptExti2(); }
