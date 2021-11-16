#include "AuroraEffect.h"

#include "HueLight.h"

AuroraEffect::AuroraEffect()
{

}

AuroraEffect::~AuroraEffect()
{
    if( m_waves )
    {
        delete m_waves;
    }
}

void AuroraEffect::init( HueLight* pLight )
{
    Effect::init(pLight);
    m_waves = (AuroraWave*)malloc(sizeof(AuroraWave) * m_waveCount);

    for(int i = 0; i < m_waveCount; i++) {
        m_waves[i].init(pLight->numPixels(), col_to_crgb(color_from_palette(random8(), false, false, random(0, 3))));
    }
}

void AuroraEffect::update(float elapsed)
{
    for(int i = 0; i < m_waveCount; i++) {
        //Update values of wave
        m_waves[i].update(light()->numPixels(), elapsed);

        if(!(m_waves[i].stillAlive())) {
            //If a wave dies, reinitialize it starts over.
            m_waves[i].init(light()->numPixels(), col_to_crgb(color_from_palette(random8(), false, false, random(0, 3))));
        }
    }

    uint8_t backlight = 1; //dimmer backlight if less active colors
    if (light()->colour(0)) backlight++;
    if (light()->colour(1)) backlight++;
    if (light()->colour(2)) backlight++;
    //Loop through LEDs to determine color
    for(int i = 0; i < light()->numPixels(); i++) {
        CRGB mixedRgb = CRGB(backlight, backlight, backlight);

        //For each LED we must check each wave if it is "active" at this position.
        //If there are multiple waves active on a LED we multiply their values.
        for(int  j = 0; j < m_waveCount; j++) {
            CRGB rgb = m_waves[j].getColorForLED(i);

            if(rgb != CRGB(0)) {
                mixedRgb += rgb;
            }
        }

        setPixelColor(i, mixedRgb);
    }
}
