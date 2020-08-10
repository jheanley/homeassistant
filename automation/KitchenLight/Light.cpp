#include "Light.h"

Light::Light()
  : m_state(Off)
  , m_stateStr("OFF")
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
