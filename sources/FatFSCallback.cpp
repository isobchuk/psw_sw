#include "integration.hpp"

extern "C" int FlashStatus() { return 0; }
extern "C" int FlashInit() { return 0; }

extern "C" int FlashRead(const unsigned long address, const uint8_t count, uint8_t *buffer) {
  constexpr auto sizeSector = integration::CIntegration::GetFlash().SizeSector();
  constexpr auto sizePage = integration::CIntegration::GetFlash().SizePage();

  for (unsigned k = 0; k < count; k++) {
    for (unsigned i = 0; i < sizeSector; i += sizePage) {
      uint8_t(&buf)[sizePage] = reinterpret_cast<uint8_t(&)[sizePage]>(buffer[i]);
      integration::CIntegration::GetFlash().PageRead(static_cast<size_t>(address * sizeSector + k * sizeSector + i), buf);
    }
  }
  return 0;
}

extern "C" int FlashWrite(const unsigned long address, const uint8_t count, const uint8_t *buffer) {
  constexpr auto sizeSector = integration::CIntegration::GetFlash().SizeSector();
  constexpr auto sizePage = integration::CIntegration::GetFlash().SizePage();

  for (unsigned k = 0; k < count; k++) {
    integration::CIntegration::GetFlash().SectorErase(static_cast<size_t>(address * sizeSector + k * sizeSector));
    for (unsigned i = 0; i < sizeSector; i += sizePage) {
      const uint8_t(&buf)[sizePage] = reinterpret_cast<const uint8_t(&)[sizePage]>(buffer[i]);
      integration::CIntegration::GetFlash().PageProgram(static_cast<size_t>(address * sizeSector + k * sizeSector + i), buf);
    }
  }
  return 0;
}