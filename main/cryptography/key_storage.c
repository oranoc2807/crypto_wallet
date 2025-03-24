#include "key_storage.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_log.h"

#define STORAGE_NAMESPACE "wallet"
#define PRIVATE_KEY_KEY   "priv_key"

int store_private_key(const uint8_t *key, size_t key_len) {
    nvs_handle_t handle;
    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READWRITE, &handle);
    if (err != ESP_OK) return err;

    err = nvs_set_blob(handle, PRIVATE_KEY_KEY, key, key_len);
    if (err == ESP_OK) err = nvs_commit(handle);
    
    nvs_close(handle);
    return err;
}

int load_private_key(uint8_t *key_out, size_t key_len) {
    nvs_handle_t handle;
    size_t required_size = 0;

    esp_err_t err = nvs_open(STORAGE_NAMESPACE, NVS_READONLY, &handle);
    if (err != ESP_OK) return err;

    err = nvs_get_blob(handle, PRIVATE_KEY_KEY, NULL, &required_size);
    if (err != ESP_OK || required_size > key_len) {
        nvs_close(handle);
        return ESP_FAIL;
    }

    err = nvs_get_blob(handle, PRIVATE_KEY_KEY, key_out, &required_size);
    nvs_close(handle);
    return err;
}
