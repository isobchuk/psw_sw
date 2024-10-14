#pragma once

#include "register.hpp"

namespace stm32f217::registers::usb_otg_fs {

template <const cpp_register::RegisterAddress address> struct USB_OTG_FS_CORE_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint32_t, struct FS_GOTGCTL> FS_GOTGCTL{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::RC_W1, uint32_t, struct FS_GOTGINT> FS_GOTGINT{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::RW, uint32_t, struct FS_GAHBCFG> FS_GAHBCFG{};
  static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::RW, uint32_t, struct FS_GUSBCFG> FS_GUSBCFG{};
  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint32_t, struct FS_GRSTCTL> FS_GRSTCTL{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::RW, uint32_t, struct FS_GINTSTS> FS_GINTSTS{};
  static constexpr cpp_register::Register<address + 0x18, cpp_register::AccessMode::RW, uint32_t, struct FS_GINTMSK> FS_GINTMSK{};
  static constexpr cpp_register::Register<address + 0x1C, cpp_register::AccessMode::R, uint32_t, struct FS_GRXSTSR_Device> FS_GRXSTSR_Device{};
  static constexpr cpp_register::Register<address + 0x1C, cpp_register::AccessMode::R, uint32_t, struct FS_GRXSTSR_Host> FS_GRXSTSR_Host{};
  static constexpr cpp_register::Register<address + 0x20, cpp_register::AccessMode::R, uint32_t, struct FS_GRXSTSR_Device> FS_GRXSTSR_POP{};
  static constexpr cpp_register::Register<address + 0x24, cpp_register::AccessMode::RW, uint32_t, struct FS_GRXFSIZ> FS_GRXFSIZ{};
  static constexpr cpp_register::Register<address + 0x28, cpp_register::AccessMode::RW, uint32_t, struct FS_GNPTXFSIZ_Device> FS_GNPTXFSIZ_Device{};
  static constexpr cpp_register::Register<address + 0x28, cpp_register::AccessMode::RW, uint32_t, struct FS_GNPTXFSIZ_Host> FS_GNPTXFSIZ_Host{};
  static constexpr cpp_register::Register<address + 0x2C, cpp_register::AccessMode::R, uint32_t, struct FS_GNPTXSTS> FS_GNPTXSTS{};
  static constexpr cpp_register::Register<address + 0x38, cpp_register::AccessMode::RW, uint32_t, struct FS_GCCFG> FS_GCCFG{};
  static constexpr cpp_register::Register<address + 0x3C, cpp_register::AccessMode::RW, uint32_t, struct FS_CID> FS_CID{};
  static constexpr cpp_register::Register<address + 0x100, cpp_register::AccessMode::RW, uint32_t, struct FS_HPTXFSIZ> FS_HPTXFSIZ{};
  static constexpr cpp_register::Register<address + 0x104, cpp_register::AccessMode::RW, uint32_t, struct FS_DIEPTXF1, 3> FS_DIEPTXF{};
};

template <const cpp_register::RegisterAddress address> struct USB_OTG_FS_DEVICE_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DCFG> OTG_FS_DCFG{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DCTL> OTG_FS_DCTL{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::R, uint32_t, struct OTG_FS_DSTS> OTG_FS_DSTS{};
  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DIEPMSK> OTG_FS_DIEPMSK{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DOEPMSK> OTG_FS_DOEPMSK{};
  static constexpr cpp_register::Register<address + 0x18, cpp_register::AccessMode::R, uint32_t, struct OTG_FS_DAINT> OTG_FS_DAINT{};
  static constexpr cpp_register::Register<address + 0x1C, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DAINTMSK> OTG_FS_DAINTMSK{};

  static constexpr cpp_register::Register<address + 0x100, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DIEPCTL0> OTG_FS_DIEPCTL0{};
  static constexpr cpp_register::Register<address + 0x120, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DIEPCTL, 3, 0x20> OTG_FS_DIEPCTL{};

  static constexpr cpp_register::Register<address + 0x300, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DOEPCTL0> OTG_FS_DOEPCTL0{};
  static constexpr cpp_register::Register<address + 0x320, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DOEPCTL, 3, 0x20> OTG_FS_DOEPCTL{};

  static constexpr cpp_register::Register<address + 0x108, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DIEPINT, 4, 0x20> OTG_FS_DIEPINT{};
  static constexpr cpp_register::Register<address + 0x308, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DOEPINT, 4, 0x20> OTG_FS_DOEPINT{};

  static constexpr cpp_register::Register<address + 0x110, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DIEPSIZ0> OTG_FS_DIEPSIZ0{};
  static constexpr cpp_register::Register<address + 0x310, cpp_register::AccessMode::RW, uint32_t, struct OTG_FS_DOEPSIZ0> OTG_FS_DOEPSIZ0{};
};

inline constexpr USB_OTG_FS_CORE_T<0x50000000> const *OTG_FS_CORE{};
// inline constexpr USB_OTG_FS_T<0x50000400> const *OTG_FS_HOST{};
inline constexpr USB_OTG_FS_DEVICE_T<0x50000800> const *OTG_FS_DEVICE{};
// inline constexpr USB_OTG_FS_T<0x50000E00> const *OTG_FS_PWRCLK{};

struct USB_OTG_FS_FS_GOTGCTL {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 0), cpp_register::AccessMode::R, 1> SRQSCS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 1), cpp_register::AccessMode::RW, 1> SRQ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 8), cpp_register::AccessMode::R, 1> HNGSCS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 9), cpp_register::AccessMode::R, 1> HNPRQ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 10), cpp_register::AccessMode::R, 1> HSHNPEN{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 11), cpp_register::AccessMode::R, 1> DHNPEN{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 16), cpp_register::AccessMode::R, 1> CIDSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 17), cpp_register::AccessMode::R, 1> DBCT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 18), cpp_register::AccessMode::R, 1> ASVLD{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGCTL), (1UL << 19), cpp_register::AccessMode::R, 1> BSVLD{};
};

