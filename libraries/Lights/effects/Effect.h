#ifndef EFFECT_H
#define EFFECT_H

#include "FastLED.h"

class HueLight;

class Effect
{
public:
    struct StripData
    {
        uint8_t* pixelData = nullptr;
        int length = 0;
        int pixelSize = 0;
        CRGB colours[3];
        uint8_t palette = 0;
    };

public:
    Effect();

    HueLight* light() const { return m_pLight; }

    virtual void init( HueLight* pLight );

    virtual void update( float elapsed ) = 0;

    uint32_t crgb_to_col(CRGB fastled);
    CRGB col_to_crgb(uint32_t color);

    /*
    * Gets a single color from the currently selected palette.
    * @param i Palette Index (if mapping is true, the full palette will be SEGLEN long, if false, 255). Will wrap around automatically.
    * @param mapping if true, LED position in segment is considered for color
    * @param wrap FastLED palettes will usally wrap back to the start smoothly. Set false to get a hard edge
    * @param mcol If the default palette 0 is selected, return the standard color 0, 1 or 2 instead. If >2, Party palette is used instead
    * @param pbri Value to scale the brightness of the returned color by. Default is 255. (no scaling)
    * @returns Single color from palette
    */
    uint32_t color_from_palette(uint16_t, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255);

    void setPixelColor(uint16_t i, const CRGB& colour );

private:
    HueLight* m_pLight;
};

#endif // EFFECT_H
