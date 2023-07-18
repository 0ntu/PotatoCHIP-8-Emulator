#include "chip8.h"
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <map>
#include <thread>

const std::map<int, int> CHIP8_KEYS = {
    std::make_pair(SDLK_x, 0x0), std::make_pair(SDLK_1, 0x1),
    std::make_pair(SDLK_2, 0x2), std::make_pair(SDLK_3, 0x3),
    std::make_pair(SDLK_q, 0x4), std::make_pair(SDLK_w, 0x5),
    std::make_pair(SDLK_e, 0x6), std::make_pair(SDLK_a, 0x7),
    std::make_pair(SDLK_s, 0x8), std::make_pair(SDLK_d, 0x9),
    std::make_pair(SDLK_z, 0xA), std::make_pair(SDLK_c, 0xB),
    std::make_pair(SDLK_4, 0xC), std::make_pair(SDLK_r, 0xD),
    std::make_pair(SDLK_f, 0xE), std::make_pair(SDLK_v, 0xF),
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    return EXIT_FAILURE;
  }

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  SDL_Window *window = nullptr;
  SDL_Renderer *renderer = nullptr;
  SDL_CreateWindowAndRenderer(1024, 512, SDL_WINDOW_RESIZABLE, &window,
                              &renderer);
  SDL_SetWindowMinimumSize(window, PChip8::DISPLAY_WIDTH,
                           PChip8::DISPLAY_HEIGHT);
  SDL_SetWindowTitle(window, "CHIP-8 Emulator");
  SDL_RenderSetLogicalSize(renderer, 1024, 512);

  auto tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                               SDL_TEXTUREACCESS_STREAMING, 64, 32);

  PChip8::Chip8 chip8;

  try {
    chip8.loadROM(argv[1]);
  } catch (std::exception &e) {
    std::cerr << "error: " << e.what() << '\n';
    return EXIT_FAILURE;
  }

  // begin emulation loop
  while (true) {

    // Process SDL events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        return EXIT_SUCCESS;
      case SDL_WINDOWEVENT:
        chip8.drawFlag = true;
        break;
      case SDL_KEYUP:
      default:
        if (CHIP8_KEYS.contains(e.key.keysym.sym)) {
          chip8.keyPress[CHIP8_KEYS.at(e.key.keysym.sym)] = false;
        }
        break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_F1:
          chip8.reset();
          chip8.loadROM(argv[1]);
          break;
        default:
          if (CHIP8_KEYS.contains(e.key.keysym.sym)) {
            chip8.keyPress[CHIP8_KEYS.at(e.key.keysym.sym)] = true;
          }
          break;
        }
        break;
      }
    }

    try {
      chip8.cpuCycle();
    } catch (std::exception &e) {
      std::cerr << "error: " << e.what() << '\n';
      return EXIT_FAILURE;
    }

    // If draw occurred, redraw SDL screen
    if (chip8.drawFlag) {
      chip8.drawFlag = false;

      // Update SDL texture
      SDL_UpdateTexture(tex, nullptr, &chip8.display.getRawPixelGrid(),
                        64 * sizeof(Uint32));
      // Clear screen and render
      SDL_RenderClear(renderer);
      SDL_RenderCopy(renderer, tex, nullptr, nullptr);
      SDL_RenderPresent(renderer);
    }
    // Sleep to slow down emulation speed
    // std::this_thread::sleep_for(std::chrono::microseconds(1200));
  }
  // end emulation loop

  SDL_Delay(3000);
  SDL_DestroyTexture(tex);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  return EXIT_SUCCESS;
}
