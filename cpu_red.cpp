#include "cpu.h"
#include <cstdint>

void
CPU::executeRED(uint8_t opcode, uint8_t addrmode, uint8_t func)
{
  switch (addrmode) {
    case 0x00: // 1st column
      if (func == 0x01) {
        getAbsolute();
      } else if (func >= 0x04) {
        getImmediate();
      }

      switch (func) {
        case 0x00:
          CPU::BRK();

          break;
        case 0x01:
          CPU::CC = 6;
          CPU::JSR();
          break;
        case 0x02:
          CPU::CC = 6;
          CPU::RTI();
          break;
        case 0x03:
          CPU::CC = 6;
          CPU::RTS();
          break;
        case 0x04:
          CPU::NOP();
          break;
        case 0x05:
          CPU::CC = 2;
          CPU::LDY();
          break;
        case 0x06:
          CPU::CC = 2;
          CPU::CPY();
          break;
        case 0x07:
          CPU::CC = 2;
          CPU::CPX();
          break;
      }
      break;
    case 0x01: // 2nd column
      getZeropage();
      switch (func) {
        case 0x00:
          CPU::NOP();
          break;
        case 0x01:
          CPU::CC = 3;
          CPU::BIT();
          break;
        case 0x02:
          CPU::NOP();
          break;
        case 0x03:
          CPU::NOP();
          break;
        case 0x04:
          CPU::CC = 3;
          CPU::STY();
          break;
        case 0x05:
          CPU::CC = 3;
          CPU::LDY();
          break;
        case 0x06:
          CPU::CC = 3;
          CPU::CPY();
          break;
        case 0x07:
          CPU::CC = 3;
          CPU::CPX();
          break;
      }
      break;
    case 0x02: // 3rd column
      // implied
      switch (func) {
        case 0x00:
          CPU::CC = 3;
          CPU::PHP();
          break;
        case 0x01:
          CPU::CC = 4;
          CPU::PLP();
          break;
        case 0x02:
          CPU::CC = 3;
          CPU::PHA();
          break;
        case 0x03:
          CPU::CC = 4;
          CPU::PLA();
          break;
        case 0x04:
          CPU::CC = 2;
          CPU::DEY();
          break;
        case 0x05:
          CPU::CC = 2;
          CPU::TAY();
          break;
        case 0x06:
          CPU::CC = 2;
          CPU::INY();
          break;
        case 0x07:
          CPU::CC = 2;
          CPU::INX();
          break;
      }
      break;
    case 0x03: // 4th column
      if (func != 0x03)
        getAbsolute();
      else
        getIndirect();
      // ONLY JUMP HAS INDIRECT
      switch (func) {
        case 0x00:
          CPU::NOP();
          break;
        case 0x01:
          CPU::CC = 4;
          CPU::BIT();
          break;
        case 0x02:
          CPU::CC = 3;
          CPU::JMP();
          break;
        case 0x03:
          CPU::CC = 5; // INDIRECT

          CPU::JMP();
          break;
        case 0x04:
          CPU::CC = 4;
          CPU::STY();
          break;
        case 0x05:
          CPU::CC = 4;
          CPU::LDY();
          break;
        case 0x06:
          CPU::CC = 4;
          CPU::CPY();
          break;
        case 0x07:
          CPU::CC = 4;
          CPU::CPX();
          break;
      }
      break;
    case 0x04: // 5th column
      getRelative();
      CPU::CC = 2;
      switch (func) {
        case 0x00:
          CPU::BPL();
          break;
        case 0x01:
          CPU::BMI();
          break;
        case 0x02:
          CPU::BVC();
          break;
        case 0x03:
          CPU::BVS();
          break;
        case 0x04:
          CPU::BCC();
          break;
        case 0x05:
          CPU::BCS();
          break;
        case 0x06:
          CPU::BNE();
          break;
        case 0x07:
          CPU::BEQ();
          break;
      }
      break;
    case 0x05: // 6th column
      getZeroPageXindx();
      switch (func) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x06:
        case 0x07:
          CPU::NOP();
          break;
        case 0x04:
          CPU::CC = 4;
          CPU::STY();
          break;
        case 0x05:
          CPU::CC = 4;
          CPU::LDY();
          break;
      }
      break;
    case 0x06: // 7th column
      // implied
      CPU::CC = 2;
      switch (func) {
        case 0x00:
          CPU::CLC();
          break;
        case 0x01:
          CPU::SEC();
          break;
        case 0x02:
          CPU::CLI();
          break;
        case 0x03:
          CPU::SEI();
          break;
        case 0x04:
          CPU::TYA();
          break;
        case 0x05:
          CPU::CLV();
          break;
        case 0x06:
          CPU::CLD();
          break;
        case 0x07:
          CPU::SED();
          break;
      }
      break;
    case 0x07: // 8th column
      bool pageCross = getAbsoluteXindx();
      switch (func) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
          CPU::NOP();
          break;
        case 0x04:
          CPU::CC = 5;
          CPU::SHY();
          break;
        case 0x05:
          CPU::C = (pageCross) ? 5 : 4;
          CPU::LDY();
          break;
        case 0x06:
        case 0x07:
          CPU::NOP();
          break;
      }
      break;
  }
}

