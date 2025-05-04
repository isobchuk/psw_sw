#pragma once

#include "hw.hpp"
#include "meta_types.hpp"

namespace integration::encoder {

enum class ERotation : uint32_t { None, Clockwise, Counterclockwise, Enter };

class CEncoder final {
  struct SCounter final {
    uint32_t clockwise;
    uint32_t counterclockwise;

    SCounter() : clockwise(), counterclockwise() {}
  };

  static constexpr hardware::mcu::drivers::exti::CExtInterrupt<hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::EncA>],
                                                               hardware::mcu::drivers::exti::ETrigger::Falling>
      _EncoderA{};
  static constexpr hardware::mcu::drivers::exti::CExtInterrupt<hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::EncB>],
                                                               hardware::mcu::drivers::exti::ETrigger::Falling>
      _EncoderB{};
  static constexpr hardware::mcu::drivers::exti::CExtInterrupt<hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::EncButton>],
                                                               hardware::mcu::drivers::exti::ETrigger::Falling>
      _EncoderButton{};

  static volatile ERotation _Rotation;

  static constexpr auto _STEP = 1UL;
  static volatile SCounter _Counter;

public:
  // Real variables

  consteval CEncoder() = default;

  inline void Init() const {
    using namespace iso::meta_type;

    _EncoderA.Init();
    _EncoderB.Init();
    _EncoderButton.Init();
    hardware::_InterruptController[const_v<hardware::mcu::drivers::interrupt::Number::EXTI0>].Enable();
    hardware::_InterruptController[const_v<hardware::mcu::drivers::interrupt::Number::EXTI1>].Enable();
    hardware::_InterruptController[const_v<hardware::mcu::drivers::interrupt::Number::EXTI2>].Enable();
  }

  inline static ERotation GetRotation() { return _Rotation; }
  inline static void ClearRotation() { _Rotation = ERotation::None; }

  inline static void InterruptExti0() {
    _EncoderA.InterruptHandler();
    if (hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::EncB>].Read()) {
      if constexpr (_STEP > 1) {
        _Counter.clockwise = _Counter.clockwise + 1;
        if (_Counter.clockwise >= _STEP) {
          _Rotation = ERotation::Clockwise;
          _Counter.counterclockwise = 0UL;
          _Counter.clockwise = 0UL;
        }
      } else {
        _Rotation = ERotation::Clockwise;
      }
    }
  }
  inline static void InterruptExti1() {
    _EncoderB.InterruptHandler();
    if (hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::EncA>].Read()) {
      if constexpr (_STEP > 1) {
        _Counter.counterclockwise = _Counter.counterclockwise + 1;
        if (_Counter.counterclockwise >= _STEP) {
          _Rotation = ERotation::Counterclockwise;
          _Counter.clockwise = 0UL;
          _Counter.counterclockwise = 0UL;
        }
      } else {
        _Rotation = ERotation::Counterclockwise;
      }
    }
  }

  inline static void InterruptExti2() {
    _EncoderButton.InterruptHandler();
    if constexpr (_STEP > 1) {
      _Counter.clockwise = 0UL;
      _Counter.counterclockwise = 0UL;
    }
    _Rotation = ERotation::Enter;
  }
};

} // namespace integration::encoder