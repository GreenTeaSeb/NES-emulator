#include "cpu.h"
#include <cstdint>

void
CPU::executeGREEN(uint8_t opcode, uint8_t addrmode, uint8_t func)
{

  switch (addrmode) { // columns
    case 0x00:
      CPU::CC = 6;
      getIndexed_Indirect();
      break;
    case 0x01:
      CPU::C = 3;
      getZeropage();
      break;
    case 0x02: // #i
      CPU::CC = 2;
      getImmediate();
      break;
    case 0x03:
      CPU::CC = 4;
      getAbsolute();
      break;
    case 0x04:
      CPU::CC = 5;
      if (getIndirect_Indexed() || func == 0x4)
        CC += 1;
      break;
    case 0x05:
      CPU::CC = 3;
      getZeroPageXindx();
    case 0x06:
      CPU::CC = 4;
      if (getAbsoluteYindx() || func == 0x4)
        CC += 1;
    case 0x07:
      CPU::CC = 4;
      if (getAbsoluteXindx() || func == 0x4)
        CC += 1;
      break;
  }
  switch (func) {
    case 0x00: // 1st row
      CPU::ORA();
      break;
    case 0x01: // 2nd row
      CPU::AND();
      break;
    case 0x02: // 3rd row
      CPU::EOR();
      break;
    case 0x03: // 4th row
      CPU::ADC();
      break;
    case 0x04: // 5th row
      if (addrmode != 0x2) {
        CPU::STA();
      }
      break;
    case 0x05: // 6th row
      CPU::LDA();
      break;
    case 0x06: // 7th row
      CPU::CMP();
      break;
    case 0x07: // 8th row
      CPU::SBC();
      break;
  }
}

void
CPU::ORA()
{
  A |= read(CPU::address);
  Z = A == 0;
  N = (A >> 7);
}
void
CPU::AND()
{
  A &= read(CPU::address);
  Z = A == 0;
  N = (A >> 7);
}
void
CPU::EOR()
{
  A ^= read(CPU::address);
  Z = A == 0;
  N = (A >> 7);
}

void
CPU::ADC()
{
  uint8_t value = read(CPU::address);
  uint16_t sum = A + value + C;
  O = (~(A ^ value) & (A ^ sum) & 0x80);

  A = sum;
  C = sum >> 8;
  Z = A == 0;
  N = (A >> 7);
}
void
CPU::STA()
{
  write(address, A);
}

void
CPU::LDA()
{
  A = memory[address];
  Z = A == 0;
  N = (A >> 7);
}

void
CPU::CMP()
{
  uint8_t M = read(CPU::address);
  C = A >= M;
  Z = A = M;
  N = (A >> 7);
}

void
CPU::SBC()
{
  uint8_t value = read(CPU::address);
  uint16_t sum = A - value - !C;
  O = (~(A ^ value) & (A ^ sum) & 0x80);

  A = sum;
  C = sum >> 8;
  Z = A == 0;
  N = (A >> 7);
}
