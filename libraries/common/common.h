#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <list>
#include <string>
#include <ArduinoOTA.h>
#include "ArduinoJson.h"

#define HW_UART_SPEED 115200L

#define JSON_DOC_SIZE 512

#define LOG_PRINTFLN(fmt, ...)  logfln(fmt, ##__VA_ARGS__)
#define LOG_SIZE_MAX 128
void logfln(const char *fmt, ...);

#endif
