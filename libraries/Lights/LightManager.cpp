#include "LightManager.h"

uint8_t LightManager::leds[1024];

LightManager::LightManager()
{

}

void LightManager::showColour(const CRGB& color)
{
    for( Light* pLight : m_lights )
    {
        pLight->showColor(color);
    }
}

void LightManager::update(float elapsed)
{
    for( Light* pLight : m_lights )
    {
        pLight->update(elapsed);
        pLight->apply();
    }
}

bool LightManager::nextStateMessage(std::string& deviceid, StaticJsonDocument<1024>& doc)
{
    for( Light* pLight : m_lights )
    {
        if( pLight->stateChanged() )
        {
            deviceid = pLight->name();
            pLight->getState(doc);
            pLight->clearStateFlags();
            return true;
        }
    }
    return false;
}

void LightManager::processMessage(const std::string& topic, StaticJsonDocument<1024>& doc)
{
    for( Light* pLight : m_lights )
    {
        if( topic.find("state/set") < topic.size()
            && topic.find(pLight->name()) < topic.size() )
        {
            pLight->processState(doc);
            break;
        }
    }
}
