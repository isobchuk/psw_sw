#pragma once

#include "hal_gpio.hpp"
#include "meta_types.hpp"
#include "reg_gpio.hpp"
#include "reg_rcc.hpp"

namespace stm32f217::drivers::gpio {

enum class Port : cpp_register::RegisterAddress {
  PA = 0x40020000UL,
  PB = 0x40020400UL,
  PC = 0x40020800UL,
  PD = 0X40020C00UL,
  PE = 0x40021000UL,
  PF = 0x40021400UL,
  PG = 0x40021800UL,
  PH = 0x40021C00UL,
  PI = 0x40022000UL
};

enum class Pin : uint32_t { Pin_0, Pin_1, Pin_2, Pin_3, Pin_4, Pin_5, Pin_6, Pin_7, Pin_8, Pin_9, Pin_10, Pin_11, Pin_12, Pin_13, Pin_14, Pin_15 };
enum class Mode : uint32_t { Input, Output, Alternate, Analog };
enum class Type : uint32_t { PushPull, OpenDrain };
enum class Speed : uint32_t { Low, Medium, High, VeryHigh };
enum class Pull : uint32_t { None, Up, Down };
enum class Alternative : uint32_t { AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15 };

template <typename T>
concept gpio_config = requires(T t) {
  { decltype(t._Port){} } -> std::same_as<Port>;
  { decltype(t._Pin){} } -> std::same_as<Pin>;
  { decltype(t._Mode){} } -> std::same_as<Mode>;
  { decltype(t._Type){} } -> std::same_as<Type>;
  { decltype(t._Speed){} } -> std::same_as<Speed>;
  { decltype(t._Pull){} } -> std::same_as<Pull>;
  { decltype(t._AlternativeFunction){} } -> std::same_as<Alternative>;

  { decltype(t._ExtendedParams){} } -> std::same_as<bool>;
};

template <::hal::gpio::pin_function Function> struct PinOut {
  const Function _Function;
  const Port _Port;
  const Pin _Pin;
  const Mode _Mode;
  const Type _Type;
  const Speed _Speed;
  const Pull _Pull;
  const Alternative _AlternativeFunction;

  const bool _ExtendedParams;

  template <typename... AddParams>
  requires iso::meta_type::var_pack::is_types_unique_v<AddParams...> &&
               iso::meta_type::var_pack::is_type_list<Type, Speed, Pull, Alternative>::contains_v<AddParams...>
  consteval PinOut(const Function func, const Port port, const Pin pin, const Mode mode, const AddParams... p_AddParams)
      : _Function(func), _Port(port), _Pin(pin), _Mode(mode), _Type(iso::meta_type::var_pack::type<Type>::get(p_AddParams...)),
        _Speed(iso::meta_type::var_pack::type<Speed>::get(p_AddParams...)), _Pull(iso::meta_type::var_pack::type<Pull>::get(p_AddParams...)),
        _AlternativeFunction(iso::meta_type::var_pack::type<Alternative>::get(p_AddParams...)),
        _ExtendedParams((sizeof...(AddParams) ? true : false)) {}
};

template <const Port port, const Pin pin, const Mode mode> class CGpioPin final {
  static constexpr registers::gpio::GPIO_T<static_cast<cpp_register::RegisterAddress>(port)> *PORT{};
  static constexpr auto PIN = cpp_register::reg_v<1UL, static_cast<uint8_t>(pin)>;
  static constexpr auto _Mode = mode;

  template <const Mode m> static constexpr auto is_mode_v = (m == _Mode);

public:
  using PinType = void;

  static constexpr auto Port = port;
  static constexpr auto Pin = pin;

  consteval CGpioPin() {
    if constexpr (Mode::Output == _Mode) {
      static_assert(::hal::gpio::gpio_out<CGpioPin>, "The class gpio output should implement a whole concept interface!");
    }
    if constexpr (Mode::Input == _Mode) {
      static_assert(::hal::gpio::gpio_in<CGpioPin>, "The class gpio input should implement a whole concept interface!");
    }
    static_assert(::hal::gpio::is_gpio<CGpioPin>, "The class gpio should implement a whole concept interface!");
  }

  template <iso::meta_type::const_value_of_type<bool> State>
  requires is_mode_v<Mode::Output>
  inline void Write(const State) const {
    using namespace stm32f217::registers::gpio;
    if constexpr (State::value) {
      PORT->BSRR = GPIO_BSRR::BS[PIN];
    } else {
      PORT->BSRR = GPIO_BSRR::BR[PIN];
    }
  }

  inline void Change() const
  requires is_mode_v<Mode::Output>
  {
    using namespace stm32f217::registers::gpio;
    PORT->ODR ^= GPIO_ODR::ODR[PIN];
  }

  [[nodiscard("If you ask - you should store the status")]] inline bool Read() const
  requires is_mode_v<Mode::Input>
  {
    using namespace registers::gpio;
    return (PORT->IDR & GPIO_IDR::IDR[PIN]);
  }
};

template <iso::meta_type::array auto &table>
requires gpio_config<decltype(table[0])>
class CGpioPinMap final {
  static constexpr auto &_GpioMap = table;
  using PinFunction = std::remove_const_t<decltype(_GpioMap[0]._Function)>;

