#include "weather_win_spaceman.h"
#include "GUI_APP/IMG/img_reference.h"
#include "weather.h"
#include "rtc_time.h"

//================================
//      Macro Definition
//================================

//================================
//      Function Declaration
//================================

//================================
//      Variable Declaration
//================================
static char humidityStr[4] = {'0', '0', '%', '\0'};

static lv_obj_t *now_weather_win_scr;

static lv_style_t default_style;
static lv_style_t chFont_style;
static lv_style_t numberSmall_style;
static lv_style_t numberBig_style;
static lv_style_t btn_style;
static lv_style_t bar_style;

static lv_obj_t *weatherImg;
static lv_obj_t *cityLabel;
static lv_obj_t *btn, *btnLabel;
static lv_obj_t *txtLabel;
static lv_obj_t *clockLabel_1, *clockLabel_2;
static lv_obj_t *dateLabel;
static lv_obj_t *tempImg, *tempBar, *tempLabel;
static lv_obj_t *humiImg, *humiBar, *humiLabel;
static lv_obj_t *spaceManImg;

static lv_timer_t *weather_win_refresh_timer;
//================================
//   Const Variable Declaration
//================================
const void *weatherImage_map[] = {&weather_sunny, &weather_cloudy, &weather_overcast,
                                  &weather_thunder, &weather_rainy, &weather_snowy,
                                  &weather_hail, &weather_foggy, &weather_sand};

const void *spaceManImage_map[] = {&man_0, &man_1, &man_2, &man_3, &man_4, &man_5, &man_6, &man_7, &man_8, &man_9};

const void *cxkImage_map[] = {&CXK00, &CXK01, &CXK02, &CXK03, &CXK04, &CXK05, &CXK06, &CXK07, &CXK08, &CXK09,
                              &CXK10, &CXK11, &CXK12, &CXK13, &CXK14, &CXK15, &CXK16, &CXK17, &CXK18, &CXK19,
                              &CXK20, &CXK21, &CXK22, &CXK23, &CXK24, &CXK25, &CXK26, &CXK27, &CXK28, &CXK29,
                              &CXK30, &CXK31, &CXK32, &CXK33, &CXK34, &CXK35, &CXK36};

static const char *weekDayCh[7] = {"日", "一", "二", "三", "四", "五", "六"};
static const char *airQualityCh[6] = {"优", "良", "轻度", "中度", "重度", "严重"};

//================================
//   Extern Variable Declaration
//================================
LV_FONT_DECLARE(lv_font_ibmplex_115);
LV_FONT_DECLARE(lv_font_ibmplex_64);
LV_FONT_DECLARE(ch_font20);

