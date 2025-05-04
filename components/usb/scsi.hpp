#pragma once

#include <bit>
#include <concepts>
#include <cstddef>
#include <cstdint>

static unsigned scsiNumber;

namespace iso::usb::scsi {

enum class ECommand : uint8_t {
  TestUnitReady,
  RequestSense = 0x03,
  Inquiry = 0x12,
  ModeSense6 = 0x1A,
  PreventAllowMediumRemoval = 0x1E,
  ReadFormatCapacities6 = 0x23,
  ReadCapacity10 = 0x25,
  Read10 = 0x28,
  Write10 = 0x2A
};

struct SCbw {
  const uint32_t dCBWSignature;
  const uint32_t dCBWTag;
  const uint32_t dCBWDataTransferLength;
  const uint8_t bmCBWFlags;
  const uint8_t bCBWLUN;
  const uint8_t bCBWCBLength;
  const uint8_t CBWCB[16];

  static consteval unsigned size() {
    // clang-format off
     constexpr auto sz =    sizeof(dCBWSignature) + 
                            sizeof(dCBWTag) + 
                            sizeof(dCBWDataTransferLength) + 
                            sizeof(bmCBWFlags) + 
                            sizeof(bCBWLUN) +
                            sizeof(bCBWCBLength) + 
                            sizeof(CBWCB);
    // clang-format on
    return sz;
  }
};

struct SCsw {
  const uint32_t dCSWSignature;
  const uint32_t dCSWTag;
  const uint32_t dCSWDataResidue;
  uint8_t bCSWStatus;

  static consteval unsigned size() {
    // clang-format off
     constexpr auto sz =    sizeof(dCSWSignature) + 
                            sizeof(dCSWTag) + 
                            sizeof(dCSWDataResidue) + 
                            sizeof(bCSWStatus);
    // clang-format on
    return sz;
  }

