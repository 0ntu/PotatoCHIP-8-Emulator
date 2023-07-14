#include "chip8.h"
#include <bitset>

namespace PChip8 {
void Chip8::opCode_CLS() {
  // 00E0
  // Clear the display.

  drawFlag = true;
  display.clear();
}

void Chip8::opCode_RET() {}

void Chip8::opCode_JP() {
  // 1NNN
  // Jump to location nnn.
  //
  // The interpreter sets the program counter to nnn.

  pc = currentOpCode & 0x0FFF;
}

void Chip8::opCode_CALL() {}
void Chip8::opCode_SE_VX_KK() {}
void Chip8::opCode_SNE_VX_KK() {}
void Chip8::opCode_SE_VX_VY() {}

void Chip8::opCode_LD_VX_KK() {
  // 6XKK
  // Set Vx = kk.
  //
  // The interpreter puts the value kk into register Vx.

  V[(currentOpCode & 0x0F00) >> 8] = currentOpCode & 0x00FF;
}

void Chip8::opCode_ADD_VX_KK() {
  // 7XKK
  // Set Vx = Vx + kk.
  //
  // Adds the value kk to the value of register Vx, then stores the result in
  // Vx.

  V[(currentOpCode & 0x0F00) >> 8] += currentOpCode & 0x00FF;
}

void Chip8::opCode_LD_VX_VY() {}
void Chip8::opCode_OR_VX_VY() {}

void Chip8::opCode_AND_VX_VY() {}
void Chip8::opCode_XOR_VX_VY() {}
void Chip8::opCode_ADD_VX_VY() {}
void Chip8::opCode_SUB_VX_VY() {}
void Chip8::opCode_SHR_VX() {}
void Chip8::opCode_SUBN_VX_VY() {}
void Chip8::opCode_SHL_VX() {}
void Chip8::opCode_SNE_VX_VY() {}

void Chip8::opCode_LD_I() {
  // ANNN
  // Set I = nnn.
  //
  // The value of register I is set to nnn.
  I = currentOpCode & 0x0FFF;
}

void Chip8::opCode_JP_V0() {}
void Chip8::opCode_RND_VX() {}

void Chip8::opCode_DRW_VX_VY() {
  // DXYN
  // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF =
  // collision.
  //
  // The interpreter reads n bytes from memory, starting at the address stored
  // in I. These bytes are then displayed as sprites on screen at coordinates
  // (Vx, Vy). Sprites are XORed onto the existing screen. If this causes any
  // pixels to be erased, VF is set to 1, otherwise it is set to 0. If the
  // sprite is positioned so part of it is outside the coordinates of the
  // display, it wraps around to the opposite side of the screen. See
  // instruction 8xy3 for more information on XOR, and section 2.4, Display, for
  // more information on the Chip-8 screen and sprites.

  drawFlag = true;

  int initialX = V[(currentOpCode & 0x0F00) >> 8];
  int initialY = V[(currentOpCode & 0x00F0) >> 4];
  int numRows = currentOpCode & 0x000F;

  V[0xF] = 0;

  for (int row = 0; row < numRows; ++row) {
    std::bitset<8> bs = memory[I + row];
    for (int col = 0; col < 8; ++col) {
      int x = initialX + col;
      int y = initialY + row;
      if (bs[7 - col]) {
        if (x > DISPLAY_WIDTH || y > DISPLAY_HEIGHT)
          continue;

        if (display.getPixel(x, y) == 0x00000000) {
          display.setPixel(x, y, 0xFFFFFFFF);
          continue;
        } else {
          display.setPixel(x, y, 0xFF000000);
          V[0xF] = 1;
          continue;
        }
      }
    }
  }
}

void Chip8::opCode_SKP_VX() {}
void Chip8::opCode_SKNP_VX() {}
void Chip8::opCode_LD_VX_DT() {}
void Chip8::opCode_LD_VX_K() {}
void Chip8::opCode_LD_DT_VX() {}
void Chip8::opCode_LD_ST_VX() {}
void Chip8::opCode_ADD_I_VX() {}
void Chip8::opCode_LD_F_VX() {}
void Chip8::opCode_LD_B_VX() {}
void Chip8::opCode_LD_I_VX() {}
void Chip8::opCode_LD_VX_I() {}
} // namespace PChip8
