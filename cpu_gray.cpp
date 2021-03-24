#include "cpu.h"
#include <cstdint>

void
CPU::executeGRAY(uint8_t opcode, uint8_t addrmode, uint8_t func)
{
  switch (func) {
    case 0x00:            // 1st row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          CPU::SLO();
          break;
        case 0x02:
          CPU::ANC();
          break;
      }
      break;
    case 0x01:            // 2nd row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          CPU::RLA();
          break;
        case 0x02:
          CPU::ANC();
          break;
      }
      break;
    case 0x02:            // 3rd row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          CPU::SRE();
          break;
        case 0x02:
          CPU::ALR();
          break;
      }
      break;
    case 0x03:            // 4th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          CPU::RRA();
          break;
        case 0x02:
          CPU::ARR();
          break;
      }
      break;
    case 0x04:            // 5th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x05:
          CPU::SAX();
          break;
        case 0x06:
          CPU::TAS();
          break;
        case 0x04:
        case 0x07:
          CPU::AHX();
        case 0x08:
          CPU::SLO();
          break;
        case 0x02:
          CPU::XAA();
          break;
      }
      break;
    case 0x05:            // 6th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x08:
          CPU::LAX();
          break;
        case 0x07:
          CPU::LAS();
          break;
      }
      break;
    case 0x06:            // 7th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          CPU::DCP();
          break;
        case 0x02:
          CPU::AXS();
          break;
      }
      break;
    case 0x07:            // 8th row
      switch (addrmode) { // columns
        case 0x00:
        case 0x01:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
        case 0x08:
          CPU::ISC();
          break;
        case 0x02:
          CPU::SBC();
          break;
      }
      break;
  }
}
