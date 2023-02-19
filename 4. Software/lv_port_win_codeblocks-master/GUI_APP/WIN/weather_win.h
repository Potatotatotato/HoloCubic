#ifndef __WEATHER_1_WIN_H
#define __WEATHER_1_WIN_H
#include <lvgl/lvgl.h>
#include "GUI_APP/IMG/img_reference.h"

void weather_win_start(lv_obj_t* scr);
void weather_win_style_init(void);
void set_airQuality_bar_color(uint16_t AQI);
#endif
