#include <MqttClient.h>

#define MQTT_ID "homeassistant/kids-bedroom"
#define MQTT_CLIENT "192.168.1.145"
const char* MQTT_TOPIC_LIGHT1 = MQTT_ID "/light1";
const char* MQTT_TOPIC_LIGHT2 = MQTT_ID "/light2";
const char* MQTT_TOPIC_PUB = MQTT_ID "/pub";

#define LOG_PRINTFLN(fmt, ...)	logfln(fmt, ##__VA_ARGS__)
#define LOG_SIZE_MAX 128
void logfln(const char *fmt, ...) {
	char buf[LOG_SIZE_MAX];
	va_list ap;
	va_start(ap, fmt);
	vsnprintf(buf, LOG_SIZE_MAX, fmt, ap);
	va_end(ap);
	Serial.println(buf);
}

static MqttClient *mqtt = NULL;
static WiFiClient network;


void processMessage2(MqttClient::MessageData& md)
{
    LOG_PRINTFLN("processMessage2");
    const MqttClient::Message& msg = md.message;
    char payload[msg.payloadLen + 1];
    memcpy(payload, msg.payload, msg.payloadLen);
    payload[msg.payloadLen] = '\0';
    LOG_PRINTFLN(
     "Message arrived: qos %d, retained %d, dup %d, packetid %d, payload:[%s]",
      msg.qos, msg.retained, msg.dup, msg.id, payload
    );
}

class System: public MqttClient::System {
public:

	unsigned long millis() const {
		return ::millis();
	}

	void yield(void) {
		::yield();
	}
};

class MQTTClient
{
public:
    void setup( MqttClient::MessageHandlerCbk cbMessageHandler )
    {
        Serial.println("MQTTClient::setup()");
        m_cbMessageHandler = cbMessageHandler;
        mqttSystem = new System;
        mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
        mqttNetwork = new MqttClient::NetworkClientImpl<Client>(network, *mqttSystem);

        Serial.println("MQTTClient::setup() - create buffers");
        //// Make 128 bytes send buffer
        MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<128>();
        //// Make 128 bytes receive buffer
        MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<128>();

        Serial.println("MQTTClient::setup() - create message handlers");
        //// Allow up to 2 subscriptions simultaneously
        MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
        //// Configure client options
        MqttClient::Options mqttOptions;
        ////// Set command timeout to 10 seconds
        mqttOptions.commandTimeoutMs = 10000;
        //// Make client object
        Serial.println("MQTTClient::setup() - create client");
        mqtt = new MqttClient(
	        mqttOptions, *mqttLogger, *mqttSystem, *mqttNetwork, *mqttSendBuffer,
	        *mqttRecvBuffer, *mqttMessageHandlers
        );

//        if (mqttNetwork->connect("weatherStation")) {
//            Serial.println("Connected to MQTT broker");
//            Serial.println("Subscribing to topic now");
//            mqttClient.subscribe("weather/temperature");
//        }
    }

    void update()
    {
        // Check connection status
	    if (mqtt
            && !mqtt->isConnected()) {
		    // Close connection if exists
		    network.stop();
		    // Re-establish TCP connection with MQTT broker
            LOG_PRINTFLN("Connecting to MQTT broker");
		    network.connect(MQTT_CLIENT, 1883);
		    if (!network.connected()) {
                LOG_PRINTFLN("Can't establish the TCP connection");
			    delay(5000);
			    ESP.reset();
		    }
            LOG_PRINTFLN("Start new connection");
		    // Start new MQTT connection
		    MqttClient::ConnectResult connectResult;
		    // Connect
		    {
			    MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
			    options.MQTTVersion = 4;
			    options.clientID.cstring = (char*)"kids-bedroom-lights";
                options.username.cstring = (char*)"KidsBedroomLights";
                options.password.cstring = (char*)"Bo11ock3";
			    options.cleansession = true;
			    options.keepAliveInterval = 15; // 15 seconds
                Serial.println("connect to mqtt");

			    MqttClient::Error::type rc = mqtt->connect(options, connectResult);
			    if (rc != MqttClient::Error::SUCCESS) {
				    LOG_PRINTFLN("Connection error: %i", rc);
				    return;
			    }

		    }
		    {
			    // Add subscribe here if required
			    LOG_PRINTFLN("Subscribing to: %s", MQTT_TOPIC_LIGHT1);
			    MqttClient::Error::type rc = mqtt->subscribe( MQTT_TOPIC_LIGHT1, MqttClient::QOS0, m_cbMessageHandler );
			    if (rc != MqttClient::Error::SUCCESS) {
				    LOG_PRINTFLN("Subscribe error: %i", rc);
				    LOG_PRINTFLN("Drop connection");
				    mqtt->disconnect();
				    return;
			    }
			    LOG_PRINTFLN("Subscribing to: %s", MQTT_TOPIC_LIGHT2);
			    rc = mqtt->subscribe( MQTT_TOPIC_LIGHT2, MqttClient::QOS0, m_cbMessageHandler );
			    if (rc != MqttClient::Error::SUCCESS) {
				    LOG_PRINTFLN("Subscribe error: %i", rc);
				    LOG_PRINTFLN("Drop connection");
				    mqtt->disconnect();
				    return;
			    }
		    }
	    } else {
		    {
			    // Add publish here if required
		    }
            mqtt->yield(5000);
	    }
    }

public:
    MqttClient::System *mqttSystem = nullptr;
    MqttClient::Logger *mqttLogger = nullptr;
    MqttClient::Network * mqttNetwork = nullptr;
    MqttClient::MessageHandlerCbk m_cbMessageHandler = nullptr;
};

