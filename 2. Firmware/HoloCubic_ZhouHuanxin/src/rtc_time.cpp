#include <WiFi.h>
#include <ArduinoJson.h>
#include "rtc_time.h"

//================================
//      Macro Definition
//================================
#define TIME_UPDATE_INTERAL 4 * 60 * 60 * 1000 // 时钟的联网更新间隔：4hours

//================================
//      Function Declaration
//================================
void rtc_time_wifi_update_timer_cb(struct _lv_timer_t *lv_timer_t);
//================================
//      Variable Declaration
//================================
ESP32Time esp32Time;
lv_timer_t *rtc_time_wifi_update_timer;
lv_timer_t *rtc_time_increase_timer;

//================================
//   Const Variable Declaration
//================================
const char *time_host = "api.m.taobao.com";
const char *time_url = "/rest/api3.do?api=mtop.common.gettimestamp";

//================================
//      Function Defination
//================================
void rtc_time_init()
{
    esp32Time.offset = 28800;
    get_timestamp();
    // 开启一个定时器，WiFi定时联网更新
    rtc_time_wifi_update_timer = lv_timer_create(rtc_time_wifi_update_timer_cb, TIME_UPDATE_INTERAL, NULL); // 30minutes
}

void get_timestamp(void)
{
    if (WiFi.status() == WL_CONNECTED) //确保WiFi连接
    {
        WiFiClient timeClient;

        String httpRequest = String("GET ") + time_url + " HTTP/1.1\r\n" +
                             "Host: " + time_host + "\r\n" +
                             "Connection: close\r\n\r\n";

        if (timeClient.connect(time_host, 80))
        {
            timeClient.print(httpRequest);
            while (timeClient.connected() || timeClient.available())
            {
                if (timeClient.available())
                {
                    //响应头第一行
                    // String responseHead = timeClient.readStringUntil('\n');
                    // Serial.println(responseHead);
                    timeClient.find("\r\n\r\n");
                    String jsonStr = timeClient.readString();

                    StaticJsonDocument<192> doc;
                    DeserializationError error = deserializeJson(doc, jsonStr);
                    if (error)
                    {
                        Serial.print("deserializeJson() failed: ");
                        Serial.println(error.c_str());
                        return;
                    }
                    String timestamp = doc["data"]["t"].as<String>(); //获取的时间戳精确到了ms
                    timestamp = timestamp.substring(0, timestamp.length() - 3);
                    uint32_t nowTime = atoll(timestamp.c_str());

                    Serial.printf("[Update timestamp]: %d\r\n\r\n", nowTime);
                    esp32Time.setTime(nowTime);
                }
            }
        }
    }

    else
    {
        Serial.println("WiFi not available");
    }
}

void rtc_time_wifi_update_timer_cb(struct _lv_timer_t *lv_timer_t)
{
    get_timestamp();
}
