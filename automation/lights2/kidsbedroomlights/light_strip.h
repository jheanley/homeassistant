
#include <FastLED.h>

const int ledPin = 4; //marked as D2 on the board
CRGB leds[NUM_LEDS];

class LightStrip {
public:
public:
  void setup(int pin) {
    m_numLEDS = NUM_LEDS_PER_METER * METERS;
    FastLED.addLeds<WS2812B, ledPin, RGB>(leds, NUM_LEDS);
    memset(leds, 0x4e, sizeof(CHSV) * NUM_LEDS);
//    leds[0] = CHSV (5, 255, 192);
//    leds[1] = CHSV (50, 255, 192);
//    leds[2] = CHSV (100, 255, 192);
//    leds[3] = CHSV (150, 255, 192);
//    leds[4] = CHSV (200, 255, 192);
//    leds[5] = CHSV (255, 255, 192);
  }

  void update(float elapsed)
  {
    Serial.println("LightStrip::update()");
    m_currentLed+=5;
    Serial.println(m_currentLed);
    for( int i = 0 ; i < NUM_LEDS ; ++i )
    {
      if( m_currentLed == NUM_LEDS )
      {
        m_currentLed = 0;
      }
      float mod((float)i/(float)NUM_LEDS);
      int h = 255 * mod;
      Serial.println(m_currentLed);
      leds[m_currentLed++] = CHSV (h, 255, 192);
    }
    delay(10);
    FastLED.show();
  }

private:
  int m_ledPin = -1;
  int m_numLEDS = 0;
  int m_currentLed = 0;
  CRGB* m_leds = nullptr;
};
