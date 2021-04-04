#ifndef BUS_H
#define BUS_H
#include "rom.h"
#include <cstdint>

class bus
{
public:
  bus();
  const uint16_t RAM_START = 0x000;
  const uint16_t RAM_MIRROR_END = 0x1FFF;

  const uint16_t PPU_REG_START = 0x2000;
  const uint16_t PPU_REG_MIRROR_END = 0x3FFF;

  ROM rom = {};

  uint8_t cpu_ram[2048] = {};

  uint8_t read(uint16_t addrss);
  void write(uint16_t addrss, uint8_t data);

  uint8_t readPRG(uint16_t addrss);
};

#endif // BUS_H