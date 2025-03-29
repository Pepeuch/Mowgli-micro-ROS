#include "config_flash.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#define FLASH_CONFIG_ADDRESS ((uint32_t)0x0800C000) // Dernière page 2KB
#define FLASH_PAGE_SIZE     ((uint32_t)0x80)      // 128 octets pour F401VC
#define CONFIG_VALID_FLAG   0xDEADBEEF              // Indicateur de validité

// Structure sauvegardée
mower_config_t mower_config_flash;

// Efface la page flash
static HAL_StatusTypeDef flash_erase_page(void) {
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase;
    erase.TypeErase   = FLASH_TYPEERASE_SECTORS;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;
    erase.Sector       = FLASH_SECTOR_3;
    erase.NbSectors    = 1;
    erase.Banks         = FLASH_BANK_1;  // <-- nécessaire pour certains STM32F4

    uint32_t error = 0;
    HAL_StatusTypeDef status = HAL_FLASHEx_Erase(&erase, &error);

    HAL_FLASH_Lock();
    return status;
}

// Sauvegarde en flash
bool config_flash_save(const mower_config_t *config) {
    if (flash_erase_page() != HAL_OK)
        return false;

    HAL_FLASH_Unlock();

    const uint32_t *src = (const uint32_t *)config;
    uint32_t address = FLASH_CONFIG_ADDRESS;
    size_t word_count = sizeof(mower_config_t) / sizeof(uint32_t);

    for (size_t i = 0; i < word_count; ++i) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, address, src[i]) != HAL_OK) {
            HAL_FLASH_Lock();
            return false;
        }
        address += 4;
    }

    HAL_FLASH_Lock();
    return true;
}

// Lecture de la config
bool config_flash_load(mower_config_t* config) {
    if (!config) return false;

    memcpy(config, (void*)FLASH_CONFIG_ADDRESS, sizeof(mower_config_t));
    if (config->valid_flag != CONFIG_VALID_FLAG) {
        return false;
    }

    return true;
}

// Initialise les valeurs par défaut
void config_flash_set_defaults(mower_config_t* config) {
    config->valid_flag = CONFIG_VALID_FLAG;
    config->MAX_CHARGE_CURRENT = 1.0f;
    config->LIMIT_VOLTAGE_150MA = 28.00f;
    config->MAX_CHARGE_VOLTAGE = 29.00f;
    config->BAT_CHARGE_CUTOFF_VOLTAGE = 28.00f;
    config->CHARGE_END_LIMIT_CURRENT = 0.08f;
    config->MIN_CHARGE_CURRENT = 0.1f;
    config->MAX_MPS = 0.5f;
    config->PWM_PER_MPS = 300.0f;
    config->TICKS_PER_M = 300.0f;
    config->WHEEL_BASE = 0.325f;
    config->I_DONT_NEED_MY_FINGERS = 0;
    config->ONE_WHEEL_LIFT_EMERGENCY_MILLIS = 10000;
    config->BOTH_WHEELS_LIFT_EMERGENCY_MILLIS = 1000;
    config->TILT_EMERGENCY_MILLIS = 500;
    config->STOP_BUTTON_EMERGENCY_MILLIS = 100;
    config->PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS = 2000;
    config->IMU_ONBOARD_INCLINATION_THRESHOLD = 0x38;
    config->EMERGENCY_DEBUG = 0;
    config->OPTION_BUMPER = 0;
    config->OPTION_ULTRASONIC = 0;
}