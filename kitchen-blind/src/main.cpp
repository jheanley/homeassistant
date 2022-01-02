#include "config.h"

#include "NodeEngine.h"
#include "Blind.h"

#define HW_UART_SPEED 115200L

#define DIRECTION_PIN   0
#define STEP_PIN        4
#define ENABLE_PIN      5

//const char* MQTT_CLIENT_ID = "kitchen";
//const char* MQTT_CLIENT_ID = "loft-bedroom";
//const char* MQTT_CLIENT_ID = "test-bed";
const char* MQTT_CLIENT_ID = "kitchen-blinds";

NodeEngine* g_pNodeEngine;

void setup() {
    Serial.println("setup()");
    g_pNodeEngine = new NodeEngine();
    g_pNodeEngine->init();
    g_pNodeEngine->setDeviceId(MQTT_CLIENT_ID);
    g_pNodeEngine->setWirelessSsid(WIRELESS_SSID);
    g_pNodeEngine->setWirelessPassword(WIRELESS_PASSWORD);
    g_pNodeEngine->setMqttServerIp(MQTT_SERVER_IP);
    g_pNodeEngine->setMqttClientDetails(MQTT_CLIENT_ID, "KidsBedroomLights", "Bo11ock3");

    Blind* pBlind(new Blind());

    g_pNodeEngine->addMqttListener(pBlind);
    g_pNodeEngine->addUpdateHandler(pBlind);

    pBlind->setName("blind1");
    pBlind->stepperMotor().setPins(DIRECTION_PIN, STEP_PIN, ENABLE_PIN);

    g_pNodeEngine->start();
}

void loop() {
    g_pNodeEngine->update();
}
