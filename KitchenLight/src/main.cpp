#include "config.h"

//#define FASTLED_ESP8266_RAW_PIN_ORDER

#include "common.h"
#include "NodeEngine.h"
#include "LightManager.h"

/**
 * When changing light
 *
 * Set correct led count to NUM_LEDS
 * Change MQTT_CLIENT_ID to match nodemcu device name in mqtt topic e.g. homeassistant/<MQTT_CLIENT_ID>/light2/state
 * Change the light name parameter when calling pLightManager->addLightStrip, this should correlate to the topic for that light e.g. homeassistant/<MQTT_CLIENT_ID>/<Light name>/state
 */

#define DATA_PIN     2 // PIN 
//#define DATA_PIN     4  // PIN 2 on 8266
//#define NUM_LEDS    170 // leds for shea's bed
#define NUM_LEDS    59 // leds for shea's bed
//#define NUM_LEDS    60
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];

//const char* MQTT_CLIENT_ID = "kitchen";
const char* MQTT_CLIENT_ID = "loft-bedroom";
//const char* MQTT_CLIENT_ID = "test-bed";
//const char* MQTT_CLIENT_ID = "kids-bedroom";
//const char* MQTT_CLIENT_ID = "shea-bedroom";
const char* LIGHT_NAME = "light1";
//const char* LIGHT_NAME = "light2"; // used for test-bed

NodeEngine* g_pNodeEngine;

void setup() {
    g_pNodeEngine = new NodeEngine();
    g_pNodeEngine->init();
    g_pNodeEngine->setDeviceId(MQTT_CLIENT_ID);
    g_pNodeEngine->setWirelessSsid(WIRELESS_SSID);
    g_pNodeEngine->setWirelessPassword(WIRELESS_PASSWORD);
    g_pNodeEngine->setMqttServerIp(MQTT_SERVER_IP);
    g_pNodeEngine->setMqttClientDetails(MQTT_CLIENT_ID, "KidsBedroomLights", "Bo11ock3");

    LightManager* pLightManager(new LightManager());

    g_pNodeEngine->addMqttListener(pLightManager);
    g_pNodeEngine->addUpdateHandler(pLightManager);

    //FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
    Light* pLight = pLightManager->addLightStrip<LED_TYPE, DATA_PIN, COLOR_ORDER>(NUM_LEDS, LIGHT_NAME);
    //Light* pLight = pLightManager->addLightStrip<SK6812, DATA_PIN, GRBW>(NUM_LEDS, LIGHT_NAME);
    pLight->setState(Light::On);
    pLightManager->showColour(CRGB(50, 0, 0));

    Serial.println(RGB_BYTE(GRBW, 0));
    Serial.println(RGB_BYTE(GRBW, 1));
    Serial.println(RGB_BYTE(GRBW, 2));
    Serial.println(RGB_BYTE(GRBW, 3));

    g_pNodeEngine->start();
    //FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() {
    static int whiteLed = 0;
    ++whiteLed;
    if( whiteLed >= NUM_LEDS )
    {
        whiteLed = 0;
    }
   // Turn our current led on to white, then show the leds
   leds[whiteLed] = CRGB::White;

   // Show the leds (only one of which is set to white, from above)
   //FastLED.show();

   // Turn our current led back to black for the next loop around
   leds[whiteLed] = CRGB::Black;

   g_pNodeEngine->update();
}
