#pragma once

#include "meta_types.hpp"
#include <concepts>

namespace hal::tim::basic {

template <typename T>
concept timer_basic = requires(const T &cl) {
  { cl.Init() } -> std::same_as<void>;
  { cl.Launch(iso::meta_type::const_v<uint32_t{}>) } -> std::same_as<void>;
  { cl.Launch(iso::meta_type::const_v<uint32_t{}>, iso::meta_type::const_v<bool{}>) } -> std::same_as<void>;
  { cl.UpdateEvent() } -> std::same_as<bool>;
  { T::InterruptHandler() } -> std::same_as<void>;
};
}