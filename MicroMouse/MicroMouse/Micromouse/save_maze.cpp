#include <EEPROM.h>
#include "globals.h"
#include "floodfill.h"
#include "save_maze.h"

void updateMazeValuesFromEEPROM() {
  for (uint8_t i = 0; i < (rows * cols); i++) {
    floodArray[i].flood = EEPROM.read(i);
    if (i == 255) break;
  }
  for (uint8_t i = 0; i < (rows * cols); i++) {
    floodArray[i].neighbours = EEPROM.read((rows * cols) + (short)i);
    if (i == 255) break;
  }
  for (uint8_t i = 0; i < (rows * cols); i++) {
    floodArray[i].visited = EEPROM.read((2 * rows * cols) + (short)i);
    if (i == 255) break;
  }
  for (uint8_t i = 0; i < 6; i++) {
    *(values[i]) = EEPROM.read((3 * rows * cols) + (short)i);
  }
}

void updateMazeValuesInEEPROM() {
  for (uint8_t i = 0; i < (rows * cols); i++) {
    EEPROM.write(i, floodArray[i].flood);
    if (i == 255) break;
  }
  for (uint8_t i = 0; i < (rows * cols); i++) {
    EEPROM.write((rows * cols) + (short)i, floodArray[i].neighbours);
    if (i == 255) break;
  }
  for (uint8_t i = 0; i < (rows * cols); i++) {
    EEPROM.write((2 * rows * cols) + (short)i, floodArray[i].visited);
    if (i == 255) break;
  }
  for (uint8_t i = 0; i < 6; i++) {
    EEPROM.write((3 * rows * cols) + (short)i, *(values[i]));
  }
}

void resetMazeValuesInEEPROM() {
  for (uint8_t i = 0; i < (rows * cols); i++) {
    floodArray[i].flood = 255;
    for (uint8_t j = 0; j < 4; j++) floodArray[i].flood = minimum(floodArray[i].flood, distance(i, targetCells[j]));
    floodArray[i].neighbours = 0;
    floodArray[i].visited = 0;
    if (delineariseRow(i) == 0) markWall(i, north);
    if (delineariseCol(i) == 0) markWall(i, west);
    if (delineariseRow(i) == (rows - 1)) markWall(i, south);
    if (delineariseCol(i) == (cols - 1)) markWall(i, east);
    if (i == 255) break;
  }
  updateMazeValuesInEEPROM();
}