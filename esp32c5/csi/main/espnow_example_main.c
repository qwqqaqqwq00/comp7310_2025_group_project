#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_now.h"

#define TAG "ESP_NOW"

uint8_t self_mac[6];

typedef struct {
    char message[32];
} esp_now_message_t;

void esp_now_send_cb(const uint8_t *mac_addr, esp_now_send_status_t status) {
    ESP_LOGI(TAG, "ESP-NOW 发送状态: %s", (status == ESP_NOW_SEND_SUCCESS) ? "成功" : "失败");
}

void esp_now_recv_cb(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    if (len < sizeof(esp_now_message_t)) {
        ESP_LOGE(TAG, "接收数据太短，长度: %d", len);
        return;
    }
    esp_now_message_t msg;
    memcpy(&msg, data, sizeof(esp_now_message_t));
    ESP_LOGI(TAG, "收到ESP-NOW消息: %s, 来源MAC: %02X:%02X:%02X:%02X:%02X:%02X",
             msg.message, info->src_addr[0], info->src_addr[1], info->src_addr[2],
             info->src_addr[3], info->src_addr[4], info->src_addr[5]);
}

void esp_event_cb(void *handler_arg, esp_event_base_t base, int32_t id, void *event_data){
    if (base == WIFI_EVENT && id == WIFI_EVENT_STA_START)
    {
        ESP_ERROR_CHECK(esp_wifi_connect());
    } else if (base == IP_EVENT && id == IP_EVENT_ETH_GOT_IP) {
        ip_event_got_ip_t *get_info_struct = (ip_event_got_ip_t *)event_data;
        esp_ip4_addr_t ip = get_info_struct->ip_info.ip;
        ESP_LOGI(TAG, "IP 地址: " IPSTR "", IP2STR(&ip));
    }
}

void wifi_init() {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_sta();

    esp_event_handler_instance_t instance_wifi, instance_ip;
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, esp_event_cb, NULL, &instance_wifi);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_ETH_GOT_IP, esp_event_cb, NULL, &instance_ip);

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "1703_5G",         
            .password = "92178750",
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .scan_method = WIFI_FAST_SCAN,
            //
        
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Wi-Fi 初始化完成.");
}

void esp_now_init_custom() {
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(esp_now_send_cb));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_recv_cb));
    
    ESP_ERROR_CHECK(esp_wifi_get_mac(WIFI_IF_STA, self_mac));
    ESP_LOGI(TAG, "本机 MAC: %02X:%02X:%02X:%02X:%02X:%02X", self_mac[0], self_mac[1], self_mac[2], self_mac[3], self_mac[4], self_mac[5]);
    
    esp_now_set_wake_window(65535); // 设置 ESP-NOW 的唤醒窗口
    uint8_t primary_channel;
    wifi_second_chan_t second_channel;
    ESP_ERROR_CHECK(esp_wifi_get_channel(&primary_channel, &second_channel));
    ESP_LOGI(TAG, "当前信道: %d", primary_channel);
    
    esp_now_peer_info_t peer = {
        .channel = primary_channel,
        .ifidx = ESP_IF_WIFI_STA,
        .encrypt = false,
    };
    memcpy(peer.peer_addr, self_mac, ESP_NOW_ETH_ALEN); // 添加自己的 MAC 地址为 peer
    ESP_ERROR_CHECK(esp_now_add_peer(&peer));

    ESP_LOGI(TAG, "ESP-NOW 初始化完成.");
}

void send_esp_now_message() {
    esp_now_message_t msg;
    strcpy(msg.message, "Hello ESP-NOW!");

    ESP_LOGI(TAG, "发送 ESP-NOW 消息到 %02X:%02X:%02X:%02X:%02X:%02X", 
             self_mac[0], self_mac[1], self_mac[2], self_mac[3], self_mac[4], self_mac[5]);
    esp_err_t ret = esp_now_send(self_mac, (uint8_t *)&msg, sizeof(msg));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "发送失败，错误码: %d", ret);
    }
}

void app_main() {
    wifi_init();
    // esp_now_init_custom();

    // vTaskDelay(pdMS_TO_TICKS(2000));

    while (1) {
    //     send_esp_now_message();
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}