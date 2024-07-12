#ifndef menu_h
#define menu_h

#define encoderStepsMenu 4
#define I2C_ADDRESS 0x3C

#include <SSD1306Ascii.h>
#include <SSD1306AsciiAvrI2c.h>

extern SSD1306AsciiAvrI2c oled;

extern uint8_t resetMazeEEPROM;

void oledSetup(void);
void updateEncoder(void);
void displayMenu(void);
void printAndWait(const char *text, uint16_t time, bool waitButton = true);

#endif