void
CPU::BRK()
{
  // TO DO
  B = 1;
}
void
CPU::JSR()
{
  CPU::pushStack(CPU::PC - 1);
  CPU::PC = read(CPU::PC);
}
void
CPU::RTI()
{
  uint8_t newFlags = popStack();
  C = newFlags & 0b00000001;
  Z = newFlags & 0b00000010;
  I = newFlags & 0b00000100;
  D = newFlags & 0b00001000;
  B = newFlags & 0b00010000;
  O = newFlags & 0b01000000; // SKIPPED BREAK2
  N = newFlags & 0b10000000;
  CPU::PC = popStack();
}

void
CPU::RTS()
{
  CPU::PC = popStack() + 1;
}

void
CPU::NOP()
{
  CC = 2;
}

void
CPU::LDY()
{
  Y = read(CPU::address);
  Z = Y == 0;
  N = (A >> 7);
}

void
CPU::CPY()
{
  uint8_t val = read(CPU::address);
  C = Y >= val;
  Z = Y == val;
  N = (A >> 7);
}

void
CPU::CPX()
{
  uint8_t val = read(CPU::address);
  C = X >= val;
  Z = X == val;
  N = (A >> 7);
}

void
CPU::BIT()
{
  uint8_t val = read(CPU::address);
  Z = (A & val) == 0;
  O = (val >> 6) & 0b1;
  N = val >> 7;
}

void
CPU::STY()
{
  CPU::memory[CPU::address] = Y;
}

void
CPU::JMP()
{
  PC = read(CPU::address);
}
void
CPU::PHP()
{
  uint8_t statusFlags =
    C + (Z << 1) + (I << 2) + (D << 3) + (B << 4) + (O << 6) + (I << 7);
  pushStack(statusFlags);
}
void
CPU::PLP()
{
  uint8_t newFlags = popStack();
  C = newFlags & 0b00000001;
  Z = newFlags & 0b00000010;
  I = newFlags & 0b00000100;
  D = newFlags & 0b00001000;
  B = newFlags & 0b00010000;
  O = newFlags & 0b01000000; // SKIPPED BREAK2
  N = newFlags & 0b10000000;
}

void
CPU::PHA()
{
  pushStack(A);
}

void
CPU::PLA()
{
  uint8_t val = popStack();
  A = val;
  Z = A == 0;
  N = A >> 7;
}

void
CPU::DEY()
{
  Y -= 1;
  Y %= 256;

  Z = Y == 0;
  N = Y >> 7;
}

void
CPU::TAY()
{
  Y = A;
  Z = Y == 0;
  N = Y >> 7;
}

void
CPU::INY()
{
  Y += 1;
  Y %= 256;

  Z = Y == 0;
  N = Y >> 7;
}

void
CPU::INX()
{
  X += 1;
  X %= 256;

  Z = X == 0;
  N = X >> 7;
}

void
CPU::BPL()
{
  branch(!N);
}

void
CPU::BMI()
{
  branch(N);
}

void
CPU::BVC()
{
  branch(!O);
}

void
CPU::BVS()
{
  branch(O);
}

void
CPU::BCC()
{
  branch(!C);
}

void
CPU::BCS()
{
  branch(C);
}

void
CPU::BNE()
{
  branch(!Z);
}

void
CPU::BEQ()
{
  branch(Z);
}

void
CPU::CLC()
{
  C = 0;
}

void
CPU::SEC()
{
  C = 1;
}

void
CPU::CLI()
{
  I = 0;
}

void
CPU::SEI()
{
  I = 1;
}

void
CPU::TYA()
{
  A = Y;
  Z = A == 0;
  N = A >> 7;
}

void
CPU::CLV()
{
  O = 0;
}

void
CPU::CLD()
{
  D = 0;
}

void
CPU::SED()
{
  D = 1;
}

void
CPU::SHY()
{
  uint8_t data = Y & ((CPU::address >> 8) + 1);
  write(CPU::address, data);
}