struct USB_OTG_FS_FS_GOTGINT {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGINT), (1UL << 2), cpp_register::AccessMode::RW, 1> SEDET{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGINT), (1UL << 8), cpp_register::AccessMode::RW, 1> SRSSCHG{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGINT), (1UL << 9), cpp_register::AccessMode::RW, 1> HNSSCHG{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGINT), (1UL << 17), cpp_register::AccessMode::RW, 1> HNGDET{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGINT), (1UL << 18), cpp_register::AccessMode::RW, 1> ADTOCHG{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GOTGINT), (1UL << 19), cpp_register::AccessMode::RW, 1> DBCDNE{};
};

struct USB_OTG_FS_FS_GAHBCFG {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GAHBCFG), (1UL << 0), cpp_register::AccessMode::RW, 1> GINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GAHBCFG), (1UL << 7), cpp_register::AccessMode::RW, 1> TXFELVL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GAHBCFG), (1UL << 8), cpp_register::AccessMode::RW, 1> PTXFELVL{};
};

struct USB_OTG_FS_FS_GUSBCFG {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 0), cpp_register::AccessMode::RW, 3> TOCAL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 6), cpp_register::AccessMode::RW, 1> PHYSEL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 8), cpp_register::AccessMode::RW, 1> SRPCAP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 9), cpp_register::AccessMode::RW, 1> HNPCAP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 10), cpp_register::AccessMode::RW, 4> TRDT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 29), cpp_register::AccessMode::RW, 1> FHMOD{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 30), cpp_register::AccessMode::RW, 1> FDMOD{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GUSBCFG), (1UL << 31), cpp_register::AccessMode::RW, 1> CTXPKT{};
};

