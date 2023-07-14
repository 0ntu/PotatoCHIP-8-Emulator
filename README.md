# 🕹️ P0TAT0:CHIP Emulator (Experimental)
<p align="left">
  <a href="#"><img src="https://img.shields.io/badge/c++-%2300599C.svg?style=flat&logo=c%2B%2B&logoColor=white"></img></a>
  <a href="http://opensource.org/license/gpl-3-0/"><img src="https://img.shields.io/github/license/0ntu/CHIP-8-Emulator?color=blue"></img></a>
  <a href="https://github.com/thibaultmeyer/chip8-emulator/releases"><img src="https://img.shields.io/badge/latest_release-0.0.1-orange.svg"></img></a>

  </p>
A *(work-in-progress)* interpreter for the CHIP-8 Programming Language written in C++ / SDL2

---

![image](https://github.com/0ntu/CHIP-8-Emulator/assets/73860587/49624f89-1220-435b-a564-969c8068580b)

# Background
*[From Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)*

CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker made on his 1802 Microprocessor. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for these computers. 

There are a number of classic video games ported to CHIP-8, such as Pong, Space Invaders, Tetris, and Pac-Man. There are also applications like a random maze generator and Conway's Game of Life. 

# Specifications

- Memory: direct access to 4KB
- Display: 64 x 32, monochrome
- Program Counter: points to the current instruction in memory
- 16-bit Index Register: points to locations in memory
- Stack for 16-bit addresses: call functions and return from them
- 8-bit delay timer: decremented at a rate of 60Hz until it reaches 0
- 8-bit sound timer: beeps if not 0, decrements at a rate of 60Hz
- 16 8-bit general purpose Variable Registers: `V0-VF`

# Building From Source
To compile and run this project, ensure that you have [CMake](https://cmake.org/download/), [Make](https://www.gnu.org/software/make/), and [SDL2](https://www.libsdl.org/) installed.

```
> mkdir build
> cd build
> cmake -DCMAKE_BUILD_TYPE=Release ../
> make
```
# License
This project is released under the [GPLv3 License](https://www.gnu.org/licenses/gpl-3.0.en.html)

# Resources Used
- https://tobiasvl.github.io/blog/write-a-chip-8-emulator/
- http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
- https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference
