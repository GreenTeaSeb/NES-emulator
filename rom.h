#include "window.h"
#include <cstdint>
#include <vector>
#ifndef ROM_H

#define ROM_H

class ROM
{
public:
  ROM();
  const uint8_t INES_TAG[4] = { 0x4E, 0x45, 0x53, 0x1A };

  enum MIRRORING
  {
    VERTICAL,
    HORIZONTAL,
    FOUR_SCREEN
  };

  std::vector<uint8_t> PRG_ROM; // connected to cpu
  std::vector<uint8_t> CHR_ROM; // connectec to ppu
  uint8_t mapper = {};
  uint8_t mirroringType;
  uint16_t prg_size = {};

  void load(std::vector<uint8_t> rom);
  // INES1 FORMAT
};

#endif // ROM_H
