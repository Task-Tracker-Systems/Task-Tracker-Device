/**
 * @file lvgl_helpers.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lvgl_helpers.h"
#include "esp_log.h"
#include "sdkconfig.h"

#include "lvgl_i2c_conf.h"

#include "driver/i2c.h"

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "src/lv_core/lv_refr.h"
#else
#include "lvgl/src/lv_core/lv_refr.h"
#endif

#define TAG "helper"

/* Config the i2c master
 *
 * This should init the i2c master to be used on display and touch controllers.
 * So we should be able to know if the display and touch controllers shares the
 * same i2c master.
 */
bool lvgl_i2c_driver_init(int port, int sda_pin, int scl_pin, int speed_hz)
{
    esp_err_t err;

    ESP_LOGI(TAG, "Initializing I2C master port %d...", port);
    ESP_LOGI(TAG, "SDA pin: %d, SCL pin: %d, Speed: %d (Hz)",
             sda_pin, scl_pin, speed_hz);

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = scl_pin,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = speed_hz,
    };

    ESP_LOGI(TAG, "Setting I2C master configuration...");
    err = i2c_param_config(port, &conf);
    assert(ESP_OK == err);

    ESP_LOGI(TAG, "Installing I2C master driver...");
    err = i2c_driver_install(port,
                             I2C_MODE_MASTER,
                             0, 0 /*I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE */,
                             0 /* intr_alloc_flags */);
    assert(ESP_OK == err);

    return ESP_OK != err;
}
