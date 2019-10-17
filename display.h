#ifndef DISP
#define DISP

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;

void drawSplash();
void drawStatus(float rate, float total, bool paused);
void drawMenu(int value, int state, float total, bool pumping);

#endif
