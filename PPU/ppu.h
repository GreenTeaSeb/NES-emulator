#ifndef PPU_H
#define PPU_H
#include <cstdint>
#include <span>
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
  PPU();
  // DATA
  std::span<uint8_t> CHR_ROM = {};
  uint8_t mirroringType = {};

  uint8_t vram[2048] = {}; // 2 nametables
  uint8_t OAM[64 * 4] = {};
  uint8_t palette[32] = {};

  int16_t scanline = {};
  uint16_t cycle = {};
  bool NMI = {};
  uint8_t status = {};
  // STATUS: _ _ _ _ _ _ _ _
  //         | | | | | | | |
  //         7 6 5 4 3 2 1 0
  // 7: VERTICAL BLANK  0 = NOT IN VBLANK 1= IN VBLANK
  // 6: SPRITE 0 HIT:  Set when a nonzero pixel of sprite 0 overlaps  a nonzero
  //                   background pixel
  // 5: SPRITE OVERFLOW
  // 4-0 UNUSED

  uint8_t mask = {};
  // MASK: _ _ _ _ _ _ _ _
  //         | | | | | | | |
  //         7 6 5 4 3 2 1 0
  // 7: emphasize_blue
  // 6: emphasize_green
  // 5: emphasize_red
  // 4: render_sprites
  // 3: render_bg
  // 2: render_sprites_left
  // 1: render_bg_left
  // 0: grayscale

  uint8_t control = {};
  // CONTROL: _ _ _ _ _ _ _ _
  //         | | | | | | | |
  //         7 6 5 4 3 2 1 0
  // 7: Generate an NMI at the start of the vertical blanking interval
  // 6: PPU master/slave select
  // 5: Sprite size (0: 8x8 pixels; 1: 8x16 pixels)
  // 4: Background pattern table address (0: $0000; 1: $1000)
  // 3: Sprite pattern table address for 8x8 sprites
  // 2: VRAM address increment per CPU read/write of PPUDATA
  // 1: nametable y
  // 0: nametable x

  // ADDRESS REGISTER 0x2006
  uint8_t address_latch = {};
  uint8_t data_buffer = {};
  uint16_t vram_address = {};

  // read write
  uint8_t reg_read(uint16_t addrss);
  void reg_write(uint16_t addrss, uint8_t data);

  uint8_t read(uint16_t addrss);
  void write(uint16_t addrss, uint8_t data);

  uint16_t mirrored_addrss(uint16_t addrss);

  void execute();

  // register functions
};

#endif // PPU_H
