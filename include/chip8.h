#pragma once
#include "display.h"
#include <array>
#include <stack>
#include <cstdint>
#include <string>

namespace PChip8 {
// --- CONSTANTS ---

inline constexpr int DISPLAY_WIDTH = 64;
inline constexpr int DISPLAY_HEIGHT = 32;
inline constexpr int MEMORY_SIZE = 4096;
inline constexpr int VREG_COUNT = 16;
inline constexpr int FONT_LOCATION = 0x50;
inline constexpr int START_EXEC_LOCATION = 0x200;

// ----- FONT -----

inline constexpr std::array<uint8_t, 80> BUILTIN_FONT = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// ----------------
class Chip8 {
public:
  void debug();
  void cpuCycle();
  void loadROM(std::string fileName);
  void printMemory() const;
  void reset();
  Display<DISPLAY_WIDTH, DISPLAY_HEIGHT, uint32_t> display;
  [[nodiscard]] const bool getDrawFlag() const;

  Chip8();
  ~Chip8();

  bool drawFlag = false;

private:
  std::array<uint8_t, MEMORY_SIZE> memory{0};
  std::array<uint8_t, VREG_COUNT> V{0};
  uint16_t I{0};

  uint16_t pc{START_EXEC_LOCATION};
  std::stack<uint16_t> stack;


  uint8_t delayTimer{0};
  uint8_t soundTimer{0};

  uint16_t currentOpCode{0};

  void opCode_CLS();        // 00E0
  void opCode_RET();        // 00EE
  void opCode_JP();         // 1NNN
  void opCode_CALL();       // 2NNN
  void opCode_SE_VX_KK();   // 3XKK
  void opCode_SNE_VX_KK();  // 4XKK
  void opCode_SE_VX_VY();   // 5XY0
  void opCode_LD_VX_KK();   // 6XKK
  void opCode_ADD_VX_KK();  // 7XKK
  void opCode_LD_VX_VY();   // 8XY0
  void opCode_OR_VX_VY();   // 8XY1
  void opCode_AND_VX_VY();  // 8XY2
  void opCode_XOR_VX_VY();  // 8XY3
  void opCode_ADD_VX_VY();  // 8XY4
  void opCode_SUB_VX_VY();  // 8XY5
  void opCode_SHR_VX();     // 8XY6
  void opCode_SUBN_VX_VY(); // 8XY7
  void opCode_SHL_VX();     // 8XYE
  void opCode_SNE_VX_VY();  // 9XY0
  void opCode_LD_I();       // ANNN
  void opCode_JP_V0();      // BNNN
  void opCode_RND_VX();     // CXKK
  void opCode_DRW_VX_VY();  // DXYN
  void opCode_SKP_VX();     // EX9E
  void opCode_SKNP_VX();    // EXA1
  void opCode_LD_VX_DT();   // FX07
  void opCode_LD_VX_K();    // FX0A
  void opCode_LD_DT_VX();   // FX15
  void opCode_LD_ST_VX();   // FX18
  void opCode_ADD_I_VX();   // FX1E
  void opCode_LD_F_VX();    // FX29
  void opCode_LD_B_VX();    // FX33
  void opCode_LD_I_VX();    // FX55
  void opCode_LD_VX_I();    // FX65
};
} // namespace PChip8
