#include "cpu.h"
#include <cstdint>
#include <stdio.h>

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
  CPU::address = PC++;
}
void
CPU::getZeropage()
{
  CPU::address = read(PC++); // address
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

  uint8_t lo = read(PC++);
  uint8_t hi = read(PC++);

  CPU::address = (hi << 8) | lo;
  //printf("\t addrs = %x \n", address);
  //printf("\t next addrs = %x \n", PC + 1);
  // CPU::address = read(PC);
}
bool
CPU::getAbsoluteXindx()
{
  uint16_t lowB = read(PC++);
  uint16_t highB = read(PC++);
  uint16_t abs = (highB << 8) | lowB;
  CPU::address = abs + X;

  return checkPageCross(abs, X);
}
bool
CPU::getAbsoluteYindx()
{
  uint16_t lowB = read(PC++);
  uint16_t highB = read(PC++);
  uint16_t abs = (highB << 8) | lowB;
  CPU::address = abs + Y;

  return checkPageCross(abs, Y);
}

void
CPU::getIndexed_Indirect()
{
  uint8_t zero = read(PC++) + X;
  CPU::address = read(zero & 0xff) | read((zero + 1) & 0xff) << 8;
}
bool
CPU::getIndirect_Indexed() // indirect Y
{
  uint8_t zero = read(PC++);
  uint16_t addr = read(zero & 0xff) | read((zero + 1) & 0xff) << 8;

  CPU::address = addr + Y;
  //printf("\t addr :%x \n", address);
  return checkPageCross(addr, Y);
  /*
  uint16_t arg = read(PC + 1);
  uint8_t base = read(arg);
  //printf("\tBase: %x \n", base);
  uint8_t index = read((arg + 1) % 256);

  CPU::address = base + (index << 8) + Y;
  PC++;
  return checkPageCross(base, Y);*/
}

void
CPU::getRelative()
{
  // uint8_t addr = read(PC);
  // int8_t offset = (int8_t)(addr);
  // PC += offset;
}

void
CPU::getIndirect()
{

  uint16_t addr = read(PC) | read(PC + 1) << 8;
  uint16_t hi = addr & 0xff00;

  // JMP BUG, if you do for example JMP(30FF)
  // it will take the High B from 0x3000 and the low B from 0x30FF
  // ie takes low byte from 0x30FF and high byte from 0300

  CPU::address = read(addr) | read(hi | (addr + 1) & 0xff) << 8;
}
