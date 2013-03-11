#include <cmath>
#include <string.h>

#include "cpu.h"
#include "cpu-arm.h"
#include "debug.h"

using namespace flags;
using namespace debug;

CPU::CPU(const byte* data) {
  this->data = data;
  registers = new word[32];
  pc = 0;
  flags = 0;
  instructions = (word*)data;
  armcpu = new CPUARM(this);
}

CPU::~CPU() {
  delete registers;
}

//----- inner workings ---------

//max of big = 31
//min of small = 0

word CPU::slice(word instr, int big, int small) {
  int r = 1;
  for (int i = 1; i <= (big - small); ++i) {
    r += pow(2, i);
  }
  return (instr >> small) & r;
}

void CPU::dataProc(word instr) {
  byte op = slice(instr, 27, 25);
  if (op == 0) {
     
  } else {
    byte op1 = slice(instr, 24, 21);
    switch (op1) {
 	case 0x8: {
	  output("16-bit immediate load");
	  break;
	}
	case 0xA: {
	  output("High halfword 16-bit immediate load");
	  break;
	}
	case 0x9: case 0xB: {
	  output("MSR and hints");
	  break;
	}
    } 
  }
}

void CPU::setflags(word val) {
  flags |= (val & flags::CPU_FLAG_N); //negative
  if (val == 0) flags |= flags::CPU_FLAG_Z; else flags &= ~(flags::CPU_FLAG_Z);//zero
  //more to come: carry, overflow, overflow | condition.
}

void CPU::step() {
  word instr = get_cur_instr();
  byte cond = instr >> 28;
 
  if (cond != COND_R) {
    byte op1 = slice(instr, 27, 25);
    output("op1: "+int_to_str(op1));
    armcpu->execute(instr);
  }
}

const word* CPU::get_registers() {
  return registers;
}

void CPU::setreg(byte rd, word imm, bool flags) {
  registers[rd] = imm;
  if (flags)
    setflags(imm);
}

word CPU::regv(byte regn) {
  return registers[regn];
}

void CPU::setpc(word address) {
  this->pc = address;
}

word CPU::getpc() {
  return pc;
}

word CPU::nextinstr() {
  pc++;
  return get_cur_instr();
}

word CPU::previnstr() {
  pc--;
  return get_cur_instr();
}

word CPU::get_cur_instr() {
  return instructions[pc];
}
