/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 * 
 * Reference: https://github.com/Lora-net/one_channel_hub
 * 
 */

/* -------------------------------------------------------------------------- */
/* --- DEPENDENCIES --------------------------------------------------------- */

#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include <driver/temperature_sensor.h>

/* Services */
#include "radio.h"




void app_main(void)
{

    // int                         i; /* loop variable and temporary variable for return value */
    // esp_err_t                   esp_err;
    temperature_sensor_handle_t temp_sensor = NULL;
    float tsens_out;

    /* Initialize temperature sensor (before running the pkt fwd thread) */
    temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT( -10, 80 );
    ESP_ERROR_CHECK( temperature_sensor_install( &temp_sensor_config, &temp_sensor ) );
    ESP_ERROR_CHECK( temperature_sensor_enable( temp_sensor ) );

    /* Start Radio */
    // launch_radio( temp_sensor );

    while (1)
    {
        ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_sensor, &tsens_out));

        printf("Hello world!\n");
        printf("Temperature in %f °C\n", tsens_out);

        vTaskDelay(CONFIG_PAUSE / portTICK_PERIOD_MS);
    }
    

}
