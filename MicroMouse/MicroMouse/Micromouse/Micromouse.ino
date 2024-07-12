/** Resources
  * https://docs.arduino.cc/learn/programming/bit-math
  * https://marsuniversity.github.io/ece387/FloodFill.pdf
  * http://craga89.github.io/Micromouse/
  * https://www.geeksforgeeks.org/set-clear-and-toggle-a-given-bit-of-a-number-in-c/
*/

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "globals.h"
#include "menu.h"
#include "floodfill.h"
#include "save_maze.h"

Encoder myEnc1(2, 8);
Encoder myEnc2(3, 12);
SSD1306AsciiAvrI2c oled;

const uint8_t rows = 16, cols = 16;

struct cell floodArray[rows * cols];  // This array stores the flood value and neighbour data for all the cells

uint8_t targetCells[4], startCell, startDir;

int sensorValue[7];

long newPosition1, newPosition2, oldPosition1, oldPosition2;

void setup() {
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);

  pinMode(button, INPUT_PULLUP);
  pinMode(sensor_On_Pin, OUTPUT);

  oledSetup();
  updateMazeValuesFromEEPROM();
  printAndWait("Configure", 500);
  newPosition1 = myEnc1.read(), newPosition2 = myEnc2.read(), oldPosition1 = myEnc1.read(), oldPosition2 = myEnc2.read();
  displayMenu();
  while (digitalRead(button)) updateEncoder();
  printAndWait("Saving", 0, false);
  if (resetMazeEEPROM) resetMazeValuesInEEPROM();
  else updateMazeValuesInEEPROM();
  resetEnc();
  printAndWait("Callibrate", 500);
  printAndWait("Callibrating", 0, false);
  calibrate();
}

void loop() {
  initialise();
  printAndWait("Start", 100);
  printAndWait("Running", 500, false);
  
  while (currentCell != targetCells[0] && currentCell != targetCells[1] && currentCell != targetCells[2] && currentCell != targetCells[3]) {
    updateWalls();
    flood();
    updateTargetCell();
    goToTargetCell();
    floodArray[currentCell].visited = 1;
  }
  
  printAndWait("Save", 100);
  printAndWait("Saving", 0, false);
  updateMazeValuesInEEPROM();
}