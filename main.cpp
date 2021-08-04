#include "CPU/cpu.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void
nestest()
{
  // TO DO ALL
  CPU cpu;
  vector<uint8_t> fails;
  uint8_t Rom[0x4000] = {};
  std::ifstream romFile("nestest.nes", std::ios::binary);
  romFile.read((char*)(Rom), 0x4000);

  cpu.BUS.rom.load(Rom);

  std::ifstream log("CORRECTnestest.log", std::ios::binary);
  int correctAddr = {};
  int correctA = {}, correctX = {}, correctY = {}, correctP = {},
      correctSP = {};

  string line;

  cpu.I = 1;
  cpu.B2 = 1;
  cpu.PC = 0xC000;
  cpu.SP = 0xFD;

  cpu.setPStatus();
  int lineN = 0;
  int nfails = 0;

  printf("size prg: %x \n", cpu.BUS.rom.prg_size);

  while (1) {
    lineN++;
    uint16_t addr = cpu.PC;
    uint8_t HighB = cpu.read(cpu.PC + 1);
    uint8_t LowB = cpu.read(cpu.PC + 2);
    uint8_t op = cpu.read(cpu.PC);
    uint16_t A = cpu.A, X = cpu.X, Y = cpu.Y, P = cpu.PStatus, SP = cpu.SP;
    // Sleep(1);

    printf("%d ", lineN);

    printf("%-10x  %-2.x %-2x %-2x                       A:%-2x X:%-2x Y:%-5x"
           "P:%x SP:%x  "
           "CYC: ? SL: ? MSG: %x %x %x \n",
           // lineN,
           addr,
           op,
           HighB,
           LowB,
           A,
           X,
           Y,
           P,
           SP,
           cpu.read(0x3),
           cpu.read(0x2),
           cpu.read(0x0));

    std::getline(log, line);
    if (line.size() > 0) {
      correctA = stoi(line.substr(50, 2), 0, 16);
      correctX = stoi(line.substr(55, 2), 0, 16);
      correctY = stoi(line.substr(60, 2), 0, 16);
      correctP = stoi(line.substr(65, 2), 0, 16);
      correctSP = stoi(line.substr(71, 2), 0, 16);
      correctAddr = stoi(line.substr(0, 4), 0, 16);
    }

    cpu.execute();

    printf("\t C:%-2xZ:%-2xI:%-2xD:%-2xB:%-2xO:%-2xN:%-2x \n",
           cpu.C,
           cpu.Z,
           cpu.I,
           cpu.D,
           cpu.B,
           cpu.O,
           cpu.N);

    printf("\n");

    if (A != correctA) {
      printf("\tAccumulator is wrong! Supposed to be: %x  at line %d\n",
             correctA,
             lineN);
      break;
    }
    if (X != correctX) {
      printf("\tX register is wrong! Supposed to be: %x  at line %d\n",
             correctX,
             lineN);
      break;
    }
    if (Y != correctY) {
      printf("\tY register is wrong! Supposed to be: %x  at line %d\n",
             correctY,
             lineN);
      break;
    }
    if (P != correctP) {
      printf("\tStatus flags are wrong! Supposed to be: %x  at line %d\n",
             correctP,
             lineN);
      break;
    }
    if (SP != correctSP) {
      printf("\tStack pointer is wrong! Supposed to be: %x  at line %d\n",
             correctSP,
             lineN);
      break;
    }

    if (addr != correctAddr) {
      printf("\tAddress is wrong! Supposed to be: %x  at line %d\n",
             correctAddr,
             lineN);
      break;
    }
    if (cpu.read(0x0) != 0) {
      printf("\tOpcode failed ->%x%x%x \n \n",
             cpu.read(0x3),
             cpu.read(0x2),
             cpu.read(0x0));
      fails.push_back(cpu.read(0x0));
      nfails++;
      // break;
    }

    if (cpu.B == 1) {
      printf("Break set 1\n");
      break;
    }
    if (cpu.opcode == 0x0) {
      printf("Break opcode 0x00\n");
      break;
    }
  }
  for (auto& f : fails) {
    printf("%d \n", f);
  }
  printf("Number of failes: %d >\n", nfails);
}

int
main()
{
  // ONLY FOR TESTING NESTEST MANUALLY
  //  printf("Starting...\n");
  CPU cpu;
  uint8_t Rom[0x4000] = {};
  std::ifstream romFile("nestest.nes", std::ios::binary);
  romFile.read((char*)(Rom), 0x4000);

  cpu.BUS.rom.load(Rom);

  //  uint16_t starting_PC = cpu.read(0xFFFD) * 256 + cpu.read(0xFFFC);
  //  printf("starting PC: %x\n", starting_PC);

  cpu.PC = 0xC000;
  //  cpu.PC = starting_PC;
  cpu.SP = 0xFD;
  int line = 0;
  cpu.setPStatus();
  //  printf("size prg: %x \n", cpu.BUS.rom.prg_size);

  while (1) {
    uint16_t addr = cpu.PC;
    uint8_t HighB = cpu.read(cpu.PC + 1);
    uint8_t LowB = cpu.read(cpu.PC + 2);
    uint8_t op = cpu.read(cpu.PC);

    printf("%-5x  %-5.x %-2x %-10x A:%-2x X:%-2x Y:%-2x P:%-2x SP:%-2x  \n",
           addr,
           op,
           HighB,
           LowB,
           cpu.A,
           cpu.X,
           cpu.Y,
           cpu.PStatus,
           cpu.SP);

    if (cpu.BUS.cpu_read(0x2) != 0) {
      printf("failed");
      break;
    }

    cpu.execute();

    if (cpu.opcode == 0x0) {
      printf("Break opcode 0x00\n");
      break;
    }

    line++;
  }

  return 0;
}
