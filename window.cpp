﻿#include "window.h"

window::window() {}

void
window::set_pixel(uint8_t x,
                  uint8_t y,
                  std::tuple<uint8_t, uint8_t, uint8_t> rgb)
{

  int base = y * 3 * WIDTH + x * 3;
  if (base + 2 < data.size()) {
    data[base] = std::get<0>(rgb);
    data[base + 1] = std::get<1>(rgb);
    data[base + 2] = std::get<2>(rgb);
  }
}

void
window::tile(std::vector<uint8_t> chr_rom, uint8_t bank)
{
  auto b = (bank * 0x1000);
  int tile_x = 0;
  int tile_y = 0;

  for (int tile_n = 0; tile_n < 255; tile_n++) {
    if (tile_n != 0 && tile_n % 20 == 0) {
      tile_y += 10;
      tile_x = 0;
    }
    std::vector<uint8_t> tiles(&chr_rom[(bank + tile_n * 16)],
                               &chr_rom[(bank + tile_n * 16 + 15)]);

    for (int y = 0; y < 8; y++) {
      auto upper = tiles[y];
      auto lower = tiles[y + 8];
      for (int x = 8; x > 0; x--) {
        auto val = (1 & upper) << 1 | (1 & lower);
        upper = upper >> 1;
        lower = lower >> 1;

        set_pixel(tile_x + x, tile_y + y, get_rgb(val));
      }
    }
    tile_x += 10;
  }
}

std::tuple<uint8_t, uint8_t, uint8_t>
window::get_rgb(int val)
{
  switch (val) {
    case 0:
      return { palette[0x1][0], palette[0x1][1], palette[0x1][2] };
    case 1:
      return { palette[0x23][0], palette[0x23][1], palette[0x23][2] };
    case 2:
      return { palette[0x27][0], palette[0x27][1], palette[0x27][2] };
    case 3:
      return { palette[0x30][0], palette[0x30][1], palette[0x30][2] };
    default:
      return { palette[0x3][0], palette[0x3][1], palette[0x3][2] };
  }
}

void
window::show_window()
{

  SDL_Window* window;
  SDL_Renderer* render;
  SDL_Texture* texture;
  SDL_Event event;

  window = SDL_CreateWindow("CHRROM",
                            SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED,
                            WIDTH * 3,
                            HEIGHT * 3,
                            SDL_WINDOW_ALWAYS_ON_TOP);

  render = SDL_CreateRenderer(window, -1, 0);
  texture = SDL_CreateTexture(
    render, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, 256, 240);

  while (1) {
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
      break;

    SDL_UpdateTexture(texture, NULL, &data, 256 * 3);
    SDL_SetRenderTarget(render, NULL);
    SDL_RenderCopy(render, texture, NULL, NULL);
    SDL_RenderPresent(render);
  }

  SDL_Quit();
}
