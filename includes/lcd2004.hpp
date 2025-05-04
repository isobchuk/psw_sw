#pragma once

#include "hw.hpp"

namespace integration {

// Lcd 2004
class ILcd2004 final {

  static constexpr hardware::mcu::drivers::tim::basic::CTimer _Timer{
      iso::meta_type::const_v<hardware::mcu::drivers::tim::basic::ETimer::Tim7>,
      iso::meta_type::const_v<hardware::_Clock.BusFrequency(hardware::mcu::drivers::clock::EBus::APB1Tim)>};
  static constexpr hardware::mcu::drivers::gpio::CGpioBus<hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::LcdDB4>], 4> _DataBus{};

public:
  consteval ILcd2004() = default;

  inline void Init(void) const { _Timer.Init(); }

  inline consteval auto Enable() const -> decltype(hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::LcdE>]) { return {}; }
  inline consteval auto ReadWrite() const -> decltype(hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::LcdRW>]) { return {}; }
  inline consteval auto InstructionData() const -> decltype(hardware::_PinMap[iso::meta_type::const_v<hardware::EPinFunction::LcdRS>]) { return {}; }

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
} // namespace integration