struct USB_OTG_FS_FS_GRSTCTL {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRSTCTL), (1UL << 0), cpp_register::AccessMode::RW, 1> CSRST{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRSTCTL), (1UL << 1), cpp_register::AccessMode::RW, 1> HSRST{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRSTCTL), (1UL << 2), cpp_register::AccessMode::RW, 1> FCRST{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRSTCTL), (1UL << 4), cpp_register::AccessMode::RW, 1> RXFFLSH{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRSTCTL), (1UL << 5), cpp_register::AccessMode::RW, 1> TXFFLSH{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRSTCTL), (1UL << 6), cpp_register::AccessMode::RW, 5> TXFNUM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRSTCTL), (1UL << 31), cpp_register::AccessMode::RW, 1> AHBIDL{};
};

struct USB_OTG_FS_FS_GINTSTS {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 0), cpp_register::AccessMode::R, 1> CMOD{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 1), cpp_register::AccessMode::R, 1> MMIS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 2), cpp_register::AccessMode::R, 1> OTGINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 3), cpp_register::AccessMode::R, 1> SOF{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 4), cpp_register::AccessMode::RC_W0, 1> RXFLVL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 5), cpp_register::AccessMode::R, 1> NPTXFE{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 6), cpp_register::AccessMode::R, 1> GINAKEFF{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 7), cpp_register::AccessMode::R, 1> GOUTNAKEFF{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 10), cpp_register::AccessMode::R, 1> ESUSP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 11), cpp_register::AccessMode::R, 1> USBSUSP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 12), cpp_register::AccessMode::RC_W1, 1> USBRST{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 13), cpp_register::AccessMode::RC_W1, 1> ENUMDNE{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 14), cpp_register::AccessMode::R, 1> ISOODRP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 15), cpp_register::AccessMode::R, 1> EOPF{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 18), cpp_register::AccessMode::RC_W1, 1> IEPINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 19), cpp_register::AccessMode::RC_W1, 1> OEPINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 20), cpp_register::AccessMode::R, 1> IISOIXFR{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 21), cpp_register::AccessMode::R, 1> IPXFR_INCOMPISOOUT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 24), cpp_register::AccessMode::R, 1> HPRTINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 25), cpp_register::AccessMode::R, 1> HCINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 26), cpp_register::AccessMode::R, 1> PTXFE{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 28), cpp_register::AccessMode::R, 1> CIDSCHG{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 29), cpp_register::AccessMode::R, 1> DISCINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 30), cpp_register::AccessMode::R, 1> SRQINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTSTS), (1UL << 31), cpp_register::AccessMode::R, 1> WKUPINT{};
};

struct USB_OTG_FS_FS_GINTMSK {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 1), cpp_register::AccessMode::RW, 1> MMISM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 2), cpp_register::AccessMode::RW, 1> OTGINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 3), cpp_register::AccessMode::RW, 1> SOFM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 4), cpp_register::AccessMode::RW, 1> RXFLVLM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 5), cpp_register::AccessMode::RW, 1> NPTXFEM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 6), cpp_register::AccessMode::RW, 1> GINAKEFFM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 7), cpp_register::AccessMode::RW, 1> GONAKEFFM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 10), cpp_register::AccessMode::RW, 1> ESUSPM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 11), cpp_register::AccessMode::RW, 1> USBSUSPM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 12), cpp_register::AccessMode::RW, 1> USBRST{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 13), cpp_register::AccessMode::RW, 1> ENUMDNEM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 14), cpp_register::AccessMode::RW, 1> ISOODRPM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 15), cpp_register::AccessMode::RW, 1> EOPFM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 17), cpp_register::AccessMode::RW, 1> EPMISM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 18), cpp_register::AccessMode::RW, 1> IEPINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 19), cpp_register::AccessMode::RW, 1> OEPINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 20), cpp_register::AccessMode::RW, 1> IISOIXFRM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 21), cpp_register::AccessMode::RW, 1> IPXFRM_IISOOXFRM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 24), cpp_register::AccessMode::RW, 1> PRTIM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 25), cpp_register::AccessMode::RW, 1> HCIM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 26), cpp_register::AccessMode::RW, 1> PTXFEM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 28), cpp_register::AccessMode::RW, 1> CIDSCHGM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 29), cpp_register::AccessMode::RW, 1> DISCINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 30), cpp_register::AccessMode::RW, 1> SRQIM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GINTMSK), (1UL << 31), cpp_register::AccessMode::RW, 1> WUIM{};
};

