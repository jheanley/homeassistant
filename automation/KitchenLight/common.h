#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>
#include "ESP8266WiFi.h"

#define HW_UART_SPEED 115200L

#define LOG_PRINTFLN(fmt, ...)  logfln(fmt, ##__VA_ARGS__)
#define LOG_SIZE_MAX 128
void logfln(const char *fmt, ...);

#endif
