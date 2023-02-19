#include <WiFi.h>
#include "boot_animation_win.h"
#include "GUI_APP/WIN/desktop_win.h"
#include "GUI_APP/IMG/img_reference.h"
#include "rtc_time.h"
#include "weather.h"
#include "network.h"
//================================
//      Macro Definition
//================================
#define BOOT_ANIMATION_PLAY_TIMES 3    // BOOT动画播放次数
#define BOOT_ANIMATION_FRAME_PERIOD 28 // BOOT动画帧周期

#define ANIMATION_MAP BOOT_ANIM_map //选择需要播放的动画map

//================================
//      Function Declaration
//================================
void play_boot_animation_timer_cb(struct _lv_timer_t *lv_timer_t);
void show_welcome_img(void);

//================================
//      Variable Declaration
//================================
static lv_style_t default_style;

static lv_obj_t *scr;
static lv_obj_t *boot_animation_img;
static lv_obj_t *welcome_label;

static uint8_t total_anim_frame; //动画总帧数
static lv_timer_t *play_boot_animation_timer;
//================================
//   Const Variable Declaration
//================================
const void *CXK_200pix_map[] = {&CXK_200pix00, &CXK_200pix01, &CXK_200pix02, &CXK_200pix03, &CXK_200pix04, &CXK_200pix05, &CXK_200pix06, &CXK_200pix07, &CXK_200pix08, &CXK_200pix09,
                                &CXK_200pix10, &CXK_200pix11, &CXK_200pix12, &CXK_200pix13, &CXK_200pix14, &CXK_200pix15, &CXK_200pix16, &CXK_200pix17, &CXK_200pix18, &CXK_200pix19,
                                &CXK_200pix20, &CXK_200pix21, &CXK_200pix22, &CXK_200pix23, &CXK_200pix24, &CXK_200pix25, &CXK_200pix26, &CXK_200pix27, &CXK_200pix28, &CXK_200pix29,
                                &CXK_200pix30, &CXK_200pix31, &CXK_200pix32, &CXK_200pix33, &CXK_200pix34, &CXK_200pix35, &CXK_200pix36};

const void *BOOT_ANIM_map[] = {&BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00,
                               &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00,
                               &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_13, &BOOT_ANIM_14,
                               &BOOT_ANIM_15, &BOOT_ANIM_16, &BOOT_ANIM_17, &BOOT_ANIM_18, &BOOT_ANIM_19, &BOOT_ANIM_20, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21,
                               &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21,
                               &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_21, &BOOT_ANIM_20, &BOOT_ANIM_19, &BOOT_ANIM_18, &BOOT_ANIM_17, &BOOT_ANIM_16,
                               &BOOT_ANIM_15, &BOOT_ANIM_14, &BOOT_ANIM_13, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00,
                               &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00,
                               &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00, &BOOT_ANIM_00};
//================================
//      Function Defination
//================================
void boot_animation_win_start(void)
{
    total_anim_frame = sizeof(ANIMATION_MAP) / 4;
    Serial.printf("\r\nanim_frame = %d\r\n", total_anim_frame);

    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, lv_color_hex(0x000000));

    scr = lv_scr_act();
    lv_obj_add_style(scr, &default_style, LV_STATE_DEFAULT);

    char bootImgPath[30];
    boot_animation_img = lv_img_create(scr);
    lv_obj_align(boot_animation_img, LV_ALIGN_CENTER, 0, 0);


    //设置播放开机动画的定时器，用定时器的目的是控制帧率
    play_boot_animation_timer = lv_timer_create(play_boot_animation_timer_cb, BOOT_ANIMATION_FRAME_PERIOD, NULL);

    // 阻塞在这里，出口在播放boot动画的定时器里
    while (boot_animation_img != NULL)
        lv_task_handler();
}

void play_boot_animation_timer_cb(struct _lv_timer_t *lv_timer_t)
{
    static uint8_t playTimes = 0;
    static uint8_t frame = 0;

    if (playTimes < BOOT_ANIMATION_PLAY_TIMES)
    {
        lv_img_set_src(boot_animation_img, ANIMATION_MAP[frame++]);

        if (frame == total_anim_frame)
        {
            frame = 0;
            playTimes++;
        }
    }
    else
    {
        if (WiFi.status() == WL_CONNECTED) // wifi上连接
        {
            Serial.println("");
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());

            //设置动画播放结束以后的画面
            show_welcome_img();
            //获取天气
            get_now_weather(&cityWeatherData);
            // get_daily_weather(&cityWeatherData);

            //开机动画结束
            desktop_win_start();
        }
    }
}

void show_welcome_img(void)
{
    rtc_time_init(); //获取时间
    lv_obj_del(boot_animation_img);
    boot_animation_img = NULL;
    lv_timer_del(play_boot_animation_timer);
}
