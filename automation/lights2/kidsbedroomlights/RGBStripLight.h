#include <FastLED.h>

#define NUM_LEDS_PER_METER 60
#define METERS 5
#define NUM_LEDS 300

const int rgbPin = 4;
CRGB leds[300];

class RGBLightStrip {
public:
public:
  void setup() {
    //m_leds = new CRGB[m_numLEDS];
//    FastLED.addLeds<WS2812B, 4, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2812B, 4, RGB>(leds, NUM_LEDS);
    setLED(-1, CHSV(70, 127, 180));
    FastLED.show();
  }

  void update(float elapsed)
  {
  }

  void setLED(int led, CHSV colour)
  {
    if( led = -1 )
    {
      for( int i = 0 ; i < NUM_LEDS ; ++i)
      {
        leds[i] = colour;
      }
    }
    if( led >= 0
        && led < NUM_LEDS )
    {
      leds[led] = colour;
    }
    else
    {
      Serial.println("Error, bad led number");
      Serial.println(led);
    }
  }
  void setLED(int led, CRGB colour)
  {
    if( led >= 0
        && led < NUM_LEDS )
    {
      leds[led] = colour;
    }
    else
    {
      Serial.println("Error, bad led number");
      Serial.println(led);
    }
  }

private:
  //int m_numLEDS = 0;
  int m_startLed = 0;
  int hue = 0;
  int satuaration = 0;
  int value = 0;
  //CRGB* m_leds = nullptr;
};
