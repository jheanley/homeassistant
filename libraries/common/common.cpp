#include "common.h"

#define LOG_SIZE_MAX 128
void logfln(const char *fmt, ...) {
    char buf[LOG_SIZE_MAX];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, LOG_SIZE_MAX, fmt, ap);
    va_end(ap);
    Serial.println(buf);
}
