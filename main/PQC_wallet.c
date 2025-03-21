#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "falcon/falcon.h"

void app_main(void) {
    printf("Starting Falcon-512 key generation...\n");

    unsigned logn = 9;

    size_t privkey_size = FALCON_PRIVKEY_SIZE(logn);
    size_t pubkey_size = FALCON_PUBKEY_SIZE(logn);
    size_t tmp_size = FALCON_TMPSIZE_KEYGEN(logn);

    uint8_t private_key[privkey_size];
    uint8_t public_key[pubkey_size];
    uint8_t tmp[tmp_size];

    shake256_context rng;
    int rng_status = shake256_init_prng_from_system(&rng);
    if (rng_status != 0) {
        printf("Error: Failed to initialize SHAKE256 RNG.\n");
        return;
    }
    
    int result = falcon_keygen_make(&rng, logn, private_key, privkey_size, public_key, pubkey_size, tmp, tmp_size);
    
    if (result == 0) {
        printf("Falcon-512 key generation successful!\n");
    } else {
        printf("Falcon-512 key generation failed. Error code: %d\n", result);
    }

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