//================================
//      Function Defination
//================================
void weather_win_spaceman_start(lv_obj_t *scr, struct city_weather_data_struct *cityWeatherData) //在scr屏幕对象上创建窗口
{
    struct tm timeinfo = esp32Time.getTimeStruct();
    now_weather_win_scr = scr;
    lv_scr_load(now_weather_win_scr);
    weather_win_spaceman_style_init();

    lv_obj_add_style(now_weather_win_scr, &default_style, LV_STATE_DEFAULT);

    // WiFi图标
    //天气图标
    weatherImg = lv_img_create(now_weather_win_scr);
    lv_img_set_src(weatherImg, weatherImage_map[cityWeatherData->weatherCode]);
    lv_obj_align(weatherImg, LV_ALIGN_TOP_RIGHT, -10, 10);
    //城市文字图标
    cityLabel = lv_label_create(now_weather_win_scr);
    lv_obj_add_style(cityLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(cityLabel, true);
    lv_label_set_text(cityLabel, cityWeatherData->cityName.c_str());
    lv_obj_align(cityLabel, LV_ALIGN_TOP_LEFT, 20, 15);
    //空气质量
    btn = lv_btn_create(now_weather_win_scr);
    lv_obj_add_style(btn, &btn_style, LV_STATE_DEFAULT);
    lv_obj_set_pos(btn, 75, 15);
    lv_obj_set_size(btn, 50, 25);
    set_airQuality_bar_color(cityWeatherData->airQulity);

    btnLabel = lv_label_create(btn);
    lv_obj_add_style(btnLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_obj_align(btnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(btnLabel, airQualityCh[cityWeatherData->airQulityGrade]);
    //今日气温
    txtLabel = lv_label_create(now_weather_win_scr);
    lv_obj_add_style(txtLabel, &chFont_style, LV_STATE_DEFAULT);
    // lvgl8之前版本，模式一旦设置 LV_LABEL_LONG_SCROLL_CIRCULAR
    // 宽度恒定等于当前文本的长度，所以下面先设置以下长度
    lv_label_set_text(txtLabel, "最低气温12°C, ");
    lv_obj_set_size(txtLabel, 120, 30);
    lv_label_set_long_mode(txtLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text_fmt(txtLabel, "最低气温%d°C, 最高气温%d°C.    ", cityWeatherData->minTemp, cityWeatherData->maxTemp);
    lv_obj_align(txtLabel, LV_ALIGN_TOP_LEFT, 10, 50);
    //时间
    clockLabel_1 = lv_label_create(now_weather_win_scr);
    lv_obj_add_style(clockLabel_1, &numberBig_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(clockLabel_1, true);
    lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#", timeinfo.tm_hour, timeinfo.tm_min);
    lv_obj_align(clockLabel_1, LV_ALIGN_LEFT_MID, 0, 10);

    clockLabel_2 = lv_label_create(now_weather_win_scr);
    lv_obj_add_style(clockLabel_2, &numberSmall_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(clockLabel_2, true);
    lv_label_set_text_fmt(clockLabel_2, "%02d", timeinfo.tm_sec);
    lv_obj_align(clockLabel_2, LV_ALIGN_LEFT_MID, 165, 9);
    //日期
    dateLabel = lv_label_create(now_weather_win_scr);
    lv_obj_add_style(dateLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s", timeinfo.tm_mon + 1, timeinfo.tm_mday, weekDayCh[timeinfo.tm_wday]);
    lv_obj_align(dateLabel, LV_ALIGN_LEFT_MID, 10, 32);
    //温度
    tempImg = lv_img_create(now_weather_win_scr);
    lv_img_set_src(tempImg, &temperatureImg);
    lv_img_set_zoom(tempImg, 180);
    lv_obj_align(tempImg, LV_ALIGN_LEFT_MID, 10, 70);
    tempBar = lv_bar_create(now_weather_win_scr);
    lv_obj_add_style(tempBar, &bar_style, LV_STATE_DEFAULT);
    lv_bar_set_range(tempBar, -20, 45); // 设置进度条表示的温度为-20~45
    lv_obj_set_size(tempBar, 60, 12);
    lv_obj_set_style_bg_color(tempBar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_bar_set_value(tempBar, cityWeatherData->temperature, LV_ANIM_ON);
    lv_obj_align(tempBar, LV_ALIGN_LEFT_MID, 35, 70);
    tempLabel = lv_label_create(now_weather_win_scr);
    lv_obj_add_style(tempLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text_fmt(tempLabel, "%2d°C", cityWeatherData->temperature);
    lv_obj_align(tempLabel, LV_ALIGN_LEFT_MID, 103, 70);
    //湿度
    humiImg = lv_img_create(now_weather_win_scr);
    lv_img_set_src(humiImg, &humidityImg);
    lv_img_set_zoom(humiImg, 180);
    lv_obj_align(humiImg, LV_ALIGN_LEFT_MID, 0, 100);
    humiBar = lv_bar_create(now_weather_win_scr);
    lv_obj_add_style(humiBar, &bar_style, LV_STATE_DEFAULT);
    lv_bar_set_range(humiBar, 0, 100);
    lv_obj_set_size(humiBar, 60, 12);
    lv_obj_set_style_bg_color(humiBar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_bar_set_value(humiBar, cityWeatherData->humidity, LV_ANIM_ON);
    lv_obj_align(humiBar, LV_ALIGN_LEFT_MID, 35, 100);
    humiLabel = lv_label_create(now_weather_win_scr);
    lv_obj_add_style(humiLabel, &chFont_style, LV_STATE_DEFAULT);
    sprintf(humidityStr, "%2d", cityWeatherData->humidity); //如果这样写："%2d%"，好像有点问题
    humidityStr[2] = '%';
    lv_label_set_text(humiLabel, humidityStr);
    lv_obj_align(humiLabel, LV_ALIGN_LEFT_MID, 103, 100);
    //太空人动画
    // 太空人图标
    spaceManImg = lv_img_create(now_weather_win_scr);
    lv_img_set_src(spaceManImg, spaceManImage_map[0]);
    lv_obj_align(spaceManImg, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
}

void weather_win_spaceman_style_init(void)
{
    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, lv_color_hex(0x000000));

    lv_style_init(&chFont_style);
    lv_style_set_text_opa(&chFont_style, LV_OPA_COVER);
    lv_style_set_text_color(&chFont_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&chFont_style, &ch_font20);

    lv_style_init(&numberSmall_style);
    lv_style_set_text_opa(&numberSmall_style, LV_OPA_COVER);
    lv_style_set_text_color(&numberSmall_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&numberSmall_style, &lv_font_ibmplex_64);

    lv_style_init(&numberBig_style);
    lv_style_set_text_opa(&numberBig_style, LV_OPA_COVER);
    lv_style_set_text_color(&numberBig_style, lv_color_hex(0xffffff));
    lv_style_set_text_font(&numberBig_style, &lv_font_ibmplex_115);

    lv_style_init(&btn_style);
    lv_style_set_border_width(&btn_style, 0);
    lv_style_set_outline_width(&btn_style, 0);
    lv_style_set_shadow_width(&btn_style, 0);

    lv_style_init(&bar_style);
    lv_style_set_bg_color(&bar_style, lv_color_hex(0x000000));
    lv_style_set_border_width(&bar_style, 2);
    lv_style_set_border_color(&bar_style, lv_color_hex(0xFFFFFF));
    lv_style_set_pad_top(&bar_style, 1); // 指示器到背景四周的距离
    lv_style_set_pad_bottom(&bar_style, 1);
    lv_style_set_pad_left(&bar_style, 1);
    lv_style_set_pad_right(&bar_style, 1);
}

void set_airQuality_bar_color(uint16_t AQI)
{
    if (AQI < 50)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_LIGHT_GREEN), LV_STATE_DEFAULT);
    else if (AQI < 100)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_YELLOW), LV_STATE_DEFAULT);
    else if (AQI < 150)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_ORANGE), LV_STATE_DEFAULT);
    else if (AQI < 200)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
    else if (AQI < 300)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_PURPLE), LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x7d1320), LV_STATE_DEFAULT);
}

void refresh_weather_win_spaceman_now_weather(struct city_weather_data_struct *cityWeatherData)
{
    //天气图标
    lv_img_set_src(weatherImg, weatherImage_map[cityWeatherData->weatherCode]);
    //空气质量
    set_airQuality_bar_color(cityWeatherData->airQulity);
    lv_label_set_text(btnLabel, airQualityCh[cityWeatherData->airQulityGrade]);
    //今日气温
    lv_label_set_text_fmt(txtLabel, "最低气温%d°C, 最高气温%d°C.    ", cityWeatherData->minTemp, cityWeatherData->maxTemp);
    //温度
    lv_bar_set_value(tempBar, cityWeatherData->temperature, LV_ANIM_ON);
    lv_label_set_text_fmt(tempLabel, "%2d°C", cityWeatherData->temperature);
    //湿度
    lv_bar_set_value(humiBar, cityWeatherData->humidity, LV_ANIM_ON);
    sprintf(humidityStr, "%2d", cityWeatherData->humidity); //如果这样写："%2d%"，好像有点问题
    humidityStr[2] = '%';

    cityWeatherData[0].newNowWeatherIsUpdated = false;
}

void refresh_weather_win_spaceman_clock()
{
    struct tm timeinfo = esp32Time.getTimeStruct();
    lv_label_set_text_fmt(clockLabel_2, "%02d", timeinfo.tm_sec);
    if (timeinfo.tm_sec == 0)
    {
        lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#", timeinfo.tm_hour, timeinfo.tm_min);
        if (timeinfo.tm_hour == 0)
            lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s", timeinfo.tm_mon + 1, timeinfo.tm_mday, weekDayCh[timeinfo.tm_wday]);
    }
}

void refresh_weather_win_spaceman_anim()
{
    static uint8_t frame = 0;
    lv_img_set_src(spaceManImg, cxkImage_map[frame]);

    frame++;
    if (frame == 37)
        frame = 0;
}
