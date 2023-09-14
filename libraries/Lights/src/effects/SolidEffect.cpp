#include "SolidEffect.h"

#include "HueLight.h"

const float SolidEffect::COLOUR_CHANGE_TIME(1.0f);

SolidEffect::SolidEffect()
{

}

void SolidEffect::init(HueLight* pLight)
{
    Effect::init(pLight);

    m_changeTime = COLOUR_CHANGE_TIME;
}

void SolidEffect::update(float elapsed)
{
    if( light()->colour(0) != m_lastColour )
    {
        if( m_changeTime >= COLOUR_CHANGE_TIME )
        {
            m_changeTime = 0.0f;
        }

        m_changeTime += elapsed;

        if( m_changeTime > COLOUR_CHANGE_TIME )
        {
            // clamp changeTime
            m_changeTime = COLOUR_CHANGE_TIME;
        }

        m_currentColour = blend(m_lastColour, light()->colour(0), m_changeTime / COLOUR_CHANGE_TIME);
    }
    for(int i = 0; i < light()->numPixels(); i++) {
        setPixelColor(i, m_currentColour);
    }
}
