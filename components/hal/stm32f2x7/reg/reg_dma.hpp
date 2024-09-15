#pragma once

#include "register.hpp"

namespace stm32f217::registers::dma {

template <const cpp_register::RegisterAddress address> struct DMA_T {
  static constexpr cpp_register::Register<address + 0x0, cpp_register::AccessMode::R, uint32_t, struct LISR> LISR{};
  static constexpr cpp_register::Register<address + 0x4, cpp_register::AccessMode::R, uint32_t, struct HISR> HISR{};
  static constexpr cpp_register::Register<address + 0x8, cpp_register::AccessMode::W, uint32_t, struct LIFCR> LIFCR{};
  static constexpr cpp_register::Register<address + 0xC, cpp_register::AccessMode::W, uint32_t, struct HIFCR> HIFCR{};

  static constexpr cpp_register::Register<address + 0x10, cpp_register::AccessMode::RW, uint32_t, struct SCR, 8, 0x18> SCR{};
  static constexpr cpp_register::Register<address + 0x14, cpp_register::AccessMode::RW, uint32_t, struct SNDTR, 8, 0x18> SNDTR{};
  static constexpr cpp_register::Register<address + 0x18, cpp_register::AccessMode::RW, uint32_t, struct SPAR, 8, 0x18> SPAR{};
  static constexpr cpp_register::Register<address + 0x1C, cpp_register::AccessMode::RW, uint32_t, struct SM0AR, 8, 0x18> SM0AR{};
  static constexpr cpp_register::Register<address + 0x20, cpp_register::AccessMode::RW, uint32_t, struct SM1AR, 8, 0x18> SM1AR{};
  static constexpr cpp_register::Register<address + 0x24, cpp_register::AccessMode::RW, uint32_t, struct SFCR, 8, 0x18> SFCR{};
};

inline constexpr DMA_T<0x40026400> const *DMA2{};
inline constexpr DMA_T<0x40026000> const *DMA1{};

struct DMA_LISR {
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 27), cpp_register::AccessMode::R, 1> TCIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 26), cpp_register::AccessMode::R, 1> HTIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 25), cpp_register::AccessMode::R, 1> TEIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 24), cpp_register::AccessMode::R, 1> DMEIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 22), cpp_register::AccessMode::R, 1> FEIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 21), cpp_register::AccessMode::R, 1> TCIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 20), cpp_register::AccessMode::R, 1> HTIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 19), cpp_register::AccessMode::R, 1> TEIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 18), cpp_register::AccessMode::R, 1> DMEIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 16), cpp_register::AccessMode::R, 1> FEIF2{};

  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 11), cpp_register::AccessMode::R, 1> TCIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 10), cpp_register::AccessMode::R, 1> HTIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 9), cpp_register::AccessMode::R, 1> TEIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 8), cpp_register::AccessMode::R, 1> DMEIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 6), cpp_register::AccessMode::R, 1> FEIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 5), cpp_register::AccessMode::R, 1> TCIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 4), cpp_register::AccessMode::R, 1> HTIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 3), cpp_register::AccessMode::R, 1> TEIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 2), cpp_register::AccessMode::R, 1> DMEIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LISR), (1UL << 0), cpp_register::AccessMode::R, 1> FEIF0{};
};

struct DMA_HISR {
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 27), cpp_register::AccessMode::R, 1> TCIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 26), cpp_register::AccessMode::R, 1> HTIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 25), cpp_register::AccessMode::R, 1> TEIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 24), cpp_register::AccessMode::R, 1> DMEIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 22), cpp_register::AccessMode::R, 1> FEIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 21), cpp_register::AccessMode::R, 1> TCIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 20), cpp_register::AccessMode::R, 1> HTIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 19), cpp_register::AccessMode::R, 1> TEIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 18), cpp_register::AccessMode::R, 1> DMEIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 16), cpp_register::AccessMode::R, 1> FEIF6{};

  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 11), cpp_register::AccessMode::R, 1> TCIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 10), cpp_register::AccessMode::R, 1> HTIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 9), cpp_register::AccessMode::R, 1> TEIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 8), cpp_register::AccessMode::R, 1> DMEIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 6), cpp_register::AccessMode::R, 1> FEIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 5), cpp_register::AccessMode::R, 1> TCIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 4), cpp_register::AccessMode::R, 1> HTIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 3), cpp_register::AccessMode::R, 1> TEIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 2), cpp_register::AccessMode::R, 1> DMEIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HISR), (1UL << 0), cpp_register::AccessMode::R, 1> FEIF4{};
};

