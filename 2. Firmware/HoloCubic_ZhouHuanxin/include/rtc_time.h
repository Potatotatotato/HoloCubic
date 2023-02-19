#ifndef __RTC_TIME_H
#define __RTC_TIME_H
#include <Arduino.h>
#include <ESP32Time.h>
#include <lvgl.h>

extern ESP32Time esp32Time;

void rtc_time_init(void);
void get_timestamp(void);

#endif
