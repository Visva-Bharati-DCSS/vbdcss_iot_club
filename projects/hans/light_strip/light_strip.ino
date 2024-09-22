#include <FastLED.h>

#define NUM_LEDS 8
#define NUM_COLORS 5
#define ANIMATION_TIME 100
#define DATA_PIN 27
#define BUTTON_PIN 26

CRGB leds[NUM_LEDS];

CRGB::HTMLColorCode colors[NUM_COLORS] = {CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::White, CRGB::Cyan};

int currentColor = 0;


void setup()
{
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}


#define IS_BUTTON_ON (digitalRead(BUTTON_PIN) == LOW)

void animateStrip()
{
    if (NUM_LEDS % 2 == 0) {
        for (int i = 0; i < NUM_LEDS/2; ++i) {
            leds[NUM_LEDS/2 + i] = colors[currentColor];
            leds[NUM_LEDS/2 - i - 1] = colors[currentColor];
            FastLED.show();
            delay(ANIMATION_TIME);
        }
    } else {
        leds[NUM_LEDS/2] = colors[currentColor];
        FastLED.show();
        delay(50);
        for (int i = 0; i < NUM_LEDS/2; ++i) {
            leds[NUM_LEDS/2 + i + 1] = colors[currentColor];
            leds[NUM_LEDS/2 - i - 1] = colors[currentColor];
            FastLED.show();
            delay(ANIMATION_TIME);
        }
    }
    currentColor = ++currentColor % NUM_COLORS;
}

void loop()
{
    if (IS_BUTTON_ON) animateStrip();
    delay(ANIMATION_TIME);
}
