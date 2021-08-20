#include "bus.h"
#include <stdio.h>

bus::bus() {}

uint8_t
bus::cpu_read(uint16_t addr)
{
  if (addr >= 0x0000 && addr <= 0x1FFF) { // reading cpu ram
    return cpu_ram[addr & 0x7FF];
  } else if (addr >= 0x2000 &&
             addr <= 0x3FFF) { // reading mirrored ppu registers
    return ppu.reg_read(addr & 0x01f);
  } else if (addr >= 0x8000 && addr <= 0xFFFF) { // reading PRG from cartridge
    return readPRG(addr);
  } else {
    return 0x0;
  }
}

void
bus::cpu_write(uint16_t addr, uint8_t data)
{
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    cpu_ram[addr & 0x7FF] = data;
  } else if (addr >= 0x2000 && addr <= 0x3FFF) {
    ppu.reg_write(addr & 0x01f, data);
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
