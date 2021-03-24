#include "cpu.h"
#include <cstdint>

void
CPU::executeRED(uint8_t opcode, uint8_t addrmode, uint8_t func)
{
  switch (addrmode) {
    case 0x00: // 1st column

      if (func >= 0x04) {
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
          CPU::C = 6;
          CPU::RTS();
          break;
        case 0x04:
          CPU::NOP();
          break;
        case 0x05:
          CPU::LDY();
          break;
        case 0x06:
          CPU::CPY();
          break;
        case 0x07:
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
          CPU::BIT();
          break;
        case 0x02:
          CPU::NOP();
          break;
        case 0x03:
          CPU::NOP();
          break;
        case 0x04:
          CPU::STY();
          break;
        case 0x05:
          CPU::LDY();
          break;
        case 0x06:
          CPU::CPY();
          break;
        case 0x07:
          CPU::CPX();
          break;
      }
      break;
    case 0x02: // 3rd column
      // implied
      switch (func) {
        case 0x00:
          CPU::PHP();
          break;
        case 0x01:
          CPU::PLP();
          break;
        case 0x02:
          CPU::PHA();
          break;
        case 0x03:
          CPU::PLA();
          break;
        case 0x04:
          CPU::DEY();
          break;
        case 0x05:
          CPU::TAY();
          break;
        case 0x06:
          CPU::INY();
          break;
        case 0x07:
          CPU::INX();
          break;
      }
      break;
    case 0x03: // 4th column
      if (func != 0x03)
        getAbsolute();
      // ONLY JUMP HAS INDIRECT
      switch (func) {
        case 0x00:
          CPU::NOP();
          break;
        case 0x01:
          CPU::BIT();
          break;
        case 0x02:
          CPU::JMP();
          break;
        case 0x03:
          CPU::JMP();
          break;
        case 0x04:
          CPU::STY();
          break;
        case 0x05:
          CPU::LDY();
          break;
        case 0x06:
          CPU::CPY();
          break;
        case 0x07:
          CPU::CPX();
          break;
      }
      break;
    case 0x04: // 5th column
      getRelative();
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
          CPU::NOP();
          break;
        case 0x04:
          CPU::STY();
          break;
        case 0x05:
          CPU::LDY();
          break;
        case 0x06:
          CPU::CPY();
          break;
        case 0x07:
          CPU::CPX();
          break;
      }
      break;
    case 0x06: // 7th column
      // implied
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
      getAbsoluteXindx();
      switch (func) {
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
          CPU::NOP();
          break;
        case 0x04:
          CPU::SHY();
          break;
        case 0x05:
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
  C = 0b00000001;
  Z = 0b00000010;
  I = 0b00000100;
  D = 0b00001000;
  B = 0b00010000;
  O = 0b01000000; // SKIPPED BREAK2
  N = 0b10000001;
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
  Z = Y = val;
  N = (A >> 7);
}
