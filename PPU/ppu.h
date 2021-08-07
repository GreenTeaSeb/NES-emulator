#ifndef PPU_H
#define PPU_H
#include <cstdint>
#include <vector>

enum MIRRORING
{
  VERTICAL,
  HORIZONTAL,
  FOUR_SCREEN
};

class PPU
{
public:
  // read write
  uint8_t cpu_read(uint16_t addrss);
  void cpu_write(uint16_t addrss, uint8_t data);

  uint8_t ppu_read(uint16_t addrss);
  void ppu_write(uint16_t addrss, uint8_t data);

  std::vector<uint8_t> CHR_ROM = {};
  uint8_t mirroringType = {};

  uint8_t ram[2048] = {};
  uint8_t OAM[64 * 4] = {};
  uint8_t palette[32] = {};
  uint8_t internal_data_buffer = {};

  // ADDRESS REGISTER 0x2006
  uint8_t hi = true;
  uint8_t lo = {};
  bool HB_Pointer = {};

  // CONTROLLER REGISTER 0x2007

  uint8_t control_flags = {};

  // control flags mask
  uint8_t NAMETABLE1 = 0b00000001;          // 0
  uint8_t NAMETABLE2 = 0b00000010;          // 1
  uint8_t VRAM_AND_INC = 0b00000100;        // 2
  uint8_t SPRITE_PATTERN = 0b00001000;      // 3
  uint8_t BACKGROUND_PATTERN = 0b00010000;  // 4
  uint8_t SPRITE_SIZE = 0b00100000;         // 5
  uint8_t MASTER_SLAVE_SELECT = 0b01000000; // 6
  uint8_t GENERATE_NMI = 0b10000000;        // 7*/
};

#endif // PPU_H
