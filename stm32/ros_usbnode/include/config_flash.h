#ifndef CONFIG_FLASH_H
#define CONFIG_FLASH_H

#include <stdint.h>
#include <stdbool.h>

// Structure contenant tous les paramètres configurables
typedef struct {
    uint32_t valid_flag;
    float MAX_CHARGE_CURRENT;
    float LIMIT_VOLTAGE_150MA;
    float MAX_CHARGE_VOLTAGE;
    float BAT_CHARGE_CUTOFF_VOLTAGE;
    float CHARGE_END_LIMIT_CURRENT;
    float MIN_CHARGE_CURRENT;
    float MAX_MPS;
    float PWM_PER_MPS;
    float TICKS_PER_M;
    float WHEEL_BASE;
    uint8_t I_DONT_NEED_MY_FINGERS;

    uint32_t ONE_WHEEL_LIFT_EMERGENCY_MILLIS;
    uint32_t BOTH_WHEELS_LIFT_EMERGENCY_MILLIS;
    uint32_t TILT_EMERGENCY_MILLIS;
    uint32_t STOP_BUTTON_EMERGENCY_MILLIS;
    uint32_t PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS;
    uint8_t IMU_ONBOARD_INCLINATION_THRESHOLD;

    uint8_t EMERGENCY_DEBUG;
    uint8_t OPTION_BUMPER;
    uint8_t OPTION_ULTRASONIC;

    uint32_t checksum; // pour vérifier l'intégrité
} mower_config_t;

// Adresse Flash utilisée pour stocker les données (dernière page = 0x0803F800 pour 256 Ko flash)
#define MOWER_CONFIG_FLASH_ADDRESS ((uint32_t)0x0800C000)

// Fonctions de gestion de la configuration
bool config_flash_load(mower_config_t *config);
bool config_flash_save(const mower_config_t *config);
void config_flash_set_defaults(mower_config_t *config);
uint32_t config_flash_compute_checksum(const mower_config_t *config);

#endif // CONFIG_FLASH_H