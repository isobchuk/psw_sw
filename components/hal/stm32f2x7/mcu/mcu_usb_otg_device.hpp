#pragma once

#include "meta_types.hpp"
#include "reg_rcc.hpp"
#include "reg_usb_otg_fs.hpp"

#include "endpoint.hpp"

static volatile bool zeroPack;

static volatile uint32_t zeroCtr;
static volatile uint32_t successfullySent;

static uint32_t write;

static uint32_t writeEmpty;

namespace stm32f217::drivers::usb_otg_fs {

template <const unsigned number> static inline constexpr bool _EP_MAX_NUMBER = (number < 4);
enum class EEndpointType : uint16_t { Control, Isochronous, Bulk, Interrupt };

template <iso::usb::endpoint::c_endpoint Buffer, const unsigned num, const EEndpointType type> class CEndpoint final {
  static constexpr auto _Number = num;
  static constexpr auto _Type = type;

  static constexpr uint32_t _AddressRxFifo = 0x50001000UL;
  static constexpr uint32_t _AddressTxFifo[] = {0x50001200UL, 0x50001400UL, 0x50001600UL, 0x50001800UL};

  volatile Buffer _EndpointReceiveBuffer;
  volatile bool isRxlvl;

public:
  inline CEndpoint() : _EndpointReceiveBuffer(num) {}

  inline void Init() const volatile {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    // Special endpoint 0
    if constexpr (0 == _Number) {
      // 1. Status NAK for OUT endpoint
      // OTG_FS_DEVICE->OTG_FS_DOEPCTL0 |= USB_OTG_FS_DOEPCTL0::SNAK;
      /*OTG_FS_DEVICE->OTG_FS_DOEPCTL[reg_v<0U>] |= USB_OTG_FS_DOEPCTL::SNAK;
      OTG_FS_DEVICE->OTG_FS_DOEPCTL[reg_v<1U>] |= USB_OTG_FS_DOEPCTL::SNAK;
      OTG_FS_DEVICE->OTG_FS_DOEPCTL[reg_v<2U>] |= USB_OTG_FS_DOEPCTL::SNAK;*/

      // Unmask Interrupts for IN and OUT EP0
      OTG_FS_DEVICE->OTG_FS_DAINTMSK |= USB_OTG_FS_DAINTMSK::OEPM(reg_v<0b1UL>) | USB_OTG_FS_DAINTMSK::IEPM(reg_v<0b1UL>);

      // 2. Unmask Transfer complete interrupt
      OTG_FS_DEVICE->OTG_FS_DOEPMSK |= USB_OTG_FS_OTG_FS_DOEPMSK::STUPM | USB_OTG_FS_OTG_FS_DOEPMSK::XFRCM;
      OTG_FS_DEVICE->OTG_FS_DIEPMSK |= USB_OTG_FS_OTG_FS_DIEPMSK::XFRCM;

      // 3. Set FIFOs sizes
      OTG_FS_CORE->FS_GRXFSIZ = USB_OTG_FS_FS_GRXFSIZ::RXFD(reg_v<0x80UL>);
      OTG_FS_CORE->FS_GNPTXFSIZ_Device =
          USB_OTG_FS_FS_GNPTXFSIZ_Device::TX0FSA(reg_v<0x200UL>) | USB_OTG_FS_FS_GNPTXFSIZ_Device::TX0FD(reg_v<0x80UL>);
      OTG_FS_CORE->FS_DIEPTXF[reg_v<0UL>] = USB_OTG_FS_FS_DIEPTXF1::INEPTXFD(reg_v<0x80UL>) | USB_OTG_FS_FS_DIEPTXF1::INEPTXSA(reg_v<0x100UL>);
      OTG_FS_CORE->FS_DIEPTXF[reg_v<1UL>] = USB_OTG_FS_FS_DIEPTXF1::INEPTXFD(reg_v<0x80UL>) | USB_OTG_FS_FS_DIEPTXF1::INEPTXSA(reg_v<0x180UL>);
      OTG_FS_CORE->FS_DIEPTXF[reg_v<2UL>] = USB_OTG_FS_FS_DIEPTXF1::INEPTXFD(reg_v<0x80UL>) | USB_OTG_FS_FS_DIEPTXF1::INEPTXSA(reg_v<0x200UL>);
      // OTG_FS_CORE->FS_GRSTCTL |= USB_OTG_FS_FS_GRSTCTL::RXFFLSH | USB_OTG_FS_FS_GRSTCTL::TXFFLSH;

      // 4.
      OTG_FS_DEVICE->OTG_FS_DOEPSIZ0 |=
          USB_OTG_FS_OTG_FS_DOEPSIZ0::STUPCNT(reg_v<3UL>) | USB_OTG_FS_OTG_FS_DOEPSIZ0::PKTCNT | USB_OTG_FS_OTG_FS_DOEPSIZ0::XFRSIZ(reg_v<64UL>);
      OTG_FS_DEVICE->OTG_FS_DOEPCTL0 |= USB_OTG_FS_DOEPCTL0::CNAK | USB_OTG_FS_DOEPCTL0::USBAEP | USB_OTG_FS_DOEPCTL0::EPENA;
    }
  }

  inline bool CorrectTransfer() volatile {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    constexpr auto statusOffset = 17U;
    enum class EStatus : uint32_t {
      GlobalOutNak = 0b0001UL << statusOffset,
      OutDataPacketReceived = 0b0010UL << statusOffset,
      OutTransferCompleted = 0b0011UL << statusOffset,
      SetupTransactionCompleted = 0b0100UL << statusOffset,
      SetupDataPacketReceived = 0b0110UL << statusOffset

    };

    const EStatus status = static_cast<EStatus>(OTG_FS_CORE->FS_GRXSTSR_Device & USB_OTG_FS_FS_GRXSTSR_Device::PKTSTS(reg_v<0b1111UL>));
    const auto endpointNumber = (OTG_FS_CORE->FS_GRXSTSR_Device & USB_OTG_FS_FS_GRXSTSR_Device::EPNUM(reg_v<0b1111UL>));
    const auto length = ((OTG_FS_CORE->FS_GRXSTSR_Device & USB_OTG_FS_FS_GRXSTSR_Device::BCNT(reg_v<0b111'1111'1111UL>)) >> 4);

    switch (status) {
    case EStatus::GlobalOutNak:
      break;
    case EStatus::OutDataPacketReceived: {
      _EndpointReceiveBuffer.sLength = length;
      const volatile uint32_t *data = reinterpret_cast<const volatile uint32_t *>(_AddressRxFifo);
      (void)*(OTG_FS_CORE->FS_GRXSTSR_POP);
      for (unsigned i = 0; i < (_EndpointReceiveBuffer.sLength + 3) / 4; i++) {
        reinterpret_cast<volatile uint32_t *>(_EndpointReceiveBuffer.aBuffer)[i] = data[0];
      }
    } break;
    case EStatus::OutTransferCompleted:
      (void)*(OTG_FS_CORE->FS_GRXSTSR_POP);
      break;
    case EStatus::SetupTransactionCompleted:
      (void)*(OTG_FS_CORE->FS_GRXSTSR_POP);
      break;
    case EStatus::SetupDataPacketReceived: {
      if ((0 == endpointNumber) && (8 == length)) {
        _EndpointReceiveBuffer.sLength = 8;
        const volatile uint32_t *data = reinterpret_cast<const volatile uint32_t *>(_AddressRxFifo);
        (void)*(OTG_FS_CORE->FS_GRXSTSR_POP);
        reinterpret_cast<volatile uint32_t *>(_EndpointReceiveBuffer.aBuffer)[0] = data[0];
        reinterpret_cast<volatile uint32_t *>(_EndpointReceiveBuffer.aBuffer)[1] = data[0];
      }
    } break;
    default:
      break;
    };
    return true;
  }

  template <const unsigned arraySize> inline bool Write(const uint8_t (&buffer)[arraySize], const unsigned size = arraySize) volatile {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    if constexpr (0 == _Number) {
      OTG_FS_DEVICE->OTG_FS_DIEPCTL0 &= USB_OTG_FS_DIEPCTL0::EPENA;
      OTG_FS_DEVICE->OTG_FS_DIEPSIZ0 = (uint32_t(size / sizeof(_EndpointReceiveBuffer.aBuffer) + 1) << 19) | size;
      OTG_FS_DEVICE->OTG_FS_DIEPCTL0 |= USB_OTG_FS_DIEPCTL0::CNAK | USB_OTG_FS_DIEPCTL0::USBAEP | USB_OTG_FS_DIEPCTL0::EPENA;
    }

    write++;

    volatile uint32_t *data = reinterpret_cast<volatile uint32_t *>(_AddressTxFifo[_Number]);
    for (unsigned i = 0; i < (size + 3) / 4; i++) {
      data[0] = (static_cast<const uint32_t *>(static_cast<const void *>(buffer)))[i];
    }

    return true;
  }

  inline bool WriteEmpty() volatile {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    if constexpr (0 == _Number) {
      OTG_FS_DEVICE->OTG_FS_DIEPCTL0 &= USB_OTG_FS_DIEPCTL0::EPENA;
      OTG_FS_DEVICE->OTG_FS_DIEPSIZ0 = (1UL << 19) | 0;
      OTG_FS_DEVICE->OTG_FS_DIEPCTL0 |= USB_OTG_FS_DIEPCTL0::USBAEP | USB_OTG_FS_DIEPCTL0::CNAK | USB_OTG_FS_DIEPCTL0::EPENA;
    }

    writeEmpty++;

    zeroPack = true;
    return true;
  }

  inline volatile Buffer &Read() volatile {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;
    return _EndpointReceiveBuffer;
  }

  inline void Ack() volatile {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;
    // Special endpoint 0
    if constexpr (0 == _Number) {
      OTG_FS_DEVICE->OTG_FS_DOEPCTL0 &= USB_OTG_FS_DOEPCTL0::EPENA;
      OTG_FS_DEVICE->OTG_FS_DOEPSIZ0 |=
          USB_OTG_FS_OTG_FS_DOEPSIZ0::STUPCNT(reg_v<3UL>) | USB_OTG_FS_OTG_FS_DOEPSIZ0::PKTCNT | USB_OTG_FS_OTG_FS_DOEPSIZ0::XFRSIZ(reg_v<64UL>);
      OTG_FS_DEVICE->OTG_FS_DOEPCTL0 |= USB_OTG_FS_DOEPCTL0::USBAEP | USB_OTG_FS_DOEPCTL0::CNAK | USB_OTG_FS_DOEPCTL0::EPENA;
    }
  }
};

template <auto &...endpoints> class CUsb final {
public:
  using SControlEndpoint = iso::usb::endpoint::CEndpoint<64U>;
  using TEndpointControl = CEndpoint<SControlEndpoint, 0U, EEndpointType::Control>;
  static volatile TEndpointControl EndpointControl;

private:
  enum class EInterruptReason : uint32_t {
    Reset = (1UL << 12),
    EnumerationDone = (1UL << 13),
    EndpointIn = (1UL << 18),
    EndpointOut = (1UL << 19),
    RxLvl = (1UL << 4)
  };
  struct SInterruptTable {
    const EInterruptReason mReason;
    void (&mInterruptHandler)(void);
  };

  static void Reset() {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;
    // USB reset interrupt
    OTG_FS_CORE->FS_GINTSTS |= USB_OTG_FS_FS_GINTSTS::USBRST; // Clear interrupt flag
                                                              // Handle reset event (e.g., configure endpoints)
  }

  static void EnumerationDone() {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    OTG_FS_CORE->FS_GINTSTS |= USB_OTG_FS_FS_GINTSTS::ENUMDNE;

    EndpointControl.Init();
  }

  static void EndpointIn() {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    OTG_FS_CORE->FS_GINTSTS |= USB_OTG_FS_FS_GINTSTS::IEPINT;

    // EndpointControl.CorrectTransfer();
  }

  static void EndpointOut() {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    OTG_FS_CORE->FS_GINTSTS |= USB_OTG_FS_FS_GINTSTS::OEPINT;

    EndpointControl.CorrectTransfer();
  }

  static void RxLvl() {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    EndpointControl.CorrectTransfer();
  }

  static constexpr SInterruptTable sInterruptHandler[] = {{EInterruptReason::Reset, Reset},
                                                          {EInterruptReason::EnumerationDone, EnumerationDone},
                                                          {EInterruptReason::EndpointIn, EndpointIn},
                                                          {EInterruptReason::EndpointOut, EndpointOut},
                                                          {EInterruptReason::RxLvl, RxLvl}};

public:
  consteval CUsb() = default;

  void Init() const {
    using namespace cpp_register;
    using namespace stm32f217::registers::rcc;
    using namespace stm32f217::registers::usb_otg_fs;

    RCC->AHB2ENR |= RCC_AHB2ENR::OTGFSEN;

    OTG_FS_CORE->FS_GUSBCFG |= USB_OTG_FS_FS_GUSBCFG::FDMOD;

    OTG_FS_CORE->FS_GRSTCTL |= USB_OTG_FS_FS_GRSTCTL::CSRST;
    while (OTG_FS_CORE->FS_GRSTCTL & USB_OTG_FS_FS_GRSTCTL::CSRST) {
    }

    OTG_FS_DEVICE->OTG_FS_DCFG |= USB_OTG_FS_OTG_FS_DCFG::NZLSOHSK | USB_OTG_FS_OTG_FS_DCFG::DSPD(reg_v<0b11UL>);

    OTG_FS_CORE->FS_GINTMSK |= USB_OTG_FS_FS_GINTMSK::USBRST | USB_OTG_FS_FS_GINTMSK::ENUMDNEM | USB_OTG_FS_FS_GINTMSK::IEPINT |
                               /* USB_OTG_FS_FS_GINTMSK::OEPINT | */ USB_OTG_FS_FS_GINTMSK::RXFLVLM;
    OTG_FS_CORE->FS_GAHBCFG |= USB_OTG_FS_FS_GAHBCFG::GINT;

    OTG_FS_CORE->FS_GCCFG |= USB_OTG_FS_FS_GCCFG::PWRDWN | USB_OTG_FS_FS_GCCFG::VBUSBSEN;
  }

  static inline void InterruptHandler() {
    using namespace cpp_register;
    using namespace stm32f217::registers::usb_otg_fs;

    const auto interruptReason =
        OTG_FS_CORE->FS_GINTSTS & (USB_OTG_FS_FS_GINTSTS::USBRST | USB_OTG_FS_FS_GINTSTS::ENUMDNE | USB_OTG_FS_FS_GINTSTS::IEPINT |
                                   /*USB_OTG_FS_FS_GINTSTS::OEPINT |*/ USB_OTG_FS_FS_GINTSTS::RXFLVL);

    for (const auto &handler : sInterruptHandler) {
      // To handle several reasons concurrently - do not break from the loop
      if ((static_cast<std::underlying_type_t<EInterruptReason>>(handler.mReason) & interruptReason)) {
        handler.mInterruptHandler();
      }
    }

    if (OTG_FS_DEVICE->OTG_FS_DIEPINT & USB_OTG_FS_OTG_FS_DIEPINT::XFRC) {
      OTG_FS_DEVICE->OTG_FS_DIEPINT |= USB_OTG_FS_OTG_FS_DIEPINT::XFRC;
      successfullySent = successfullySent + 1UL;
      zeroPack = false;
    }

    if (OTG_FS_DEVICE->OTG_FS_DOEPINT & USB_OTG_FS_OTG_FS_DOEPINT::XFRC) {
      OTG_FS_DEVICE->OTG_FS_DOEPINT |= USB_OTG_FS_OTG_FS_DOEPINT::XFRC;
    }
  }

  inline auto ControlEndpoint() const -> decltype(EndpointControl) & { return EndpointControl; }
  inline void Address(const uint8_t addr) const {
    using namespace stm32f217::registers::usb_otg_fs;
    const uint32_t adddress = addr << 4;
    /* while (zeroPack) {
     }*/
    *(reinterpret_cast<uint32_t *>(0x5000'0800UL)) |= adddress;
  }
};

template <auto &...endpoints> volatile CUsb<endpoints...>::TEndpointControl CUsb<endpoints...>::EndpointControl;

} // namespace stm32f217::drivers::usb_otg_fs