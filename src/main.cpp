#include "chip8.h"
#include <SDL2/SDL.h>
#include <array>
#include <cstdint>
#include <iostream>
#include <thread>

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

  while (true) {
    try {
      chip8.cpuCycle();
    } catch (std::exception &e) {
      std::cerr << "error: " << e.what() << '\n';
      return EXIT_FAILURE;
    }

    // Process SDL events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT:
        return EXIT_SUCCESS;
      case SDL_WINDOWEVENT:
        chip8.drawFlag = true;
        break;
      case SDL_KEYDOWN:
        switch (e.key.keysym.sym) {
        case SDLK_F1:
          chip8.reset();
          chip8.loadROM(argv[1]);
          break;
        }
      }
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
    std::this_thread::sleep_for(std::chrono::microseconds(1200));
  }

  SDL_Delay(3000);
  SDL_DestroyTexture(tex);
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  return EXIT_SUCCESS;
}
