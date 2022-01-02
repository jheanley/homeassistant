#ifndef NODEENGINEMQTTLISTENER_H
#define NODEENGINEMQTTLISTENER_H

#include <string>
#include <list>

#include "ArduinoJson.h"

#define MAX_JSON_SIZE 1024



class NodeEngineMqttListener
{
public:
    NodeEngineMqttListener();

    virtual std::list<std::string> getTopics() const { return std::list<std::string>(); }

    virtual bool nextStateMessage( std::string& deviceid, std::string& messagePayload) = 0;

    virtual void processMessage( const std::string& topic, const std::string& messagePayload ) = 0;
};

#endif // NODEENGINEMQTTLISTENER_H
