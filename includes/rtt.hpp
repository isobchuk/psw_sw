
extern "C" {
#include "SEGGER_RTT.h"
}

#include "hw.hpp"

namespace integration {

// Debug tracing
class CRttOutput final {
public:
  inline consteval CRttOutput() = default;

  inline void Init() const { SEGGER_RTT_Init(); }
  template <const std::size_t N> inline void puts(const char (&buf)[N]) const { SEGGER_RTT_WriteString(0, buf); }
  inline unsigned tick() const { return hardware::_SystemTime.Time(); }
};
} // namespace integration