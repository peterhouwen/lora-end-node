set(radio "radio.c" "hello_world_main.c")

idf_component_register(SRCS "${radio}"
                       PRIV_REQUIRES "spi_flash" "esp_driver_tsens"
                       INCLUDE_DIRS "")

