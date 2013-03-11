#ifndef _CPU_H
#define _CPU_H

#include "rom.h"

namespace flags {
  const byte COND_EQ = 0x0;
  const byte COND_NQ = 0x1;
  const byte COND_AL = 0xE;
  const byte COND_R = 0xF;

  const word CPU_FLAG_N = 0x80000000;
  const word CPU_FLAG_Z = 0x40000000;
  const word CPU_FLAG_C = 0x20000000;
}

class CPUARM;

class CPU { 
  private:
    const byte* data;
    word* instructions;
    word* registers;

    word pc;
    
    CPUARM* armcpu; 
    

    word slice(word instr, int big, int small);
    void dataProc(word instr);
  public:
    word flags;

    CPU(const byte* data);

    //register functions
    const word* get_registers();
    word regv(byte regn);
    void setreg(byte rd, word imm, bool flags);

    //instruction functions
    word nextinstr();
    word previnstr();
    word get_cur_instr();

    //pc
    void setpc(word address);    
    word getpc();

    //flags
    void setflags(word val);

    //process
    void step();
    ~CPU();
};

#endif
