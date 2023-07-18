#include "chip8.h"
#include <bitset>
#include <cstdlib>

#define VX (V[(currentOpCode & 0x0F00) >> 8])
#define VY (V[(currentOpCode & 0x00F0) >> 4])

namespace PChip8 {
void Chip8::opCode_CLS() {
  // 00E0
  // Clear the display.

  drawFlag = true;
  display.clear();
}

void Chip8::opCode_RET() {
  // 00EE
  // Return from a subroutine.
  //
  // The interpreter sets the program counter to the address at the top of the
  // stack, then subtracts 1 from the stack pointer.
  pc = stack.top();
  stack.pop();
}

void Chip8::opCode_JP() {
  // 1NNN
  // Jump to location nnn.
  //
  // The interpreter sets the program counter to nnn.

  pc = currentOpCode & 0x0FFF;
}

void Chip8::opCode_CALL() {
  // 2NNN
  // Call subroutine at nnn.
  //
  // The interpreter increments the stack pointer, then puts the current PC on
  // the top of the stack. The PC is then set to nnn.
  stack.push(pc);
  pc = currentOpCode & 0x0FFF;
}
void Chip8::opCode_SE_VX_KK() {
  // 3XKK
  // Skip next instruction if Vx = kk.
  //
  // The interpreter compares register Vx to kk, and if they are equal,
  // increments the program counter by 2.
  if (VX == (currentOpCode & 0x00FF)) {
    pc += 2;
  }
}
void Chip8::opCode_SNE_VX_KK() {
  // 4XKK
  // Skip next instruction if Vx != kk.
  //
  //  The interpreter compares register Vx to kk, and if they are not equal,
  //  increments the program counter by 2.
  if (VX != (currentOpCode & 0x00FF)) {
    pc += 2;
  }
}
void Chip8::opCode_SE_VX_VY() {
  // 5XY0
  // Skip next instruction if Vx = Vy.
  //
  // The interpreter compares register Vx to register Vy, and if they are equal,
  // increments the program counter by 2.

  if (VX == VY) {
    pc += 2;
  }
}

void Chip8::opCode_LD_VX_KK() {
  // 6XKK
  // Set Vx = kk.
  //
  // The interpreter puts the value kk into register Vx.

  VX = currentOpCode & 0x00FF;
}

void Chip8::opCode_ADD_VX_KK() {
  // 7XKK
  // Set Vx = Vx + kk.
  //
  // Adds the value kk to the value of register Vx, then stores the result in
  // Vx.

  VX += currentOpCode & 0x00FF;
}

void Chip8::opCode_LD_VX_VY() {
  // 8XY0
  // Set Vx = Vy.
  //
  //  Stores the value of register Vy in register Vx.

  VX = VY;
}
void Chip8::opCode_OR_VX_VY() {
  // 8XY1
  // Set VX = VX OR VY
  //
  // Performs a bitwise OR on the values of Vx and Vy, then stores the result in
  // Vx. A bitwise OR compares the corrseponding bits from two values, and if
  // either bit is 1, then the same bit in the result is also 1. Otherwise, it
  // is 0.

  VX = VX | VY;
}

void Chip8::opCode_AND_VX_VY() {
  // 8XY2
  // Set VX = VX AND Vy
  //
  // Performs a bitwise AND on the values of Vx and Vy, then stores the result
  // in Vx. A bitwise AND compares the corrseponding bits from two values, and
  // if both bits are 1, then the same bit in the result is also 1. Otherwise,
  // it is 0.

  VX = VX & VY;
}
void Chip8::opCode_XOR_VX_VY() {
  // 8XY3
  // Set VX = VX XOR VY
  //
  //  Performs a bitwise exclusive OR on the values of Vx and Vy, then stores
  //  the result in Vx. An exclusive OR compares the corrseponding bits from two
  //  values, and if the bits are not both the same, then the corresponding bit
  //  in the result is set to 1. Otherwise, it is 0.

  VX = VX ^ VY;
}
void Chip8::opCode_ADD_VX_VY() {
  // 8XY4
  // Set Vx = Vx + Vy, set VF = carry.
  //
  // The values of Vx and Vy are added together. If the result is greater than 8
  // bits (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of
  // the result are kept, and stored in Vx.

  uint16_t sum = VX + VY;
  VX = (sum & 0x00FF);

  if (sum > UINT8_MAX) {
    V[0xF] = 1;
  } else {
    V[0xF] = 0;
  }
}
void Chip8::opCode_SUB_VX_VY() {
  // 8XY5
  // Set Vx = Vx - Vy, set VF = NOT borrow.
  //
  // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from
  // Vx, and the results stored in Vx.

  int result = VX - VY;
  VX = result;

  if (result > 0)
    V[0xF] = 1;
  else {
    V[0xF] = 0;
  }
}
void Chip8::opCode_SHR_VX() {
  // 8XY6
  // Set Vx = Vx SHR 1.
  //
  // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
  // Then Vx is divided by 2.

  bool overflows = false;

  if ((VX & 0b00000001) == 0b00000001)
    overflows = true;

  VX >>= 1;

  if (overflows)
    V[0xF] = 1;
  else
    V[0xF] = 0;
}
void Chip8::opCode_SUBN_VX_VY() {
  // 8XY7
  // Set Vx = Vy - Vx, set VF = NOT borrow.
  //
  // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from
  // Vy, and the results stored in Vx.

  int result = VY - VX;
  VX = result;

  if (result > 0)
    V[0xF] = 1;
  else {
    V[0xF] = 0;
  }
}
void Chip8::opCode_SHL_VX() {
  // 8XYE
  // Set Vx = Vx SHL 1.
  //
  // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to
  // 0. Then Vx is multiplied by 2.

  bool overflows = false;

  if ((VY & 0b10000000) == 0b10000000)
    overflows = true;

  VX = (VY << 1);

  if (overflows)
    V[0xF] = 1;
  else
    V[0xF] = 0;
}
void Chip8::opCode_SNE_VX_VY() {
  // 9XY0
  // Skip next instruction if Vx != Vy.
  //
  //  The values of Vx and Vy are compared, and if they are not equal, the
  //  program counter is increased by 2.

  if (VX != VY)
    pc += 2;
}

void Chip8::opCode_LD_I() {
  // ANNN
  // Set I = nnn.
  //
  // The value of register I is set to nnn.
  I = currentOpCode & 0x0FFF;
}

void Chip8::opCode_JP_V0() {
  // BNNN
  // Jump to location nnn + V0.
  //
  // The program counter is set to nnn plus the value of V0.

  pc = (currentOpCode & 0x0FFF) + V[0x0];
}
void Chip8::opCode_RND_VX() {
  // CXKK
  // Set Vx = random byte AND kk.
  //
  // The interpreter generates a random number from 0 to 255, which is then
  // ANDed with the value kk. The results are stored in Vx. See instruction 8xy2
  // for more information on AND.

  VX = (rand() % 256) & (currentOpCode & 0x00FF);
}

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

