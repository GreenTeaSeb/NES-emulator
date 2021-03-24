#include "cpu.h"
#include <cstdint>

void
CPU::executeBLUE(uint8_t opcode, uint8_t addrmode, uint8_t func)
{
  switch (func) {
    case 0x00:            // 1st row
      switch (addrmode) { // columns
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x05:
        case 0x07:
          CPU::ASL();
          break;
        case 0x06:
          CPU::NOP();
          break;
        case 0x00:
        case 0x04:
          CPU::STP();
          break;
      }
      break;
    case 0x01:            // 2nd row
      switch (addrmode) { // columns
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x05:
        case 0x07:
          CPU::ROL();
          break;
        case 0x06:
          CPU::NOP();
          break;
        case 0x00:
        case 0x04:
          CPU::STP();
          break;
      }
      break;
    case 0x02:            // 3rd row
      switch (addrmode) { // columns
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x05:
        case 0x07:
          CPU::LSR();
          break;
        case 0x06:
          CPU::NOP();
          break;
        case 0x00:
        case 0x04:
          CPU::STP();
          break;
      }
      break;
    case 0x03:            // 4th row
      switch (addrmode) { // columns
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x05:
        case 0x07:
          CPU::ROR();
          break;
        case 0x06:
          CPU::NOP();
          break;
        case 0x00:
        case 0x04:
          CPU::STP();
          break;
      }
      break;

    case 0x04:            // 5th row
      switch (addrmode) { // columns
        case 0x00:
          CPU::NOP();
          break;
        case 0x01:
        case 0x03:
        case 0x05:
          CPU::STX();
          break;
        case 0x02:
          CPU::TXA();
          break;
        case 0x06:
          CPU::TXS();
          break;

        case 0x04:
          CPU::STP();
          break;
        case 0x07:
          CPU::SHX();
      }
      break;
    case 0x05:            // 6th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x05:
        case 0x07:
          CPU::LDX();
          break;
        case 0x02:
          CPU::TAX();
          break;
        case 0x04:
          CPU::STP();
          break;
        case 0x06:
          CPU::TSX();
          break;
      }
      break;
    case 0x06:            // 7th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x06:
          CPU::NOP();
        case 0x01:
        case 0x03:
        case 0x05:
        case 0x07:
          CPU::DEC();
          break;
        case 0x02:
          CPU::DEX();
          break;
        case 0x04:
          CPU::STP();
          break;
      }
      break;
    case 0x07:            // 8th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x02:
        case 0x06:
          CPU::NOP();
        case 0x01:
        case 0x03:
        case 0x05:
        case 0x07:
          CPU::INC();
          break;
        case 0x04:
          CPU::STP();
          break;
      }
      break;
  }
}
