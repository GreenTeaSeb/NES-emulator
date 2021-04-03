#include "cpu.h"
#include <Windows.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int
main()
{
  // TO DO ALL
  CPU cpu;
  vector<uint16_t> fails;
  std::ifstream input("nestest.nes", std::ios::binary);
  std::ifstream log("CORRECTnestest.log", std::ios::binary);
  int correctAddr = {};
  int correctA = {}, correctX = {}, correctY = {}, correctP = {},
      correctSP = {};

  string line;
  uint8_t ROM[0x4000] = {};

  input.seekg(0x10);
  input.read((char*)(ROM), 0x4000);

  for (int i = 0; i < 0x4000; i++) {
    if (0x8000 + i <= 0xBFFF)
      cpu.write(0x8000 + i, ROM[i]);
  }
  for (int i = 0; i < 0x4000; i++) {
    if (0xC000 + i <= 0xFFFF)
      cpu.write(0xC000 + i, ROM[i]);
  }

  cpu.I = 1;
  cpu.B2 = 1;
  cpu.PC = 0xC000;
  cpu.SP = 0xFD;

  cpu.setPStatus();
  int lineN = 0;
  int nfails = 0;

  while (1) {
    lineN++;
    uint16_t addr = cpu.PC;
    uint8_t HighB = cpu.read(cpu.PC + 1);
    uint8_t LowB = cpu.read(cpu.PC + 2);
    uint8_t op = cpu.read(cpu.PC);
    uint8_t A = cpu.A, X = cpu.X, Y = cpu.Y, P = cpu.PStatus, SP = cpu.SP;
    // Sleep(1);
    printf("%d ", lineN);

    printf("%-10x  %-2.x %-2x %-2x                       A:%-2x X:%-2x Y:%-5x "
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

    // log >> hex >> logLine;
    // log.ignore(88, '\n');

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
  printf("Number of failes: %d >\n", nfails);
  for (auto i : fails) {
    // printf("\t%x\n", i);
  }
  return 0;
}
