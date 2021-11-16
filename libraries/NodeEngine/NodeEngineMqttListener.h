#ifndef NODEENGINEMQTTLISTENER_H
#define NODEENGINEMQTTLISTENER_H

#include <string>

#include "ArduinoJson.h"

class NodeEngineMqttListener
{
public:
    NodeEngineMqttListener();

    virtual bool nextStateMessage( std::string& deviceid, StaticJsonDocument<1024>& doc) = 0;

    virtual void processMessage( const std::string& topic, StaticJsonDocument<1024>& doc ) = 0;
};

#endif // NODEENGINEMQTTLISTENER_H
