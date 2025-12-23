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
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#include <driver/temperature_sensor.h>

static const char *TAG = "example";

// #define USER_LED_GPIO 35 /* User LED */

/* Services */
#include "heartbeat.h"
#include "radio.h"
#include "hw_board_defs.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DECLARATION ---------------------------------------- */

static void configure_user_led( void );
static void set_user_led( bool on );

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */

// void wait_on_error( lorahub_error_t error, int line )
// {
//     /* Send error code to display (if available) */
//     display_error_t err = { .err = error, .line = line };
//     display_update_error( &err );

//     /* loop and blink LED for 10 seconds */
//     bool led_status = true;
//     for( int i = 0; i < 10; i++ )
//     {
//         led_status = !led_status;
//         set_user_led( led_status );
//         vTaskDelay( 1000 / portTICK_PERIOD_MS );
//     };

//     /* Reset the ESP32 */
//     esp_restart( );
// }

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void configure_user_led( void )
{
#ifdef USER_LED_GPIO
    gpio_reset_pin( USER_LED_GPIO );
    gpio_set_direction( USER_LED_GPIO, GPIO_MODE_OUTPUT );
    gpio_set_level( USER_LED_GPIO, 0 );
#endif
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void set_user_led( bool on )
{
#ifdef USER_LED_GPIO
    gpio_set_level( USER_LED_GPIO, ( on == true ) ? 1 : 0 );
#endif
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

void app_main(void)
{

    ESP_LOGI(TAG, "Install temperature sensor, expected temp ranger range: 10-50 °C");
    // int                         i; /* loop variable and temporary variable for return value */
    // esp_err_t                   esp_err;
    temperature_sensor_handle_t temp_sensor = NULL;

    /* configure LED */
    // configure_user_led( );


    /* Initialize temperature sensor (before running the pkt fwd thread) */
    temperature_sensor_config_t temp_sensor_config = TEMPERATURE_SENSOR_CONFIG_DEFAULT( 10, 50 );
    ESP_ERROR_CHECK( temperature_sensor_install( &temp_sensor_config, &temp_sensor ) );

    ESP_LOGI(TAG, "Enable temperature sensor");
    ESP_ERROR_CHECK( temperature_sensor_enable( temp_sensor ) );


    /* Start Heartbeat */
    launch_heartbeat( );

    /* Start Radio */
    launch_radio( temp_sensor );

    // ESP_LOGI(TAG, "Read temperature");
    // float tsens_out;

    bool led_status = true;

    while (1)
    {
        // ESP_ERROR_CHECK(temperature_sensor_get_celsius(temp_sensor, &tsens_out));

        // printf("Hello world!\n");
        // ESP_LOGI(TAG, "Temperature value %.02f °C", tsens_out);
        // led_status = !led_status;
        // set_user_led( led_status );

        vTaskDelay(CONFIG_PAUSE / portTICK_PERIOD_MS);
    }
    

}
