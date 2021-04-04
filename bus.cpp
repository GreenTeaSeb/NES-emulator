#include "bus.h"
#include <stdio.h>

bus::bus() {}

uint8_t
bus::read(uint16_t addr)
{
  if (addr >= RAM_START && addr <= RAM_MIRROR_END) {
    return cpu_ram[addr & 0x7FF];
  } else if (addr >= PPU_REG_START && addr <= PPU_REG_MIRROR_END) {
    // TO DO PPU
  } else if (addr >= 0x8000 && addr <= 0xFFFF) {
    readPRG(addr);
  }
}

void
bus::write(uint16_t addr, uint8_t data)
{
  if (addr >= RAM_START && addr <= RAM_MIRROR_END) {
    cpu_ram[addr & 0x7FF] = data;
  } else if (addr >= PPU_REG_START && addr <= PPU_REG_MIRROR_END) {
    // TO DO PPU
  } else {
    printf("Ignoring memory write at %x \n", addr);
  }
}

uint8_t
bus::readPRG(uint16_t addrss)
{

  addrss -= 0x8000;
  if (this->rom.prg_size == 0x4000 && addrss >= 0x4000) {
    // mirror if game is only 16KiB;
    addrss %= 0x4000;
  }
  return this->rom.PRG_ROM[addrss];
}