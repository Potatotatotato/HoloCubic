#include <lvgl.h>
#include "led_ws2812.h"

//================================
//      Macro Definition
//================================
#define LED_WS2812_REFRESH_PERIOD 60

//================================
//      Function Declaration
//================================
void led_ws2812_timer_cb(struct _lv_timer_t *lv_timer_t);

//================================
//      Variable Declaration
//================================
lv_timer_t *led_ws2812_timer;

CRGB leds[1];
CHSV myColor(200, 200, 110);

void led_ws2812_init(uint8_t ledBrightness)
{
    LEDS.addLeds<WS2812, 27, GRB>(leds, 1);
    FastLED.setBrightness(ledBrightness);

    led_ws2812_timer = lv_timer_create(led_ws2812_timer_cb, LED_WS2812_REFRESH_PERIOD, NULL);
}

void led_ws2812_timer_cb(struct _lv_timer_t *lv_timer_t)
{
    myColor.h = beatsin8(2, 120, 255);
    myColor.v = beatsin8(5, 210, 255);

    fill_solid(leds, 1, myColor);
    FastLED.show();
}