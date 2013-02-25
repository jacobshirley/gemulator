#include <iostream>
#include <cmath>

#include "emulator.h"
#include "debug.h"
#include "cpu.h"

using namespace debug;
using namespace flags;

GBAEmulator::GBAEmulator(const char* file_name) {
   rom = new ROM(file_name);
   cpu = new CPU(rom->get_bytes());
}

void GBAEmulator::start() {
  output("step 1------------------------");
  step();
  output("step 2------------------------");
  step();
}

void GBAEmulator::step() {
  cpu->step();
}

void GBAEmulator::stop() {

}

GBAEmulator::~GBAEmulator() {
  delete rom;
}
