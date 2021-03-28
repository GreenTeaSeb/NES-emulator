#include "cpu.h"
#include <fstream>
#include <iostream>
using namespace std;

int main()
{
  // TO DO ALL
  CPU cpu;
  std::ifstream input("official_only.nes", std::ios::binary);
  input.read((char*)(cpu.memory[0]), 0xFFFF);
  printf("%d", cpu.read(0x0000));
  cpu.execute();

  return 0;
}
