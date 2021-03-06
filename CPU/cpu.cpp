#include "cpu.h"
#include "../bus.h"
#include <cstdint>
#include <stdexcept>
#include <stdio.h>

CPU::CPU(std::vector<uint8_t>& rom, window& win)
{
  BUS.rom.load(rom);
  PC = read(0xFFFD) * 256 + read(0xFFFC);
  SP = 0xFD;
  setPStatus();

  BUS.ppu.CHR_ROM = { BUS.rom.CHR_ROM };
  BUS.ppu.mirroringType = &BUS.rom.mirroringType;
  BUS.ppu.win = win;
}
void
CPU::write(uint16_t addrss, uint8_t data) // Write to Ram
{
  BUS.cpu_write(addrss, data);
}
uint8_t
CPU::read(uint16_t addrss) // read RAM
{
  return BUS.cpu_read(addrss);
}

void
CPU::pushStack(uint8_t data)
{
  write(STACKSTART + CPU::SP, data);
  // printf("\tstack pushed %x -> %x \n", data, STACKSTART + CPU::SP);
  CPU::SP--;
}

uint8_t
CPU::popStack()
{
  uint16_t val = read(STACKSTART | ++CPU::SP);
  // printf("\tstack popped %x\n", val);
  return val;
}
void
CPU::branch(bool condition)
{
  int8_t jump = read(PC);
  PC++;
  if (condition) {
    if ((PC & 0xFF + jump) > 0xFF)
      CC++;
    PC += jump;
    CC++;
    // printf("\tBranched -> %x\n", PC);
    return;
  }
  // printf("\tBranch failed\n");
}

void
CPU::setZN(uint8_t result)
{
  // printf(" \t Checking if %x == 0 \n", result);
  Z = !result;
  N = (result >> 7);
}
void
CPU::setPStatus()
{
  PStatus = C + (Z << 1) + (I << 2) + (D << 3) + (B << 4) + (O << 6) +
            (N << 7) + (1 << 5); // BREAK and BREAK2 IS ALWAYS 1
}
void
CPU::initialise()
{}

void
CPU::NMI()
{
  pushStack((PC >> 8) & 0x00ff);
  pushStack(PC & 0x00ff);
  B = 0;
  I = 1;
  setPStatus();
  pushStack(PStatus);
  PC = (read(0xfffb) << 8) | read(0xfffa);
  CC = 8;
}
void
CPU::execute()
{

  opcode = read(PC++);
  uint8_t color = opcode & 0b11;            // region
  uint8_t addrmode = (opcode >> 2) & 0b111; // column 0-31
  uint8_t func = (opcode >> 5) & 0b111;     // row 0-7
  CC = 0;

  switch (color) {
    case 0b00: // red control instructions
      executeRED(opcode, addrmode, func);
      break;
    case 0b01: // green ALU
      executeGREEN(opcode, addrmode, func);
      break;
    case 0b10: // blue  Read-modify-write
      executeBLUE(opcode, addrmode, func);
      break;
    case 0b11: // gray unofficial
      executeGRAY(opcode, addrmode, func);
      break;
    default:
      printf("UNKOWN OPCODE\n");
      break;
  }
  setPStatus();
  // 1 CPU CYCLE = 3 PPU CYCLE
  for (int i = 0; i < CC; i++) {
    for (int j = 0; j < 3; j++) {
      BUS.ppu.execute();
      if (BUS.ppu.NMI) {
        BUS.ppu.NMI = false;
        NMI();
      }
    }
  }
}
