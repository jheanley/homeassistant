#include "ESP8266WiFi.h"
#include "mqtt_network.h"
#include <LittleFS.h>
#include <Arduino.h>

#define HW_UART_SPEED 115200L

// WiFi parameters to be configured
const char* ssid = "SH1T";
const char* password = "Bo11ock3";
#define MQTT_SERVER_IP "192.168.0.26"
MQTTClient* pMQTTClient(nullptr);
int LIGHT1 = 0;
int LIGHT2 = 2;

void connectWifi()
{
  WiFi.begin(ssid, password);

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

void setup()
{
  Serial.begin(HW_UART_SPEED);
  while (!Serial);
  connectWifi();
  //connectMqtt();
  pinMode(LIGHT1, OUTPUT);
  pinMode(LIGHT2, OUTPUT);
}

byte b = 0;
void loop()
{
  // put your main code here, to run repeatedly:
  pMQTTClient->update();
}
