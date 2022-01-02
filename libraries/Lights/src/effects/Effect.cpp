#include "Effect.h"

#include "HueLight.h"

Effect::Effect()
    : m_pLight(nullptr)
{

}

void Effect::init( HueLight* pLight )
{
    m_pLight = pLight;
}

void Effect::setPixelColor(uint16_t i, const CRGB& colour)
{
    if( m_pLight )
    {
        m_pLight->setLed(i, colour);
    }
}

uint32_t Effect::color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri)
{
    if (mcol < 3) {
        uint32_t color = crgb_to_col(m_pLight->colour(mcol));
        if (pbri != 255) {
            CRGB crgb_color = col_to_crgb(color);
            crgb_color.nscale8_video(pbri);
            return crgb_to_col(crgb_color);
        } else {
            return color;
        }
    }

    uint8_t paletteIndex = i;
    if (mapping && m_pLight->numPixels() > 1) paletteIndex = (i*255)/(m_pLight->numPixels() -1);
    if (!wrap) paletteIndex = scale8(paletteIndex, 240); //cut off blend at palette "end"
    CRGB fastled_col;
    fastled_col = ColorFromPalette( m_pLight->palette(), paletteIndex, pbri, LINEARBLEND);

    return crgb_to_col(fastled_col);
}

uint32_t Effect::crgb_to_col(CRGB fastled)
{
  return (((uint32_t)fastled.red << 16) | ((uint32_t)fastled.green << 8) | fastled.blue);
}

CRGB Effect::col_to_crgb(uint32_t color)
{
    CRGB fastled_col;
    fastled_col.red =   (color >> 16 & 0xFF);
    fastled_col.green = (color >> 8  & 0xFF);
    fastled_col.blue =  (color       & 0xFF);
    return fastled_col;
}
