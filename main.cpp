#include "CPU/cpu.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int
main()
{

  std::ifstream romFile("donkey kong.nes", std::ios::in | std::ios::binary);
  std::vector<uint8_t> rom((std::istreambuf_iterator<char>(romFile)),
                           std::istreambuf_iterator<char>());
  CPU cpu{ rom };
  auto system_clock = 0;

  while (1) {
    cpu.BUS.ppu.execute();

    if (system_clock % 3 == 0) {

      printf("op: %-2x %-2x%-2x\n",
             cpu.opcode,
             cpu.read(cpu.PC + 1),
             cpu.read(cpu.PC + 2));
      cpu.execute();
    }

    if (cpu.BUS.ppu.NMI) {
      cpu.BUS.ppu.NMI = false;
      cpu.NMI();
    }
    system_clock++;
  }

  return 0;
}
