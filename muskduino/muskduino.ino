// FastLED - Version: Latest 
#include <FastLED.h>

#define LED_PIN     9
#define NUM_LEDS    24
#define BRIGHTNESS  200
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND (20)

CRGBPalette16 currentPalette;

CRGB colors[] = { CRGB(0xff0000), CRGB(0xe1cf04), CRGB::Black };

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    
    SetupPalette();

    random16_add_entropy(analogRead(A0));
}

void loop()
{
    static uint8_t startIndex = 0;
    startIndex++; /* motion speed */
    
    FillLEDsFromPaletteColors(startIndex);
    
    FastLED.show();
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, BRIGHTNESS, LINEARBLEND);
        colorIndex += 3;
    }
}

void SetupPalette()
{
    for (int i = 0; i < 16; i++) {
        currentPalette[i] = colors[random8() % 3];
    }
}