struct USB_OTG_FS_FS_GRXSTSR_Device {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Device), (1UL << 0), cpp_register::AccessMode::R, 4> EPNUM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Device), (1UL << 4), cpp_register::AccessMode::R, 11> BCNT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Device), (1UL << 15), cpp_register::AccessMode::R, 2> DPID{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Device), (1UL << 17), cpp_register::AccessMode::R, 4> PKTSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Device), (1UL << 21), cpp_register::AccessMode::R, 4> FRMNUM{};
};

struct USB_OTG_FS_FS_GRXSTSR_Host {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Host), (1UL << 0), cpp_register::AccessMode::R, 4> EPNUM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Host), (1UL << 4), cpp_register::AccessMode::R, 11> BCNT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Host), (1UL << 15), cpp_register::AccessMode::R, 2> DPID{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Host), (1UL << 17), cpp_register::AccessMode::R, 4> PKTSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXSTSR_Host), (1UL << 21), cpp_register::AccessMode::R, 4> FRMNUM{};
};

struct USB_OTG_FS_FS_GRXFSIZ {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GRXFSIZ), (1UL << 0), cpp_register::AccessMode::RW, 16> RXFD{};
};

struct USB_OTG_FS_FS_GNPTXFSIZ_Device {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GNPTXFSIZ_Device), (1UL << 0), cpp_register::AccessMode::RW, 16> TX0FSA{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GNPTXFSIZ_Device), (1UL << 16), cpp_register::AccessMode::RW, 16> TX0FD{};
};

struct USB_OTG_FS_FS_GNPTXFSIZ_Host {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GNPTXFSIZ_Host), (1UL << 0), cpp_register::AccessMode::RW, 16> NPTXFSA{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GNPTXFSIZ_Host), (1UL << 16), cpp_register::AccessMode::RW, 16> NPTXFD{};
};

struct USB_OTG_FS_FS_GNPTXSTS {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GNPTXSTS), (1UL << 0), cpp_register::AccessMode::R, 16> NPTXFSAV{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GNPTXSTS), (1UL << 16), cpp_register::AccessMode::R, 8> NPTQXSAV{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GNPTXSTS), (1UL << 24), cpp_register::AccessMode::R, 7> NPTXQTOP{};
};

struct USB_OTG_FS_FS_GCCFG {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GCCFG), (1UL << 16), cpp_register::AccessMode::RW, 1> PWRDWN{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GCCFG), (1UL << 18), cpp_register::AccessMode::RW, 1> VBUSASEN{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GCCFG), (1UL << 19), cpp_register::AccessMode::RW, 1> VBUSBSEN{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_GCCFG), (1UL << 20), cpp_register::AccessMode::RW, 1> SOFOUTEN{};
};

struct USB_OTG_FS_FS_CID {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_CID), (1UL << 0), cpp_register::AccessMode::RW, 32> PRODUCT_ID{};
};

struct USB_OTG_FS_FS_HPTXFSIZ {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_HPTXFSIZ), (1UL << 0), cpp_register::AccessMode::RW, 16> PTXSA{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_HPTXFSIZ), (1UL << 16), cpp_register::AccessMode::RW, 16> PTXFSIZ{};
};

struct USB_OTG_FS_FS_DIEPTXF1 {
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_DIEPTXF), (1UL << 0), cpp_register::AccessMode::RW, 16> INEPTXSA{};
  static constexpr cpp_register::Field<decltype(OTG_FS_CORE->FS_DIEPTXF), (1UL << 16), cpp_register::AccessMode::RW, 16> INEPTXFD{};
};

// -----------------------Device-------------------

