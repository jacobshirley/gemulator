#ifndef _CPU_H
#define _CPU_H

#define INSTR_SIZE_B 4
#include "rom.h"

namespace flags {
  const byte COND_EQ = 0x0;
  const byte COND_NQ = 0x1;
  const byte COND_AL = 0xE;
  const byte COND_R = 0xF;
}

class CPU {
  private:
    const byte* data;

    word* instructions;
    word* registers;

    word pc;

    word slice(word instr, int big, int small);
    void dataProc(word instr);
  public:
    CPU(const byte* data);

    //register functions
    const word* get_registers();
    word regv(byte regn);
    void setreg(byte rd, byte imm);

    //instruction functions
    word nextinstr();
    word previnstr();
    word get_cur_instr();

    //pc
    void setpc(word address);    
    word getpc();

    //process
    void step();
    ~CPU();
};

#endif
