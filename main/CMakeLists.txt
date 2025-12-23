set(radio "src/heartbeat.c" "src/radio.c" "src/hello_world_main.c")

idf_component_register(
    SRCS "${radio}"
    PRIV_INCLUDE_DIRS "include"
    PRIV_REQUIRES "spi_flash" "esp_driver_tsens"
    INCLUDE_DIRS "."
    REQUIRES driver
)

