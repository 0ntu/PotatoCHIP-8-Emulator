#pragma once
#include <array>
#include <cstdint>

template<int xSize, int ySize, typename PixelType>
class Display {
public:
  Display();
  ~Display();

  void setPixel(unsigned int xCoord, unsigned int yCoord, PixelType pixelData);
  PixelType getPixel(unsigned int xCoord, unsigned int yCoord);

  void clear();
  [[nodiscard]] const int getPitch() const;
  [[nodiscard]] const std::array<PixelType, xSize*ySize>& getRawPixelGrid() const;

private:
  std::array<PixelType, xSize*ySize> rawPixelGrid {0};
};

#include "../src/display.cpp"
