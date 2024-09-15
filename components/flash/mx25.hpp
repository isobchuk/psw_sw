#pragma once

#include "flash.hpp"
#include "hal_gpio.hpp"
#include "hal_spim.hpp"
#include "meta_types.hpp"

namespace flash::mx25 {

consteval unsigned long long int operator""_K(unsigned long long int val) { return val * 1024; }
consteval unsigned long long int operator""_M(unsigned long long int val) { return val * 1024 * 1024; }

template <hal::spim::spi_master Spi, hal::gpio::gpio_out Wp> class CMX25 final {

  static constexpr auto PAGE_SIZE = 256U;
  static constexpr auto SECTOR_SIZE = 4_K;

  template <const size_t address> inline static constexpr auto _IsPageAligned = (0U == address % PAGE_SIZE);
  template <const size_t address> inline static constexpr auto _IsSectorAligned = (0U == address % SECTOR_SIZE);

  struct SJedegId {
    static constexpr auto _IdSize = 2U;
    const uint8_t _Mf;
    const uint8_t _Id[_IdSize];

    constexpr SJedegId(const uint8_t mf, const uint8_t (&id)[_IdSize]) : _Mf(mf), _Id{id[0], id[1]} {}

    inline bool operator==(const SJedegId &oth) const { return (_Mf == oth._Mf) && (_Id[0] == oth._Id[0]) && (_Id[1] == oth._Id[1]); }
  };

  static constexpr SJedegId _JedecIdList[] = {SJedegId(0xC2, {0x28, 0x17}), SJedegId(0xEF, {0x40, 0x16})};

  enum class ECommands : uint8_t {
    PageProgram = 0x02U,
    PageRead = 0x03U,
    GetStatus = 0x05U,
    WriteEnable = 0x06U,
    SectorErase = 0x20U,
    ChipErase = 0x60U,
    ReadId = 0x9FU

  };

  const Spi &_Spi;
  const Wp _Wp;

public:
  consteval CMX25(const Spi &spi, const Wp we)
  requires ::hardware::flash::flash_memory_spi<CMX25>
      : _Spi(spi), _Wp(we) {}

  [[nodiscard]] inline bool Connect() const {
    hal::spim::STransmitReceiveBuffer messageGetId({static_cast<std::underlying_type_t<ECommands>>(ECommands::ReadId)}, iso::meta_type::const_v<3U>);

    if (_Spi.TransmitReceive(messageGetId)) {
      const SJedegId realId(messageGetId._Rx[0], {messageGetId._Rx[1], messageGetId._Rx[2]});

      bool found = false;
      for (const auto &jid : _JedecIdList) {
        if (jid == realId) {
          found = true;
          break;
        }
      }
      return found;
    }

    return false;
  }

  inline bool WriteEnable() const {
    _Wp.Write(iso::meta_type::const_v<true>);
    hal::spim::STransmitReceiveBuffer message({static_cast<std::underlying_type_t<ECommands>>(ECommands::WriteEnable)});
    return _Spi.TransmitReceive(message);
  }

  inline uint8_t GetStatus() const {
    hal::spim::STransmitReceiveBuffer message({static_cast<std::underlying_type_t<ECommands>>(ECommands::GetStatus)}, iso::meta_type::const_v<1U>);
    if (_Spi.TransmitReceive(message)) {
      return message._Rx[0];
    }
    return 0xFFU;
  }

  inline bool SectorErase(const size_t address) const {

    uint8_t message[4];

    message[0] = static_cast<std::underlying_type_t<ECommands>>(ECommands::SectorErase);
    message[1] = static_cast<uint8_t>((address & 0x00FF0000) >> 16);
    message[2] = static_cast<uint8_t>((address & 0x0000FF00) >> 8);
    message[3] = static_cast<uint8_t>((address & 0x000000FF) >> 0);

    while (1U & GetStatus()) {
    }

    WriteEnable();

    hal::spim::STransmitReceiveBuffer messageWrite(message);
    if (!_Spi.TransmitReceive(messageWrite)) {
      return false;
    }
    return true;
  }

  template <iso::meta_type::const_value_of_type<size_t> Address>
  requires _IsSectorAligned<Address::value>
  inline bool SectorErase(const Address) const {

    constexpr uint8_t message[4] = {
        static_cast<std::underlying_type_t<ECommands>>(ECommands::SectorErase), static_cast<uint8_t>((Address::value & 0x00FF0000) >> 16),
        static_cast<uint8_t>((Address::value & 0x0000FF00) >> 8), static_cast<uint8_t>((Address::value & 0x000000FF) >> 0)};

    while (1U & GetStatus()) {
    }

    WriteEnable();

    hal::spim::STransmitReceiveBuffer messageWrite(message);
    if (!_Spi.TransmitReceive(messageWrite)) {
      return false;
    }
    return true;
  }

  inline bool ChipErase() const {
    hal::spim::STransmitReceiveBuffer message({static_cast<std::underlying_type_t<ECommands>>(ECommands::ChipErase)});
    return _Spi.TransmitReceive(message);
  }

  template <const unsigned N> inline bool PageProgram(const size_t address, const uint8_t (&buffer)[N]) const {
    uint8_t message[N + 4];
    message[0] = static_cast<std::underlying_type_t<ECommands>>(ECommands::PageProgram);
    message[1] = static_cast<uint8_t>((address & 0x00FF0000) >> 16);
    message[2] = static_cast<uint8_t>((address & 0x0000FF00) >> 8);
    message[3] = static_cast<uint8_t>((address & 0x000000FF) >> 0);

    for (unsigned i = 0; i < sizeof(buffer); i++) {
      message[i + 4] = buffer[i];
    }

    while (1U & GetStatus()) {
    }

    WriteEnable();

    hal::spim::STransmitReceiveBuffer messageWrite(message);
    if (!_Spi.TransmitReceive(messageWrite)) {
      return false;
    }

    return true;
  }

  template <iso::meta_type::const_value_of_type<size_t> Address, const unsigned N>
  requires _IsPageAligned<Address::value>
  inline bool PageProgram(const Address, const uint8_t (&buffer)[N]) const {
    uint8_t message[N + 4];
    message[0] = static_cast<std::underlying_type_t<ECommands>>(ECommands::PageProgram);
    message[1] = static_cast<uint8_t>((Address::value & 0x00FF0000) >> 16);
    message[2] = static_cast<uint8_t>((Address::value & 0x0000FF00) >> 8);
    message[3] = static_cast<uint8_t>((Address::value & 0x000000FF) >> 0);

    for (unsigned i = 0; i < sizeof(buffer); i++) {
      message[i + 4] = buffer[i];
    }

    while (1U & GetStatus()) {
    }

    WriteEnable();

    hal::spim::STransmitReceiveBuffer messageWrite(message);
    if (!_Spi.TransmitReceive(messageWrite)) {
      return false;
    }

    return true;
  }

  template <const unsigned N> inline bool PageRead(const size_t address, uint8_t (&buffer)[N]) const {
    uint8_t message[4];

    message[0] = static_cast<std::underlying_type_t<ECommands>>(ECommands::PageRead);
    message[1] = static_cast<uint8_t>((address & 0x00FF0000) >> 16);
    message[2] = static_cast<uint8_t>((address & 0x0000FF00) >> 8);
    message[3] = static_cast<uint8_t>((address & 0x000000FF) >> 0);

    while (1U & GetStatus()) {
    }

    hal::spim::STransmitReceiveBuffer messageWrite(message, iso::meta_type::const_v<N>);
    if (!_Spi.TransmitReceive(messageWrite)) {
      return false;
    }

    for (unsigned i = 0; i < sizeof(buffer); i++) {
      buffer[i] = messageWrite._Rx[i];
    }
    return true;
  }

  template <iso::meta_type::const_value_of_type<size_t> Address, const unsigned N>
  requires _IsPageAligned<Address::value>
  inline bool PageRead(const Address, uint8_t (&buffer)[N]) const {
    constexpr uint8_t message[] = {
        static_cast<std::underlying_type_t<ECommands>>(ECommands::PageRead), static_cast<uint8_t>((Address::value & 0x00FF0000) >> 16),
        static_cast<uint8_t>((Address::value & 0x0000FF00) >> 8), static_cast<uint8_t>((Address::value & 0x000000FF) >> 0)};

    while (1U & GetStatus()) {
    }

    hal::spim::STransmitReceiveBuffer messageWrite(message, iso::meta_type::const_v<N>);
    if (!_Spi.TransmitReceive(messageWrite)) {
      return false;
    }

    for (unsigned i = 0; i < sizeof(buffer); i++) {
      buffer[i] = messageWrite._Rx[i];
    }
    return true;
  }
};
} // namespace flash::mx25