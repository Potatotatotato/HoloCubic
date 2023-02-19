#include <WiFi.h>
#include "desktop_win.h"
#include "weather_win_spaceman.h"
#include "sd_card.h"
#include "weather.h"
#include "rtc_time.h"
#include "imu.h"

//================================
//      Macro Definition
//================================
#define WEATHER_WIFI_UPDATE_INTERAL 60 * 60 * 1000 // 天气的联网更新间隔：60minutes
#define WIN_REFRESH_INTERAL 100                    // 屏幕刷新间隔：100ms

//================================
//      Function Declaration
//================================
void win_refresh_timer_cb(struct _lv_timer_t *lv_timer_t);
void weather_wifi_update_timer_cb(struct _lv_timer_t *lv_timer_t);

//================================
//      Variable Declaration
//================================
static lv_obj_t *scr_now_weather;

lv_timer_t *appHandlerTimer;
lv_timer_t *winRefreshTimer;
lv_timer_t *weatherWiFiUpdateTimer;

//模拟的互斥信号量，app切换期间不读取imu数据
bool appSwitchHandlerLock = false;

//================================
//   Const Variable Declaration
//================================

//================================
//      Function Defination
//================================
void desktop_win_start(void)
{
    scr_now_weather = lv_obj_create(NULL);

    //创建app handler
    // appHandlerTimer = lv_timer_create(app_handler_timer_cb, APP_HANDLER_INTERAL, NULL);
    //创建刷新屏幕的定时器
    winRefreshTimer = lv_timer_create(win_refresh_timer_cb, WIN_REFRESH_INTERAL, NULL);
    //创建更新天气的定时器
    weatherWiFiUpdateTimer = lv_timer_create(weather_wifi_update_timer_cb, WEATHER_WIFI_UPDATE_INTERAL, NULL);

    weather_win_spaceman_start(scr_now_weather, &cityWeatherData);
    //创建窗口结束，回到main函数
}

//刷新屏幕定时器的回调函数
void win_refresh_timer_cb(struct _lv_timer_t *lv_timer_t)
{
    // 刷新太空人动画
    refresh_weather_win_spaceman_anim();
    //刷新时间
    refresh_weather_win_spaceman_clock();

    //判断是否需要刷新天气到屏幕
    if (cityWeatherData.newNowWeatherIsUpdated == true)
    {
        refresh_weather_win_spaceman_now_weather(&cityWeatherData);
        Serial.println("\r\nNew weather data refreshed to scree.");
    }
}

//更新天气的回调函数
void weather_wifi_update_timer_cb(struct _lv_timer_t *lv_timer_t)
{
    get_now_weather(&cityWeatherData);
    // get_daily_weather(&cityWeatherData);
}
