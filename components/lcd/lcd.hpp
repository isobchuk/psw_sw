#pragma once

#include "hal_gpio.hpp"
#include <concepts>
#include <cstdint>

namespace iso::lcd {

template <typename T>
concept lcd_interface = requires(const T interface) {
  { T() };
  { interface.Init() } -> std::same_as<void>;
  { interface.Enable() } -> hal::gpio::gpio_out;          // E:   Enable signal
  { interface.ReadWrite() } -> hal::gpio::gpio_out;       // RW:  0 - Register Write, 1 - Register Read
  { interface.InstructionData() } -> hal::gpio::gpio_out; // RS:  0 - Instruction, 1 - Data

  { interface.Write(uint8_t{}) } -> std::same_as<void>;
  { interface.Write(iso::meta_type::const_v<uint8_t{}>) } -> std::same_as<void>;
  { interface.Read() } -> std::same_as<unsigned>;
  { interface.ToInput() } -> std::same_as<void>;
  { interface.ToOutput() } -> std::same_as<void>;

  { interface.Delay(iso::meta_type::const_v<(unsigned long){}>) } -> std::same_as<void>;
};

// For delay function
consteval long unsigned operator""_uS(long long unsigned time) { return static_cast<unsigned long>(time); }
consteval long unsigned operator""_mS(long long unsigned time) { return static_cast<unsigned long>(1000UL * time); }

enum class EMode { Bit4, Bit8 };

template <lcd_interface Interface, const EMode mode = EMode::Bit4> class CLcd {
  static constexpr Interface _Interface{};
  static constexpr auto _Is4BitMode = (EMode::Bit4 == mode);

  inline void Enable() const {
    using namespace iso::meta_type;
    _Interface.Enable().Write(const_v<true>);
    _Interface.Delay(const_v<2_uS>);
    _Interface.Enable().Write(const_v<false>);
  }

  template <iso::meta_type::const_value_of_type<uint8_t> Command> inline void InitCommand(const Command) const {
    using namespace iso::meta_type;
    _Interface.Write(const_v<Command::value>);
    Enable();
    _Interface.Delay(const_v<100_uS>);
  }

  bool IsBusy() const {
    using namespace iso::meta_type;

    // Read Instruction
    _Interface.ReadWrite().Write(const_v<true>);
    _Interface.InstructionData().Write(const_v<false>);
    _Interface.ToInput();

    bool isBusy = true;
    constexpr auto flag = (!_Is4BitMode) ? uint8_t(0x80) : uint8_t(0x8);

    _Interface.Enable().Write(const_v<true>);
    _Interface.Delay(const_v<2_uS>);
    isBusy = static_cast<bool>(_Interface.Read() & flag);
    _Interface.Enable().Write(const_v<false>);
    _Interface.Delay(const_v<2_uS>);

    if constexpr (_Is4BitMode) {
      _Interface.Enable().Write(const_v<true>);
      _Interface.Delay(const_v<2_uS>);
      _Interface.Enable().Write(const_v<false>);
      _Interface.Delay(const_v<2_uS>);
    }

    _Interface.ToOutput();
    return isBusy;
  }

public:
  consteval CLcd() = default;

  inline void Init() const {
    using namespace iso::meta_type;

    _Interface.Init();

    // Write instructions (can check busy flag only after initialization)
    _Interface.ReadWrite().Write(const_v<false>);
    _Interface.InstructionData().Write(const_v<false>);

    _Interface.Delay(const_v<40_mS>);

    InitCommand(const_v<uint8_t(0b0011U)>);
    _Interface.Delay(const_v<5_mS>);
    InitCommand(const_v<uint8_t(0b0011U)>);
    _Interface.Delay(const_v<100_uS>);
    InitCommand(const_v<uint8_t(0b0011U)>);
    _Interface.Delay(const_v<100_uS>);

    InitCommand(const_v<uint8_t(0b0010U)>);
    InitCommand(const_v<uint8_t(0b0010U)>);
    InitCommand(const_v<uint8_t(0b1100U)>);
    _Interface.Delay(const_v<50_uS>);

    InitCommand(const_v<uint8_t(0b0000U)>);
    InitCommand(const_v<uint8_t(0b1100U)>);
    _Interface.Delay(const_v<50_uS>);

    InitCommand(const_v<uint8_t(0b0000U)>);
    InitCommand(const_v<uint8_t(0b0001U)>);
    _Interface.Delay(const_v<2_mS>);

    InitCommand(const_v<uint8_t(0b0000U)>);
    InitCommand(const_v<uint8_t(0b0110U)>);
    _Interface.Delay(const_v<2_mS>);
  }

  template <iso::meta_type::const_value_of_type<bool> Data = iso::meta_type::const_t<true>>
  void Write(const uint8_t value, const Data = iso::meta_type::const_v<true>) const {
    using namespace iso::meta_type;

    while (IsBusy()) {
    }

    _Interface.ReadWrite().Write(const_v<false>);
    _Interface.InstructionData().Write(const_v<Data::value>);

    const uint8_t realValue = (!_Is4BitMode) ? value : ((value & 0xF0) >> 4);
    _Interface.Write(realValue);
    Enable();

    if constexpr (_Is4BitMode) {
      _Interface.Write((value & 0x0F));
      Enable();
    }
  }
  template <iso::meta_type::const_value_of_type<bool> State>
  inline void Cursor(const State) const {
    using namespace iso::meta_type;
    Write(uint8_t(0b00001100U | (State::value ? 0b11U : 0b00U)), const_v<false>);
  }

  inline void Address(const uint8_t address) const {
    using namespace iso::meta_type;
    Write(uint8_t(0x80U | address), const_v<false>);
  }

  inline void Clear() const {
    using namespace iso::meta_type;
    Write(uint8_t(0x1U), const_v<false>);
  }

  template <const unsigned N> inline void String(const char (&str)[N]) const {
    unsigned i = 0;
    while ('\0' != str[i] && i < 20) {
      Write(str[i++]);
    }
  }

  inline void SetCursor(const uint8_t line, const uint8_t row) const {
    uint8_t address = [&line, &row]() {
      uint8_t addr = 0;
      if (0 == line) {
        addr = 0 + row;
      } else if (1 == line) {
        addr = 0x40 + row;
      } else if (2 == line) {
        addr = 20 + row;
      } else if (3 == line) {
        addr = 0x40 + 20 + row;
      }
      return addr;
    }();

    Address(address);
  }
};

} // namespace iso::lcd