#ifndef MQTT_NETWORK_H
#define MQTT_NETWORK_H

#include <common.h>
#include <MqttClient.h>

static MqttClient *mqtt = NULL;
static WiFiClient network;

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
    void setup( std::string serverIP, std::string clientid, std::string username, std::string password, MqttClient::MessageHandlerCbk cbMessageHandler )
    {
        LOG_PRINTFLN("MQTTClient::setup()");
        m_serverId = serverIP;
        m_clientId = clientid;
        m_username = username;
        m_password = password;
        m_cbMessageHandler = cbMessageHandler;
        mqttSystem = new System;
        mqttLogger = new MqttClient::LoggerImpl<HardwareSerial>(Serial);
        mqttNetwork = new MqttClient::NetworkClientImpl<Client>(network, *mqttSystem);

        LOG_PRINTFLN("MQTTClient::setup() - create buffers");
        //// Make 128 bytes send buffer
        MqttClient::Buffer *mqttSendBuffer = new MqttClient::ArrayBuffer<1024>();
        //// Make 128 bytes receive buffer
        MqttClient::Buffer *mqttRecvBuffer = new MqttClient::ArrayBuffer<1024>();

        LOG_PRINTFLN("MQTTClient::setup() - create message handlers");
        //// Allow up to 2 subscriptions simultaneously
        MqttClient::MessageHandlers *mqttMessageHandlers = new MqttClient::MessageHandlersImpl<2>();
        //// Configure client options
        MqttClient::Options mqttOptions;
        ////// Set command timeout to 10 seconds
        mqttOptions.commandTimeoutMs = 10000;
        //// Make client object
        LOG_PRINTFLN("MQTTClient::setup() - create client");
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

    void addSubscribeTopic( const std::string& topic )
    {
        m_topics.push_back(topic);
    }

    bool update()
    {
        // Check connection status
	    if (mqtt
            && !mqtt->isConnected()) {
		    // Close connection if exists
		    network.stop();
		    // Re-establish TCP connection with MQTT broker
            LOG_PRINTFLN("Connecting to MQTT broker");
            network.connect(m_serverId.c_str(), 1883);
		    if (!network.connected()) {
                LOG_PRINTFLN("Can't establish the TCP connection");
			    delay(5000);
                ESP.restart();
		    }
            LOG_PRINTFLN("Start new connection");
		    // Start new MQTT connection
		    MqttClient::ConnectResult connectResult;
		    // Connect
		    {
                MQTTPacket_connectData options = MQTTPacket_connectData_initializer;
                options.MQTTVersion = 4;
                options.clientID.cstring = (char*)m_clientId.c_str();
                options.username.cstring = (char*)m_username.c_str();
                options.password.cstring = (char*)m_password.c_str();
			    options.cleansession = true;
			    options.keepAliveInterval = 60; // 15 seconds
                LOG_PRINTFLN("connect to mqtt");

			    MqttClient::Error::type rc = mqtt->connect(options, connectResult);
			    if (rc != MqttClient::Error::SUCCESS) {
				    LOG_PRINTFLN("Connection error: %i", rc);
                    return false;
			    }

		    }
		    {
			    // Add subscribe here if required
                for( const std::string& topic : m_topics )
                {
                    LOG_PRINTFLN("Subscribing to: %s", topic.c_str());
                    Serial.println(topic.c_str());
                    MqttClient::Error::type rc = mqtt->subscribe( topic.c_str(), MqttClient::QOS0, m_cbMessageHandler );
                    if (rc != MqttClient::Error::SUCCESS) {
                        LOG_PRINTFLN("Subscribe error: %i", rc);
                        LOG_PRINTFLN("Drop connection");
                        mqtt->disconnect();
                        return false;
                    }
                }
		    }
	    } else {
		    {
			    // Add publish here if required
		    }
            mqtt->yield(5);
	    }
        return true;
    }

public:
    std::string         m_serverId;
    std::string         m_clientId;
    std::string         m_username;
    std::string         m_password;
    MqttClient::System *mqttSystem = nullptr;
    MqttClient::Logger *mqttLogger = nullptr;
    MqttClient::Network * mqttNetwork = nullptr;
    MqttClient::MessageHandlerCbk m_cbMessageHandler = nullptr;
    std::list<std::string>   m_topics;
};

#endif //MQTT_NETWORK_H
