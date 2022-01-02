#include "HueLight.h"

#include "effects/AuroraEffect.h"

HueLight::HueLight()
{

}

HueLight::~HueLight()
{
}

void HueLight::setPixelController(CLEDController* controller)
{
    m_controller = controller;
}

void HueLight::init(uint8_t* pixels, uint8_t pixelSize, uint numPixels)
{
    m_pixels = pixels;
    m_numPixels = numPixels;
    m_pixelSize = pixelSize;
    m_currentPalette = PartyColors_p;
    setStateFlag(Power);
    setStateFlag(Colour);
    setStateFlag(Brightness);
}
  
void HueLight::update( float elapsed )
{
    if( m_currentEffect )
    {
        m_currentEffect->update(elapsed);
        setUpdatePixels(true);
    }
}

void HueLight::apply()
{
    if( updatePixels() )
    {
        uint8_t currentBrightness = brightness();
        if( state() == Off )
        {
            currentBrightness = 0;
        }
        CRGB scale(currentBrightness, currentBrightness, currentBrightness);
        m_controller->setDither(0);
        m_controller->show(m_pixels, m_pixelSize, m_numPixels, currentBrightness);
    }
    setUpdatePixels(false);
}

void HueLight::setColour(const CRGB& colour)
{
    for( int i = 0 ; i < MAX_STORED_COLOURS - 1 ; ++i )
    {
        m_colours[i+1] = m_colours[i];
    }
    m_colours[0] = colour;
    fill_solid(m_pixels, m_pixelSize, m_numPixels, colour);
    setStateFlag(Colour);
    setUpdatePixels(true);
}

void HueLight::setColour( const CRGBW& colour )
{
    for( int i = 0 ; i < MAX_STORED_COLOURS - 1 ; ++i )
    {
        m_colours[i+1] = m_colours[i];
    }
    m_colours[0] = colour;
    fill_solid(m_pixels, m_pixelSize, m_numPixels, colour);
    setStateFlag(Colour);
    setUpdatePixels(true);
}

void HueLight::setWhite( uint8_t white )
{
    m_white = white;
    if( isValidState(White) )
    {
        //CRGBW colourm_colours[0]);
        setStateFlag(White);
        setUpdatePixels(true);
    }
}

void HueLight::setEffect( const std::string& effect )
{
    if( m_currentEffectName != effect )
    {
        m_currentEffectName = effect;
        if( m_currentEffect )
        {
            delete m_currentEffect;
            m_currentEffect = nullptr;
        }
        if( effect == "rainbow" )
        {
            m_currentEffect = new AuroraEffect();
            m_currentEffect->init(this);
        }
    }
}

void HueLight::setLed( int index, const CRGB& colour )
{
    if( index >= 0
        && index < m_numPixels )
    {
        // Serial.print("HueLight::setLed - index: ");
        // Serial.print(index);
        // Serial.print(", R: ");
        // Serial.print(colour.r);
        // Serial.print(", G: ");
        // Serial.print(colour.g);
        // Serial.print(", B: ");
        // Serial.println(colour.b);
        uint32_t byteIndex = index * m_pixelSize;
        CRGB* pixel = (CRGB*)&(m_pixels[byteIndex]);
        *pixel = colour;
        // Serial.print("HueLight::setLed - byteIndex: ");
        // Serial.print(byteIndex);
        // Serial.print(", R: ");
        // Serial.print(pixel->r);
        // Serial.print(", G: ");
        // Serial.print(pixel->g);
        // Serial.print(", B: ");
        // Serial.println(pixel->b);
    }
}

void HueLight::showColor(const CRGB& data)
{
    m_controller->showColor(data);
}

void HueLight::processState(const StaticJsonDocument<1024>& jsonDoc)
{
    Serial.print(name().c_str());
    Serial.println(": received new state");

    String state = jsonDoc["state"];
    Serial.println(state.c_str());
    setState(state);

    if( jsonDoc.containsKey("brightness") )
    {
        int newBrightness = jsonDoc["brightness"];
        setBrightness(newBrightness);
    }

    if( jsonDoc.containsKey("color") )
    {
        DynamicJsonDocument colourDoc = jsonDoc["color"];
        CRGB newColour;
        newColour.r = colourDoc["r"];
        newColour.g = colourDoc["g"];
        newColour.b = colourDoc["b"];

        setColour(newColour);
    }

    if( jsonDoc.containsKey("effect") )
    {
        setEffect(jsonDoc["effect"]);
    }
}

void HueLight::getState(StaticJsonDocument<1024>& jsonDoc)
{
    jsonDoc["state"] = getStateString();
    StaticJsonDocument<200> doc;
    doc["r"] = m_colours[0].r;
    doc["g"] = m_colours[0].g;
    doc["b"] = m_colours[0].b;

    jsonDoc["color"] = doc;
}
