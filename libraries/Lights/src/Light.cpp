#include "Light.h"

Light::Light()
    : m_validStateFlags(0)
    , m_stateUpdateFlags(0)
    , m_controlPin(-1)
    , m_state(Off)
    , m_stateStr("OFF")
    , m_brightness(150)
    , m_updatePixels(true)
{

}

Light::~Light()
{

}

void Light::init()
{
    m_stateUpdateFlags |= Power;
    m_stateUpdateFlags |= Brightness;
}

void Light::setState(State state)
{
    if( state != m_state )
    {
        m_state = state;
        if( m_state == On )
        {
            m_stateStr = "ON";
        }
        else
        {
            m_stateStr = "OFF";
        }
        setUpdatePixels(true);
    }
    setStateFlag(Power);
}

void Light::setState( const String& state )
{
    m_stateStr = state;
    State newState(Off);
    if( state == "ON" )
    {
        newState = On;
    }
    setState(newState);
}

int Light::brightness() const
{
    return m_brightness;
}

void Light::setBrightness(int brightness)
{
    Serial.print("Light::setBrightness - ");
    Serial.println(brightness);
    m_brightness = max(min(brightness, 255), 0);
    m_brightness = brightness;
    setStateFlag(Brightness);
    setUpdatePixels(true);
}

void Light::apply()
{
    if( m_controlPin != -1 )
    {
        //analogWrite(m_controlPin, m_brightness);
    }
}

void Light::processState(const StaticJsonDocument<1024> &jsonDoc)
{
    Serial.print(m_name.c_str());
    Serial.println(": received new state");
}

void Light::getState(StaticJsonDocument<1024>& jsonDoc)
{

}
