#include "../bus.h"
#include <cstdint>
#ifndef CPU_H
#define CPU_H

class CPU
{
public:
  CPU();
  // registers
  uint8_t A = {};
  uint8_t X = {};
  uint8_t Y = {};

  uint8_t PStatus = {};

  uint16_t SP = {};    // Stack pointer
  uint16_t PC = {};    // Pc counter
  uint16_t CC = {};    // Cycle counter
  uint8_t opcode = {}; // opcode
  uint16_t address = {};
  uint8_t memory[64 * 1024] = {};
  const uint16_t STACKSTART = 0x0100;
  const uint16_t STACKEND = 0x01FF;

  bus BUS = {};

  // status flags
  bool C = {}; // carry
  bool Z = {}; // zero
  bool I = 1;  // interrupt
  bool D = {}; // decimal
  bool B = {}; // break
  bool B2 = 1; // break2
  bool O = {}; // overflow
  bool N = {}; // negative

  void write(uint16_t addrss, uint8_t data);
  uint8_t read(uint16_t addrss);

  void execute();

  void executeRED(uint8_t opcode, uint8_t addrmode, uint8_t func);
  void executeGREEN(uint8_t opcode, uint8_t addrmode, uint8_t func);
  void executeBLUE(uint8_t opcode, uint8_t addrmode, uint8_t func);
  void executeGRAY(uint8_t opcode, uint8_t addrmode, uint8_t func);

  void cycle();

  void initialise();
  // AddressingModes
  void getImmediate();
  void getZeropage();
  void getZeroPageXindx();
  void getZeroPageYindx();
  void getIndexed_Indirect();
  bool getIndirect_Indexed();
  void getAbsolute();
  bool getAbsoluteXindx();
  bool getAbsoluteYindx();
  void getRelative();
  void getIndirect();

  // MISC
  void add(uint8_t val);
  bool checkPageCross(uint8_t B1, uint8_t B2);
  void pushStack(uint8_t data);
  uint8_t popStack();
  void branch(bool condition);
  void setZN(uint8_t result);

  void setPStatus();

  // Instructions
  void ADC();
  void AND();
  void ASL();
  //---------
  void BCC();
  void BCS();
  void BEQ();
  void BIT();
  void BMI();
  void BNE();
  void BPL();
  void BRK();
  void BVC();
  void BVS();
  //---------
  void CMP();
  void CPX();
  void CPY();
  //---------
  void DEC();
  //---------
  void EOR();
  //---------
  void INC();
  //---------
  void JMP();
  void JSR();
  //---------
  void LDA();
  void LDX();
  void LDY();
  void LSR();
  //---------
  void NOP();
  //---------
  void ORA();
  //---------
  void INY();
  //---------
  void ROL();
  void ROR();
  void RTI();
  void RTS();
  //---------
  void SBC();
  void STA();
  void STX();
  void STP();
  void STY();
  void SHX();
  //---------

  // REGISTER INSTRUCTIONS -- implied, 1 byte, 2 cycles, T - transfer,
  // DE-decrement,IN-increment
  void TAX();
  void TXA();
  void DEX();
  void INX();
  void TAY();
  void DEY();

  // SET FLAG INSTRUCTIONS -- implied, 1 byte , 2 cycles
  void CLC(); // clear carry
  void SEC(); // set carry
  void CLI(); // clear interrupt
  void SEI(); // set interrupt
  void CLV(); // clear overfloww
  void CLD(); // clear decimal
  void SED(); // set decimal

  // STACK INSTRUCTIONS -- implied, 1 byte, 2 cycles
  void TXS(); // transfer X to stack pointer
  void TSX(); // transfer stack pointer to X
  void PHA(); // push accumulator
  void PLA(); // pull accumulator
  void PHP(); // push accumulator status
  void PLP(); // pull processor status

  void SLO();
  void ANC();
  void RNA();
  void SRE();
  void ALR();
  void ARR();
  void RRA();
  void SAX();
  void TAS();
  void RLA();
  void AHX();
  void XAA();
  void TYA();
  void LAX();
  void SHY();
  void LAS();
  void DCP();
  void AXS();
  void ISC();
};

#endif // CPU_H