  CGpioPinMap(const CGpioPinMap &) = delete;
  CGpioPinMap &operator=(const CGpioPinMap &) = delete;
  CGpioPinMap(CGpioPinMap &&) = delete;
  CGpioPinMap &operator=(CGpioPinMap &&) = delete;

  template <iso::meta_type::const_value_of_type<Port> PortValue, iso::meta_type::const_value_of_type<unsigned> Step>
  static consteval auto GetMode(const PortValue, const Step) {
    using namespace stm32f217::registers::gpio;
    using namespace cpp_register;

    if constexpr ((sizeof(_GpioMap) / sizeof(_GpioMap[0])) == Step::value + 1) {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GPIO_MODER::MODER[constants::ZERO](constants::ZERO);
      } else {
        return GPIO_MODER::MODER[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Mode>);
      }
    } else {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GetMode(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      } else {
        return GPIO_MODER::MODER[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Mode>) |
               GetMode(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      }
    }
  }

  template <iso::meta_type::const_value_of_type<Port> PortValue, iso::meta_type::const_value_of_type<unsigned> Step>
  static consteval auto GetType(const PortValue, const Step) {
    using namespace stm32f217::registers::gpio;
    using namespace cpp_register;

    if constexpr ((sizeof(_GpioMap) / sizeof(_GpioMap[0])) == Step::value + 1) {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GPIO_OTYPER::OT[constants::ZERO](constants::ZERO);
      } else {
        return GPIO_OTYPER::OT[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Type>);
      }
    } else {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GetType(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      } else {
        return GPIO_OTYPER::OT[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Type>) |
               GetType(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      }
    }
  }

  template <iso::meta_type::const_value_of_type<Port> PortValue, iso::meta_type::const_value_of_type<unsigned> Step>
  static consteval auto GetSpeed(const PortValue, const Step) {
    using namespace stm32f217::registers::gpio;
    using namespace cpp_register;

    if constexpr ((sizeof(_GpioMap) / sizeof(_GpioMap[0])) == Step::value + 1) {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GPIO_OSPEEDR::OSPEEDR[constants::ZERO](constants::ZERO);
      } else {
        return GPIO_OSPEEDR::OSPEEDR[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Speed>);
      }
    } else {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GetSpeed(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      } else {
        return GPIO_OSPEEDR::OSPEEDR[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Speed>) |
               GetSpeed(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      }
    }
  }

  template <iso::meta_type::const_value_of_type<Port> PortValue, iso::meta_type::const_value_of_type<unsigned> Step>
  static consteval auto GetPull(const PortValue, const Step) {
    using namespace stm32f217::registers::gpio;
    using namespace cpp_register;

    if constexpr ((sizeof(_GpioMap) / sizeof(_GpioMap[0])) == Step::value + 1) {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GPIO_PUPDR::PUPDR[constants::ZERO](constants::ZERO);
      } else {
        return GPIO_PUPDR::PUPDR[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Pull>);
      }
    } else {
      if constexpr (_GpioMap[Step::value]._Port != PortValue::value) {
        return GetPull(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      } else {
        return GPIO_PUPDR::PUPDR[reg_v<1UL, static_cast<uint8_t>(_GpioMap[Step::value]._Pin)>](reg_v<_GpioMap[Step::value]._Pull>) |
               GetPull(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Step::value + 1>);
      }
    }
  }

