#pragma once
#include "display.h"
#include <algorithm>
#include <iostream>

template <int xSize, int ySize, typename T>
void Display<xSize, ySize, T>::setPixel(unsigned int xCoord, unsigned int yCoord, T pixelData) {
  // 2D array (grid of pixels) into 1D
  rawPixelGrid[(yCoord * xSize) + xCoord] = pixelData;
}

template <int xSize, int ySize, typename T>
T Display<xSize, ySize, T>::getPixel(unsigned int xCoord, unsigned int yCoord) {

  return rawPixelGrid[(yCoord * xSize) + xCoord];
}

template <int xSize, int ySize, typename T>
void Display<xSize, ySize, T>::clear() {
  std::fill(rawPixelGrid.begin(), rawPixelGrid.end(), 0);
}

template <int xSize, int ySize, typename T>
const std::array<T, xSize * ySize> &
Display<xSize, ySize, T>::getRawPixelGrid() const {
  return rawPixelGrid;
}

template <int xSize, int ySize, typename T>
const int Display<xSize, ySize, T>::getPitch() const {
  return sizeof(T) * xSize;
}

template <int xSize, int ySize, typename T>
Display<xSize, ySize, T>::Display() = default;

template <int xSize, int ySize, typename T>
Display<xSize, ySize, T>::~Display() = default;