struct USB_OTG_FS_OTG_FS_DCFG {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCFG), (1UL << 0), cpp_register::AccessMode::RW, 2> DSPD{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCFG), (1UL << 2), cpp_register::AccessMode::RW, 1> NZLSOHSK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCFG), (1UL << 4), cpp_register::AccessMode::RW, 1> DAD{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCFG), (1UL << 11), cpp_register::AccessMode::RW, 1> PFIVL{};
};

struct USB_OTG_FS_OTG_FS_DCTL {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 0), cpp_register::AccessMode::RW, 1> RWUSIG{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 1), cpp_register::AccessMode::RW, 1> SDIS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 2), cpp_register::AccessMode::R, 1> GINSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 3), cpp_register::AccessMode::R, 1> GONSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 4), cpp_register::AccessMode::RW, 3> TCTL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 7), cpp_register::AccessMode::W, 1> SGINAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 8), cpp_register::AccessMode::W, 1> CGINAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 9), cpp_register::AccessMode::W, 1> SGONAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 10), cpp_register::AccessMode::W, 1> CGONAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DCTL), (1UL << 11), cpp_register::AccessMode::RW, 1> POPRGDNE{};
};

struct USB_OTG_FS_OTG_FS_DSTS {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DSTS), (1UL << 0), cpp_register::AccessMode::R, 1> SUSPSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DSTS), (1UL << 1), cpp_register::AccessMode::R, 2> ENUMSPD{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DSTS), (1UL << 3), cpp_register::AccessMode::R, 1> EERR{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DSTS), (1UL << 8), cpp_register::AccessMode::R, 14> FNSOF{};
};

struct USB_OTG_FS_OTG_FS_DIEPMSK {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPMSK), (1UL << 0), cpp_register::AccessMode::RW, 1> XFRCM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPMSK), (1UL << 1), cpp_register::AccessMode::RW, 1> EPDM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPMSK), (1UL << 3), cpp_register::AccessMode::RW, 1> TOM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPMSK), (1UL << 4), cpp_register::AccessMode::RW, 1> ITTXFEMSK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPMSK), (1UL << 5), cpp_register::AccessMode::RW, 1> INEPNMM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPMSK), (1UL << 6), cpp_register::AccessMode::RW, 1> INEPNEM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPMSK), (1UL << 13), cpp_register::AccessMode::RW, 1> NAKM{};
};

struct USB_OTG_FS_OTG_FS_DOEPMSK {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 0), cpp_register::AccessMode::RW, 1> XFRCM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 1), cpp_register::AccessMode::RW, 1> EPDM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 3), cpp_register::AccessMode::RW, 1> STUPM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 4), cpp_register::AccessMode::RW, 1> OTEPDM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 5), cpp_register::AccessMode::RW, 1> STSPHSRXM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 8), cpp_register::AccessMode::RW, 1> OUTPKTERRM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 12), cpp_register::AccessMode::RW, 1> BERRM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPMSK), (1UL << 13), cpp_register::AccessMode::RW, 1> NAK{};
};

struct USB_OTG_FS_DAINT {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DAINT), (1UL << 0), cpp_register::AccessMode::R, 4> IEPINT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DAINT), (1UL << 16), cpp_register::AccessMode::R, 4> OEPINT{};
};

struct USB_OTG_FS_DAINTMSK {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DAINTMSK), (1UL << 0), cpp_register::AccessMode::RW, 4> IEPM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DAINTMSK), (1UL << 16), cpp_register::AccessMode::RW, 4> OEPM{};
};

struct USB_OTG_FS_DIEPCTL0 {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 0), cpp_register::AccessMode::RW, 2> MPSIZ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 15), cpp_register::AccessMode::RW, 1> USBAEP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 17), cpp_register::AccessMode::R, 1> NAKSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 18), cpp_register::AccessMode::R, 2> EPTYP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 21), cpp_register::AccessMode::RS, 1> STALL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 22), cpp_register::AccessMode::RW, 4> TXFNUM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 26), cpp_register::AccessMode::W, 1> CNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 27), cpp_register::AccessMode::W, 1> SNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 30), cpp_register::AccessMode::R, 1> EPDIS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL0), (1UL << 31), cpp_register::AccessMode::RW, 1> EPENA{};
};