  enum class AlFunctRegGrade : unsigned { Junior, Senior };
  template <iso::meta_type::const_value_of_type<Port> PortValue, iso::meta_type::const_value_of_type<AlFunctRegGrade> Grade,
            iso::meta_type::const_value_of_type<unsigned> Step>
  static consteval auto GetAlternative(const PortValue, const Grade, const Step) {
    using namespace stm32f217::registers::gpio;
    using namespace cpp_register;

    if constexpr ((sizeof(_GpioMap) / sizeof(_GpioMap[0])) == Step::value + 1) {
      if constexpr ((_GpioMap[Step::value]._Port != PortValue::value) ||
                    ((AlFunctRegGrade::Junior == Grade::value) && (_GpioMap[Step::value]._Pin) > Pin::Pin_7) ||
                    ((AlFunctRegGrade::Senior == Grade::value) && (_GpioMap[Step::value]._Pin <= Pin::Pin_7))) {
        return GPIO_AFR::AFR[constants::ZERO](constants::ZERO);
      } else {
        constexpr auto pinNum = ((AlFunctRegGrade::Junior == Grade::value) ? static_cast<uint8_t>(_GpioMap[Step::value]._Pin)
                                                                           : static_cast<uint8_t>(_GpioMap[Step::value]._Pin) - 8);
        return GPIO_AFR::AFR[reg_v<1UL, pinNum>](reg_v<_GpioMap[Step::value]._AlternativeFunction>);
      }
    } else {
      if constexpr ((_GpioMap[Step::value]._Port != PortValue::value) ||
                    ((AlFunctRegGrade::Junior == Grade::value) && (_GpioMap[Step::value]._Pin) > Pin::Pin_7) ||
                    ((AlFunctRegGrade::Senior == Grade::value) && (_GpioMap[Step::value]._Pin) <= Pin::Pin_7)) {
        return GetAlternative(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Grade::value>,
                              iso::meta_type::const_v<Step::value + 1>);
      } else {
        constexpr auto pinNum = ((AlFunctRegGrade::Junior == Grade::value) ? static_cast<uint8_t>(_GpioMap[Step::value]._Pin)
                                                                           : static_cast<uint8_t>(_GpioMap[Step::value]._Pin) - 8);
        return GPIO_AFR::AFR[reg_v<1UL, pinNum>](reg_v<_GpioMap[Step::value]._AlternativeFunction>) |
               GetAlternative(iso::meta_type::const_v<PortValue::value>, iso::meta_type::const_v<Grade::value>,
                              iso::meta_type::const_v<Step::value + 1>);
      }
    }
  }

  //------------------------------------------------------------------//
  inline void SetMode(void) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::gpio;

    constexpr auto defaultValue = iso::meta_type::const_v<0U>;

    constexpr auto moderPA = GetMode(iso::meta_type::const_v<Port::PA>, defaultValue);
    if constexpr (moderPA) {
      GPIOA->MODER |= moderPA;
    }
    constexpr auto moderPB = GetMode(iso::meta_type::const_v<Port::PB>, defaultValue);
    if constexpr (moderPB) {
      GPIOB->MODER |= moderPB;
    }
    constexpr auto moderPC = GetMode(iso::meta_type::const_v<Port::PC>, defaultValue);
    if constexpr (moderPC) {
      GPIOC->MODER |= moderPC;
    }
    constexpr auto moderPD = GetMode(iso::meta_type::const_v<Port::PD>, defaultValue);
    if constexpr (moderPD) {
      GPIOD->MODER |= moderPD;
    }
    constexpr auto moderPE = GetMode(iso::meta_type::const_v<Port::PE>, defaultValue);
    if constexpr (moderPE) {
      GPIOE->MODER |= moderPE;
    }
    constexpr auto moderPF = GetMode(iso::meta_type::const_v<Port::PF>, defaultValue);
    if constexpr (moderPF) {
      GPIOF->MODER |= moderPF;
    }
    constexpr auto moderPG = GetMode(iso::meta_type::const_v<Port::PG>, defaultValue);
    if constexpr (moderPG) {
      GPIOG->MODER |= moderPG;
    }
    constexpr auto moderPH = GetMode(iso::meta_type::const_v<Port::PH>, defaultValue);
    if constexpr (moderPH) {
      GPIOH->MODER |= moderPH;
    }
    constexpr auto moderPI = GetMode(iso::meta_type::const_v<Port::PI>, defaultValue);
    if constexpr (moderPI) {
      GPIOI->MODER |= moderPI;
    }
  }

