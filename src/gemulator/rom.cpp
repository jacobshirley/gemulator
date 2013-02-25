#include <iostream>
#include <fstream>
#include <sstream>
#include <ios>
#include <cmath>
#include <string.h>

#include "rom.h"
#include "emulator.h"
//#include "debug.h"

ROM::ROM(const char* file_name) {
  address = 0;
  ifstream rom(file_name, ios::binary); 
  
  if (!rom.is_open()) {
    throw 1;
  }
  
  rom.seekg(0, ios_base::end);
  int len = rom.tellg();
  rom.seekg(0, ios_base::beg);

  bytes = new byte[len];
  rom.read(bytes, len);
}

const word ROM::get_word() {
  word rw;
  memcpy(&rw, bytes+address, 4);
  return rw;
}

void ROM::inc_offset(word inc) {
  address += inc;
}

void ROM::dec_offset(word dec) {
  address -= dec;
}

word ROM::get_offset() {
  return address;
}

const byte* ROM::get_bytes() {
  return bytes;
}

void ROM::set_offset(int address) {
  this->address = address;
}

ROM::~ROM() {
  delete bytes;
}
