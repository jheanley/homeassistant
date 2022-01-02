#ifndef AURORAEFFECT_H
#define AURORAEFFECT_H

#include "Effect.h"

//CONFIG
#ifdef ESP8266
  #define W_MAX_COUNT  9          //Number of simultaneous waves
#else
  #define W_MAX_COUNT 20          //Number of simultaneous waves
#endif
#define W_MAX_SPEED 6             //Higher number, higher speed
#define W_WIDTH_FACTOR 6          //Higher number, smaller waves

class AuroraWave {
private:
    uint32_t stripLength;
    float maxAge;
    CRGB basecolor;
    float basealpha;
    float age;
    uint16_t width;
    float center;
    bool goingleft;
    float speed_factor;
    bool alive = true;

public:
    void init(uint32_t segment_length, CRGB color) {

        stripLength = segment_length;
        maxAge = (random(80, 255) / (float)255) * 10.0f;
        basecolor = color;
        basealpha = random(60, 101) / (float)100;
        age = 0;
        width = random(segment_length / 20, segment_length / W_WIDTH_FACTOR); //half of width to make math easier
        if (!width) width = 1;
        center = random(101) / (float)100 * segment_length;
        goingleft = random(0, 2) == 0;
        speed_factor = 6.0f;
        alive = true;
        
        // Serial.print("AuroraWave - baseColour - R: ");
        // Serial.print(basecolor.r);
        // Serial.print(", G: ");
        // Serial.print(basecolor.g);
        // Serial.print(", B: ");
        // Serial.print(basecolor.b);
        // Serial.print(", width: ");
        // Serial.print(width);
        // Serial.print(", center: ");
        // Serial.print(center);
        // Serial.print(", maxAge: ");
        // Serial.println(maxAge);
    }

    CRGB getColorForLED(int ledIndex) {
        if(ledIndex < center - width || ledIndex > center + width) return 0; //Position out of range of this wave

        CRGB rgb;

        //Offset of this led from center of wave
        //The further away from the center, the dimmer the LED
        float offset = ledIndex - center;
        if (offset < 0) offset = -offset;
        float offsetFactor = offset / width;

        //The age of the wave determines it brightness.
        //At half its maximum age it will be the brightest.
        float ageFactor = 0.1;
        if(age / maxAge < 0.5) {
            ageFactor = (float)age / (maxAge * 0.5);
        } else {
            ageFactor = (float)(maxAge - age) / ((float)maxAge * 0.5);
        }

        //Calculate color based on above factors and basealpha value
        float factor = (1 - offsetFactor) * ageFactor * basealpha;
        rgb.r = basecolor.r * factor;
        rgb.g = basecolor.g * factor;
        rgb.b = basecolor.b * factor;

        return rgb;
    };

    //Change position and age of wave
    //Determine if its sill "alive"
    void update(uint32_t segment_length, float elapsed) {
        if(goingleft) {
            center -= speed_factor * elapsed;
        } else {
            center += speed_factor * elapsed;
        }

        age += elapsed;

        if(age > maxAge) {
            alive = false;
        } else {
            if(goingleft) {
                if(center + width < 0) {
                    alive = false;
                }
            } else {
                if(center - width > segment_length) {
                    alive = false;
                }
            }
        }
    };

    bool stillAlive() {
        return alive;
    };
};

class AuroraEffect : public Effect
{
public:
    AuroraEffect();

    ~AuroraEffect();

    void init( HueLight* pLight ) override;

    void update( float elapsed ) override;

private:
    AuroraWave* m_waves = nullptr;
    int         m_waveCount = 20;
};

#endif // AURORAEFFECT_H
