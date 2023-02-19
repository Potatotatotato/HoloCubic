#include "desktop_win.h"
#include "weather_win.h"

static lv_obj_t* scr_weather_1;


void desktop_win_start(void)
{
    scr_weather_1 = lv_obj_create(NULL);
    weather_win_start(scr_weather_1);

}


