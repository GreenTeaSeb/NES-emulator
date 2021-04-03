#include "cpu.h"
#include <cstdint>
#include <stdio.h>

void
CPU::executeGRAY(uint8_t opcode, uint8_t addrmode, uint8_t func)
{
  printf("UNOFFICIAL OPCODE at  row %x column %x \n", func, addrmode);
  switch (addrmode) { // columns
    case 0x00:
      getIndexed_Indirect();
      break;
    case 0x01:
      getZeropage();
      break;
    case 0x02:
      getImmediate();
      break;
    case 0x03:
      getAbsolute();
      break;
    case 0x04:
      getIndirect_Indexed();
      break;
    case 0x05:
      if (func == 0x05 || func == 0x04) {
        getZeroPageYindx();
      } else
        getZeroPageXindx();
      break;
    case 0x06:
      getAbsoluteYindx();
      break;
    case 0x07:
      if (func == 0x05 || func == 0x04)
        getAbsoluteYindx();
      else
        getAbsoluteXindx();
      break;
  }

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
        case 0x07:
          CPU::LAX();
          break;
        case 0x06:
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
          CPU::ISC();
          break;
        case 0x02:
          CPU::SBC();
          break;
      }
      break;
  }
}

void
CPU::LAX()
{
  uint8_t data = read(CPU::address);
  printf("\t A,X = $%x %x\n", address, data);
  A = data;
  X = data;
  setZN(A);
}

void
CPU::SLO()
{
  // asl
  uint8_t data = read(CPU::address);
  C = data >> 7;
  data <<= 1;

  write(CPU::address, data);
  setZN(data);
  // a
  A |= data;
  setZN(A);
}

void
CPU::ANC()
{
  uint8_t data = read(CPU::address);
  A &= data;
  setZN(A);
  C = N;
}

void
CPU::SRE()
{
  uint8_t data = read(CPU::address);
  C = data & 1;
  data >>= 1;
  setZN(data);
  write(CPU::address, data);
  A ^= data;
  setZN(A);
}
void
CPU::ALR()
{
  A &= read(CPU::address);

  uint8_t data = A;
  C = data & 1;
  data >>= 1;
  setZN(data);
  write(CPU::address, data);
}
void
CPU::ARR()
{
  bool oldcarry = C;
  A &= read(CPU::address);
  uint8_t data = A;
  //----
  C = data & 1;
  data >>= 1;
  data |= oldcarry << 7;
  setZN(data);
  A = data;
}
void
CPU::RRA()
{
  bool oldcarry = C;
  uint8_t data = read(CPU::address);
  //---- ROR
  C = data & 1;
  data >>= 1;
  data |= oldcarry << 7;
  setZN(data);
  write(CPU::address, data);

  //---- ADC

  uint16_t sum = A + data + C;
  O = (~(A ^ data) & (A ^ sum) & 0x80);

  A = sum;
  C = sum >> 8;
  setZN(sum);
}
void
CPU::SAX()
{
  uint8_t data = A & X;
  write(CPU::address, data);
}
void
CPU::TAS()
{
  uint8_t data = A & X;
  CPU::SP = data;
  write(CPU::address, SP & ((CPU::address >> 8) + 1));
}
void
CPU::RLA()
{
  // ROL
  bool oldcarry = C;
  uint8_t data = read(CPU::address);
  C = data >> 7;
  data <<= 1;
  data |= oldcarry;
  setZN(data);
  write(CPU::address, data);
  // AND
  A &= data;
  setZN(A);
}
void
CPU::AHX()
{
  uint8_t data = A & X & (CPU::address >> 8);
  write(CPU::address, data);
}
void
CPU::XAA()
{
  A = X;
  A &= read(CPU::address);
  setZN(A);
}
void
CPU::LAS()
{
  uint8_t data = read(CPU::address) & CPU::SP;
  A = data;
  SP = data;
  X = data;
  SP = data;
  setZN(data);
}
void
CPU::DCP()
{
  // DEC
  uint8_t data = read(CPU::address);
  data -= 1;
  write(CPU::address, data);
  // CMP
  C = !((A - data) & 0x100);
  setZN(A - data);
}
void
CPU::AXS()
{
  write(CPU::address, A & X);
}
void
CPU::ISC()
{
  // INC
  uint8_t data = read(CPU::address);
  data += 1;
  setZN(data);
  write(CPU::address, data);

  // SBC

  uint16_t diff = A - data - !C;

  C = !(diff >> 8); // checks bit 8
  O = ((A ^ diff) & (~data ^ diff) & 0x80);
  A = diff;

  setZN(diff);
}
