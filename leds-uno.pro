#-------------------------------------------------
#
# Project created by QtCreator
#
#-------------------------------------------------

SOURCES += leds-uno/main.cpp

HEADERS +=  \

DISTFILES += Makefile

INCLUDEPATH = /home/jheanley/.arduino15/packages/esp8266/hardware/esp8266/3.0.2/libraries \
              /usr/share/arduino/libraries/Bridge/src \
              /usr/share/arduino/libraries/Esplora/src \
              /usr/share/arduino/libraries/Ethernet/src \
              /usr/share/arduino/libraries/Firmata/src \
              /usr/share/arduino/libraries/GSM/src \
              /usr/share/arduino/libraries/LiquidCrystal/src \
              /usr/share/arduino/libraries/Robot_Control/src \
              /usr/share/arduino/libraries/RobotIRremote/src \
              /usr/share/arduino/libraries/Robot_Motor/src \
              /usr/share/arduino/libraries/SD/src \
              /usr/share/arduino/libraries/Servo/src \
              /usr/share/arduino/libraries/SpacebrewYun/src \
              /usr/share/arduino/libraries/Stepper/src \
              /usr/share/arduino/libraries/Temboo/src \
              /usr/share/arduino/libraries/TFT/src \
              /usr/share/arduino/libraries/WiFi/src \
              /usr/share/arduino/hardware/arduino/avr/libraries/EEPROM \
              /usr/share/arduino/hardware/arduino/avr/libraries/SoftwareSerial \
              /usr/share/arduino/hardware/arduino/avr/libraries/SPI \
              /usr/share/arduino/hardware/arduino/avr/libraries/Wire

include(libraries/FastLED/FastLED.pri)
include(libraries/common/common.pri)
include(libraries/Lights/lights.pri)
include(libraries/ArduinoMqtt/ArduinoMqtt.pri)
include(libraries/ArduinoJson/ArduinoJson.pri)
include(libraries/NodeEngine/node-engine.pri)
include(automation/KitchenLight/KitchenLight.pri)
