#include <WiFi.h>
#include "network.h"
#include "sd_card.h"
#include "rtc_time.h"
#include "weather.h"
#include "GUI_APP/WIN/desktop_win.h"

//================================
//      Macro Definition
//================================

//================================
//      Function Declaration
//================================

//================================
//      Variable Declaration
//================================
bool wifiConnectionLastStatus = true;
uint32_t lastCheckWiFiConnectionTime = 0;
//================================
//      Function Defination
//================================
void startWiFiConnection(void)
{
    Serial.print("\r\nssid: ");
    Serial.println(tfCard.ssid);
    Serial.print("password: ");
    Serial.println(tfCard.password);
    WiFi.begin(tfCard.ssid.c_str(), tfCard.password.c_str());
    WiFi.setAutoReconnect(true);
    Serial.print("Connecting to WiFi...");
}

void checkWiFiConnection(uint32_t interval)
{
    if (millis() - lastCheckWiFiConnectionTime > interval)
    {
        lastCheckWiFiConnectionTime = millis();
        if (WiFi.status() == WL_CONNECTED) //已连接WiFi
        {
            if (wifiConnectionLastStatus == false) //如果刚刚连接上WiFi
            {
                get_timestamp();
                get_now_weather(&cityWeatherData);
                get_daily_weather(&cityWeatherData);
                cityWeatherData.newDailyWeatherIsUpdated = true; //标记获取到了新的天气
            }
            wifiConnectionLastStatus = true;
        }
        else
        {
            WiFi.reconnect();
            wifiConnectionLastStatus = false;
        }
    }
}

void start_AP_mode()
{
}
