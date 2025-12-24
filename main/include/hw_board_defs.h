/*

Description:
    Radio Hardware specific definitions
*/

#ifndef _HW_BOARD_DEFS_H
#define _HW_BOARD_DEFS_H


/* -------------------------------------------------------------------------- */
/* --- DEPENDENCIES --------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* --- PUBLIC MACROS -------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* --- PUBLIC CONSTANTS ----------------------------------------------------- */

/* Board specific constants */

#if defined( CONFIG_HELTEC_WIFI_LORA_32_V3 )
#define USER_LED_GPIO 35 /* User LED */
#define VEXT_CTRL_GPIO 36 /* Vext_Ctrl */
#elif defined( CONFIG_SEEED_XIAO_ESP32S3_DEVKIT )
#define USER_LED_GPIO 48            /* User LED from Xiao Wio-sx1262 board */
#else
#error "No supported board selected"
#endif


/* -------------------------------------------------------------------------- */
/* --- PUBLIC TYPES --------------------------------------------------------- */

#endif  // _HW_BOARD_DEFS_H

/* --- EOF ------------------------------------------------------------------ */