#ifndef _CPU_ARM_H
#define _CPU_ARM_H

#include <assert.h>
#include "cpu.h"
#include "debug.h"
#include "llvm/ARMDefines.h"
#include "llvm/ARMUtils.h"

#include <vector>
#include <cmath>

using namespace debug;

class CPU;

class CPUARM {
  CPU* cpu; 
  
  typedef void (*ARMInstr) (CPU* cpu, word instr);
  typedef vector<ARMInstr> instruction_v;

  instruction_v* arm_instrs;
  instruction_v* data_proc_instrs; 

  //----- inner workings ---------

  //max of big = 31
  //min of small = 0

  static word slice(word instr, int big, int small) {
    int r = 1;
    for (int i = 1; i <= (big - small); ++i) {
      r += pow(2, i);
    }
    return (instr >> small) & r;
  }

  static inline void dataProc_handle_imm(CPU* cpu, word instr, bool &sf, byte &rn, byte &rd, byte &imm12) {
    sf = slice(instr, 21, 20);
    rn = slice(instr, 19, 16);
    rd = slice(instr, 15, 12);
    imm12 = slice(instr, 11, 0);
  }

  static void cpuNULL(CPU* cpu, word instr) { output("Unknown func: "+int_to_str(instr));}

  static void dataProc_Imm(CPU* cpu, word instr) {
    byte op1 = slice(instr, 24, 21);
    uint32_t carry;
    bool sf;
    byte rn, rd, imm12;
    dataProc_handle_imm(cpu, instr, sf, rn, rd, imm12);

    word imm32 = lldb_private::ARMExpandImm_C(imm12, 0, carry);
    switch (op1) {
      case 0x0: { word r = cpu->regv(rn) & imm32; cpu->setreg(rd, r, sf); break;} //bitwise AND
      case 0x1: { word r = cpu->regv(rn) ^ imm32; cpu->setreg(rd, r, sf); break;} //bitwise exclusive OR
      case 0x2: { word r = cpu->regv(rn) - imm32; cpu->setreg(rd, r, sf); break;} //subtract
      case 0x3: { word r = imm32 - cpu->regv(rn); cpu->setreg(rd, r, sf); break;} //reverse subtract
      case 0x4: { word r = cpu->regv(rn) + imm32; cpu->setreg(rd, r, sf); break;} //add

      case 0x5: { word r = cpu->regv(rn) + imm32 + (cpu->flags & flags::CPU_FLAG_C); 
		  cpu->setreg(rd, r, sf);  
		  break;
		} //add with carry
      case 0x6: { word r = cpu->regv(rn) + ~imm32 + (cpu->flags & flags::CPU_FLAG_C);
      		  cpu->setreg(rd, r, sf); 
		  break;
		} //sub with carry
      case 0x7: { word r = imm32 + ~(cpu->regv(rn)) + (cpu->flags & flags::CPU_FLAG_C);
      		  cpu->setreg(rd, r, sf);
		  break;
		} //reverse sub with carry

      case 0x8: { cpu->setflags(cpu->regv(rn) & imm32); break;} //test
      case 0x9: { cpu->setflags(cpu->regv(rn) ^ imm32); break;} //test equiv

      case 0xA: { cpu->setflags(cpu->regv(rn) + ~imm32 + 1); break;} //cmp
      case 0xB: { cpu->setflags(cpu->regv(rn) + imm32); break;} //cmp neg

      case 0xC: { word r = cpu->regv(rn) | imm32; cpu->setreg(rd, r, sf); break;} //bitwise OR
      case 0xD: { cpu->setreg(rd, imm32, sf); break;} //mov
      case 0xE: { word r = cpu->regv(rn) & ~imm32; cpu->setreg(rd, r, sf); break;} //bitwise bit clear
      case 0xF: { cpu->setreg(rd, ~imm32, sf); break;} //bitwise NOT (mvn)
    }
  }
  static inline word shift(CPU* cpu, byte shift_t, byte imm5) {
    switch (shift_t) {
      case 0x0: return 0; 
    }
  }

  static void dataProc_Reg(CPU* cpu, word instr) {
    byte op1 = slice(instr, 24, 21);

    //ops
    bool c = slice(instr, 21, 20);
    byte rn = slice(instr, 19, 16);
    byte rd = slice(instr, 15, 12);
    byte imm5 = slice(instr, 11, 7);
    byte type = slice(instr, 6, 5);
    byte rm = slice(instr, 3, 0);
    
    lldb_private::ARM_ShifterType shift_t;
    uint32_t shift_n = lldb_private::DecodeImmShift(type, imm5, shift_t);
    
    int fg = lldb_private::Bits32(254, 7, 1);
    output(fg);

    uint32_t carry_out;
    uint32_t shifted = lldb_private::Shift_C(cpu->regv(rm), shift_t, shift_n, 0, carry_out);

    output("Func to-be-added: "+int_to_str(op1));
    
    switch (op1) {
      case 0x9: {
      		  output("TEQ");
		  cpu->setreg(rd, rn ^ shifted, c);
		  break;
		}
    }
    
    /*switch (op1) {
      case 0x10: {}
      case 0x11: {}
      case 0x12: {}
      case 0x13: {}
      case 0x14: {}
      case 0x15: {}
      case 0x16: {output("On of these");}
    }*/
  }

  static void branch(CPU* cpu, word instr) {
    byte op = slice(instr, 24, 23);
                
    if (op)
      debug::output("branch with link");
    else { 
      debug::output("branch with no link");
      word target = slice(instr, 23, 0);
      cpu->setpc(2+target);
    } 
  }
  
  public:
  
  CPUARM(CPU* cpu) {
    this->cpu = cpu;
    arm_instrs = new instruction_v{dataProc_Reg, dataProc_Imm, cpuNULL, cpuNULL, cpuNULL, branch};
  }

  ~CPUARM() {
    delete arm_instrs;
  }

  int execute(word instr) {
    (*arm_instrs)[slice(instr, 27, 25)](cpu, instr);  
  }
};

#endif