  constexpr SCsw(const uint32_t tag, const uint32_t residue, const uint8_t status)
      : dCSWSignature(0x53425355UL), dCSWTag(tag), dCSWDataResidue(residue), bCSWStatus(status) {}
};

template <typename T>
concept interface = requires(const T interface, const uint8_t (&buffer)[16], uint8_t (&buffer2)[4096]) {
  { interface.SizePacketMax() } -> std::convertible_to<std::size_t>;
  { interface.Transmit(buffer) } -> std::convertible_to<bool>;
  { interface.Transmit(buffer, 10) } -> std::convertible_to<bool>;
  { interface.Receive(buffer2) } -> std::convertible_to<bool>;
  { interface.Receive(buffer2, 10) } -> std::convertible_to<bool>;
  { interface.OnRead(uint32_t{}, buffer2) } -> std::same_as<void>;
  { interface.OnWrite(uint32_t{}, buffer2) } -> std::same_as<void>;
};

template <typename T>
concept descriptor = requires(const T desc) {
  { T() };
  { desc.Inquiry() } -> std::convertible_to<const uint8_t(&)[36]>;
  { desc.SenseData() } -> std::convertible_to<const uint8_t(&)[18]>;
  { desc.FormatCapacity() } -> std::convertible_to<const uint8_t(&)[12]>;
  { desc.Capacity() } -> std::convertible_to<const uint8_t(&)[8]>;
  { desc.ModeSense() } -> std::convertible_to<const uint8_t(&)[4]>;
};

template <interface TEndpoint, descriptor TDescriptor, const std::endian end = std::endian::native> class CScsi final {
  constexpr CScsi() = delete;
};

template <interface TEndpoint, descriptor TDescriptor> class CScsi<TEndpoint, TDescriptor, std::endian::little> {

  const TEndpoint &ep;
  static constexpr TDescriptor desc{};

  static constexpr auto _SectorSize =
      uint32_t(desc.Capacity()[4]) << 24 | uint32_t(desc.Capacity()[5]) << 16 | uint32_t(desc.Capacity()[6]) << 8 | uint32_t(desc.Capacity()[7]);

  static uint8_t sector[_SectorSize];

  inline void TestUnitReady(const SCbw &cbw) const {
    const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength, 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void RequestSense(const SCbw &cbw) const {
    ep.Transmit(desc.SenseData(), sizeof(desc.SenseData()));
    const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength - cbw.CBWCB[4], 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void Inquiry(const SCbw &cbw) const {
    if (0 == cbw.CBWCB[1]) {
      ep.Transmit(desc.Inquiry(), cbw.CBWCB[4]);
      const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength - cbw.CBWCB[4], 0U);
      const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
      ep.Transmit(buff);
    } else {
      SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength, 1U);
      const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
      ep.Transmit(buff);
      csw.bCSWStatus = 0U;
      ep.Transmit(buff);
    }
  }

  inline void ModeSense6(const SCbw &cbw) const {
    ep.Transmit(desc.ModeSense(), sizeof(desc.ModeSense()));
    const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength - cbw.CBWCB[4], 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void PreventAllowMediumRemoval(const SCbw &cbw) const {
    const SCsw csw(cbw.dCBWTag, 0, 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void ReadFormatCapacities6(const SCbw &cbw) const {
    ep.Transmit(desc.FormatCapacity(), sizeof(desc.FormatCapacity()));
    const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength - cbw.CBWCB[4], 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void ReadCapacity10(const SCbw &cbw) const {
    ep.Transmit(desc.Capacity(), sizeof(desc.Capacity()));
    const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength - cbw.CBWCB[4], 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void Read10(const SCbw &cbw) const {
    const std::unsigned_integral auto startAddress =
        ((uint32_t(cbw.CBWCB[2]) << 24) | (uint32_t(cbw.CBWCB[3]) << 16) | (uint32_t(cbw.CBWCB[4]) << 8) | (uint32_t(cbw.CBWCB[5])));
    const std::unsigned_integral auto size = ((uint32_t(cbw.CBWCB[7]) << 8) | (uint32_t(cbw.CBWCB[8])));

    for (unsigned k = 0U; k < size; k++) {

      ep.OnRead(startAddress * _SectorSize + k * _SectorSize, sector);

      const auto packetSize = ep.SizePacketMax();
      const auto packetNum = _SectorSize / packetSize;
      for (uint16_t j = 0; j < packetNum; j++) {
        const uint8_t(&buffer)[packetSize] = reinterpret_cast<uint8_t(&)[packetSize]>(sector[j * packetSize]);
        ep.Transmit(buffer, packetSize);
      }
    }

    const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength - cbw.CBWCB[4], 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void Write10(const SCbw &cbw) const {
    const std::unsigned_integral auto startAddress =
        ((uint32_t(cbw.CBWCB[2]) << 24) | (uint32_t(cbw.CBWCB[3]) << 16) | (uint32_t(cbw.CBWCB[4]) << 8) | (uint32_t(cbw.CBWCB[5])));
    const std::unsigned_integral auto size = ((uint32_t(cbw.CBWCB[7]) << 8) | (uint32_t(cbw.CBWCB[8])));

    for (unsigned k = 0U; k < size; k++) {
      const auto packetSize = ep.SizePacketMax();
      const auto packetNum = _SectorSize / packetSize;
      for (uint16_t j = 0; j < packetNum; j++) {
        uint8_t(&buffer)[packetSize] = reinterpret_cast<uint8_t(&)[packetSize]>(sector[j * packetSize]);
        while (!ep.Receive(buffer, packetSize)) {
        }
      }

      ep.OnWrite(startAddress * _SectorSize + k * _SectorSize, sector);
    }
    const SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength - cbw.CBWCB[4], 0U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
  }

  inline void Error(const SCbw &cbw) const {
    SCsw csw(cbw.dCBWTag, cbw.dCBWDataTransferLength, 1U);
    const uint8_t(&buff)[SCsw::size()] = reinterpret_cast<const uint8_t(&)[SCsw::size()]>(csw);
    ep.Transmit(buff);
    csw.bCSWStatus = 0U;
    ep.Transmit(buff);
  }

  struct STable {
    const ECommand command;
    void (CScsi::*function)(const SCbw &cbw) const;
  };

  static constexpr STable mTable[] = {{ECommand::TestUnitReady, &CScsi::TestUnitReady},
                                      {ECommand::RequestSense, &CScsi::RequestSense},
                                      {ECommand::Inquiry, &CScsi::Inquiry},
                                      {ECommand::ModeSense6, &CScsi::ModeSense6},
                                      {ECommand::PreventAllowMediumRemoval, &CScsi::PreventAllowMediumRemoval},
                                      {ECommand::ReadFormatCapacities6, &CScsi::ReadFormatCapacities6},
                                      {ECommand::ReadCapacity10, &CScsi::ReadCapacity10},
                                      {ECommand::Read10, &CScsi::Read10},
                                      {ECommand::Write10, &CScsi::Write10}};

public:
  constexpr CScsi(const TEndpoint &endpoint) : ep(endpoint) {}

  inline void operator()(const Type::Byte (&buf)[SCbw::size()]) const {
    const SCbw &Cbw = reinterpret_cast<const SCbw &>(buf);

    for (const auto &com : mTable) {
      if (com.command == static_cast<ECommand>(Cbw.CBWCB[0])) {
        (this->*com.function)(Cbw);
        scsiNumber++;
        return;
      }
    }

    Error(Cbw);
  }
};

template <interface TEndpoint, descriptor TDescriptor> uint8_t CScsi<TEndpoint, TDescriptor, std::endian::little>::sector[_SectorSize];

} // namespace iso::usb::scsi