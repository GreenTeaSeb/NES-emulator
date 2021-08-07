#include "CPU/cpu.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int
main()
{
  // ONLY FOR TESTING NESTEST MANUALLY
  //  printf("Starting...\n");
  CPU cpu;
  std::ifstream romFile("pacman.nes", std::ios::in | std::ios::binary);
  std::vector<uint8_t> rom((std::istreambuf_iterator<char>(romFile)),
                           std::istreambuf_iterator<char>());

  cpu.BUS.rom.load(rom);

  uint16_t starting_PC = cpu.read(0xFFFD) * 256 + cpu.read(0xFFFC);

  cpu.PC = 0xC000;
  // cpu.PC = starting_PC;
  cpu.SP = 0xFD;
  int line = 0;
  cpu.setPStatus();
  printf("starting PC: %x\n", cpu.PC);

  window win = {};
  win.tile(cpu.BUS.rom.CHR_ROM, 1);
  win.show_window();

  //  while (1) {
  //    uint16_t addr = cpu.PC;
  //    uint8_t HighB = cpu.read(cpu.PC + 1);
  //    uint8_t LowB = cpu.read(cpu.PC + 2);
  //    uint8_t op = cpu.read(cpu.PC);

  //    printf("%-5x  %-5.x %-2x %-10x A:%-2x X:%-2x Y:%-2x P:%-2x SP: % -2x\n
  //    ",
  //           addr,
  //           op,
  //           HighB,
  //           LowB,
  //           cpu.A,
  //           cpu.X,
  //           cpu.Y,
  //           cpu.PStatus,
  //           cpu.SP);

  //    if (cpu.BUS.cpu_read(0x2) != 0) {
  //      printf("failed");
  //      break;
  //    }

  //    cpu.execute();

  //    if (cpu.opcode == 0x0) {
  //      printf("Break opcode 0x00\n");
  //      break;
  //    }

  //    line++;
  //  }

  return 0;
}
