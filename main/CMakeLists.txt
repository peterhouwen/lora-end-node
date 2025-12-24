set(radio "src/heartbeat.c" "src/radio.c" "src/hello_world_main.c")
set(display "src/lvgl_demo_ui.c")

idf_component_register(
    SRCS "${radio}" "${display}"
    PRIV_INCLUDE_DIRS "include"
    PRIV_REQUIRES "spi_flash" "esp_driver_tsens"
    INCLUDE_DIRS "."
    REQUIRES driver
)

