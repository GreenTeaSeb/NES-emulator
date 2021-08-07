#include "rom.h"
#include "window.h"
#include <stdio.h>

ROM::ROM() {}

void
ROM::load(std::vector<uint8_t> rom)
{

  mapper = (rom[7] & 0xF0) | (rom[6] >> 4);

  // INES1
  printf("mapper: %x\n", mapper);
  uint8_t INESver = (rom[7] >> 2) & 0b11;
  printf("Ines: %x\n", INESver);
  if (INESver)
    printf("Currently only supports iNES1");
  uint8_t four_screen = (rom[6] & 0b1000) != 0;
  uint8_t vertical_mirror = (rom[6] & 0b1) != 0;

  if (!four_screen)
    if (vertical_mirror)
      mirroringType = VERTICAL;
    else
      mirroringType = HORIZONTAL;
  else
    mirroringType = FOUR_SCREEN;

  prg_size = rom[4] * 16384;
  uint16_t chr_size = rom[5] * 8192;

  bool skip_trainer = (rom[6] & 0b100) != 0;

  int prg_start = +(skip_trainer) ? 16 + 512 : 16;
  int chr_start = prg_start + prg_size;

  std::vector<uint8_t> prg(&rom[prg_start], &rom[prg_start + prg_size]);
  std::vector<uint8_t> chr(&rom[chr_start], &rom[chr_start + chr_size]);

  PRG_ROM = prg;
  CHR_ROM = chr;
  // PRG_ROM.insert(PRG_ROM.begin(), rom + prg_start, rom + prg_size -
  // prg_start);
  // CHR_ROM.insert(CHR_ROM.begin(), rom + chr_start, rom + chr_size -
  // chr_start);
}
