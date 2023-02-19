#include "boot_animation_win.h"
#include "GUI_APP/WIN/desktop_win.h"
#include "GUI_APP/IMG/img_reference.h"

const void *spaceManImage_map1[] = {&man_0, &man_1, &man_2, &man_3, &man_4, &man_5, &man_6, &man_7, &man_8, &man_9};

void boot_animation_img_cb(struct _lv_timer_t *lv_timer_t);

static lv_style_t default_style;
static lv_obj_t *boot_animation_img;
lv_timer_t *boot_animation_img_cb_timer;

void boot_animation_win_start(void)
{
    lv_style_init(&default_style);
    lv_style_set_bg_color(&default_style, lv_color_make(0xff, 0xff, 0xff));

    lv_obj_t *scr = lv_scr_act();
    lv_obj_add_style(scr, &default_style, LV_STATE_DEFAULT);

    boot_animation_img = lv_img_create(scr);
    lv_img_set_src(boot_animation_img, &man_0);
    lv_obj_align(boot_animation_img, LV_ALIGN_CENTER, 0, 0);

    boot_animation_img_cb_timer = lv_timer_create(boot_animation_img_cb, 100, NULL);
}

void boot_animation_img_cb(struct _lv_timer_t *lv_timer_t)
{
    static uint8_t frame = 0;

    lv_img_set_src(boot_animation_img, spaceManImage_map1[frame]);
    frame++;
    if (frame == (sizeof(spaceManImage_map1) / sizeof(spaceManImage_map1[0])))
    {
        lv_timer_del(boot_animation_img_cb_timer);
        lv_obj_del(boot_animation_img);
        boot_animation_img = NULL;
        desktop_win_start(); //跳转到主界面
    }
}
