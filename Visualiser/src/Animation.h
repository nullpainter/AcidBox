#pragma once

#include <FastLED.h>

#define LED_PIN     17
#define NUM_LEDS    64
#define BRIGHTNESS  128 
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

#define UPDATES_PER_SECOND 15

void ChangePalettePeriodically();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void SetupPurpleAndGreenPalette();
void SetupTotallyRandomPalette();
void SetupBlackAndWhiteStripedPalette();

