#include <cstdint>
#include <vector>
#ifndef ROM_H
#define ROM_H

enum MIRRORING
{
  VERTICAL,
  HORIZONTAL,
  FOUR_SCREEN
};

class ROM
{
public:
  ROM();
  const uint8_t INES_TAG[4] = { 0x4E, 0x45, 0x53, 0x1A };

  std::vector<uint8_t> PRG_ROM; // connected to cpu
  std::vector<uint8_t> CHR_ROM; // connectec to ppu
  uint8_t mapper = {};
  uint8_t mirroringType;
  uint16_t prg_size = {};

  void load(uint8_t rom[]);
  // INES1 FORMAT
};

#endif // ROM_H
