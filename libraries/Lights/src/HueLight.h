/*****************  NEEDED TO MAKE NODEMCU WORK ***************************/
//#define FASTLED_INTERRUPT_RETRY_COUNT 0
//#define FASTLED_ESP8266_RAW_PIN_ORDER
//#define FASTLED_ALLOW_INTERRUPTS 0

#include "FastLED.h"
#include "Light.h"

#define MAX_STORED_COLOURS 3

class Effect;

class HueLight : public Light
{

public:
    enum HueStateFlags
    {
        Colour = 0x04,
        White = 0x08
    };

public:
  HueLight();
  virtual ~HueLight();

  uint8_t* pixels() const { return m_pixels; }

  uint     numPixels() const { return m_numPixels; }

  uint8_t  pixelSize() const { return m_pixelSize; }

  const CRGBPalette16& palette() const { return m_currentPalette; }

  const CRGB& colour( int index = 0 ) { return m_colours[index]; }

  void setPixelController(CLEDController* controller);

  virtual void init( uint8_t* pixels, uint8_t pixelSize, uint numPixels);
  
  virtual void update( float elapsed ) override;

  virtual void apply() override;

  void setColour( const CRGB& colour );

  void setColour( const CRGBW& colour );

  void setWhite( uint8_t white );

  void setEffect( const std::string& effect );

  void setLed( int index, const CRGB& colour );

  virtual void showColor( const struct CRGB & data ) override;

  virtual void processState( const StaticJsonDocument<1024>& jsonDoc ) override;

  virtual void getState( StaticJsonDocument<1024>& jsonDoc ) override;

private:
  uint8_t*        m_pixels;
  uint            m_numPixels;
  uint8_t         m_pixelSize;
  CLEDController* m_controller;
  CRGB            m_colours[MAX_STORED_COLOURS];
  uint8_t         m_white;
  CRGBPalette16   m_currentPalette;
  std::string     m_currentEffectName;
  Effect*         m_currentEffect = nullptr;
};
