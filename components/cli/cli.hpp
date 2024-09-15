#pragma once

#include <concepts>
#include <cstddef>
#include <cstdint>

namespace iso::cli {

struct SCommand {
  static constexpr auto _CommandLengthMax = 32U;
  static constexpr auto _DescriptionLengthMax = 128U;

  char command[_CommandLengthMax];
  void (&function)(unsigned argc, char *argv[]);
  char description[_DescriptionLengthMax];

  template <const std::size_t NC, const std::size_t ND>
  consteval SCommand(const char (&c)[NC], void (&f)(unsigned argc, char *argv[]), const char (&d)[ND]) : command{}, function(f), description{} {
    for (decltype(NC) i = 0; i < NC; i++) {
      command[i] = c[i];
    }
    for (decltype(ND) i = 0; i < ND; i++) {
      description[i] = d[i];
    }
  }
};

template <typename T>
concept interface = requires(const T interface, const char (&buffer)[16]) {
  { interface.GetByte(*new char) } -> std::same_as<bool>;
  { interface.Transmit(buffer) } -> std::same_as<bool>;
};

template <const unsigned N, interface Interface> class CCommandLineInterface final {

  const SCommand (&_TableCommands)[N];
  const Interface &_Interface;

public:
  consteval CCommandLineInterface(const SCommand (&table)[N], const Interface &inter) : _TableCommands(table), _Interface(inter) {}

  inline void operator()() const {
    enum class EState { Invitation, ReadCommand, PerformCommand };

    static EState state;
    static char commandBuffer[32];
    static unsigned counter = 0;

    static unsigned argc = 0;
    static char *argv[5] = {};

    switch (state) {
    case EState::Invitation: {
      constexpr char invitation[] = "$ ";
      _Interface.Transmit(invitation);
      state = EState::ReadCommand;
    } break;

    case EState::ReadCommand: {
      char byte = 0;
      if (_Interface.GetByte(byte)) {
        constexpr char enter = 0xD;
        if (enter != byte) {
          if ((sizeof(commandBuffer) - 1) != counter) {
            const char echo[1] = {byte};
            _Interface.Transmit(echo);
            commandBuffer[counter++] = byte;
          }
        } else {
          commandBuffer[counter] = '\0';
          constexpr char end[] = "\r\n";
          _Interface.Transmit(end);

          for (std::size_t i = 0; i < counter; i++) {
            if (' ' == commandBuffer[i]) {
              commandBuffer[i++] = '\0';

              while (('-' != commandBuffer[i]) || (enter != commandBuffer[i])) {
              }
              argv[argc++] = &commandBuffer[++i];
            }
          }

          state = EState::PerformCommand;
        }
      }
    } break;

    case EState::PerformCommand: {
      for (const auto &comm : _TableCommands) {
        bool notEqual = false;
        for (std::size_t i = 0; i < sizeof(comm.command); i++) {
          if (comm.command[i] != commandBuffer[i]) {
            notEqual = true;
            break;
          }
        }

        if (notEqual) {
          continue;
        }

        comm.function(argc, argv);
      }
      for (auto &b : commandBuffer) {
        b = '\0';
      }
      counter = 0;
      state = EState::Invitation;
    } break;

    default:
      break;
    }
  }
};

} // namespace iso::cli