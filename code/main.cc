#include "bits/RogueCMI.h"
#include <ctime>

int main() {
  std::srand(static_cast<unsigned int>(std::time(nullptr)));
  rCMI::RogueCMI rogueCMI;
  rogueCMI.run();
  return 0;
}