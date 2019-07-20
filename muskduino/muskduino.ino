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
#define BLUE            0x0000ff
#define WHITE           0xffffff

#define UPDATES_PER_SECOND (20)

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

CRGB leds[NUM_LEDS];

CRGBPalette16 currentPalette;

volatile uint8_t mode = 0;

void setup() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(200);

    random16_add_entropy(analogRead(A0));

    pinMode(MODE_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MODE_PIN), changeMode, FALLING);
}

void loop()
{
    int val = analogRead(BRIGHTNESS_PIN);
	int brightness = map(val, 0, 1023, 25, 200);
	
    FastLED.setBrightness(brightness);

    if (mode == 0)
    {
        SetupPaletteFire();

        static uint8_t startIndex = 0;
        startIndex++; /* motion speed */
	    FillLEDsFromPaletteColors(startIndex, brightness, true);
        
        FastLED.show();
	
        FastLED.delay(1000 / UPDATES_PER_SECOND);
    }
    else if (mode == 1)
    {
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds[i] = CRGB::White;
            FastLED.show();
        }
    }
    else if (mode == 2)
    {
        SetupPaletteMurica();

        static uint8_t startIndex = 0;
        startIndex++; /* motion speed */
	    FillLEDsFromPaletteColors(startIndex, brightness, true);
        
        FastLED.show();
	
        FastLED.delay(800 / UPDATES_PER_SECOND);
    }	
}

void FillLEDsFromPaletteColors(uint8_t colorIndex, int brightness, bool blend)
{
    for (int i = 0; i < NUM_LEDS; i++) {
        if (blend)
        {
            leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, LINEARBLEND);
        } else
        {
            leds[i] = ColorFromPalette(currentPalette, colorIndex, brightness, NOBLEND);
        }
        
        colorIndex += 3;
    }
}


void changeMode()
{
    static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    // If interrupts come faster than 200ms, assume it's a bounce and ignore
    if (interrupt_time - last_interrupt_time > 200)
    {
        if (mode > 1)
        {
            mode = 0;
        } else 
        {
            mode++;
        }
    last_interrupt_time = interrupt_time;
    }
}

// **************************************************************************************
// Palette Setups
// **************************************************************************************

void SetupPaletteFire()
{
    CRGB fire[] = { CRGB(RED), CRGB(YELLOW), CRGB::Black };

    for (int i = 0; i < 16; i++) {
        currentPalette[i] = fire[random8() % 3];
    }
}

void SetupPaletteMurica()
{
    currentPalette = myRedWhiteBluePalette_p;
}

// **************************************************************************************
// Palettes
// **************************************************************************************

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
    CRGB::Black,
    CRGB::Red,
    CRGB::Red,
    CRGB::Red,
    
    CRGB::Black,
    CRGB::Black,

    CRGB::Gray,     // 'white' is too bright compared to red and blue
    CRGB::Gray,
    CRGB::Gray,
    CRGB::Gray,
    
    CRGB::Black,
    CRGB::Black,
    
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Blue,
    CRGB::Black
};