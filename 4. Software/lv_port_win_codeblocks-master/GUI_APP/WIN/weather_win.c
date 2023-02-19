#include "weather_win.h"
#include "GUI_APP/IMG/img_reference.h"

LV_FONT_DECLARE(lv_font_ibmplex_115);
LV_FONT_DECLARE(lv_font_ibmplex_64);
LV_FONT_DECLARE(ch_font20);

const void *weatherImage_map[] = {&weather_sunny, &weather_cloudy, &weather_overcast,
                                  &weather_thunder, &weather_rainy, &weather_snowy,
                                  &weather_hail, &weather_foggy, &weather_sand};

const void *spaceManImage_map[] = {&man_0, &man_1, &man_2, &man_3, &man_4, &man_5, &man_6, &man_7, &man_8, &man_9};

static const char *weekDayCh[7] = {"日", "一", "二", "三", "四", "五", "六"};
static const char *airQualityCh[6] = {"优", "良", "轻度", "中度", "重度", "严重"};

static lv_obj_t *weather_win_scr;

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

void weather_win_start(lv_obj_t *scr) //在scr屏幕对象上创建窗口
{
    weather_win_scr = scr;
    lv_scr_load(weather_win_scr);
    weather_win_style_init();

    lv_obj_add_style(weather_win_scr, &default_style, LV_STATE_DEFAULT);

    // WiFi图标
    //天气图标
    weatherImg = lv_img_create(weather_win_scr);
    lv_img_set_src(weatherImg, weatherImage_map[0]);
    lv_obj_align(weatherImg, LV_ALIGN_TOP_RIGHT, -10, 10);
    //城市文字图标
    cityLabel = lv_label_create(weather_win_scr);
    lv_obj_add_style(cityLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(cityLabel, true);
    lv_label_set_text(cityLabel, "上海");
    lv_obj_align(cityLabel, LV_ALIGN_TOP_LEFT, 20, 15);
    //空气质量
    btn = lv_btn_create(weather_win_scr);
    lv_obj_add_style(btn, &btn_style, LV_STATE_DEFAULT);
    lv_obj_set_pos(btn, 75, 15);
    lv_obj_set_size(btn, 50, 25);
    //set_airQuality_bar_color(20);

    btnLabel = lv_label_create(btn);
    lv_obj_add_style(btnLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_obj_align(btnLabel, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(btnLabel, airQualityCh[0]);
    //气温
    txtLabel = lv_label_create(weather_win_scr);
    lv_obj_add_style(txtLabel, &chFont_style, LV_STATE_DEFAULT);
    // lvgl8之前版本，模式一旦设置 LV_LABEL_LONG_SCROLL_CIRCULAR
    // 宽度恒定等于当前文本的长度，所以下面先设置以下长度
    lv_label_set_text(txtLabel, "最低气温12°C, ");
    lv_obj_set_size(txtLabel, 120, 30);
    lv_label_set_long_mode(txtLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text_fmt(txtLabel, "最低气温%d°C, 最高气温%d°C.    ", 15, 20, "西北风", 0);
    lv_obj_align(txtLabel, LV_ALIGN_TOP_LEFT, 10, 50);
    //时间
    clockLabel_1 = lv_label_create(weather_win_scr);
    lv_obj_add_style(clockLabel_1, &numberBig_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(clockLabel_1, true);
    lv_label_set_text_fmt(clockLabel_1, "%02d#ffa500 %02d#", 10, 52);
    lv_obj_align(clockLabel_1, LV_ALIGN_LEFT_MID, 0, 10);

    clockLabel_2 = lv_label_create(weather_win_scr);
    lv_obj_add_style(clockLabel_2, &numberSmall_style, LV_STATE_DEFAULT);
    lv_label_set_recolor(clockLabel_2, true);
    lv_label_set_text_fmt(clockLabel_2, "%02d", 00);
    lv_obj_align(clockLabel_2, LV_ALIGN_LEFT_MID, 165, 9);
    //日期
    dateLabel = lv_label_create(weather_win_scr);
    lv_obj_add_style(dateLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text_fmt(dateLabel, "%2d月%2d日   周%s", 11, 23, weekDayCh[1]);
    lv_obj_align(dateLabel, LV_ALIGN_LEFT_MID, 10, 32);
    //温度
    tempImg = lv_img_create(weather_win_scr);
    lv_img_set_src(tempImg, &temperatureImg);
    lv_img_set_zoom(tempImg, 180);
    lv_obj_align(tempImg, LV_ALIGN_LEFT_MID, 10, 70);
    tempBar = lv_bar_create(weather_win_scr);
    lv_obj_add_style(tempBar, &bar_style, LV_STATE_DEFAULT);
    lv_bar_set_range(tempBar, -20, 45); // 设置进度条表示的温度为-20~45
    lv_obj_set_size(tempBar, 60, 12);
    lv_obj_set_style_bg_color(tempBar, lv_palette_main(LV_PALETTE_RED), LV_PART_INDICATOR);
    lv_bar_set_value(tempBar, 40, LV_ANIM_ON);
    lv_obj_align(tempBar, LV_ALIGN_LEFT_MID, 35, 70);
    tempLabel = lv_label_create(weather_win_scr);
    lv_obj_add_style(tempLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text_fmt(tempLabel, "%2d°C", 40);
    lv_obj_align(tempLabel, LV_ALIGN_LEFT_MID, 103, 70);
    //湿度
    humiImg = lv_img_create(weather_win_scr);
    lv_img_set_src(humiImg, &humidityImg);
    lv_img_set_zoom(humiImg, 180);
    lv_obj_align(humiImg, LV_ALIGN_LEFT_MID, 0, 100);
    humiBar = lv_bar_create(weather_win_scr);
    lv_obj_add_style(humiBar, &bar_style, LV_STATE_DEFAULT);
    lv_bar_set_range(humiBar, 0, 100);
    lv_obj_set_size(humiBar, 60, 12);
    lv_obj_set_style_bg_color(humiBar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_bar_set_value(humiBar, 49, LV_ANIM_ON);
    lv_obj_align(humiBar, LV_ALIGN_LEFT_MID, 35, 100);
    humiLabel = lv_label_create(weather_win_scr);
    lv_obj_add_style(humiLabel, &chFont_style, LV_STATE_DEFAULT);
    lv_label_set_text(humiLabel, "50%");
    lv_obj_align(humiLabel, LV_ALIGN_LEFT_MID, 103, 100);
    //太空人动画
    // 太空人图标
    spaceManImg = lv_img_create(weather_win_scr);
    lv_img_set_src(spaceManImg, spaceManImage_map[9]);
    lv_obj_align(spaceManImg, LV_ALIGN_BOTTOM_RIGHT, -10, -10);
}

void weather_win_style_init(void)
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
/*
void set_airQuality_bar_color(uint16_t AQI)
{
    if (cityWeatherData->airQulity < 50)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_LIGHT_GREEN), LV_STATE_DEFAULT);
    else if (cityWeatherData->airQulity < 100)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_YELLOW), LV_STATE_DEFAULT);
    else if (cityWeatherData->airQulity < 150)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_ORANGE), LV_STATE_DEFAULT);
    else if (cityWeatherData->airQulity < 200)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_RED), LV_STATE_DEFAULT);
    else if (cityWeatherData->airQulity < 300)
        lv_obj_set_style_bg_color(btn, lv_palette_main(LV_PALETTE_PURPLE), LV_STATE_DEFAULT);
    else
        lv_obj_set_style_bg_color(btn, lv_color_hex(0x7d1320), LV_STATE_DEFAULT);
}
*/
