#include "LightManager.h"

StaticJsonDocument<1024> doc;

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

bool LightManager::nextStateMessage(std::string& deviceTopic, std::string& messagePayload)
{
    for( Light* pLight : m_lights )
    {
        if( pLight->stateChanged() )
        {
            deviceTopic = pLight->name();
            deviceTopic.append("/state");
            pLight->getState(doc);
            pLight->clearStateFlags();
            String output;
            serializeJson(doc, output);

            messagePayload = output.c_str();

            doc.clear();
            return true;
        }
    }
    return false;
}

void LightManager::processMessage(const std::string& topic, const std::string& messagePayload)
{
    DeserializationError error = deserializeJson(doc, messagePayload);
    for( Light* pLight : m_lights )
    {
        if( topic.find("state/set") < topic.size()
            && topic.find(pLight->name()) < topic.size() )
        {
            pLight->processState(doc);
            break;
        }
    }

    doc.clear();
}
