// FastLED - Version: Latest 
#include <FastLED.h>

#define BRIGHTNESS_PIN  2
#define LED_PIN         9
#define MODE_PIN        3

#define NUM_LEDS        24
#define LED_TYPE        WS2812
#define COLOR_ORDER     GRB

#define RED             0xff0000
#define YELLOW          0xe1cf04

#define UPDATES_PER_SECOND (20)

CRGB leds[NUM_LEDS];
CRGB colors[] = { CRGB(RED), CRGB(YELLOW), CRGB::Black };

CRGBPalette16 currentPalette;

uint8_t mode = 0;

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(200);
    
    SetupPalette();

    random16_add_entropy(analogRead(A0));

    pinMode(MODE_PIN, INPUT_PULLUP);
}

void loop()
{
    int val = analogRead(BRIGHTNESS_PIN);
	int brightness = map(val, 0, 1023, 25, 200);
	
    FastLED.setBrightness(brightness);

    if (digitalRead(MODE_PIN) == LOW)
    {
        delay(50);
        if (digitalRead(MODE_PIN) == HIGH)
        {
            if (mode > 1)
            {
                mode = 0;
            } else {
                mode++;
            }
        }
    }

    if (mode == 0)
    {
        static uint8_t startIndex = 0;
        startIndex++; /* motion speed */
	    FillLEDsFromPaletteColors(startIndex, brightness);
    }
    else if (mode == 1)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::White;
        }
    }

	FastLED.show();
	
    FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors(uint8_t colorIndex, int brightness)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, LINEARBLEND);
        colorIndex += 3;
    }
}

void SetupPalette()
{
    for (int i = 0; i < 16; i++) {
        currentPalette[i] = colors[random8() % 3];
    }
}