  inline void SetType(void) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::gpio;

    constexpr auto defaultValue = iso::meta_type::const_v<0U>;

    constexpr auto typePA = GetType(iso::meta_type::const_v<Port::PA>, defaultValue);
    if constexpr (typePA) {
      GPIOA->OTYPER |= typePA;
    }
    constexpr auto typePB = GetType(iso::meta_type::const_v<Port::PB>, defaultValue);
    if constexpr (typePB) {
      GPIOB->OTYPER |= typePB;
    }
    constexpr auto typePC = GetType(iso::meta_type::const_v<Port::PC>, defaultValue);
    if constexpr (typePC) {
      GPIOC->OTYPER |= typePC;
    }
    constexpr auto typePD = GetType(iso::meta_type::const_v<Port::PD>, defaultValue);
    if constexpr (typePD) {
      GPIOD->OTYPER |= typePD;
    }
    constexpr auto typePE = GetType(iso::meta_type::const_v<Port::PE>, defaultValue);
    if constexpr (typePE) {
      GPIOE->OTYPER |= typePE;
    }
    constexpr auto typePF = GetType(iso::meta_type::const_v<Port::PF>, defaultValue);
    if constexpr (typePF) {
      GPIOF->OTYPER |= typePF;
    }
    constexpr auto typePG = GetType(iso::meta_type::const_v<Port::PG>, defaultValue);
    if constexpr (typePG) {
      GPIOG->OTYPER |= typePG;
    }
    constexpr auto typePH = GetType(iso::meta_type::const_v<Port::PH>, defaultValue);
    if constexpr (typePH) {
      GPIOH->OTYPER |= typePH;
    }
    constexpr auto typePI = GetType(iso::meta_type::const_v<Port::PI>, defaultValue);
    if constexpr (typePI) {
      GPIOI->OTYPER |= typePI;
    }
  }

  inline void SetSpeed(void) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::gpio;

    constexpr auto defaultValue = iso::meta_type::const_v<0U>;

    constexpr auto speedPA = GetSpeed(iso::meta_type::const_v<Port::PA>, defaultValue);
    if constexpr (speedPA) {
      GPIOA->OSPEEDR |= speedPA;
    }
    constexpr auto speedPB = GetSpeed(iso::meta_type::const_v<Port::PB>, defaultValue);
    if constexpr (speedPB) {
      GPIOB->OSPEEDR |= speedPB;
    }
    constexpr auto speedPC = GetSpeed(iso::meta_type::const_v<Port::PC>, defaultValue);
    if constexpr (speedPC) {
      GPIOC->OSPEEDR |= speedPC;
    }
    constexpr auto speedPD = GetSpeed(iso::meta_type::const_v<Port::PD>, defaultValue);
    if constexpr (speedPD) {
      GPIOD->OSPEEDR |= speedPD;
    }
    constexpr auto speedPE = GetSpeed(iso::meta_type::const_v<Port::PE>, defaultValue);
    if constexpr (speedPE) {
      GPIOE->OSPEEDR |= speedPE;
    }
    constexpr auto speedPF = GetSpeed(iso::meta_type::const_v<Port::PF>, defaultValue);
    if constexpr (speedPF) {
      GPIOF->OSPEEDR |= speedPF;
    }
    constexpr auto speedPG = GetSpeed(iso::meta_type::const_v<Port::PG>, defaultValue);
    if constexpr (speedPG) {
      GPIOG->OSPEEDR |= speedPG;
    }
    constexpr auto speedPH = GetSpeed(iso::meta_type::const_v<Port::PH>, defaultValue);
    if constexpr (speedPH) {
      GPIOH->OSPEEDR |= speedPH;
    }
    constexpr auto speedPI = GetSpeed(iso::meta_type::const_v<Port::PI>, defaultValue);
    if constexpr (speedPI) {
      GPIOI->OSPEEDR |= speedPI;
    }
  }

  inline void SetPull(void) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::gpio;

    constexpr auto defaultValue = iso::meta_type::const_v<0U>;

    constexpr auto pullPA = GetPull(iso::meta_type::const_v<Port::PA>, defaultValue);
    if constexpr (pullPA) {
      GPIOA->PUPDR |= pullPA;
    }
    constexpr auto pullPB = GetPull(iso::meta_type::const_v<Port::PB>, defaultValue);
    if constexpr (pullPB) {
      GPIOB->PUPDR |= pullPB;
    }
    constexpr auto pullPC = GetPull(iso::meta_type::const_v<Port::PC>, defaultValue);
    if constexpr (pullPC) {
      GPIOC->PUPDR |= pullPC;
    }
    constexpr auto pullPD = GetPull(iso::meta_type::const_v<Port::PD>, defaultValue);
    if constexpr (pullPD) {
      GPIOD->PUPDR |= pullPD;
    }
    constexpr auto pullPE = GetPull(iso::meta_type::const_v<Port::PE>, defaultValue);
    if constexpr (pullPE) {
      GPIOE->PUPDR |= pullPE;
    }
    constexpr auto pullPF = GetPull(iso::meta_type::const_v<Port::PF>, defaultValue);
    if constexpr (pullPF) {
      GPIOF->PUPDR |= pullPF;
    }
    constexpr auto pullPG = GetPull(iso::meta_type::const_v<Port::PG>, defaultValue);
    if constexpr (pullPG) {
      GPIOG->PUPDR |= pullPG;
    }
    constexpr auto pullPH = GetPull(iso::meta_type::const_v<Port::PH>, defaultValue);
    if constexpr (pullPH) {
      GPIOH->PUPDR |= pullPH;
    }
    constexpr auto pullPI = GetPull(iso::meta_type::const_v<Port::PI>, defaultValue);
    if constexpr (pullPI) {
      GPIOI->PUPDR |= pullPI;
    }
  }

  inline void SetAlternativeFunction(void) const {
    using namespace cpp_register;
    using namespace stm32f217::registers::gpio;

    constexpr auto defaultValue = iso::meta_type::const_v<0U>;

    constexpr auto afJrPA = GetAlternative(iso::meta_type::const_v<Port::PA>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPA) {
      GPIOA->AFR[reg_v<0UL>] |= afJrPA;
    }
    constexpr auto afJrPB = GetAlternative(iso::meta_type::const_v<Port::PB>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPB) {
      GPIOB->AFR[reg_v<0UL>] |= afJrPB;
    }
    constexpr auto afJrPC = GetAlternative(iso::meta_type::const_v<Port::PC>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPC) {
      GPIOC->AFR[reg_v<0UL>] |= afJrPC;
    }
    constexpr auto afJrPD = GetAlternative(iso::meta_type::const_v<Port::PD>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPD) {
      GPIOD->AFR[reg_v<0UL>] |= afJrPD;
    }
    constexpr auto afJrPE = GetAlternative(iso::meta_type::const_v<Port::PE>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPE) {
      GPIOE->AFR[reg_v<0UL>] |= afJrPE;
    }
    constexpr auto afJrPF = GetAlternative(iso::meta_type::const_v<Port::PF>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPF) {
      GPIOF->AFR[reg_v<0UL>] |= afJrPF;
    }
    constexpr auto afJrPG = GetAlternative(iso::meta_type::const_v<Port::PG>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPG) {
      GPIOG->AFR[reg_v<0UL>] |= afJrPG;
    }
    constexpr auto afJrPH = GetAlternative(iso::meta_type::const_v<Port::PH>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPH) {
      GPIOH->AFR[reg_v<0UL>] |= afJrPH;
    }
    constexpr auto afJrPI = GetAlternative(iso::meta_type::const_v<Port::PI>, iso::meta_type::const_v<AlFunctRegGrade::Junior>, defaultValue);
    if constexpr (afJrPI) {
      GPIOI->AFR[reg_v<0UL>] |= afJrPI;
    }

    constexpr auto afSrPA = GetAlternative(iso::meta_type::const_v<Port::PA>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPA) {
      GPIOA->AFR[reg_v<1UL>] |= afSrPA;
    }
    constexpr auto afSrPB = GetAlternative(iso::meta_type::const_v<Port::PB>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPB) {
      GPIOB->AFR[reg_v<1UL>] |= afSrPB;
    }
    constexpr auto afSrPC = GetAlternative(iso::meta_type::const_v<Port::PC>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPC) {
      GPIOC->AFR[reg_v<1UL>] |= afSrPC;
    }
    constexpr auto afSrPD = GetAlternative(iso::meta_type::const_v<Port::PD>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPD) {
      GPIOD->AFR[reg_v<1UL>] |= afSrPD;
    }
    constexpr auto afSrPE = GetAlternative(iso::meta_type::const_v<Port::PE>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPE) {
      GPIOE->AFR[reg_v<1UL>] |= afSrPE;
    }
    constexpr auto afSrPF = GetAlternative(iso::meta_type::const_v<Port::PF>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPF) {
      GPIOF->AFR[reg_v<1UL>] |= afSrPF;
    }
    constexpr auto afSrPG = GetAlternative(iso::meta_type::const_v<Port::PG>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPG) {
      GPIOG->AFR[reg_v<1UL>] |= afSrPG;
    }
    constexpr auto afSrPH = GetAlternative(iso::meta_type::const_v<Port::PH>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPH) {
      GPIOH->AFR[reg_v<1UL>] |= afSrPH;
    }
    constexpr auto afSrPI = GetAlternative(iso::meta_type::const_v<Port::PI>, iso::meta_type::const_v<AlFunctRegGrade::Senior>, defaultValue);
    if constexpr (afSrPI) {
      GPIOI->AFR[reg_v<1UL>] |= afSrPI;
    }
  }