  int initialX = VX; // support wrapping for 1st pixel
  int initialY = VY;
  int numRows = currentOpCode & 0x000F;

  V[0xF] = 0;

  auto togglePixel = [this](int x, int y) {
    if (display.getPixel(x, y) == 0x00000000) {
      display.setPixel(x, y, 0xFFFFFFFF);
    } else {
      display.setPixel(x, y, 0xFF000000);
      V[0xF] = 1;
    }
  };

  for (int row = 0; row < numRows; ++row) {
    std::bitset<8> bs = memory[I + row];
    for (int col = 1; col <= bs.size(); ++col) {
      int x = initialX + col - 1;
      int y = initialY + row;
      if (bs[bs.size() - col]) {
        if (x < DISPLAY_WIDTH && y < DISPLAY_HEIGHT)
          togglePixel(x, y);
      }
    }
  }
}

void Chip8::opCode_SKP_VX() {
  // EX9E
  // Skip next instruction if key with the value of Vx is pressed.
  //
  // Checks the keyboard, and if the key corresponding to the value of Vx is
  // currently in the down position, PC is increased by 2.

  if (keyPress[VX]) {
    // skip instruction
    pc += 2;
  }
}
void Chip8::opCode_SKNP_VX() {
  // EXA1
  // Skip next instruction if key with the value of Vx is not pressed.
  //
  //  Checks the keyboard, and if the key corresponding to the value of Vx is
  //  currently in the up position, PC is increased by 2.

  if (!(keyPress[VX])) {
    // skip instruction
    pc += 2;
  }
}

