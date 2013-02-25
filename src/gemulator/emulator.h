#ifndef _EMULATOR_H
#define _EMULATOR_H

#include "rom.h"
#include "cpu.h"

class GBAEmulator {
  private:
    ROM* rom;
    CPU* cpu;
    void step();
    word slice(word instr, int big, int small);

    void dataProc(word instr);
  public:
    GBAEmulator(const char* file_name);
    void start();
    void stop();
    void pause();

    ~GBAEmulator();
};

#endif
