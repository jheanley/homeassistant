#ifndef NODEENGINE_H
#define NODEENGINE_H

#include "common.h"
#include "mqtt_network.h"

class NodeEngineObject;
class NodeEngineMqttListener;

class NodeEngine
{
public:
    NodeEngine();

    void setDeviceId( const std::string& deviceId ) { m_deviceId = deviceId; }
    void setWirelessSsid( const std::string& wirelessSsid ) { m_wirelessSsid = wirelessSsid; }
    void setWirelessPassword( const std::string& wirelessPassword ) { m_wirelessPassword = wirelessPassword; }
    void setMqttServerIp( const std::string& mqttServerIP ) { m_mqttServerIP = mqttServerIP; }
    void setMqttClientDetails( const std::string& mqttClientId, const std::string& mqttClientUsername, const std::string& mqttClientPassword ) {
        m_mqttClientId = mqttClientId;
        m_mqttClientUsername = mqttClientUsername;
        m_mqttClientPassword = mqttClientPassword;
    }

    void addMqttListener( NodeEngineMqttListener* pListener );
    void addUpdateHandler( NodeEngineObject* pHandler );

    void init();
    void start();
    void update();

private:
    void connectWiFi();
    void initOTA();
    void initMqtt();
    void checkMqttObjectStateMessages();
    void processMessage(MqttClient::MessageData& md);

private:
    std::string m_deviceId;
    std::string m_wirelessSsid;
    std::string m_wirelessPassword;
    std::string m_mqttServerIP;
    std::string m_mqttClientId;
    std::string m_mqttClientUsername;
    std::string m_mqttClientPassword;

    MQTTClient* m_pMQTTClient = nullptr;

    // handlers
    std::list<NodeEngineObject*> m_updateHandlers;
    std::list<NodeEngineMqttListener*> m_mqttListeners;

    unsigned long m_previousMillis;
};

#endif // NODEENGINE_H
