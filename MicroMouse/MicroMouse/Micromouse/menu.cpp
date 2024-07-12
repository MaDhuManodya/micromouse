#include "globals.h"
#include "menu.h"

#define delineariseRow(location) (location / cols)
#define delineariseCol(location) (location % cols)

uint8_t menu = 0;  // This determines which value the encoder updates
short change;
uint8_t* values[7] = { &startCell, &(targetCells[0]), &(targetCells[1]), &(targetCells[2]), &(targetCells[3]), &startDir, &resetMazeEEPROM };

uint8_t resetMazeEEPROM = 0;  // This determines whether or not the maze data stored in the EEPROM should be reset

void oledSetup() {
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.set2X();
  oled.setLetterSpacing(1.5);
}

void updateEncoder() {
  newPosition1 = myEnc1.read();
  newPosition2 = myEnc2.read();
  if (abs(newPosition1 - oldPosition1) >= encoderStepsMenu) {
    menu += (newPosition1 - oldPosition1) / encoderStepsMenu;
    oldPosition1 = newPosition1;
    if (menu > 100) menu = 0;
    if (menu > 6) menu = 6;
    displayMenu();
  }
  if (abs(newPosition2 - oldPosition2) >= encoderStepsMenu) {
    change = (newPosition2 - oldPosition2) / encoderStepsMenu;
    oldPosition2 = newPosition2;
    if (menu <= 4) {
      if (change < 0 && *(values[menu]) < absolute(change)) *(values[menu]) = (rows * cols) - 1;
      else *(values[menu]) = (*(values[menu]) + change) % (rows * cols);
    } else if (menu == 5) {
      if (change < 0 && *(values[menu]) < absolute(change)) *(values[menu]) = 3;
      else *(values[menu]) = (*(values[menu]) + change) % 4;
    } else if (menu == 6) {
      *(values[menu]) = (*(values[menu]) + change);
      if(*(values[menu]) > 200) *(values[menu]) = 0;
      else if (*(values[menu]) > 1) *(values[menu]) = 1;
    }
    displayMenu();
  }
}

void displayMenu() {
  oled.clear();

  char menuTitle[6];
  char menuValue[7];
  const char directions[4][6] = { "North", "East", "South", "West" };

  if (menu <= 4) {
    if (menu == 0) sprintf(menuTitle, "Start");
    else sprintf(menuTitle, "End %u", menu);
    sprintf(menuValue, "%u, %u", delineariseRow(*(values[menu])), delineariseCol(*(values[menu])));
    oled.println(menuTitle);
    oled.println(menuValue);
  } else if (menu == 5) {
    oled.println("Direction");
    oled.println(directions[(*(values[menu]))]);
  } else if (menu == 6) {
    oled.println("Reset Maze");
    oled.println((*(values[menu])) ? "Yes" : "No");
  }
}

void printAndWait(const char *text, uint16_t time, bool waitButton) {
  oled.clear();
  oled.println(text);
  if (waitButton) while (digitalRead(button));
  delay(time);
}