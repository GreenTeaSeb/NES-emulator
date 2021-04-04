#include "cpu.h"
#include <cstdint>
#include <stdio.h>

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
      // 0x0,0x2,0x3 are implied
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
          CPU::CC = (pageCross) ? 5 : 4;
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
  PC = read(0xFFFF);
  B = 1;
}
void
CPU::JSR()
{
  PC--;
  CPU::pushStack((PC >> 8) & 0xff);
  CPU::pushStack(PC & 0xff);

  CPU::PC = CPU::address;
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
  CPU::PC = popStack() | popStack() << 8;
}

void
CPU::RTS()
{
  uint16_t lo = popStack();
  uint16_t hi = popStack();
  CPU::PC = ((hi << 8) | lo) + 1;
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
  setZN(Y);
  printf("\t Y = %x \n\t N = %x \n", Y, Y >> 7);
}

void
CPU::CPY()
{
  uint8_t val = read(CPU::address);
  uint16_t diff = Y - val;
  C = Y >= val;
  setZN(diff);
}

void
CPU::CPX()
{
  uint8_t val = read(CPU::address);
  uint16_t diff = X - val;
  C = X >= val;
  setZN(diff);
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
  write(CPU::address, Y);
}

void
CPU::JMP()
{
  printf("\t jump to addr %x \n", CPU::address);
  PC = CPU::address;
  printf("\tjumped -> %x \n", PC);
}
void
CPU::PHP()
{
  uint8_t statusFlags = C + (Z << 1) + (I << 2) + (D << 3) + (1 << 4) +
                        (O << 6) + (N << 7) +
                        (1 << 5); // BREAK and BREAK2 IS ALWAYS 1
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
  // B = newFlags & 0b00010000;
  O = newFlags & 0b01000000; // SKIPPED BREAK2 and BREAK1
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
  A = popStack();
  printf("\t A = %-2x Z = %x \n", A, Z);
  setZN(A);
}

void
CPU::DEY()
{
  Y -= 1;
  setZN(Y);
}

void
CPU::TAY()
{
  Y = A;
  setZN(Y);
}

void
CPU::INY()
{
  Y += 1;

  setZN(Y);
}

void
CPU::INX()
{
  X += 1;
  setZN(X);
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
  printf("\t C -> %x\n", C);
}

void
CPU::SEC()
{
  C = 1;
  printf("\t C -> %x\n", C);
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
  setZN(A);
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