public:
  consteval CGpioPinMap() {
    static_assert(::hal::gpio::gpio_controller<CGpioPinMap>, "The class gpio controller should implement a whole concept interface!");

    constexpr auto uniqueCheck = []() -> bool {
      constexpr auto _MapSize = sizeof(_GpioMap) / sizeof(_GpioMap[0]);
      unsigned i, j;
      for (i = 0; i < _MapSize; i++) {
        const Port portI = _GpioMap[i]._Port;
        const Pin pinI = _GpioMap[i]._Pin;
        for (j = i + 1; j < _MapSize; j++) {
          const Port portJ = _GpioMap[j]._Port;
          const Pin pinJ = _GpioMap[j]._Pin;
          if ((portI == portJ) && (pinI == pinJ)) {
            return false;
          }
        }
      }
      return true;
    }();

    static_assert(uniqueCheck, "Driver GPIO: No unique Port/Pin configuration!");
  }

  inline void Init() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::gpio;

    constexpr auto _IsPortInside = [](const Port port) -> uint32_t {
      uint32_t isIn = 0UL;
      for (const auto &el : _GpioMap) {
        if (port == el._Port) {
          isIn = 1UL;
          break;
        }
      }
      return isIn;
    };

    RCC->AHB1ENR |= RCC_AHB1ENR::GPIOAEN(reg_v<_IsPortInside(Port::PA)>) | RCC_AHB1ENR::GPIOBEN(reg_v<_IsPortInside(Port::PB)>) |
                    RCC_AHB1ENR::GPIOCEN(reg_v<_IsPortInside(Port::PC)>) | RCC_AHB1ENR::GPIODEN(reg_v<_IsPortInside(Port::PD)>) |
                    RCC_AHB1ENR::GPIOFEN(reg_v<_IsPortInside(Port::PF)>) | RCC_AHB1ENR::GPIOEEN(reg_v<_IsPortInside(Port::PE)>) |
                    RCC_AHB1ENR::GPIOGEN(reg_v<_IsPortInside(Port::PG)>) | RCC_AHB1ENR::GPIOHEN(reg_v<_IsPortInside(Port::PH)>) |
                    RCC_AHB1ENR::GPIOIEN(reg_v<_IsPortInside(Port::PI)>);

    SetMode();
    SetType();
    SetSpeed();
    SetPull();
    SetAlternativeFunction();
  }

  template <iso::meta_type::const_value_of_type<PinFunction> Function> [[nodiscard]] inline consteval auto operator[](const Function) const {
    constexpr auto _MapSize = sizeof(_GpioMap) / sizeof(_GpioMap[0]);
    constexpr auto index = [_MapSize]() {
      unsigned i = 0;
      while ((Function::value != _GpioMap[i]._Function) && (_MapSize > i)) {
        i++;
      }
      return i;
    }();
    static_assert((_MapSize >= index), "Gpio Controller: No function pin inside!");
    return CGpioPin<_GpioMap[index]._Port, _GpioMap[index]._Pin, _GpioMap[index]._Mode>{};
  }
};

