#include "cpu.h"
#include <cstdint>

void
CPU::add(uint8_t val)
{
  uint8_t value = read(CPU::address);
  uint16_t sum = A + value + C;
  O = (~(A ^ value) & (A ^ sum) & 0x80);

  A = sum;
  C = sum >> 8;
  Z = A == 0;
  N = (A >> 7);
}

bool
CPU::checkPageCross(uint8_t B1, uint8_t B2)
{
  return ((B1 & 0xFF) + (B2 & 0xFF) > 255);
}

// ADDRESSING MODES

void
CPU::getImmediate()
{
  CPU::address = PC;
  PC++;
}
void
CPU::getZeropage()
{
  CPU::address = read(PC); // address
  PC++;
}
void
CPU::getZeroPageXindx()
{
  CPU::address = (read(PC) + X) & 0x00FF;
  PC++;
}
void
CPU::getZeroPageYindx()
{
  CPU::address = (read(PC) + Y) & 0x00FF;
  PC++;
}
void
CPU::getAbsolute()
{
  uint16_t lowB = read(PC);
  PC++;
  uint16_t highB = read(PC);
  PC++;
  CPU::address = (highB << 8) | lowB;
}
bool
CPU::getAbsoluteXindx()
{
  uint16_t lowB = read(PC);
  PC++;
  uint16_t highB = read(PC);
  PC++;
  uint16_t abs = (highB << 8) | lowB;
  CPU::address = abs + X;

  return checkPageCross(abs, X);
}
bool
CPU::getAbsoluteYindx()
{
  uint16_t lowB = read(PC);
  PC++;
  uint16_t highB = read(PC);
  PC++;
  uint16_t abs = (highB << 8) | lowB;
  CPU::address = abs + Y;

  return checkPageCross(abs, Y);
}

void
CPU::getIndexed_Indirect()
{
  CPU::address =
    (read((read(PC) + X) & 0x00FF) + read((read(PC) + X + 1) & 0x00FF)) * 256;
  PC++;
}
bool
CPU::getIndirect_Indexed()
{
  uint16_t arg = read(PC);
  uint8_t base = read(arg);
  uint8_t index = read((arg + 1) % 256);

  CPU::address = base + (index << 8) + Y;
  PC++;
  return checkPageCross(base, Y);
}

void
CPU::getRelative()
{
  uint8_t addr = read(PC);
  int8_t offset = (int8_t)(addr);
  PC += offset;
}
