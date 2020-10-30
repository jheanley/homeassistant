#include "Light.h"

Light::Light()
  : m_state(Off)
  , m_stateStr("OFF")
{
  
}

Light::~Light()
{
  
}

void Light::setState(State state)
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
}

void Light::setState( const std::string& state )
{
  m_stateStr = state;
  if( state == "ON" )
  {
    m_state = On;
  }
  else
  {
    m_state = Off;
  }
}

int Light::brightness() const
{
  return m_brightness;
}

void Light::setBrightness(int brightness)
{
  m_brightness = max(min(brightness, 255), 0);
  m_brightness = brightness;
}

void Light::apply()
{
  if( m_controlPin != -1 )
  {
    analogWrite(m_controlPin, m_brightness);
  }
}

void Light::mqttMessage(MqttClient::MessageData& md)
{
  
}
