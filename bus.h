#ifndef BUS_H
#define BUS_H
#include "PPU/ppu.h"
#include "rom.h"
#include <cstdint>

class bus
{
public:
  bus();

  ROM rom = {};
  PPU ppu = {};

  uint8_t cpu_ram[2048] = {};

  uint8_t cpu_read(uint16_t addrss);
  void cpu_write(uint16_t addrss, uint8_t data);

  uint8_t readPRG(uint16_t addrss);
};

#endif // BUS_H