template <const ::hal::gpio::gpio_out auto firstPin, const unsigned size> class CGpioBus final {

  static constexpr registers::gpio::GPIO_T<static_cast<cpp_register::RegisterAddress>(firstPin.Port)> *PORT{};
  static constexpr unsigned _PinOffset = static_cast<unsigned>(firstPin.Pin);

  static constexpr auto mask = []() {
    uint32_t busMask = 0;
    uint32_t busSize = size;
    while (busSize--) {
      busMask |= (1UL << (size - busSize));
    }
    return busMask >> 1;
  }();

  template <iso::meta_type::const_value_of_type<decltype(size)> Quantity, iso::meta_type::const_value_of_type<uint32_t> Value>
  static consteval auto Moder(const Quantity, const Value) {
    using namespace cpp_register;
    using namespace stm32f217::registers::gpio;
    if constexpr (0U != Quantity::value) {
      return GPIO_MODER::MODER[reg_v<1UL, (Quantity::value + _PinOffset - 1)>](reg_v<Value::value>) |
             Moder(iso::meta_type::const_v<Quantity::value - 1>, iso::meta_type::const_v<Value::value>);
    } else {
      return GPIO_MODER::MODER[constants::ZERO](constants::ZERO);
    }
  }

  template <iso::meta_type::const_value_of_type<decltype(size)> Quantity, iso::meta_type::const_value_of_type<uint32_t> Value>
  static consteval auto Pupdr(const Quantity, const Value) {
    using namespace cpp_register;
    using namespace stm32f217::registers::gpio;
    if constexpr (0U != Quantity::value) {
      return GPIO_PUPDR::PUPDR[reg_v<1UL, (Quantity::value + _PinOffset - 1)>](reg_v<Value::value>) |
             Pupdr(iso::meta_type::const_v<Quantity::value - 1>, iso::meta_type::const_v<Value::value>);
    } else {
      return GPIO_PUPDR::PUPDR[constants::ZERO](constants::ZERO);
    }
  }

public:
  consteval CGpioBus() = default;

  template <iso::meta_type::const_value_of_type<unsigned> Value> inline void Write(const Value) const {
    using namespace stm32f217::registers::gpio;
    PORT->BSRR = mask << (_PinOffset + 16);
    PORT->BSRR = Value::value << _PinOffset;
  }

  inline void Write(const uint8_t value) const {
    using namespace stm32f217::registers::gpio;
    PORT->BSRR = mask << (_PinOffset + 16);
    PORT->BSRR = static_cast<uint32_t>(value << _PinOffset);
  }

  inline void ToInput() const {
    using namespace iso::meta_type;
    PORT->MODER &= Moder(const_v<size>, const_v<0b11UL>);
    // PORT->PUPDR |= Pupdr(const_v<size>, const_v<0b10UL>);
  }

  inline void ToOutput() const {
    using namespace iso::meta_type;
    // PORT->PUPDR &= Pupdr(const_v<size>, const_v<0b11UL>);
    PORT->MODER |= Moder(const_v<size>, const_v<0b01UL>);
  }

  inline unsigned Read() const {
    using namespace stm32f217::registers::gpio;
    return (*(PORT->IDR) >> _PinOffset) & mask;
  }
};

} // namespace stm32f217::drivers::gpio
