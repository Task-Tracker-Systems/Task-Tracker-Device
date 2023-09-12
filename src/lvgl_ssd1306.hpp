/**
 * @file lv_templ.h
 *
 */

#ifndef SSD1306_H
#define SSD1306_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 * GLOBAL PROTOTYPES
 **********************/
void lv_ssd1306_init(void);
void lv_ssd1306_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_map);
void lv_ssd1306_rounder(lv_disp_drv_t * disp_drv, lv_area_t *area);
void lv_ssd1306_set_px_cb(lv_disp_drv_t * disp_drv, uint8_t * buf, lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
    lv_color_t color, lv_opa_t opa);

void lv_ssd1306_sleep_in(void);
void lv_ssd1306_sleep_out(void);

/**********************
 *      MACROS
 **********************/




#endif /*SSD1306_H*/
