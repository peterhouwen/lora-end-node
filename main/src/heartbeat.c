/*
 * Reference: examples/peripherals/ledc/ledc_basic/main/ledc_basic_example_main.c
 *
 */

/* -------------------------------------------------------------------------- */
/* --- DEPENDENCIES --------------------------------------------------------- */

#include <stdio.h>
#include <pthread.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>

#include "driver/ledc.h"
#include "esp_err.h"
#include "sdkconfig.h"
#include "esp_pm.h"

#include "heartbeat.h"

/* ESP32 logging tags */
static const char* TAG_HEARTBEAT = "heartbeat";

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (35) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (2048) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_DUTY2              (128) // Set duty to 50%. (2 ** 13) * 50% = 4096
#if CONFIG_PM_ENABLE
#define LEDC_CLK_SRC            LEDC_USE_RC_FAST_CLK // choose a clock source that can maintain during light sleep
#define LEDC_FREQUENCY          (400) // Frequency in Hertz. Set frequency at 400 Hz
#else
#define LEDC_CLK_SRC            LEDC_AUTO_CLK
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz
#endif

#define LEDC_TEST_DUTY         (4000)
#define LEDC_TEST_FADE_TIME    (48)
#define LEDC_TEST_FADE_TIME2   (96)


/* Warning:
 * For ESP32, ESP32S2, ESP32S3, ESP32C3, ESP32C2, ESP32C6, ESP32H2 (rev < 1.2), ESP32P4 (rev < 3.0) targets,
 * when LEDC_DUTY_RES selects the maximum duty resolution (i.e. value equal to SOC_LEDC_TIMER_BIT_WIDTH),
 * 100% duty cycle is not reachable (duty cannot be set to (2 ** SOC_LEDC_TIMER_BIT_WIDTH)).
 */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE VARIABLES (GLOBAL) ------------------------------------------- */



/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */



/* -------------------------------------------------------------------------- */
/* --- THREAD: INITIALIZE AND RUN HEARTBEAT --------------------------------- */
/* Reference:  */

static void example_ledc_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_CLK_SRC,
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0,
#if CONFIG_PM_ENABLE
        .sleep_mode     = LEDC_SLEEP_MODE_KEEP_ALIVE,
#endif
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));

    // Initialize fade service
    ledc_fade_func_install(0);

}

void thread_heartbeat( void )
{
    int       i; /* loop variable and temporary variable for return value */
    esp_err_t esp_err;


    #if CONFIG_PM_ENABLE
    esp_pm_config_t pm_config = {
        .max_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
        .min_freq_mhz = CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ,
    #if CONFIG_FREERTOS_USE_TICKLESS_IDLE
        .light_sleep_enable = true
    #endif
    };
    ESP_ERROR_CHECK(esp_pm_configure(&pm_config));
    #endif
    // Set the LEDC peripheral configuration
    example_ledc_init();

    while (1)
    {
        /* wait for next reporting interval */
        // Set duty to 0%
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0));
        // Update duty to apply the new value
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
        vTaskDelay( 900 / portTICK_PERIOD_MS );
            
        ledc_set_fade_with_time(LEDC_MODE,
                                LEDC_CHANNEL, LEDC_DUTY, LEDC_TEST_FADE_TIME);
        ledc_fade_start(LEDC_MODE,
                        LEDC_CHANNEL, LEDC_FADE_NO_WAIT);

        ledc_set_fade_with_time(LEDC_MODE,
                                LEDC_CHANNEL, 0, LEDC_TEST_FADE_TIME);
        ledc_fade_start(LEDC_MODE,
                        LEDC_CHANNEL, LEDC_FADE_NO_WAIT);

        vTaskDelay( 300 / portTICK_PERIOD_MS );

        ledc_set_fade_with_time(LEDC_MODE,
                                LEDC_CHANNEL, LEDC_DUTY2, LEDC_TEST_FADE_TIME2);
        ledc_fade_start(LEDC_MODE,
                        LEDC_CHANNEL, LEDC_FADE_NO_WAIT);

        ledc_set_fade_with_time(LEDC_MODE,
                                LEDC_CHANNEL, 0, LEDC_TEST_FADE_TIME2);
        ledc_fade_start(LEDC_MODE,
                        LEDC_CHANNEL, LEDC_FADE_NO_WAIT);


        #if (0)
        // Set duty to 50%
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY));
        // Update duty to apply the new value
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
        vTaskDelay( 100 / portTICK_PERIOD_MS );
        #endif


        // printf("heartbeat pulse\n");


    }

}

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */
/* Reference:  */

int launch_heartbeat( )
{
    int       i;
    pthread_t thrid_heartbeat;


    i = pthread_create( &thrid_heartbeat, NULL, ( void* ( * ) ( void* ) ) thread_heartbeat, NULL );
    if( i != 0 )
    {
        ESP_LOGE( TAG_HEARTBEAT, "ERROR: [main] impossible to create heartbeat thread\n" );
        // wait_on_error( LRHB_ERROR_UNKNOWN, __LINE__ );
    }

    return 0;
}