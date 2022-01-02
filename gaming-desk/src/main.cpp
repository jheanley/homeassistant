#include "config.h"

#include "NodeEngine.h"
#include "LightManager.h"

#define HW_UART_SPEED 115200L

#define DATA_PIN     2
#define NUM_LEDS    60
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB

//const char* MQTT_CLIENT_ID = "kitchen";
//const char* MQTT_CLIENT_ID = "loft-bedroom";
const char* MQTT_CLIENT_ID = "test-bed";

NodeEngine* g_pNodeEngine;

void setup() {
    g_pNodeEngine = new NodeEngine();
    g_pNodeEngine->setDeviceId(MQTT_CLIENT_ID);
    g_pNodeEngine->setWirelessSsid(WIRELESS_SSID);
    g_pNodeEngine->setWirelessPassword(WIRELESS_PASSWORD);
    g_pNodeEngine->setMqttServerIp(MQTT_SERVER_IP);
    g_pNodeEngine->setMqttClientDetails(MQTT_CLIENT_ID, "KidsBedroomLights", "Bo11ock3");

    LightManager* pLightManager(new LightManager());

    g_pNodeEngine->addMqttListener(pLightManager);
    g_pNodeEngine->addUpdateHandler(pLightManager);

    //FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    pLightManager->addLightStrip<SK6812, DATA_PIN, RGBW>(NUM_LEDS, "light1");
    //pLightManager->addLightStrip<SK6812, DATA_PIN, RGBW>(60);
    //pLightManager->showColour(CRGB(0, 0, 0));

    g_pNodeEngine->start();
}

void loop() {
    g_pNodeEngine->update();
}
