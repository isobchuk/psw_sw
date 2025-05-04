#include "integration.hpp"
#include "cryptoIntegration.hpp"
#include "passwordStorage.hpp"

namespace integration {

//#define ISO_DEBUG

CIntegration::CIntegration() : fileBuffer{}, buffer{}, len(), fileSystem() {
  using namespace iso::meta_type;

  hardware::Initialize();

  _TerminalOutput.Init();
  _Encoder.Init();

  const bool result = _Flash.Connect();
  if (result) {
    _Flash.GetStatus();
    _Flash.WriteEnable();
    _Flash.GetStatus();
  }

  usbDevice.Init();

  _Lcd2004.Init();

  if (integration::fatfs::EError::NoError == fileSystem.Init()) {
    const auto bytes = fileSystem.Read(fileBuffer, iso::meta_type::const_v<1024U>);
    if (std::size_t(-1) == bytes) {
      // Error handling
    }
    len = bytes + (bytes % 16);
  }

  // record = 1U;
  // firstFrame = true;

  /* if (fatfs::EError::NoError == fileSystem.Init()) {
     debug.message(iso::format::string<PROJECT_NAME>);
     debug.message(iso::format::string<CONFIGURATION>);
     debug.message(iso::format::string<GIT_BRANCH>);
     debug.message(iso::format::string<GIT_COMMIT>);
   }*/
}

void CIntegration::operator()() {
  using namespace encoder;

  HeartBeat();
  Application();
  Usb();
}

void CIntegration::HeartBeat() const {
  constexpr auto PERIOD_HEART_BEAT = 1000U;
  static hardware::Timeout timeoutHeartBeat;

  hardware::_SystemTime.Start(timeoutHeartBeat);
  if (hardware::_SystemTime.Check(timeoutHeartBeat, PERIOD_HEART_BEAT)) {
    _Led.Change();
  }
}

void CIntegration::Application() {
  using namespace encoder;

  enum class EState { Launch, LogIn, Checking, FillBase, ShowRecords };

#ifdef ISO_DEBUG
  static EState state = EState::Checking;
  static char pinCode[8] = {'1', '9', '4', '0', '1', '8', '0', '2'};
#else
  static EState state = EState::Launch;
  static char pinCode[8] = {};
#endif
  static uint16_t record = 1;
  static bool firstFrame = true;

  if (state == EState::Launch) {
    _Lcd2004.Clear();
    _Lcd2004.SetCursor(0, 6);
    _Lcd2004.String("PIN CODE");
    _Lcd2004.SetCursor(1, 6);
    _Lcd2004.String("0");
    state = EState::LogIn;
  } else if (state == EState::LogIn) {
    static uint8_t digit;
    static uint8_t row;
    if (ERotation::None != _Encoder.GetRotation()) {
      if (ERotation::Clockwise == _Encoder.GetRotation()) {
        if (digit < 9) {
          digit++;
        } else {
          digit = 0;
        }
      } else if (ERotation::Counterclockwise == _Encoder.GetRotation()) {
        if (digit > 0) {
          digit--;
        } else {
          digit = 9;
        }
      } else if (ERotation::Enter == _Encoder.GetRotation()) {
        if (row < (sizeof(pinCode) - 1)) {
          pinCode[row++] = char(digit + 0x30);
          digit = 0;
        } else {
          pinCode[row] = char(digit + 0x30);
          digit = 0;
          row = 0;
          state = EState::Checking;
        }
      }
      _Lcd2004.SetCursor(1, row + 6);
      char symbol[2] = {char(digit + 0x30), 0};
      _Lcd2004.String(symbol);
      _Encoder.ClearRotation();
    }
  } else if (state == EState::Checking) {
    integration::crypto::CCryptoIntegration _Crypto(pinCode);
    _Crypto.Decrypt(fileBuffer, buffer, len);
    firstFrame = true;
    record = 1U;
    state = EState::FillBase;
  } else if (state == EState::FillBase) {
    integration::pss::CPasswordStorage pss(buffer, len);    
    if (pss.IsValid()) {
      const auto number = pss.GetNumber();
      if (ERotation::Clockwise == _Encoder.GetRotation()) {
        if (record < number) {
          record++;
        } else {
          record = 1U;
        }
      } else if (ERotation::Counterclockwise == _Encoder.GetRotation()) {
        if (record > 1U) {
          record--;
        } else {
          record = uint16_t(number);
        }
      }

      if (_Encoder.GetRotation() != ERotation::None || firstFrame) {
        _Encoder.ClearRotation();
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

      
      constexpr auto TIMEOUT = 1000U;
      static hardware::Timeout timeout;

      hardware::_SystemTime.Start(timeout);
      if (hardware::_SystemTime.Check(timeout, TIMEOUT)) {
        const auto &frame = pss.GetFrame(record, true);

        _Lcd2004.Clear();
        _Lcd2004.SetCursor(0, 0);
        _Lcd2004.String(frame[0]);
        _Lcd2004.SetCursor(1, 0);
        _Lcd2004.String(frame[1]);
        _Lcd2004.SetCursor(2, 0);
        _Lcd2004.String(frame[2]);
      }
    } else {
      static bool wait;
      if (!wait) {
        _Lcd2004.Clear();
        _Lcd2004.String("   INCORRECT PIN   ");
      }

      constexpr auto TIMEOUT = 3000U;
      static hardware::Timeout timeout;

      hardware::_SystemTime.Start(timeout);
      if (hardware::_SystemTime.Check(timeout, TIMEOUT)) {
        wait = false;
        state = EState::Launch;
      } else {
        wait = true;
      }
    }
  }
}

void CIntegration::Usb() const { usbDevice(); }

} // namespace integration

void SysTick_Handler(void) { integration::CIntegration::InterruptSystemTick(); }
void OTG_FS_IRQHandler(void) { integration::CIntegration::InterruptUsb(); }
void EXTI0_IRQHandler(void) { integration::CIntegration::InterruptExti0(); }
void EXTI1_IRQHandler(void) { integration::CIntegration::InterruptExti1(); }
void EXTI2_IRQHandler(void) { integration::CIntegration::InterruptExti2(); }
