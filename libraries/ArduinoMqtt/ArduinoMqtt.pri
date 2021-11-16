HEADERS += \
    $$PWD/src/MQTTPacket/MQTTConnect.h \
    $$PWD/src/MQTTPacket/MQTTFormat.h \
    $$PWD/src/MQTTPacket/MQTTPacket.h \
    $$PWD/src/MQTTPacket/MQTTPublish.h \
    $$PWD/src/MQTTPacket/MQTTSubscribe.h \
    $$PWD/src/MQTTPacket/MQTTUnsubscribe.h \
    $$PWD/src/MQTTPacket/StackTrace.h \
    $$PWD/src/MqttClient.h

SOURCES += \
    $$PWD/examplesMain.cpp \
    $$PWD/extras/examples/x86_Connect.cpp \
    $$PWD/src/MQTTPacket/MQTTConnectClient.c \
    $$PWD/src/MQTTPacket/MQTTConnectServer.c \
    $$PWD/src/MQTTPacket/MQTTDeserializePublish.c \
    $$PWD/src/MQTTPacket/MQTTFormat.c \
    $$PWD/src/MQTTPacket/MQTTPacket.c \
    $$PWD/src/MQTTPacket/MQTTSerializePublish.c \
    $$PWD/src/MQTTPacket/MQTTSubscribeClient.c \
    $$PWD/src/MQTTPacket/MQTTSubscribeServer.c \
    $$PWD/src/MQTTPacket/MQTTUnsubscribeClient.c \
    $$PWD/src/MQTTPacket/MQTTUnsubscribeServer.c

INCLUDEPATH += $$PWD/src
