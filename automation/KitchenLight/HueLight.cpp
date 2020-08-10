#include "HueLight.h"

HueLight::~HueLight()
{
  if( m_pLEDS )
  {
    delete m_pLEDS;
  }
}
