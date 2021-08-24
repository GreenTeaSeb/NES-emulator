#include "ppu.h"
#include "../bitops.h"
#include <stdio.h>
// PPU
PPU::PPU() {}

uint8_t
PPU::reg_read(uint16_t addrss)
{
  uint8_t ret = {};
  switch (addrss) {
    case 0x2002: { // status
      ret = (uint8_t)(status.to_ulong());
      status[7] = 0; // clear vblank
      address_latch = 0;
      break;
    }
    case 0x2004: { // OAM DATA
      break;
    }
    case 0x2007: { // data
      ret = data_buffer;
      data_buffer = read((uint16_t)(vram_address.to_ulong()));
      if (addrss >= 0x3F00)
        ret = data_buffer;
      vram_address =
        control[2] ? vram_address.to_ulong() + 32 : vram_address.to_ulong() + 1;

      break;
    }
  }
  return ret;
}

void
PPU::reg_write(uint16_t addrss, uint8_t data)
{
  switch (addrss) {
    case 0x2000: { // control
      control = data;
      // nametable data
      tram_address[10] = control[0];
      tram_address[11] = control[1];
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
    case 0x2005: {            // SCROLL
      if (!address_latch) {   // high byte
        fine_x = data & 0x07; // getting first 3 bits for fine_X
        tram_address = bit_set(tram_address.to_ulong(), data >> 3, 0x7fe0);
        address_latch = 1;
      } else { // lower byte
        bit_set<15>(tram_address, 12, 14, data & 0x7);
        bit_set<15>(tram_address, 5, 9, data >> 3);

        address_latch = 0;
      }
      break;
    }
    case 0x2006: {
      if (!address_latch) { // high byte
        tram_address = bit_set(tram_address.to_ulong(), data & 0x3F, 0x00FF);
        address_latch = 1;
      } else {
        tram_address = bit_set(tram_address.to_ulong(), data, 0xFF00);
        address_latch = 0;

        vram_address = tram_address;
      }
      break;
    }
    case 0x2007: {
      write((uint16_t)(vram_address.to_ulong()), data);
      vram_address =
        control[2] ? vram_address.to_ulong() + 32 : vram_address.to_ulong() + 1;
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
  } else if (addrss >= 0x3f00) {
    addrss &= 0x3f1f;
    if (addrss == 0x3f10)
      addrss = 0x3f00;
    if (addrss == 0x3f14)
      addrss = 0x3f04;
    if (addrss == 0x3f18)
      addrss = 0x3f08;
    if (addrss == 0x3f1c)
      addrss = 0x3f0c;

    return vram[addrss];
  }
  return {};
}

void
PPU::write(uint16_t addrss, uint8_t data)
{
  addrss &= 0x3fff;
  if (addrss >= 0x2000 && addrss < 0x3f00) {
    vram[mirrored_addrss(addrss)] = data;

  } else if (addrss >= 0x3f00) {
    addrss &= 0x3f1f;
    if (addrss == 0x3f10)
      addrss = 0x3f00;
    if (addrss == 0x3f14)
      addrss = 0x3f04;
    if (addrss == 0x3f18)
      addrss = 0x3f08;
    if (addrss == 0x3f1c)
      addrss = 0x3f0c;
    vram[addrss] = data;
  }
}

uint16_t
PPU::mirrored_addrss(uint16_t addrss)
{

  if (*mirroringType == HORIZONTAL) {
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
  } else if (*mirroringType == VERTICAL) {
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
PPU::increment_x()
{
  if (mask[3] || mask[4]) { // if render bg or sprits is set
    if (((uint16_t)vram_address.to_ulong() & 0x001f) == 31) {
      vram_address &= ~0x001f;
      vram_address ^= 0x0400;
    } else
      vram_address = vram_address.to_ulong() + 1;
  }
}

void
PPU::increment_y()
{
  if (mask[3] || mask[4]) { // if render bg or sprits is set
    if ((vram_address.to_ulong() & 0x7000) != 0x7000) {
      vram_address = vram_address.to_ulong() + 0x1000;
    } else {
      vram_address &= 0x7ff;
      auto y = (vram_address.to_ulong() & 0x03e0) >> 5;
      if (y == 29) {
        y = 0;
        vram_address ^= 0x0800;
      } else if (y == 31) {
        y = 0;
      } else {
        y += 1;
      }
      vram_address = (vram_address.to_ulong() & ~0x03e0) | (y << 5);
    }
  }
}

void
PPU::transfer_hori()
{
  if (mask[3] || mask[4]) {
    vram_address[10] = tram_address[10];
    bit_set<15, 15>(vram_address, 0, 4, tram_address, 0, 4);
  }
}

void
PPU::transfer_vert()
{
  if (mask[3] || mask[4]) {
    vram_address =
      bit_replace_all(vram_address.to_ulong(), tram_address.to_ulong(), ~0x41f);
  }
}

void
PPU::execute()
{

  if ((scanline >= 0 && scanline < 240) || scanline == 261) {
    if ((cycle > 0 && cycle < 257) || (cycle >= 321 && cycle < 341)) {
      if (scanline == 261) {
        if (cycle == 1) {
          status &= 0x1f; // clear vblank,overflow and sprite 0
        } else if (cycle >= 280 && cycle < 305) {
          transfer_vert();
        }
      }

      switch (cycle % 8) {
        case 0: { // last

          increment_x();
          break;
        }
        case 2: { // NT byte
          uint16_t address =
            0x2000 | ((uint16_t)(vram_address.to_ulong()) & 0x0FFF);
          NT_byte = read(address);

          printf("NT %x : %x\n",
                 0x2000 | ((uint16_t)(vram_address.to_ulong()) & 0x0FFF),
                 NT_byte);
          break;
        }
        case 4: { // AT byte
          AT_byte =
            read(0x23C0 | ((uint16_t)(vram_address.to_ulong()) & 0x0c00) |
                 (((uint16_t)(vram_address.to_ulong()) >> 4) & 0x38) |
                 (((uint16_t)(vram_address.to_ulong()) >> 2) & 0x07));
          break;
        }
        case 6: { // Low BG tile byte
          low_bg_byte = (control[3] << 12) | (NT_byte * 16) |
                        ((vram_address.to_ulong() & 0x7000) >> 12);

          break;
        }
        case 7: { // High BG tile byte1
          high_bg_byte = read((control[3] << 12) | (NT_byte * 16) |
                              ((vram_address.to_ulong() & 0x7000) >> 12) + 8);

          //          if (NT_byte != 0x0 && NT_byte != 0x20)
          //            printf("%x%x\n", high_bg_byte, low_bg_byte);

          break;
        }
      }
    }
    if (cycle == 256)
      increment_y();
    if (cycle == 257)
      transfer_hori();

  } else if (scanline == 241 && cycle == 1) {
    status[7] = 1;
    if (control[7])
      NMI = true;
  }
  cycle++;

  if (cycle == 341) {
    cycle = 0;
    scanline = scanline <= 261 ? ++scanline : 0;
  }
}
