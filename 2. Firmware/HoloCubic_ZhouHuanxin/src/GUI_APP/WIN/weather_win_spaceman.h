#ifndef __WEATHER_1_WIN_H
#define __WEATHER_1_WIN_H
#include <Arduino.h>
#include <lvgl.h>
#include "GUI_APP/IMG/img_reference.h"

void weather_win_spaceman_start(lv_obj_t *scr, struct city_weather_data_struct *weatherData);
void weather_win_spaceman_style_init(void);
void set_airQuality_bar_color(uint16_t AQI);

void refresh_weather_win_spaceman_now_weather(struct city_weather_data_struct *cityWeatherData);
void refresh_weather_win_spaceman_clock();
void refresh_weather_win_spaceman_anim();

#endif
