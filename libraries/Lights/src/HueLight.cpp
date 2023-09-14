#include "HueLight.h"

#include "effects/SolidEffect.h"
#include "effects/AuroraEffect.h"

HueLight::HueLight()
{
    memset(m_touchReadings, 0x00, sizeof(uint16_t) * MAX_TOUCH_READINGS);
}

HueLight::~HueLight()
{
}

bool HueLight::aboveTouchThreshold(uint32_t threshold) const
{
    int maxValue(0);
    for( int i = 0 ; i < MAX_TOUCH_READINGS ; ++i )
    {
        if( m_touchReadings[i] > maxValue )
        {
            maxValue = m_touchReadings[i];
        }
    }
    return maxValue > threshold;
}

void HueLight::printTouchReadings() const
{
        Serial.print("Touch Readings: ");
        for( int i = 0 ; i < MAX_TOUCH_READINGS ; ++i )
        {
            Serial.print(m_touchReadings[i]);
            Serial.print(", ");
        }
        Serial.println("");
}

void HueLight::handleTouch(float elapsed)
{
    memmove(m_touchReadings + 1, m_touchReadings, sizeof(uint16_t) * (MAX_TOUCH_READINGS-1));
    m_touchReadings[0] = analogRead(A0);

    bool touched(false);

    if( !m_touchedLastFrame )
    {
        touched = aboveTouchThreshold(25);
    }
    else
    {
        touched = aboveTouchThreshold(12);
    }

    if( touched
            && !m_touchedLastFrame )
    {
        Serial.println("Started touching sensor");
        m_touchTime = 0.0f;
        m_changingHue = false;
    }
    else if( touched
             && m_touchedLastFrame )
    {
        m_touchTime += elapsed;
        if( m_touchTime > 1.5f )
        {
            if( !m_changingHue )
            {
                Serial.println("Change hue mode");
                m_changingHue = true;

                m_startHue = rgb2hsv_approximate(m_colours[0]).hue;

                m_touchUpdateTimer = 0.0f;
            }
            m_startHue += elapsed * 5;
            if( m_startHue > 255 )
            {
                m_startHue = 0.0f;
            }
            CHSV hsv;
            hsv.hue = m_startHue;
            hsv.val = 255;
            hsv.sat = 240;

            setColour(CRGB(hsv));
        }
    }
    else if( !touched
             && m_touchedLastFrame )
    {
        //Serial.println("Stopped touching sensor");
        //printTouchReadings();
        m_changingHue = false;
        if( m_touchTime < 1.5f )
        {
            //Serial.println("Change brightness");
            if( brightness() < 80 )
            {
                setBrightness(80);
            }
            else if( brightness() < 160 )
            {
                setBrightness(160);
            }
            else if( brightness() < 255 )
            {
                setBrightness(255);
            }
            else
            {
                setBrightness(0);
            }
        }
    }

    m_touchedLastFrame = touched;

    if( m_touchedLastFrame
            && m_touchTime > 1.5f )
    {
        m_touchUpdateTimer += elapsed;
        if( m_touchUpdateTimer < 1.0f )
        {
            clearStateFlags();
        }
        if( m_touchUpdateTimer > 1.0f )
        {
            m_touchUpdateTimer = 0.0f;
        }
    }
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
    if( pixelSize == 4 )
    {
        setStateFlag(White);
    }
}
  
void HueLight::update( float elapsed )
{
    //Serial.println("HueLight::update");
    handleTouch(elapsed);

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
        //Serial.println("HueLight::apply");
        uint8_t currentBrightness = brightness();
        if( state() == Off )
        {
            currentBrightness = 0;
        }
        CRGB scale(currentBrightness, currentBrightness, currentBrightness);
        m_controller->setDither(0);
        //FastLED.show();
        m_controller->show(m_pixels, m_pixelSize, m_numPixels, currentBrightness);
    }
    setUpdatePixels(false);
}

void HueLight::setColour(const CRGB& colour)
{
    Serial.print("HueLight::setColour - R: ");
    Serial.print(colour.r);
    Serial.print(", G: ");
    Serial.print(colour.g);
    Serial.print(", B: ");
    Serial.println(colour.b);
    for( int i = 0 ; i < MAX_STORED_COLOURS - 1 ; ++i )
    {
        m_colours[i+1] = m_colours[i];
    }
    m_colours[0] = colour;
    fill_solid(m_pixels, m_pixelSize, m_numPixels, colour);
    setStateFlag(Colour);
}

void HueLight::setColour( const CRGBW& colour )
{
    Serial.print("HueLight::setColour - R: ");
    Serial.print(colour.r);
    Serial.print(", G: ");
    Serial.print(colour.g);
    Serial.print(", B: ");
    Serial.print(colour.b);
    Serial.print(", W: ");
    Serial.println(colour.w);
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
}

void HueLight::setEffect( const String& effect )
{
    if( m_currentEffectName != effect )
    {
        m_currentEffectName = effect;
        if( m_currentEffect )
        {
            delete m_currentEffect;
            m_currentEffect = nullptr;
        }
        if( effect == "none"
                || effect == "solid" )
        {
            m_currentEffect = new SolidEffect();
            m_currentEffect->init(this);
        }
        if( effect == "rainbow"
                || effect == "aurora" )
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
        if( colourDoc.containsKey("w") )
        {
            uint8_t white = colourDoc["w"];
            setWhite(white);
        }

        setColour(newColour);
    }

    if( jsonDoc.containsKey("effect") )
    {
        setEffect(jsonDoc["effect"]);
    }
    setUpdatePixels(true);
}

void HueLight::getState(StaticJsonDocument<1024>& jsonDoc)
{
    jsonDoc["state"] = getStateString();
    jsonDoc["brightness"] = brightness();
    StaticJsonDocument<200> doc;
    doc["r"] = m_colours[0].r;
    doc["g"] = m_colours[0].g;
    doc["b"] = m_colours[0].b;
    doc["w"] = m_white;

    jsonDoc["color"] = doc;
}
