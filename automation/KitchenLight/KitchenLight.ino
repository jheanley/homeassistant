#include "HueLight.h"

#include "common.h"
#include "config.h"

#include "mqtt_network.h"

#define MQTT_ID "homeassistant/kitchen"

const char* MQTT_TOPIC_LIGHT1 = MQTT_ID "/light1";
const char* MQTT_TOPIC_LIGHT2 = MQTT_ID "/light2";
const char* MQTT_TOPIC_PUB = MQTT_ID "/pub";

MQTTClient* pMQTTClient(nullptr);
HueLight*   m_pHueLight(nullptr);

void connectWifi()
{
  WiFi.begin(WIRELESS_SSID, WIRELESS_PASSWORD);

  // while wifi not connected yet, print '.'
  // then after it connected, get out of the loop
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  //print a new line, then print WiFi connected and the IP address
  Serial.println("");
  Serial.println("WiFi connected");
  // Print the IP address
  Serial.println(WiFi.localIP());  
}

void initMqtt()
{
  pMQTTClient = new MQTTClient();
  pMQTTClient->setup("", MQTT_SERVER_IP, processMessage);
}

void setup() {
  Serial.begin(HW_UART_SPEED);
  while (!Serial);
  connectWifi();
  initMqtt();

  m_pHueLight = new HueLight();
}

void loop() {
  // put your main code here, to run repeatedly:

}

void processMessage(MqttClient::MessageData& md)
{
    const MqttClient::Message& msg = md.message;
    const MQTTString& topicName = md.topicName;
    char topicStr[topicName.lenstring.len + 1];
    memcpy(topicStr, topicName.lenstring.data, topicName.lenstring.len);
    topicStr[topicName.lenstring.len] = '\0';
    char payload[msg.payloadLen + 1];
    memcpy(payload, msg.payload, msg.payloadLen);
    payload[msg.payloadLen] = '\0';
    std::string topicStdStr(topicStr);
    LOG_PRINTFLN(
     "Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]",
      msg.qos, msg.retained, msg.dup, msg.id, payload
    );
    LOG_PRINTFLN("topic: %s", topicStr);
    int light = LIGHT1;
    if( topicStdStr.find("light2") != std::string::npos )
    {
        light = LIGHT2;
    }
    if( strcmp(payload, "ON") == 0 )
    {
      analogWrite(light, 255);
    }
    else if( strcmp(payload, "OFF") == 0 )
    {
      analogWrite(light, 0);
    }
}
