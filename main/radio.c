/*
 * Reference: pkt_fwd.c
 *
 */

/* -------------------------------------------------------------------------- */
/* --- DEPENDENCIES --------------------------------------------------------- */

#include <pthread.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_log.h>
#include <esp_pthread.h>

/* Radio helpers and HAL */
#include "radio.h"

/* -------------------------------------------------------------------------- */
/* --- PRIVATE TYPES -------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- PRIVATE VARIABLES (GLOBAL) ------------------------------------------- */


static temperature_sensor_handle_t temp_sensor = NULL;

/* -------------------------------------------------------------------------- */
/* --- PRIVATE FUNCTIONS DEFINITION ----------------------------------------- */



/* -------------------------------------------------------------------------- */
/* --- THREAD: INITIALIZE AND RUN RADIO ------------------------------------- */
/* Reference: thread_pktfwd */

void thread_radio( void )
{
    int       i; /* loop variable and temporary variable for return value */
    esp_err_t esp_err;

    /* Configure radio parameters */
    i = parse_radio_configuration( );
    if( i != 0 )
    {
        ESP_LOGE( TAG_PKT_FWD, "ERROR: failed to parse radio configuration\n ");
        wait_on_error( LRHB_ERROR_UNKNOWN, __LINE__ );
    }

    /* next: pkt_fwd.c line 1698*/ 
}

/* -------------------------------------------------------------------------- */
/* --- PUBLIC FUNCTIONS DEFINITION ------------------------------------------ */
/* Reference: launch_pkt_fwd */

int launch_radio( temperature_sensor_handle_t temperature_sensor )
{
    int       i;
    pthread_t thrid_radio;

    temp_sensor = temperature_sensor;

    i = pthread_create( &thrid_radio, NULL, ( void* ( * ) ( void* ) ) thread_radio, NULL );
    if( i != 0 )
    {
        ESP_LOGE( TAG_PKT_FWD, "ERROR: [main] impossible to create packet forwarder thread\n" );
        wait_on_error( LRHB_ERROR_UNKNOWN, __LINE__ );
    }

    return 0;
}