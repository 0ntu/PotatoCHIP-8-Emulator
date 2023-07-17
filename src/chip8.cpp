#include "chip8.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace PChip8 {
Chip8::Chip8() {
  // Copy builtin font to memory
  // starting at 0x50

  std::copy(BUILTIN_FONT.begin(), BUILTIN_FONT.end(),
            memory.begin() + FONT_LOCATION);

  // Seed random function
  srand(time(nullptr));
}

Chip8::~Chip8() = default;

void Chip8::cpuCycle() {
  currentOpCode = (memory[pc] << 8) | memory[pc + 1];

  // next instruction
  // skip 2 bc instructions are 16 bits
  // while memory is stored in 8 bit blocks
  pc += 2;

  // parse first nibble first
  switch (currentOpCode & 0xF000) {
  case 0x0000:
    switch (currentOpCode & 0x000F) {
    case 0x0000:
      return opCode_CLS();
    case 0x000E:
      return opCode_RET();
    default:
      throw std::runtime_error(&"Unimplemented Opcode: "[static_cast<int>(currentOpCode)]);
    }
    break;
  case 0x1000:
    return opCode_JP();
  case 0x2000:
    return opCode_CALL();
  case 0x3000:
    return opCode_SE_VX_KK();
  case 0x4000:
    return opCode_SNE_VX_KK();
  case 0x5000:
    return opCode_SE_VX_VY();
  case 0x6000:
    return opCode_LD_VX_KK();
  case 0x7000:
    return opCode_ADD_VX_KK();
  case 0x8000:
    switch (currentOpCode & 0x000F) {
    case 0x0000:
      return opCode_LD_VX_VY();
    case 0x0001:
      return opCode_OR_VX_VY();
    case 0x0002:
      return opCode_AND_VX_VY();
    case 0x0003:
      return opCode_XOR_VX_VY();
    case 0x0004:
      return opCode_ADD_VX_VY();
    case 0x0005:
      return opCode_SUB_VX_VY();
    case 0x0006:
      return opCode_SHR_VX();
    case 0x0007:
      return opCode_SUBN_VX_VY();
    case 0x000E:
      return opCode_SHL_VX();
    default:
      throw std::runtime_error(&"Bad Opcode: "[static_cast<int>(currentOpCode)]);
    }
    break;
  case 0x9000:
    return opCode_SNE_VX_VY();
  case 0xA000:
    return opCode_LD_I();
  case 0xB000:
    return opCode_JP_V0();
  case 0xC000:
    return opCode_RND_VX();
  case 0xD000:
    return opCode_DRW_VX_VY();
  case 0xE000:
    switch (currentOpCode & 0x00FF) {
      case 0x009E:
        return opCode_SKP_VX();
      case 0x00A1:
        return opCode_SKNP_VX();
      default:
        throw std::runtime_error(&"Bad Opcode: "[static_cast<int>(currentOpCode)]);
    }
  case 0xF000:
    switch (currentOpCode & 0x00FF) {
      case 0x0007:
        return opCode_LD_VX_DT();
      case 0x000A:
        return opCode_LD_VX_K();
      case 0x0015:
        return opCode_LD_DT_VX();
      case 0x0018:
        return opCode_LD_ST_VX();
      case 0x001E:
        return opCode_ADD_I_VX();
      case 0x0029:
        return opCode_LD_F_VX();
      case 0x0033:
        return opCode_LD_B_VX();
      case 0x0055:
        return opCode_LD_I_VX();
      case 0x0065:
        return opCode_LD_VX_I();
      default:
        throw std::runtime_error(&"Bad Opcode: "[static_cast<int>(currentOpCode)]);
    }
  default:
    throw std::runtime_error(&"Bad Opcode: "[static_cast<int>(currentOpCode)]);
  }
}

void Chip8::loadROM(std::string fileName) {

  if (!std::filesystem::exists(fileName))
    throw std::runtime_error(fileName + " does not exist");
  if (std::filesystem::file_size(fileName) > MEMORY_SIZE - START_EXEC_LOCATION)
    throw std::runtime_error(fileName + " is too large");

  std::ifstream romData{fileName, std::ios::binary};
  if (!romData)
    throw std::runtime_error("Could not open " + fileName);

  romData.read(reinterpret_cast<char *>(memory.data() + START_EXEC_LOCATION),
               std::filesystem::file_size(fileName));
  // if read failed
  if (!romData) {
    throw std::runtime_error(fileName + " read failed");
  }
}

void Chip8::reset() {
  std::fill(memory.begin(), memory.end(), 0);
  std::fill(V.begin(), V.end(), 0);
  I = 0;
  delayTimer = 0;
  soundTimer = 0;
  display.clear();
  currentOpCode = 0;
  pc = START_EXEC_LOCATION;
  while (!stack.empty()) {
    stack.pop();
  }

  std::copy(BUILTIN_FONT.begin(), BUILTIN_FONT.end(),
            memory.begin() + FONT_LOCATION);

  drawFlag = true;
}

void Chip8::printMemory() const {

  std::cout << std::hex;
  std::cout << "Reserved for interpreter:\n";
  for (auto i = memory.begin(); i != memory.begin() + START_EXEC_LOCATION;
       ++i) {
    std::cout << static_cast<int>(*i) << " ";
    ;
  }

  std::cout << "\n\n\n";

  std::cout << "ROM Execution:\n";
  for (auto i = memory.begin() + START_EXEC_LOCATION; i != memory.end(); ++i) {
    std::cout << static_cast<int>(*i) << " ";
    ;
  }

  std::cout << std::dec;
}

const bool Chip8::getDrawFlag() const { return drawFlag; }

void Chip8::debug() {
  std::cout << std::hex;
  std::cout << "V Registers:\n";
  for (auto i : V) {
    std::cout << static_cast<int>(i) << '\n';
  }
  std::cout << "\nI Register: " << static_cast<int>(I) << "\n\n";
  std::cout << "pc: " << static_cast<int>(pc) << "\n";
}

} // namespace PChip8
