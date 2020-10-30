#ifndef LIGHT_H
#define LIGHT_H

#include "common.h"
#include <csignal>

#include <string.h>

#include "mqtt_network.h"

#define STATE_STRING_LENGTH 5

class Light : public MqttListener
{
  enum State
  {
    On,
    Off
  };
  
public:
  Light();
  virtual ~Light();
  
  State state() const { return m_state; }
  void setState( State state );
  void setState( const std::string& state );

  virtual int brightness() const;
  virtual void setBrightness(int brightness);

  std::string getStateString() const { return m_stateStr; }

  virtual void apply();

  virtual void mqttMessage(MqttClient::MessageData& md);
  
private:
  int m_controlPin = -1;
  State m_state = Off;
  std::string m_stateStr;
  char m_stateString[STATE_STRING_LENGTH];
  int m_brightness;
};

#endif
