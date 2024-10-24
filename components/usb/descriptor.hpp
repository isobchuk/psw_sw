#pragma once

#include "endpoint.hpp"
#include "type.hpp"

namespace iso::usb::descriptor {

template <typename D>
concept descriptor = requires(D d) {
  d._Descriptor;
  { D::size() } -> std::same_as<unsigned>;
};

enum class EDescriptorRepresentation { Descriptor, Buffer };

class CDescriptorDummyBuffer {

public:
  const uint8_t _Descriptor[sizeof(std::size_t)];
  consteval CDescriptorDummyBuffer() : _Descriptor() {}
  static consteval unsigned size() { return sizeof(_Descriptor); }
};

template <const EDescriptorRepresentation representation> class CDescriptorDevice {
  consteval CDescriptorDevice() = default;
};

template <const EDescriptorRepresentation representation> class CDescriptorEndpoint {
  consteval CDescriptorEndpoint() = default;
};

template <const EDescriptorRepresentation representation> class CDescriptorInterface {
  consteval CDescriptorInterface() = default;
};

template <const EDescriptorRepresentation representation, descriptor... TDescriptor> class CDescriptorConfiguration {
  consteval CDescriptorConfiguration() = default;
};

template <const EDescriptorRepresentation representation, const unsigned N> class CDescriptorString {
  consteval CDescriptorString() = default;
};

template <const EDescriptorRepresentation representation> class CDescriptorCustomHid {
  consteval CDescriptorCustomHid() = default;
};

template <> class CDescriptorDevice<EDescriptorRepresentation::Buffer> {

  static constexpr auto _DESCRIPTOR_STANDARD_SIZE = 18U;
  static constexpr auto _Type = EDescriptorType::Device;

public:
  const uint8_t _Descriptor[_DESCRIPTOR_STANDARD_SIZE];
  consteval CDescriptorDevice(const EBcdUsb usbVersion, const Type::Word vendorId, const Type::Word productId, const Type::Byte numConfigs = 1)
      : _Descriptor{sizeof(_Descriptor),
                    static_cast<std::underlying_type_t<EDescriptorType>>(EDescriptorType::Device),
                    static_cast<Type::Byte>(
                        architecture::cast(static_cast<Type::Word>(static_cast<std::underlying_type_t<decltype(usbVersion)>>(usbVersion) & 0xFFU))),
                    static_cast<Type::Byte>(
                        architecture::cast(static_cast<Type::Word>(static_cast<std::underlying_type_t<decltype(usbVersion)>>(usbVersion) >> 8))),
                    0x00, // static_cast<std::underlying_type_t<decltype(deviceClass)>>(deviceClass),
                    0x00,
                    0x00,
                    64,
                    static_cast<Type::Byte>(architecture::cast(static_cast<Type::Word>(vendorId >> 8))),
                    static_cast<Type::Byte>(architecture::cast(static_cast<Type::Word>(vendorId & 0xFFU))),
                    static_cast<Type::Byte>(architecture::cast(static_cast<Type::Word>(productId >> 8))),
                    static_cast<Type::Byte>(architecture::cast(static_cast<Type::Word>(productId & 0xFFU))),
                    0x00,
                    0x01,
                    0x01,
                    0x02,
                    0x03,
                    numConfigs} {}

  static consteval unsigned size() { return sizeof(_Descriptor); }
};

/*
template <> class CDescriptorDevice<EDescriptorRepresentation::Descriptor> {
  static constexpr auto _DESCRIPTOR_DEVICE_SIZE = 18U;

  const uint8_t _Descriptor[_DESCRIPTOR_DEVICE_SIZE];

public:
  consteval CDescriptorDevice(const uint8_t (&buf)[_DESCRIPTOR_DEVICE_SIZE]) : _Descriptor() {}

  auto operator()() const -> const uint8_t (&)[sizeof(_Descriptor)] { return _Descriptor; }
  static consteval unsigned size() { return sizeof(_Descriptor); }
};*/

template <> class CDescriptorEndpoint<EDescriptorRepresentation::Buffer> {
  static constexpr auto _DESCRIPTOR_ENDPOINT_SIZE = 7U;
  static constexpr auto _Type = EDescriptorType::Endpoint;

public:
  const uint8_t _Descriptor[_DESCRIPTOR_ENDPOINT_SIZE];
  consteval CDescriptorEndpoint(const endpoint::EType type, const endpoint::EDirection dir, const Type::Byte addr, const Type::Byte pollInterval = 10)
      : _Descriptor{sizeof(_Descriptor),
                    static_cast<std::underlying_type_t<EDescriptorType>>(EDescriptorType::Endpoint),
                    static_cast<Type::Byte>(addr | static_cast<std::underlying_type_t<endpoint::EDirection>>(dir)),
                    static_cast<std::underlying_type_t<endpoint::EType>>(type),
                    architecture::cast(Type::Word{64U}) & 0xFF,
                    architecture::cast(Type::Word{64U}) >> 8,
                    pollInterval} {}

  static consteval unsigned size() { return sizeof(_Descriptor); }
};

template <> class CDescriptorInterface<EDescriptorRepresentation::Buffer> {
  static constexpr auto _DESCRIPTOR_INTERFACE_SIZE = 9U;
  static constexpr auto _Type = EDescriptorType::Interface;

public:
  const uint8_t _Descriptor[_DESCRIPTOR_INTERFACE_SIZE];
  consteval CDescriptorInterface(const Type::Byte numInterface, const Type::Byte numEndpoints, const EDeviceClass deviceClass,
                                 const Type::Byte deviceSubClass, const Type::Byte protocol = 2)
      : _Descriptor{sizeof(_Descriptor),
                    static_cast<std::underlying_type_t<EDescriptorType>>(EDescriptorType::Interface),
                    numInterface,
                    0x00,
                    numEndpoints,
                    static_cast<std::underlying_type_t<EDeviceClass>>(deviceClass),
                    deviceSubClass,
                    protocol,
                    0} {}

  static consteval unsigned size() { return sizeof(_Descriptor); }
};

template <descriptor... TDescriptor> class CDescriptorConfiguration<EDescriptorRepresentation::Buffer, TDescriptor...> {
  static constexpr auto _DESCRIPTOR_CONFIGURATION_SIZE = 9U;

  static constexpr uint8_t _TOTAL_LENGTH = (TDescriptor::size() + ...);

  template <descriptor First> consteval void Unpack(unsigned position, const First &descFirst) {
    for (const auto &byte : descFirst._Descriptor) {
      _Descriptor[position++] = byte;
    }
  }

  template <descriptor First, descriptor... Rest> consteval void Unpack(unsigned position, const First &descFirst, const Rest &...descRest) {
    for (const auto &byte : descFirst._Descriptor) {
      _Descriptor[position++] = byte;
    }
    Unpack(position, descRest...);
  }

public:
  uint8_t _Descriptor[_DESCRIPTOR_CONFIGURATION_SIZE + _TOTAL_LENGTH];
  consteval CDescriptorConfiguration(const Type::Byte numInterfaces, const Type::Byte index, const TDescriptor &...desc) {
    _Descriptor[0] = _DESCRIPTOR_CONFIGURATION_SIZE;
    _Descriptor[1] = static_cast<std::underlying_type_t<EDescriptorType>>(EDescriptorType::Configuration);
    _Descriptor[2] = architecture::cast(Type::Word{sizeof(_Descriptor)}) & 0xFF;
    _Descriptor[3] = architecture::cast(Type::Word{sizeof(_Descriptor)}) >> 8;
    _Descriptor[4] = numInterfaces;
    _Descriptor[5] = index;
    _Descriptor[6] = 0x00U;
    _Descriptor[7] = 0xA0U;
    _Descriptor[8] = 0x32U;

    Unpack(_DESCRIPTOR_CONFIGURATION_SIZE, desc...);
  }
  consteval unsigned size() const { return sizeof(_Descriptor); }
};

template <const EDescriptorRepresentation representation, descriptor... TDescriptor>
CDescriptorConfiguration(const Type::Byte numInterfaces, const Type::Byte index, const TDescriptor &...descr)
    -> CDescriptorConfiguration<representation, TDescriptor...>;

template <const unsigned N> class CDescriptorString<EDescriptorRepresentation::Buffer, N> {
  static constexpr auto _DESCRIPTOR_STRING_SIZE = 2 * (N - 1) + 2;
  static constexpr auto _Type = EDescriptorType::String;

public:
  uint8_t _Descriptor[_DESCRIPTOR_STRING_SIZE];

  consteval CDescriptorString(const char (&string)[N]) : _Descriptor{} {

    _Descriptor[0] = sizeof(_Descriptor);
    _Descriptor[1] = static_cast<std::underlying_type_t<EDescriptorType>>(EDescriptorType::String);

    unsigned position = 2;
    for (unsigned i = 0; i < sizeof(string) - 1; i++) {
      _Descriptor[position++] = string[i];
      _Descriptor[position++] = 0;
    }
  }

  static consteval unsigned size() { return sizeof(_Descriptor); }
};

template <> class CDescriptorString<EDescriptorRepresentation::Buffer, 0> {
  static constexpr auto _DESCRIPTOR_STRING_SIZE = 4;
  static constexpr auto _Type = EDescriptorType::Interface;

public:
  uint8_t _Descriptor[_DESCRIPTOR_STRING_SIZE];

  consteval CDescriptorString() {
    _Descriptor[0] = sizeof(_Descriptor);
    _Descriptor[1] = static_cast<std::underlying_type_t<EDescriptorType>>(EDescriptorType::String);
    _Descriptor[2] = 0x09;
    _Descriptor[3] = 0x04;
  }

  static consteval unsigned size() { return sizeof(_Descriptor); }
};

template <const EDescriptorRepresentation representation, const unsigned N>
CDescriptorString(const char (&string)[N]) -> CDescriptorString<representation, N>;
CDescriptorString() -> CDescriptorString<EDescriptorRepresentation::Buffer, 0U>;

template <> class CDescriptorCustomHid<EDescriptorRepresentation::Buffer> {
  static constexpr auto _DESCRIPTOR_CUSTOM_HID_SIZE = 9U;
  static constexpr auto _Type = EDescriptorType::CustomHid;

public:
  const uint8_t _Descriptor[_DESCRIPTOR_CUSTOM_HID_SIZE];
  consteval CDescriptorCustomHid(const uint8_t size)
      : _Descriptor{sizeof(_Descriptor),
                    static_cast<std::underlying_type_t<EDescriptorType>>(EDescriptorType::CustomHid),
                    0x11,
                    0x1,
                    0x00,
                    0x01,
                    0x22,
                    size,
                    0x00} {}

  static consteval unsigned size() { return sizeof(_Descriptor); }
};

using CDescriptorDeviceBuffer = CDescriptorDevice<EDescriptorRepresentation::Buffer>;
using CDescriptorDeviceDescriptor = CDescriptorDevice<EDescriptorRepresentation::Descriptor>;

using CDescriptorEndpointBuffer = CDescriptorEndpoint<EDescriptorRepresentation::Buffer>;

using CDescriptorInterfaceBuffer = CDescriptorInterface<EDescriptorRepresentation::Buffer>;

template <descriptor... TDescriptor>
using CDescriptorConfigurationBuffer = CDescriptorConfiguration<EDescriptorRepresentation::Buffer, TDescriptor...>;

template <const unsigned N> using CDescriptorStringBuffer = CDescriptorString<EDescriptorRepresentation::Buffer, N>;

using CDescriptorCustomHidBuffer = CDescriptorCustomHid<EDescriptorRepresentation::Buffer>;

} // namespace iso::usb::descriptor
