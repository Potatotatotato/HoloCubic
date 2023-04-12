#ifndef __WEATHER_H
#define __WEATHER_H
#include <Arduino.h>

struct city_weather_data_struct
{
    String cityName;
    uint8_t weatherCode;
    int8_t temperature;
    uint8_t humidity;
    int8_t maxTemp, minTemp;
    // char windDir[20];
    // uint8_t windLevel;
    uint16_t airQulity;
    uint8_t airQulityGrade;

    uint8_t dailyWeatherCode[7];
    int8_t dailyMax[7];
    int8_t dailyMin[7];

    bool newNowWeatherIsUpdated;   // true表示已经更新
    bool newDailyWeatherIsUpdated; // true表示已经更新
};

extern struct city_weather_data_struct cityWeatherData;

void get_now_weather(struct city_weather_data_struct *weatherData);
void get_daily_weather(struct city_weather_data_struct *weatherData);

#endif