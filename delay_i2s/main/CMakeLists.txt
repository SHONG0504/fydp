idf_component_register(SRCS "main.c" "../src/esp_it.c"
                       PRIV_REQUIRES esp_driver_gpio esp_driver_i2s
                       INCLUDE_DIRS "." "../inc")
