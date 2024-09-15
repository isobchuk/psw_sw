#include "integration.hpp"

static integration::CIntegration sIntegration;

int main() {
  while (true) {
    sIntegration();
  }

  return 0;
}