struct USB_OTG_FS_DIEPCTL {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 0), cpp_register::AccessMode::RW, 11> MPSIZ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 15), cpp_register::AccessMode::RW, 1> USBAEP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 16), cpp_register::AccessMode::R, 1> EONUM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 17), cpp_register::AccessMode::R, 1> NAKSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 18), cpp_register::AccessMode::RW, 2> EPTYP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 21), cpp_register::AccessMode::RW, 1> STALL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 22), cpp_register::AccessMode::RW, 4> TXFNUM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 26), cpp_register::AccessMode::W, 1> CNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 27), cpp_register::AccessMode::W, 1> SNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 28), cpp_register::AccessMode::W, 1> SD0PID{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 29), cpp_register::AccessMode::W, 1> SODDFRM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 30), cpp_register::AccessMode::RS, 1> EPDIS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPCTL), (1UL << 31), cpp_register::AccessMode::RS, 1> EPENA{};
};

struct USB_OTG_FS_DOEPCTL0 {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 0), cpp_register::AccessMode::R, 2> MPSIZ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 15), cpp_register::AccessMode::RW, 1> USBAEP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 17), cpp_register::AccessMode::R, 1> NAKSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 18), cpp_register::AccessMode::R, 2> EPTYP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 20), cpp_register::AccessMode::RW, 1> SNPM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 21), cpp_register::AccessMode::RS, 1> STALL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 26), cpp_register::AccessMode::W, 1> CNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 27), cpp_register::AccessMode::W, 1> SNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 30), cpp_register::AccessMode::R, 1> EPDIS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL0), (1UL << 31), cpp_register::AccessMode::W, 1> EPENA{};
};

struct USB_OTG_FS_DOEPCTL {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 0), cpp_register::AccessMode::R, 11> MPSIZ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 15), cpp_register::AccessMode::RW, 1> USBAEP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 16), cpp_register::AccessMode::R, 1> EONUM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 17), cpp_register::AccessMode::R, 1> NAKSTS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 18), cpp_register::AccessMode::RW, 2> EPTYP{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 20), cpp_register::AccessMode::RW, 2> SNPM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 21), cpp_register::AccessMode::RW, 1> STALL{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 26), cpp_register::AccessMode::W, 1> CNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 27), cpp_register::AccessMode::W, 1> SNAK{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 28), cpp_register::AccessMode::W, 1> SD0PID{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 29), cpp_register::AccessMode::W, 1> SODDFRM{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 30), cpp_register::AccessMode::RS, 1> EPDIS{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPCTL), (1UL << 31), cpp_register::AccessMode::RS, 1> EPENA{};
};

struct USB_OTG_FS_OTG_FS_DIEPINT {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPINT), (1UL << 0), cpp_register::AccessMode::RC_W1, 1> XFRC{};
};

struct USB_OTG_FS_OTG_FS_DOEPINT {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPINT), (1UL << 0), cpp_register::AccessMode::RC_W1, 1> XFRC{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPINT), (1UL << 3), cpp_register::AccessMode::RC_W1, 1> STUP{};
};

struct USB_OTG_FS_OTG_FS_DIEPSIZ0 {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPSIZ0), (1UL << 0), cpp_register::AccessMode::RW, 7> XFRSIZ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DIEPSIZ0), (1UL << 19), cpp_register::AccessMode::RW, 2> PKTCNT{};
};

struct USB_OTG_FS_OTG_FS_DOEPSIZ0 {
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPSIZ0), (1UL << 0), cpp_register::AccessMode::RW, 7> XFRSIZ{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPSIZ0), (1UL << 19), cpp_register::AccessMode::RW, 1> PKTCNT{};
  static constexpr cpp_register::Field<decltype(OTG_FS_DEVICE->OTG_FS_DOEPSIZ0), (1UL << 29), cpp_register::AccessMode::RW, 2> STUPCNT{};
};
} // namespace stm32f217::registers::usb_otg_fs
