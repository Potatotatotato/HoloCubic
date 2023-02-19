#include <Arduino.h>
#include <string.h>
#include <SPI.h>
#include <FS.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <FastLED.h>
#include "ws2812_led.h"
#include "imu.h"
#include "sd_card.h"
#include "lv_port_disp.h"
#include "lv_port_fs.h"
#include "GUI_APP/WIN/boot_animation_win.h"
#include "network.h"
#include "led_ws2812.h"

//================================
//      Macro Definition
//================================

//================================
//      Function Declaration
//================================
void my_print(const char *buf);

//================================
//      Variable Declaration
//================================
TFT_eSPI tft = TFT_eSPI();

//================================
//      Function Defination
//================================
void setup()
{
  Serial.begin(115200);

  // lv_log_register_print_cb(my_print);   //记录log
  lv_init();
  lv_port_disp_init(240); //  亮度 0-255
  lv_port_indev_init();
  lv_port_fs_init();

    //初始化tf卡，读取wifi密码信息、城市信息
  if (tfCard.init() == 0)
    tfCard.readConfigFile();
  startWiFiConnection();

  boot_animation_win_start();

  led_ws2812_init(35);
  //最后初始化imu
  //imu.init();
}

void loop()
{
  checkWiFiConnection(10000);
  //imu.update(100);
  lv_task_handler();
}

void my_print(const char *buf)
{
  Serial.println(buf);
}
