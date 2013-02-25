#include <cmath>
#include <string.h>

#include "cpu.h"
#include "debug.h"

using namespace flags;
using namespace debug;

CPU::CPU(const byte* data) {
  this->data = data;

  pc = 0;

  int len = sizeof(data);
  output(len/4);
  instructions = (word*)data;
  //memcpy(instructions, data, len);
}

CPU::~CPU() {
  delete instructions;
  delete registers;
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
	default: {
	  output("Other data-processing");

	  switch (op1) {
	    case 0x0: {break;} //bitwise AND
	    case 0x1: {break;} //bitwise exclusive OR
	    case 0x2: {break;} //subtract
	    case 0x3: {break;} //reverse subtract
	    case 0x4: {break;} //add
	    case 0x5: {break;} //add with carry
	    case 0x6: {break;} //sub with carry
	    case 0x7: {break;} //reverse sub with carry
	    case 0x8: {break;} //test
	    case 0x9: {break;} //test equiv
	    case 0xA: {break;} //cmp
	    case 0xB: {break;} //cmp neg
	    case 0xC: {break;} //bitwise OR
	    case 0xD: { //mov
	                output("MOV!!");
			bool s = slice(instr, 20, 20);
			byte rd = slice(instr, 15, 12);
			byte imm12 = slice(instr, 11, 0);
		        break;
		      } 
	    case 0xE: {break;} //bitwise bit clear
	    case 0xF: {break;} //bitwise NOT
	  }

	  break;
	}
    } 
  }  
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

void CPU::step() {
  word instr = get_cur_instr();
  byte cond = slice(instr, 31, 28);
 
  if (cond != COND_R) {
    byte op1 = slice(instr, 27, 25);
    output("op1: "+int_to_str(op1));
    switch (op1) {
      case 0x0: case 0x1: {
	output("data processing");

	dataProc(instr);	
  
	break;
      }
      case 0x2: {
	output("load/store byte");
	break;
      }
      case 0x3: {
	output("load/store byte again");
	break;
      }
      case 0x4: {
	output("block data");
	break;
      }
      case 0x5: {
        //output("branch data");
	
	byte op = slice(instr, 25, 20);
        
 	output(op);
            
	if ((op >> 4) == 0x3)
          output("branch with link");
        else { 
          //output("branch with no link");
	  word target = slice(instr, 23, 0);
          pc += 2+target;
	  output(pc);
	}
	break;
      }
      case 0x6: case 0x7: {
        output("supervisor, co-processor");
        break;
      }
    }
  }
}

void CPU::setpc(word address) {
  this->pc = address;
}

word CPU::getpc() {
  return pc;
}

word CPU::nextinstr() {
  pc += INSTR_SIZE_B;
  return get_cur_instr();
}

word CPU::previnstr() {
  pc -= INSTR_SIZE_B;
  return get_cur_instr();
}

word CPU::get_cur_instr() {
  return instructions[pc];
}
