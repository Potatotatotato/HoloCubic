#ifndef __NETWORK_H
#define __NETWORK_H
#include <Arduino.h>

void startWiFiConnection(void);
void start_AP_mode();
void checkWiFiConnection(uint32_t interval);

#endif