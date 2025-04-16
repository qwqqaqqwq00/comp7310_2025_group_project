idf_component_register(SRCS app_main.cc model_data.cc NeuralNetwork.cpp
                       PRIV_REQUIRES spi_flash esp-tflite-micro
                       INCLUDE_DIRS "."
                       REQUIRES esp_wifi esp_netif nvs_flash esp_timer mqtt)
