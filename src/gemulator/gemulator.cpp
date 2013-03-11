#include "emulator.h"

int main() {
  GBAEmulator emu("roms/rom.gba");
  emu.start();
  return 0;
}
