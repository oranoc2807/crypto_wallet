#include <stdio.h>
#include <stdlib.h>  // <-- Needed for malloc/free
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "falcon/falcon.h"
#include "key_storage.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_random.h"

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    printf("Starting Falcon-512 key generation...\n");

    // Falcon-512 parameters
    const unsigned logn = 9;
    const size_t privkey_size = FALCON_PRIVKEY_SIZE(logn);
    const size_t pubkey_size  = FALCON_PUBKEY_SIZE(logn);
    const size_t tmp_size     = FALCON_TMPSIZE_KEYGEN(logn);

    // Allocate buffers on the heap
    uint8_t *private_key = malloc(privkey_size);
    uint8_t *public_key  = malloc(pubkey_size);
    uint8_t *tmp         = malloc(tmp_size);

    if (!private_key || !public_key || !tmp) {
        printf("Memory allocation failed!\n");
        goto cleanup;
    }

    // Securely seed the SHAKE256 PRNG using ESP32's TRNG
    uint8_t seed[48];
    for (int i = 0; i < sizeof(seed); i += 4) {
        uint32_t r = esp_random();
        memcpy(&seed[i], &r, (sizeof(seed) - i < 4) ? (sizeof(seed) - i) : 4);
    }

    shake256_context rng;
    shake256_init_prng_from_seed(&rng, seed, sizeof(seed));
    memset(seed, 0, sizeof(seed));  // Wipe seed

    // Generate key pair
    int result = falcon_keygen_make(&rng, logn,
                                    private_key, privkey_size,
                                    public_key, pubkey_size,
                                    tmp, tmp_size);

    if (result == 0) {
        printf("Key generation successful. Storing key...\n");
        int store_result = store_private_key(private_key, privkey_size);
        if (store_result == 0) {
            printf("Private key stored successfully.\n");
        } else {
            printf("Error storing private key: %d\n", store_result);
        }
    } else {
        printf("Key generation failed.\n");
    }

cleanup:
    // Free memory
    if (private_key) free(private_key);
    if (public_key)  free(public_key);
    if (tmp)         free(tmp);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