void Chip8::opCode_LD_VX_DT() {
  // FX07
  // Set Vx = delay timer value.
  //
  // The value of DT is placed into Vx.

  VX = delayTimer;
}

void Chip8::opCode_LD_VX_K() {
  // FX0A
  // Wait for a key press, store the value of the key in VX
  //
  // All execution stops until a key is pressed, then the value of that key is
  // stored in Vx.


  bool keyPressed = false;

  for (int i = 0; i < keyPress.size(); ++i) {
    if (keyPress[i]) {
      VX = i;
      keyPressed = true;
    }
  }

  if (!keyPressed)
  // pause execution if no key is pressed
  // by resetting program counter to this opcode
    pc -= 2;

  return;
}

void Chip8::opCode_LD_DT_VX() {
  // FX15
  // Set delay timer = Vx.
  //
  // DT is set equal to the value of Vx.

  delayTimer = VX;
}

void Chip8::opCode_LD_ST_VX() {
  // FX18
  // Set sound timer = Vx.
  //
  // ST is set equal to the value of Vx.
  soundTimer = VX;
}

void Chip8::opCode_ADD_I_VX() {
  // FX1E
  // Set I = I + Vx.
  //
  // The values of I and Vx are added, and the results are stored in I.

  I += VX;
}
void Chip8::opCode_LD_F_VX() {
  // FX29
  // Set I = location of sprite for digit Vx.
  //
  // The value of I is set to the location for the hexadecimal sprite
  // corresponding to the value of Vx. See section 2.4, Display, for more
  // information on the Chip-8 hexadecimal font.

  // font is stored 0, 1, 2, 3, ... A, B, C
  // char of VX is offset
  // offset is only first nibble
  int fontOffset = static_cast<int>((VX & 0x0F) * 0x5);

  I = FONT_LOCATION + fontOffset;
}
void Chip8::opCode_LD_B_VX() {
  // FX33
  // Store BCD representation of Vx in memory locations I, I+1, and I+2.
  //
  // The interpreter takes the decimal value of Vx, and places the hundreds
  // digit in memory at location in I, the tens digit at location I+1, and the
  // ones digit at location I+2.

  memory[I] = (VX) / 100;
  memory[I + 1] = (VX / 10) % 10;
  memory[I + 2] = (VX) % 10;
}
void Chip8::opCode_LD_I_VX() {
  // FX55
  // Store registers V0 through Vx in memory starting at location I.
  //
  // The interpreter copies the values of registers V0 through Vx into memory,
  // starting at the address in I.

  for (uint8_t offset = 0; offset <= ((currentOpCode & 0x0F00) >> 8);
       ++offset) {
    memory[I + offset] = V[offset];
  }
}
void Chip8::opCode_LD_VX_I() {
  // FX65
  // Read registers V0 through Vx from memory starting at location I.
  //
  // The interpreter reads values from memory starting at location I into
  // registers V0 through Vx.

  for (int offset = 0; offset <= ((currentOpCode & 0x0F00) >> 8); ++offset) {
    V[offset] = memory[I + offset];
  }
}

} // namespace PChip8
