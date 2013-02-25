#include "emulator.h"

int main() { 
  GBAEmulator emulator("roms/rom.gba");
  emulator.start();
  return 0;
}
