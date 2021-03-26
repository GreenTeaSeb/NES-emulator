#include "cpu.h"
#include <cstdint>
#include <stdio.h>
CPU::CPU()
{

}
void
CPU::write(uint16_t addrss, uint8_t data) // Write to Ram
{
  CPU::memory[addrss] = data;
  printf("%u\n", read(0x6000));
}
uint8_t
CPU::read(uint16_t addrss) // read RAM
{
  return memory[addrss];
}

void
CPU::pushStack(uint16_t data)
{
  write(STACKSTART + CPU::SP, data);
  CPU::SP -= 1;
  SP %= 2048;
}
uint8_t
CPU::popStack()
{
  CPU::SP += 1;
  SP %= 2048;
  return read(STACKSTART + CPU::SP);
}
void
CPU::branch(bool condition)
{
  int8_t jump = read(PC);
  PC++;
  if (condition) {
    if (PC & 0xFF + jump > 0xFF)
      CC++;
    PC += jump;
    CC++;
  }
}

void
CPU::initialise()
{}
void
CPU::execute()
{

  opcode = read(PC);
  uint8_t color = opcode & 0b11;            // region
  uint8_t addrmode = (opcode >> 2) & 0b111; // column 0-31
  uint8_t func = (opcode >> 5) & 0b111;     // row 0-7
  CC = 0;
  PC++;
  switch (color) {
    case 0b00: // red control instructions
      executeRED(opcode, addrmode, func);
      break;
    case 0b01: // green ALU
      executeGREEN(opcode, addrmode, func);
      break;
    case 0b10: // blue  Read-modify-write
      // executeBLUE(opcode, addrmode, func);
      break;
    case 0b11: // gray unofficial
      // executeGRAY(opcode, addrmode, func);
      break;
  }
}
