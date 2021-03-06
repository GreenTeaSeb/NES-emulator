#include "window.h"

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
window::create_window()
{

  window_ = SDL_CreateWindow("CHRROM",
                             SDL_WINDOWPOS_CENTERED,
                             SDL_WINDOWPOS_CENTERED,
                             WIDTH * 3,
                             HEIGHT * 3,
                             SDL_WINDOW_ALWAYS_ON_TOP);

  render = SDL_CreateRenderer(window_, -1, 0);
  texture = SDL_CreateTexture(
    render, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_TARGET, 256, 240);
}

int
window::draw()
{
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT)
    return 0;

  SDL_UpdateTexture(texture, NULL, &data, 256 * 3);
  SDL_SetRenderTarget(render, NULL);
  SDL_RenderCopy(render, texture, NULL, NULL);
  SDL_RenderPresent(render);
  return 1;
}
