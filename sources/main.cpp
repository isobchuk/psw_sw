#include "integration.hpp"

static integration::CIntegration sIntegration;

int main() {
  while (true) {
    sIntegration();
  }

  return 0;
}
/*
static constexpr float Iout = 0.010;
static constexpr float Uout = 200.0;
static constexpr float Uin = 12.0;
static constexpr float Vr = 0.1;

static constexpr float D = 1 - (Uin / Uout);
static constexpr float R = Uout / Iout;
static constexpr float f = 50000.0;
static constexpr float k = 2;

static constexpr float r = (((Uin / Uout) * (1 / (1 - D))) - 1) * (R / ((1 - D) * (1 - D)));
static constexpr float L = (Uin * (Uout - Uin)) / ((k * Iout * (Uout / Uin)) * f * Uout);
static constexpr float dIl = (Uin * D) / (f * L);
static constexpr float Imax = (dIl / k) + (Iout / (1 - D));
static constexpr float Cmin = Iout / (Vr * f);*/
