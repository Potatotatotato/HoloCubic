#include <WiFi.h>
#include <ArduinoJson.h>
#include <map>
#include "weather.h"
#include "rtc_time.h"
#include "sd_card.h"

//================================
//      Macro Definition
//================================
#define WEATHER_UPDATE_INTERAL 30 * 60 * 1000 // 30minutes

//================================
//      Function Declaration
//================================

//================================
//      Variable Declaration
//================================
struct city_weather_data_struct cityWeatherData = {"", 0};
std::map<String, uint8_t> weatherImgMap = {{"qing", 0}, {"yun", 1}, {"yin", 2}, {"lei", 3}, {"yu", 4}, {"xue", 5}, {"bingbao", 6}, {"wu", 7}, {"shachen", 8}};

//================================
//   Const Variable Declaration
//================================
const char *weather_host = "www.yiketianqi.com";
const char *weather_now_url = "/free/day?unescape=1&city=";
const char *weather_daily_url = "/free/week?unescape=1&city=";

//================================
//      Function Defination
//================================
void get_now_weather(struct city_weather_data_struct *cityWeatherData)
{
    if (WiFi.status() == WL_CONNECTED) //确保WiFi连接
    {
        WiFiClient weatherWifiClient;

        String httpRequest = String("GET ") + weather_now_url + cityWeatherData->cityName + tfCard.weather_apply_id_and_secret + " HTTP/1.1\r\n" +
                             "Host: " + weather_host + "\r\n" +
                             "Connection: close\r\n\r\n";

        if (weatherWifiClient.connect(weather_host, 80))
        {
            weatherWifiClient.print(httpRequest);
            while (weatherWifiClient.connected() || weatherWifiClient.available())
            {
                if (weatherWifiClient.available())
                {
                    //响应头第一行
                    // String responseHead = weatherWifiClient.readStringUntil('\n');
                    // Serial.println(responseHead);
                    String jsonStr = weatherWifiClient.readString();
                    // Serial.println(jsonStr);

                    //不知为啥，返回的响应体是不规范的json数据，这里直接跳过响应头，以及响应体前面的几个字符
                    while (jsonStr[0] != '{')
                        jsonStr = jsonStr.substring(1);
                    while (jsonStr[jsonStr.length() - 1] != '}')
                        jsonStr = jsonStr.substring(0, jsonStr.length() - 1);

                    //开始json数据解析
                    StaticJsonDocument<512> doc;
                    DeserializationError error = deserializeJson(doc, jsonStr);
                    if (error)
                    {
                        Serial.print("deserializeJson() failed: ");
                        Serial.println(error.c_str());
                        return;
                    }
                    cityWeatherData->weatherCode = weatherImgMap[doc["wea_img"].as<String>()];
                    cityWeatherData->temperature = doc["tem"].as<int8_t>();
                    cityWeatherData->maxTemp = doc["tem_day"].as<int8_t>();
                    cityWeatherData->minTemp = doc["tem_night"].as<int8_t>();
                    // strcpy(cityWeatherData.windDir, doc["win"].as<String>().c_str());
                    String humidityStr = doc["humidity"].as<String>();
                    cityWeatherData->humidity = atoll(humidityStr.substring(0, humidityStr.length() - 1).c_str());
                    if (cityWeatherData->humidity == 100)
                        cityWeatherData->humidity = 99; //显示不了100%湿度

                    cityWeatherData->airQulity = doc["air"].as<uint16_t>();
                    if (cityWeatherData->airQulity < 50)
                        cityWeatherData->airQulityGrade = 0; //优
                    else if (cityWeatherData->airQulity < 100)
                        cityWeatherData->airQulityGrade = 1; //良
                    else if (cityWeatherData->airQulity < 150)
                        cityWeatherData->airQulityGrade = 2; //轻度污染
                    else if (cityWeatherData->airQulity < 200)
                        cityWeatherData->airQulityGrade = 3; //中度污染
                    else if (cityWeatherData->airQulity < 300)
                        cityWeatherData->airQulityGrade = 4; //重度污染
                    else
                        cityWeatherData->airQulityGrade = 5; //严重污染
                    cityWeatherData->newNowWeatherIsUpdated = true;
                }
            }
        }
    }

    else
    {
        Serial.println("WiFi not available");
    }
}

void get_daily_weather(struct city_weather_data_struct *cityWeatherData)
{
    if (WiFi.status() == WL_CONNECTED) //确保WiFi连接
    {
        WiFiClient weatherWifiClient;

        String httpRequest = String("GET ") + weather_daily_url + cityWeatherData->cityName + tfCard.weather_apply_id_and_secret + " HTTP/1.1\r\n" +
                             "Host: " + weather_host + "\r\n" +
                             "Connection: close\r\n\r\n";

        if (weatherWifiClient.connect(weather_host, 80))
        {
            weatherWifiClient.print(httpRequest);
            while (weatherWifiClient.connected() || weatherWifiClient.available())
            {
                if (weatherWifiClient.available())
                {
                    //响应头第一行
                    // String responseHead = weatherWifiClient.readStringUntil('\n');
                    // Serial.println(responseHead);
                    StaticJsonDocument<1536> doc;

                    String jsonStr = weatherWifiClient.readString();

                    //不知为啥，返回的响应体是不规范的json数据，这里直接跳过响应头，以及响应体前面的几个字符
                    while (jsonStr[0] != '{')
                        jsonStr = jsonStr.substring(1);
                    while (jsonStr[jsonStr.length() - 1] != '}')
                        jsonStr = jsonStr.substring(0, jsonStr.length() - 1);

                    DeserializationError error = deserializeJson(doc, jsonStr);
                    if (error)
                    {
                        Serial.print("deserializeJson() failed: ");
                        Serial.println(error.c_str());
                        return;
                    }

                    for (int i = 0; i < 7; i++)
                    {
                        cityWeatherData->dailyMax[i] = doc["data"][i]["tem_day"].as<int8_t>();
                        cityWeatherData->dailyMin[i] = doc["data"][i]["tem_night"].as<int8_t>();
                        cityWeatherData->dailyWeatherCode[i] = weatherImgMap[doc["data"][i]["wea_img"].as<String>()];
                    }
                    cityWeatherData->newDailyWeatherIsUpdated = true;
                }
            }
        }
    }
    else
    {
        Serial.println("WiFi not available");
    }
}
