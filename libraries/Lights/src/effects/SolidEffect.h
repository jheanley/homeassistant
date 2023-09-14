#ifndef SOLIDEFFECT_H
#define SOLIDEFFECT_H

#include "Effect.h"

class SolidEffect : public Effect
{
    static const float COLOUR_CHANGE_TIME;
public:
    SolidEffect();

    std::string name() const override { return "solid"; }

    void init( HueLight* pLight ) override;

    void update( float elapsed ) override;

private:
    CRGB m_lastColour;
    CRGB m_currentColour;
    float m_changeTime;
};

#endif // SOLIDEFFECT_H
