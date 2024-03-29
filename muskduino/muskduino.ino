// FastLED - Version: Latest 
#include <FastLED.h>

// Pins
#define BRIGHTNESS_PIN  2
#define LED_PIN         9
#define MODE_PIN        3

// Colors
#define RED             0xff0000
#define YELLOW          0xe1cf04

// Other
#define NUM_LEDS        24
#define LED_TYPE        WS2812
#define COLOR_ORDER     GRB
#define UPDATES_PER_SECOND (20)

const CRGB fire[] = { CRGB(RED), CRGB(YELLOW), CRGB::Black };
const extern TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;

int val, brightness;

volatile uint8_t mode = 0;
volatile static unsigned long last_interrupt_time = 0;

void setup() {   
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(200);

    pinMode(MODE_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MODE_PIN), changeMode, FALLING);
}

void loop()
{
    val = analogRead(BRIGHTNESS_PIN);
	brightness = map(val, 0, 1023, 25, 200);
	
    FastLED.setBrightness(brightness);

    if (mode == 0)
    {
        SetupPaletteFire();

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
    else if (mode == 2)
    {
        currentPalette = myRedWhiteBluePalette_p;

        static uint8_t startIndex = 0;
        startIndex++; /* motion speed */
	    FillLEDsFromPaletteColors(startIndex, brightness);
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

//**************************************************************************************
// Palette Setups
//**************************************************************************************
void SetupPaletteFire()
{
    for (int i = 0; i < 16; i++)
    {
        currentPalette[i] = fire[i % 3];
    }
}

//**************************************************************************************
// ISR
//**************************************************************************************
void changeMode()
{
    volatile unsigned long interrupt_time = millis();
    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 200)
    {
        if (++mode > 2) mode = 0;
        last_interrupt_time = interrupt_time;
    }
}

//**************************************************************************************
// Palettes
//**************************************************************************************
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Red,
    CRGB::Gray,     // 'white' is too bright compared to red and blue
    CRGB::Blue,
    CRGB::Black,
    
    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black,

    CRGB::Red,
    CRGB::Gray,
    CRGB::Blue,
    CRGB::Black
};