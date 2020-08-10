/*****************  NEEDED TO MAKE NODEMCU WORK ***************************/
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER

#include "FastLED.h"
#include "Light.h"

class HueLight
{
public:
  HueLight(int numLeds);
  virtual ~HueLight();

private:
  char* m_pLEDS;
};
