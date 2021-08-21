#include "ppu.h"
#include <stdio.h>
// PPU
PPU::PPU() {}

uint8_t
PPU::reg_read(uint16_t addrss)
{
  printf("reading at: %x\n", addrss);
  uint8_t ret = {};
  switch (addrss) {
    case 0x2002: { // status
      ret = status;
      status &= 0x7F; // clear vblank
                      // status |= 0x80;
      address_latch = 0;
      break;
    }
    case 0x2004: { // OAM DATA
      break;
    }
    case 0x2007: { // data
      ret = data_buffer;
      data_buffer = read(vram_address);
      if (addrss >= 0x3F00)
        ret = data_buffer;
      vram_address +=
        (control & 0b10)
          ? 32
          : 1; // if bit 2 of CONTROL is set, increment by 32, else by 1;
      break;
    }
  }
  return ret;
}

void
PPU::reg_write(uint16_t addrss, uint8_t data)
{
  printf("writing at: %x\n", addrss);
  switch (addrss) {
    case 0x2000: { // control
      control = data;
      break;
    }
    case 0x2001: { // mask
      mask = data;
      break;
    }
    case 0x2003: { // OAMADDR

      break;
    }
    case 0x2004: { // OAMDATA
      break;
    }
    case 0x2005: { // SCROLL

      break;
    }
    case 0x2006: {
      if (!address_latch) {
        vram_address =
          (uint16_t)((data & 0x3F) << 8) |
          (vram_address & 0x00FF); // bit shiftinf data to higher btye then
                                   // adding existing lower byte;
        address_latch = 1;
      } else { // lower byte
        vram_address =
          (vram_address & 0xFF00) |
          data; // clearing lower byte,  adding the data for lower byte;
        address_latch = 0;
      }
      break;
    }
    case 0x2007: {
      write(vram_address, data);
      vram_address +=
        (control & 0b10)
          ? 32
          : 1; // if bit 2 of CONTROL is set, increment by 32, else by 1;
      break;
    }
  }
}

uint8_t
PPU::read(uint16_t addrss)
{
  addrss &= 0x3fff;
  if (addrss <= 0x1fff) {
    return CHR_ROM[addrss];
  } else if (addrss >= 0x2000 && addrss < 0x3f00) {
    return vram[mirrored_addrss(addrss)];
  }
  return {};
}

void
PPU::write(uint16_t addrss, uint8_t data)
{
  addrss &= 0x3fff;
  if (addrss >= 0x2000 && addrss < 0x3f00) {
    vram[mirrored_addrss(addrss)] = data;
  }
}

uint16_t
PPU::mirrored_addrss(uint16_t addrss)
{

  if (mirroringType == HORIZONTAL) {
    // A | a
    // -----
    // B | b
    if (addrss >= 0x2000 && addrss < 0x2400)
      return addrss & 0x03ff; // A
    else if (addrss >= 0x2400 && addrss < 0x2800)
      return addrss & 0x3ff; // a
    else if (addrss >= 0x2800 && addrss < 0x2c00)
      return (addrss & 0x3ff) + 0x400; // B
    else {                             // b
      return (addrss & 0x3ff) + 0x400;
    }
  } else if (mirroringType == VERTICAL) {
    // A | B
    // -----
    // a | b
    if (addrss >= 0x2000 && addrss < 0x2400) // A
      return addrss & 0x03ff;
    else if (addrss >= 0x2400 && addrss < 0x2800) // B
      return (addrss & 0x3ff) + 0x400;
    else if (addrss >= 0x2800 && addrss < 0x2c00) // a
      return addrss & 0x3ff;
    else { // b
      return (addrss & 0x3ff) + 0x400;
    }
  }
  return {};
}

void
PPU::execute()
{
  if (scanline >= -1 && scanline < 240) {
  }

  if (scanline == 241 && cycle == 1) {
    status |= 0x80;     // set vblank
    if (control & 0x80) // if 7th bit is set
      NMI = true;
  }
  cycle++;
  if (cycle >= 341) {
    cycle = 0;
    scanline++;
    if (scanline >= 261) {
      scanline = -1;
      status &= 0x7f; // reset vblank
    }
  }
}
