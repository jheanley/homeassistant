#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include "NodeEngineObject.h"
#include "NodeEngineMqttListener.h"
#include "HueLight.h"

#include "mqtt_network.h"

class LightManager : public NodeEngineObject, public NodeEngineMqttListener
{
    static uint8_t leds[1024];
public:
    LightManager();

    template<template<uint8_t DATA_PIN, EOrder RGB_ORDER> class CHIPSET, uint8_t DATA_PIN, EOrder RGB_ORDER>
    Light* addLightStrip( int numLeds, const std::string& name )
    {
        uint8_t pixelSize = RGB_ORDER == RGBW ? 4 : 3;
        //uint8_t* pixels = (uint8_t*)malloc(numLeds * pixelSize);
        memset((void*)leds, 0x00, numLeds * pixelSize);
        CLEDController* controller(nullptr);
        HueLight* pLight(new HueLight);
        if( RGB_ORDER == RGBW )
        {
            controller = &FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>((CRGBW*)leds, numLeds);
            pLight->setValidState(HueLight::White);
        }
        else
        {
            controller = &FastLED.addLeds<CHIPSET, DATA_PIN, RGB_ORDER>((CRGB*)leds, numLeds);
        }

        pLight->init(leds, pixelSize, numLeds);
        pLight->setName(name);
        pLight->setPixelController(controller);

        if( RGB_ORDER == RGBW )
        {
            pLight->setColour(CRGB(255,171,64));
        }
        else
        {
            pLight->setColour(CRGBW(0,0,0, 125));
        }

        m_lights.push_back(pLight);
        return pLight;
    }

    void showColour(const struct CRGB & color);

    void update( float elapsed ) override;

    bool nextStateMessage(std::string& deviceTopic, std::string& messagePayload ) override;

    void processMessage(const std::string& topic, const std::string& messagePayload ) override;

private:
    std::list<Light*> m_lights;
};

#endif // LIGHTMANAGER_H
