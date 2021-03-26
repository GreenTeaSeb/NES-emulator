#include "cpu.h"
#include <fstream>
#include <iostream>
using namespace std;

int main()
{
  // TO DO ALL
  CPU cpu;
  // std::ifstream input("official_only.nes", std::ios::binary);

  cpu.write(0x0000, 0xF8);
  cpu.execute();
  printf("%d\n", cpu.D);
  return 0;
}