struct DMA_LIFCR {
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 27), cpp_register::AccessMode::W, 1> CTCIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 26), cpp_register::AccessMode::W, 1> CHTIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 25), cpp_register::AccessMode::W, 1> CTEIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 24), cpp_register::AccessMode::W, 1> CDMEIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 22), cpp_register::AccessMode::W, 1> CFEIF3{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 21), cpp_register::AccessMode::W, 1> CTCIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 20), cpp_register::AccessMode::W, 1> CHTIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 19), cpp_register::AccessMode::W, 1> CTEIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 18), cpp_register::AccessMode::W, 1> CDMEIF2{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 16), cpp_register::AccessMode::W, 1> CFEIF2{};

  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 11), cpp_register::AccessMode::W, 1> CTCIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 10), cpp_register::AccessMode::W, 1> CHTIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 9), cpp_register::AccessMode::W, 1> CTEIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 8), cpp_register::AccessMode::W, 1> CDMEIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 6), cpp_register::AccessMode::W, 1> CFEIF1{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 5), cpp_register::AccessMode::W, 1> CTCIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 4), cpp_register::AccessMode::W, 1> CHTIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 3), cpp_register::AccessMode::W, 1> CTEIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 2), cpp_register::AccessMode::W, 1> CDMEIF0{};
  static constexpr cpp_register::Field<decltype(DMA2->LIFCR), (1UL << 0), cpp_register::AccessMode::W, 1> CFEIF0{};
};

struct DMA_HIFCR {
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 27), cpp_register::AccessMode::W, 1> CTCIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 26), cpp_register::AccessMode::W, 1> CHTIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 25), cpp_register::AccessMode::W, 1> CTEIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 24), cpp_register::AccessMode::W, 1> CDMEIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 22), cpp_register::AccessMode::W, 1> CFEIF7{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 21), cpp_register::AccessMode::W, 1> CTCIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 20), cpp_register::AccessMode::W, 1> CHTIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 19), cpp_register::AccessMode::W, 1> CTEIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 18), cpp_register::AccessMode::W, 1> CDMEIF6{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 16), cpp_register::AccessMode::W, 1> CFEIF6{};

  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 11), cpp_register::AccessMode::W, 1> CTCIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 10), cpp_register::AccessMode::W, 1> CHTIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 9), cpp_register::AccessMode::W, 1> CTEIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 8), cpp_register::AccessMode::W, 1> CDMEIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 6), cpp_register::AccessMode::W, 1> CFEIF5{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 5), cpp_register::AccessMode::W, 1> CTCIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 4), cpp_register::AccessMode::W, 1> CHTIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 3), cpp_register::AccessMode::W, 1> CTEIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 2), cpp_register::AccessMode::W, 1> CDMEIF4{};
  static constexpr cpp_register::Field<decltype(DMA2->HIFCR), (1UL << 0), cpp_register::AccessMode::W, 1> CFEIF4{};
};

struct DMA_SCR {
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 25), cpp_register::AccessMode::RW, 3> CHSEL{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 23), cpp_register::AccessMode::RW, 2> MBURST{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 21), cpp_register::AccessMode::RW, 2> PBURST{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 19), cpp_register::AccessMode::RW, 1> CT{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 18), cpp_register::AccessMode::RW, 1> DBM{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 16), cpp_register::AccessMode::RW, 2> PL{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 15), cpp_register::AccessMode::RW, 1> PINCOS{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 13), cpp_register::AccessMode::RW, 2> MSIZE{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 11), cpp_register::AccessMode::RW, 2> PSIZE{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 10), cpp_register::AccessMode::RW, 1> MINC{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 9), cpp_register::AccessMode::RW, 1> PINC{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 8), cpp_register::AccessMode::RW, 1> CIRC{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 6), cpp_register::AccessMode::RW, 2> DIR{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 5), cpp_register::AccessMode::RW, 1> PFCTRL{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 4), cpp_register::AccessMode::RW, 1> TCIE{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 3), cpp_register::AccessMode::RW, 1> HTIE{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 2), cpp_register::AccessMode::RW, 1> TEIE{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 1), cpp_register::AccessMode::RW, 1> DMEIE{};
  static constexpr cpp_register::Field<decltype(DMA2->SCR), (1UL << 0), cpp_register::AccessMode::RW, 1> EN{};
};

struct DMA_SNDTR {
  static constexpr cpp_register::Field<decltype(DMA2->SNDTR), (1UL << 0), cpp_register::AccessMode::RW, 16> NDT{};
};

struct DMA_SPAR {
  static constexpr cpp_register::Field<decltype(DMA2->SPAR), (1UL << 0), cpp_register::AccessMode::RW, 32> PA{};
};

struct DMA_SM0AR {
  static constexpr cpp_register::Field<decltype(DMA2->SM0AR), (1UL << 0), cpp_register::AccessMode::RW, 32> M0A{};
};

struct DMA_SM1AR {
  static constexpr cpp_register::Field<decltype(DMA2->SM1AR), (1UL << 0), cpp_register::AccessMode::RW, 32> M1A{};
};

struct DMA_SFCR {
  static constexpr cpp_register::Field<decltype(DMA2->SFCR), (1UL << 7), cpp_register::AccessMode::RW, 1> FEIE{};
  static constexpr cpp_register::Field<decltype(DMA2->SFCR), (1UL << 3), cpp_register::AccessMode::R, 3> FS{};
  static constexpr cpp_register::Field<decltype(DMA2->SFCR), (1UL << 2), cpp_register::AccessMode::RW, 1> DMDIS{};
  static constexpr cpp_register::Field<decltype(DMA2->SFCR), (1UL << 0), cpp_register::AccessMode::RW, 2> FTH{};
};

} // namespace stm32f217::registers::dma
