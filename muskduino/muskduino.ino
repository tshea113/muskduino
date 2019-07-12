// FastLED - Version: Latest 
#include <FastLED.h>

#define BRIGHTNESS_PIN	2
#define LED_PIN     	9
#define NUM_LEDS   		24
#define BRIGHTNESS  	200
#define LED_TYPE    	WS2812
#define COLOR_ORDER 	GRB

#define RED 			0xff0000
#define YELLOW			0xe1cf04

#define UPDATES_PER_SECOND (20)

CRGB leds[NUM_LEDS];
CRGB colors[] = { CRGB(RED), CRGB(YELLOW), CRGB::Black };

CRGBPalette16 currentPalette;

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    
    SetupPalette();

    random16_add_entropy(analogRead(A0));
}

void loop()
{
    int val = analogRead(BRIGHTNESS_PIN);
	int numLedsToLight = map(val, 0, 1023, 0, NUM_LEDS);
	
    static uint8_t startIndex = 0;
    startIndex++; /* motion speed */
	FillLEDsFromPaletteColors(startIndex, numLedsToLight);
	
	FastLED.show();
	
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex, int numLedsToLight)
{
    for (int i = 0; i < numLedsToLight; i++) {
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

