#include "CPU/cpu.h"
#include "window.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int
main()
{

  std::ifstream romFile("nestest.nes", std::ios::in | std::ios::binary);
  std::vector<uint8_t> rom((std::istreambuf_iterator<char>(romFile)),
                           std::istreambuf_iterator<char>());
  CPU cpu{ rom };

  window win = { cpu.BUS.ppu };
  win.create_window();
  while (1) {
    printf("PC: %-4x A:%-2x X:%-2x Y:%-2x stack: %-2x\n",
           cpu.PC,
           cpu.A,
           cpu.X,
           cpu.Y,
           cpu.SP);
    // win.tile();
    cpu.execute();
  }

  return 0;
}
