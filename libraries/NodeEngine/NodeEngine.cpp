#include "NodeEngine.h"

#include "NodeEngineMqttListener.h"
#include "NodeEngineObject.h"

#define FPS 10

NodeEngine::NodeEngine()
{

}

void NodeEngine::addMqttListener(NodeEngineMqttListener* pListener)
{
    m_mqttListeners.push_back(pListener);
}

void NodeEngine::addUpdateHandler(NodeEngineObject* pHandler)
{
    m_updateHandlers.push_back(pHandler);
}

void NodeEngine::init()
{
    Serial.begin(HW_UART_SPEED);
    while (!Serial);
    delay( 2000 ); // power-up safety delay
    //Serial.println("");
    Serial.println(" NodeEngine::init()");
}

void NodeEngine::start()
{
    connectWiFi();
    initOTA();
    initMqtt();

    m_previousMillis = millis();
}

void NodeEngine::update()
{
    unsigned long currentMillis = millis();
    unsigned long elapsedMillis = currentMillis - m_previousMillis;
    float elapsed(elapsedMillis / 1000.0f);
    ArduinoOTA.handle();
    m_previousMillis = currentMillis;
    for( NodeEngineObject* pObject : m_updateHandlers )
    {
        pObject->update(elapsed);
    }
    if( m_pMQTTClient->update() )
    {
        // mqtt should be ready
        checkMqttObjectStateMessages();
    }
    unsigned long updateMillis = millis() - m_previousMillis;
    int32_t delayMillis = (1000 / FPS) - updateMillis;
    if( delayMillis > 0 )
    {
        delay(delayMillis);
    }
}

void NodeEngine::connectWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(m_wirelessSsid.c_str(), m_wirelessPassword.c_str());

    // while wifi not connected yet, print '.'
    // then after it connected, get out of the loop
    int maxWait(10000);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    //print a new line, then print WiFi connected and the IP address
    //Serial.println("");
    Serial.println("WiFi connected");
    // Print the IP address
    Serial.println(WiFi.localIP());
}

void NodeEngine::initOTA()
{
    ArduinoOTA.setHostname(m_deviceId.c_str());

    // we may not be near WiFi
    ArduinoOTA
        .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
            type = "sketch";
        else // U_SPIFFS
            type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
        Serial.println("Start updating " + type);
        });
    ArduinoOTA
        .onEnd([]() {
        Serial.println("\nEnd");
        });
    ArduinoOTA
        .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        });
    ArduinoOTA
        .onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
        });
    ArduinoOTA.begin();
}

void NodeEngine::initMqtt()
{
    m_pMQTTClient = new MQTTClient();
    auto callback3 = std::bind(&NodeEngine::processMessage,
                               this, std::placeholders::_1);
    m_pMQTTClient->setup(m_mqttServerIP, m_mqttClientId, m_mqttClientUsername, m_mqttClientPassword, callback3);
    std::string topic("homeassistant/");
    topic.append(m_mqttClientId);
    topic.append("/#");

    m_pMQTTClient->addSubscribeTopic(topic.c_str());
}

void NodeEngine::checkMqttObjectStateMessages()
{
    // device should set deviceTopic inside nextStateMessage, e.g. light1/state
    // this will then be added to the client topic
    // e.g. homeassistant/kitchen-lights/light1/state
    std::string deviceTopic;

    for( NodeEngineMqttListener* pListener : m_mqttListeners )
    {
        std::string messagePayload;
        while( pListener->nextStateMessage(deviceTopic, messagePayload) )
        {
            Serial.print("state change for: ");

            MqttClient::Message message;

            message.qos = MqttClient::QOS0;
            message.retained = false;
            message.dup = false;
            message.payload = (void*) messagePayload.c_str();
            message.payloadLen = strlen(messagePayload.c_str());

            std::string topic("homeassistant/");
            topic.append(m_mqttClientId);
            topic.append("/");
            topic.append(deviceTopic);

            Serial.println(topic.c_str());

            mqtt->publish(topic.c_str(), message);
        }
    }
}

void NodeEngine::processMessage(MqttClient::MessageData& md)
{
    const MqttClient::Message& msg = md.message;
    const MQTTString& topicName = md.topicName;
    char topicStr[topicName.lenstring.len + 1];
    memcpy(topicStr, topicName.lenstring.data, topicName.lenstring.len);
    topicStr[topicName.lenstring.len] = '\0';
    char payload[msg.payloadLen + 1];
    memcpy(payload, msg.payload, msg.payloadLen);
    payload[msg.payloadLen] = '\0';
    String topicStdStr(topicStr);
    std::string payloadStr(payload);
    LOG_PRINTFLN(
                "Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]",
                msg.qos, msg.retained, msg.dup, msg.id, payload
                );
    LOG_PRINTFLN("topic: %s", topicStr);

    for( NodeEngineMqttListener* pListener : m_mqttListeners )
    {
        pListener->processMessage(topicStr, payloadStr);
    }
}
