#pragma once

#include "mcu_usb_otg_device.hpp"

#include "descriptor.hpp"
#include "device.hpp"
#include "endpoint.hpp"
#include "mx25.hpp"
#include "request.hpp"
#include "scsi.hpp"
#include "system_time.hpp"
#include "type.hpp"

#include <cassert>

namespace integration::usb::device {

template <typename TTimer, hardware::flash::flash_memory_spi TFlash> class CUsbIntegration {

  static constexpr iso::usb::descriptor::CDescriptorDummyBuffer _DescriptorDummy{};

  static constexpr iso::usb::descriptor::CDescriptorDeviceBuffer _DescriptorDevice{iso::usb::EBcdUsb::Usb2_0, 0xB6AU, 0x146U};

  static constexpr iso::usb::descriptor::CDescriptorInterfaceBuffer _DescriptorInterface{0U, 2U, iso::usb::EDeviceClass::MassStorageDevice, 0x6U,
                                                                                         0x50U};
  static constexpr iso::usb::descriptor::CDescriptorEndpointBuffer _DescriptorEndpointBulkIn{iso::usb::endpoint::EType::Bulk,
                                                                                             iso::usb::endpoint::EDirection::In, 1U, 0};
  static constexpr iso::usb::descriptor::CDescriptorEndpointBuffer _DescriptorEndpointBulkOut{iso::usb::endpoint::EType::Bulk,
                                                                                              iso::usb::endpoint::EDirection::Out, 1U, 0};

  static constexpr iso::usb::descriptor::CDescriptorConfigurationBuffer _DescriptorConfiguration{
      1U, 1U, _DescriptorInterface, _DescriptorEndpointBulkIn, _DescriptorEndpointBulkOut};

  static constexpr iso::usb::descriptor::CDescriptorStringBuffer _DescriptorStringSerialNumber{"1234567890"};
  static constexpr iso::usb::descriptor::CDescriptorStringBuffer _DescriptorStringLanguage{};
  static constexpr iso::usb::descriptor::CDescriptorStringBuffer _DescriptorStringProduct{"Password Storage"};
  static constexpr iso::usb::descriptor::CDescriptorStringBuffer _DescriptorStringManufacturer{"Ivan Sobchuk"};

  using SBulkEndpoint = iso::usb::endpoint::CEndpoint<64U>;
  using TEndpointBulk = stm32f217::drivers::usb_otg_fs::CEndpoint<SBulkEndpoint, 1UL, stm32f217::drivers::usb_otg_fs::EEndpointType::Bulk>;
  static volatile TEndpointBulk EndpointBulk;

  const TFlash &_Flash;

  class CCallback final {
    const TFlash &_Flash;

  public:
    constexpr CCallback(const TFlash &flash) : _Flash(flash) {}

    static consteval auto SizePacketMax() { return 64UL; }

    template <const std::size_t N> inline bool Transmit(const uint8_t (&buff)[N], const std::size_t len = N) const {
      return EndpointBulk.Write(buff, len);
    }

    template <const std::size_t N> inline bool Receive(uint8_t (&buff)[N], const std::size_t = N) const {
      volatile auto &epBuffer = EndpointBulk.Read();

      if (epBuffer) {
        for (unsigned i = 0; i < sizeof(buff); i++) {
          buff[i] = epBuffer.aBuffer[i];
        }

        EndpointBulk.Ack();
        epBuffer();
      } else {
        return false;
      }

      return true;
    }

    template <const std::size_t N> inline void OnRead(const uint32_t startAddress, uint8_t (&sector)[N]) const {

      for (unsigned i = 0; i < _Flash.SizeSector(); i += _Flash.SizePage()) {
        uint8_t(&buffer)[_Flash.SizePage()] = reinterpret_cast<uint8_t(&)[_Flash.SizePage()]>(sector[i]);
        _Flash.PageRead(startAddress + i, buffer);
      }
    }

    template <const std::size_t N> inline void OnWrite(const uint32_t startAddress, uint8_t (&sector)[N]) const {
      _Flash.SectorErase(startAddress);
      for (unsigned i = 0; i < _Flash.SizeSector(); i += _Flash.SizePage()) {
        uint8_t(&buffer)[_Flash.SizePage()] = reinterpret_cast<uint8_t(&)[_Flash.SizePage()]>(sector[i]);
        _Flash.PageProgram(startAddress + i, buffer);
      }
    }
  };

  class IScsiDescriptor final {
    // clang-format off
    // TODO: Rewrite similar to usb descriptors

    //----------------------SCSI_INQUIRY----------------------
    static constexpr uint8_t _Inquiry[] = {
      0x00,           //Block device
      0x80,           //Removable media
      0x04,           //SPC-2
      0x02,           //Response data format = 0x02
      0x1F,           //Additional_length = length - 5
      0x00,
      0x00,
      0x00,
      'S', 'O', 'B', 'A', ' ', 'I', 'n', 'c',
      'I', 'V', 'A', 'N', ' ', 'S', 'O', 'B', 'C', 'H', 'U', 'K', ' ', ' ', ' ', ' ',
      '0', '0', '0', '1'
    };

  //-----------------SCSI_REQUEST_SENSE--------------------
  static constexpr uint8_t _SenseData[] =
  {
          0x70,       //VALID = 1, RESRONSE_CODE = 0x70
          0x00,
          0x05,       //S_ILLEGAL_REQUEST
          0x00, 
          0x00, 
          0x00, 
          0x00, 
          0x00,
          0x00, 
          0x00, 
          0x00,
          0x00, 
          0x00, 
          0x00, 
          0x00, 
          0x00, 
          0x00, 
          0x00
  };

  
  //----------------READ_FORMAT_CAPACITY----------------
  //----------------READ_CAPACITY------------------------
  static constexpr uint8_t _FormatCapacity[] = 
  {
          0x00, 0x00, 0x00, 0x00,      
          0x00, 0x00, 0x00, 0x00,    
          0x02, 0x00, 0x00, 0x00
  };

  //----------------READ_CAPACITY------------------------
  static constexpr uint8_t _Capacity[] = 
  {
          0x00, 0x00, 0x03, 0xFF,     //Addr last blocks = 4MB - 512B
          0x00, 0x00, 0x10, 0x00   
  };

    //---------------MODE_SENSE----------------------------
  static constexpr uint8_t _ModeSense[] = 
  {
          0x03, 0x00, 0x00, 0x00
  };

    // clang-format on
  public:
    consteval IScsiDescriptor() = default;

    inline constexpr auto Inquiry() const -> const uint8_t (&)[sizeof(_Inquiry)] { return _Inquiry; }
    inline constexpr auto SenseData() const -> const uint8_t (&)[sizeof(_SenseData)] { return _SenseData; }
    inline constexpr auto FormatCapacity() const -> const uint8_t (&)[sizeof(_FormatCapacity)] { return _FormatCapacity; }
    inline constexpr auto Capacity() const -> const uint8_t (&)[sizeof(_Capacity)] { return _Capacity; }
    inline constexpr auto ModeSense() const -> const uint8_t (&)[sizeof(_ModeSense)] { return _ModeSense; }
  };

public:
  static constexpr stm32f217::drivers::usb_otg_fs::CUsb<EndpointBulk> _Hardware{};
  const TTimer &_Timer;
  consteval CUsbIntegration(const TTimer &timer, const TFlash &flash) : _Flash(flash), _Timer(timer) {}

  inline void Init() const { _Hardware.Init(); }

  inline constexpr auto DeviceDescriptor() const -> const iso::usb::Type::Byte (&)[_DescriptorDevice.size()] { return _DescriptorDevice._Descriptor; }
  inline constexpr auto ConfigurationDescriptor() const -> const iso::usb::Type::Byte (&)[_DescriptorConfiguration.size()] {

    return _DescriptorConfiguration._Descriptor;
  }

  inline constexpr auto StringSerialNumberDescriptor() const -> const iso::usb::Type::Byte (&)[_DescriptorStringSerialNumber.size()] {
    return _DescriptorStringSerialNumber._Descriptor;
  }

  inline constexpr auto StringLanguageDescriptor() const -> const iso::usb::Type::Byte (&)[_DescriptorStringLanguage.size()] {
    return _DescriptorStringLanguage._Descriptor;
  }

  inline constexpr auto StringProductDescriptor() const -> const iso::usb::Type::Byte (&)[_DescriptorStringProduct.size()] {
    return _DescriptorStringProduct._Descriptor;
  }

  inline constexpr auto StringManufacturerDescriptor() const -> const iso::usb::Type::Byte (&)[_DescriptorStringManufacturer.size()] {
    return _DescriptorStringManufacturer._Descriptor;
  }

  inline void SetConfiguration() const { EndpointBulk.Init(); }

  inline constexpr auto CustomHid() const -> const iso::usb::Type::Byte (&)[_DescriptorDummy.size()] { return _DescriptorDummy._Descriptor; }

  void operator()() const {
    using namespace iso::usb;

    auto &epBuffer = EndpointBulk.Read();

    if (epBuffer) {
      CCallback callback{_Flash};
      iso::usb::scsi::CScsi<CCallback, IScsiDescriptor> scsi{callback};

      // Away from volatile
      Type::Byte buffer[iso::usb::scsi::SCbw::size()];
      for (unsigned i = 0; i < sizeof(buffer); i++) {
        buffer[i] = epBuffer.aBuffer[i];
      }

      epBuffer();
      EndpointBulk.Ack();

      scsi(buffer);
    }
  }

  static inline void InterruptHandler() { _Hardware.InterruptHandler(); }
};

template <typename TTimer, hardware::flash::flash_memory_spi TFlash>
class CUsbIntegration<TTimer, TFlash>::TEndpointBulk volatile CUsbIntegration<TTimer, TFlash>::EndpointBulk;

} // namespace integration::usb::device