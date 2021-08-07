
#include "cpu.h"
#include <cstdint>
#include <stdio.h>

void
CPU::executeBLUE(uint8_t opcode, uint8_t addrmode, uint8_t func)
{
  switch (addrmode) { // column
    case 0x0:
      if (func >= 0x04)
        getImmediate();
      switch (func) { // row
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
          CPU::STP();
          break;
        case 0x5:
          CPU::CC = 2;
          CPU::LDX();
          break;
        case 0x4:
        case 0x6:
        case 0x7:
          CPU::NOP();
          break;
      }
      break;
    case 0x1:
      getZeropage();
      switch (func) { // row
        case 0x0:
          CPU::CC = 5;
          CPU::ASL();
          break;
        case 0x1:
          CPU::CC = 5;
          CPU::ROL();
          break;
        case 0x2:
          CPU::CC = 5;
          CPU::LSR();
          break;
        case 0x3:
          CPU::CC = 5;
          CPU::ROR();
          break;
        case 0x4:
          CPU::CC = 3;
          CPU::STX();
          break;
        case 0x5:
          CPU::CC = 3;
          CPU::LDX();
          break;
        case 0x6:
          CPU::CC = 5;
          CPU::DEC();
          break;
        case 0x7:
          CPU::CC = 5;
          CPU::INC();
          break;
      }
      break;
    case 0x2:
      // implied
      switch (func) { // row
        CPU::CC = 2; // NEEDS TO BE FIXED, I HAVE NO CLUE WHERE THIS IS MEANT TO
                     // GO, ACCIDENTALLY TYPED IT HERE MONTHS AGO
        case 0x0:
          CPU::ASL();
          break;
        case 0x1:
          CPU::ROL();
          break;
        case 0x2:
          CPU::LSR();
          break;
        case 0x3:
          CPU::ROR();
          break;
        case 0x4:
          CPU::TXA();
          break;
        case 0x5:
          CPU::TAX();
          break;
        case 0x6:
          CPU::DEX();
          break;
        case 0x7:
          CPU::NOP();
          break;
      }
      break;
    case 0x3:
      getAbsolute();
      if (func == 0x04 || func == 0x05)
        CPU::CC = 4;
      else
        CPU::CC = 6;
      switch (func) { // row
        case 0x0:
          CPU::ASL();
          break;
        case 0x1:
          CPU::ROL();
          break;
        case 0x2:
          CPU::LSR();
          break;
        case 0x3:
          CPU::ROR();
          break;
        case 0x4:
          CPU::STX();
          break;
        case 0x5:
          CPU::LDX();
          break;
        case 0x6:
          CPU::DEC();
          break;
        case 0x7:
          CPU::INC();
          break;
      }
      break;
    case 0x4:
      // implied
      switch (func) { // row
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
          CPU::STP();
          break;
      }
      break;
    case 0x5:
      if (func == 0x04 || func == 0x05) {
        getZeroPageYindx();
        CPU::CC = 4;
      } else {
        getZeroPageXindx();
        CPU::CC = 6;
      }
      switch (func) { // row
        case 0x0:
          CPU::CC = 6;
          CPU::ASL();
          break;
        case 0x1:
          CPU::ROL();
          break;
        case 0x2:
          CPU::LSR();
          break;
        case 0x3:
          CPU::ROR();
          break;
        case 0x4:
          CPU::STX();
          break;
        case 0x5:
          CPU::LDX();
          break;
        case 0x6:
          CPU::DEC();
          break;
        case 0x7:
          CPU::INC();
          break;
      }
      break;
    case 0x6:
      // implied
      CPU::CC = 2;
      switch (func) { // row
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x6:
        case 0x7:
          CPU::NOP();
          break;

        case 0x4:
          CPU::TXS();
          break;
        case 0x5:
          CPU::TSX();
          break;
      }
      break;
    case 0x7:
      if (func == 0x04 || func == 0x05) {
        bool pageCrossed = getAbsoluteYindx();
        CPU::CC = 4;
        if (pageCrossed)
          CPU::CC += 1;
      } else {
        getAbsoluteXindx();
        CPU::CC = 7;
      }
      switch (func) { // row
        case 0x0:
          CPU::ASL();
          break;
        case 0x1:
          CPU::ROL();
          break;
        case 0x2:
          CPU::LSR();
          break;
        case 0x3:
          CPU::ROR();
          break;
        case 0x4:
          CPU::SHX();
          break;
        case 0x5:
          CPU::LDX();
          break;
        case 0x6:
          CPU::DEC();
          break;
        case 0x7:
          CPU::INC();
          break;
      }
      break;
  }
}

void
CPU::STP()
{
  CPU::CC = 3;
  // stops clock input
}
void
CPU::LDX()
{
  X = read(CPU::address);
  setZN(X);
}

void
CPU::ASL()
{
  uint8_t AccumOpcode = 0x0A;

  // getting
  uint8_t data;
  if (CPU::opcode == AccumOpcode)
    data = A;
  else
    data = read(CPU::address);

  // updating
  C = data >> 7;
  data <<= 1;
  setZN(data);
  // setting
  if (CPU::opcode == AccumOpcode)
    A = data;
  else
    write(CPU::address, data);
}

void
CPU::ROL()
{
  uint8_t AccumOpcode = 0x2A;

  // getting
  uint8_t data;
  bool oldcarry = C;
  if (CPU::opcode == AccumOpcode)
    data = A;
  else
    data = read(CPU::address);
  // updating

  C = data >> 7;
  data <<= 1;
  data |= oldcarry;
  Z = data == 0;
  N = data >> 7;

  // setting
  if (CPU::opcode == AccumOpcode)
    A = data;
  else
    write(CPU::address, data);
}

void
CPU::ROR()
{
  uint8_t AccumOpcode = 0x6A;

  // getting
  uint8_t data;
  bool oldcarry = C;
  if (CPU::opcode == AccumOpcode)
    data = A;
  else
    data = read(CPU::address);
  // updating

  C = data & 1; // new carry set to bit 0
  data >>= 1;
  data |= oldcarry << 7; // bit 7 set to old carry

  Z = data == 0;
  N = data >> 7;

  // setting
  if (CPU::opcode == AccumOpcode)
    A = data;
  else
    write(CPU::address, data);
}

void
CPU::LSR()
{
  uint8_t AccumOpcode = 0x4A;

  // getting
  uint8_t data;
  if (CPU::opcode == AccumOpcode)
    data = A;
  else
    data = read(CPU::address);

  // updating
  C = data & 1;
  data >>= 1;
  setZN(data);
  // setting
  if (CPU::opcode == AccumOpcode)
    A = data;
  else
    write(CPU::address, data);
}

void
CPU::STX()
{

  write(CPU::address, X);
}

void
CPU::DEC()
{
  uint8_t data = read(CPU::address);
  data -= 1;
  Z = data == 0;
  N = data >> 7;
  write(CPU::address, data);
}

void
CPU::INC()
{
  uint8_t data = read(CPU::address);
  data += 1;
  Z = data == 0;
  N = data >> 7;
  write(CPU::address, data);
}

void
CPU::SHX()
{
  uint8_t data = X & ((CPU::address >> 8) + 1);
  write(CPU::address, data);
}

void
CPU::TAX()
{
  X = A;
  Z = X == 0;
  N = X >> 7;
}
void
CPU::TXA()
{
  A = X;
  setZN(A);
}

void
CPU::DEX()
{
  --X;
  setZN(X);
}

void
CPU::TXS()
{
  SP = X;
}

void
CPU::TSX()
{
  X = SP;
  setZN(X);
}
