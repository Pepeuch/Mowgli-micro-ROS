#include "config.h"
#include "config_flash.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// --- Variables dynamiques ---
float MAX_CHARGE_CURRENT;
float LIMIT_VOLTAGE_150MA;
float MAX_CHARGE_VOLTAGE;
float BAT_CHARGE_CUTOFF_VOLTAGE;
float CHARGE_END_LIMIT_CURRENT;
float MIN_CHARGE_CURRENT;
float WHEEL_BASE;
float MAX_MPS;
float PWM_PER_MPS;
float TICKS_PER_M;
uint8_t I_DONT_NEED_MY_FINGERS;
uint8_t OPTION_BUMPER;
uint8_t OPTION_ULTRASONIC;
uint8_t EMERGENCY_DEBUG;
uint8_t IMU_ONBOARD_INCLINATION_THRESHOLD;
uint32_t ONE_WHEEL_LIFT_EMERGENCY_MILLIS;
uint32_t BOTH_WHEELS_LIFT_EMERGENCY_MILLIS;
uint32_t TILT_EMERGENCY_MILLIS;
uint32_t STOP_BUTTON_EMERGENCY_MILLIS;
uint32_t PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS;

// --- Helper de validation ---
static float validate_float(float value, float min, float max, float def, const char* name) {
    if (value < min || value > max) {
        printf("[WARN] %s=%f is out of bounds [%f - %f], defaulting to %f\n", name, value, min, max, def);
        return def;
    }
    return value;
}

static uint32_t validate_u32(float value, float min, float max, uint32_t def, const char* name) {
    if (value < min || value > max) {
        printf("[WARN] %s=%lu is out of bounds [%lu - %lu], defaulting to %lu\n", name,
               (unsigned long)value, (unsigned long)min, (unsigned long)max, (unsigned long)def);
        return def;
    }
    return (uint32_t)value;
}

void config_init_defaults(void) {
    MAX_CHARGE_CURRENT = 1.0f;
    LIMIT_VOLTAGE_150MA = 28.00f;
    MAX_CHARGE_VOLTAGE = 29.00f;
    BAT_CHARGE_CUTOFF_VOLTAGE = 28.00f;
    CHARGE_END_LIMIT_CURRENT = 0.08f;
    MIN_CHARGE_CURRENT = 0.1f;
    MAX_MPS = 0.5f;
    PWM_PER_MPS = 300.0f;
    TICKS_PER_M = 300.0f;
    WHEEL_BASE = 0.325f;
    I_DONT_NEED_MY_FINGERS = 0;
    ONE_WHEEL_LIFT_EMERGENCY_MILLIS = 10000;
    BOTH_WHEELS_LIFT_EMERGENCY_MILLIS = 1000;
    TILT_EMERGENCY_MILLIS = 500;
    STOP_BUTTON_EMERGENCY_MILLIS = 100;
    PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS = 2000;
    IMU_ONBOARD_INCLINATION_THRESHOLD = 0x38;
    EMERGENCY_DEBUG = 0;
    OPTION_BUMPER = 0;
    OPTION_ULTRASONIC = 0;
}

void config_set_value(const char* key, const char* value) {
    float f = atof(value);

    if (strcmp(key, "OM_MAX_CHARGE_CURRENT") == 0)
        MAX_CHARGE_CURRENT = validate_float(f, MIN_SAFE_MAX_CHARGE_CURRENT, MAX_SAFE_MAX_CHARGE_CURRENT, 1.0f, key);
    else if (strcmp(key, "OM_LIMIT_VOLTAGE_150MA") == 0)
        LIMIT_VOLTAGE_150MA = validate_float(f, MIN_SAFE_LIMIT_VOLTAGE_150MA, MAX_SAFE_LIMIT_VOLTAGE_150MA, 28.0f, key);
    else if (strcmp(key, "OM_MAX_CHARGE_VOLTAGE") == 0)
        MAX_CHARGE_VOLTAGE = validate_float(f, MIN_SAFE_MAX_CHARGE_VOLTAGE, MAX_SAFE_MAX_CHARGE_VOLTAGE, 28.5f, key);
    else if (strcmp(key, "OM_BAT_CUTOFF") == 0)
        BAT_CHARGE_CUTOFF_VOLTAGE = validate_float(f, MIN_SAFE_BAT_CHARGE_CUTOFF_VOLTAGE, MAX_SAFE_BAT_CHARGE_CUTOFF_VOLTAGE, 28.5f, key);
    else if (strcmp(key, "OM_CHARGE_END_CURRENT") == 0)
        MIN_CHARGE_CURRENT = validate_float(f, MIN_SAFE_MIN_CHARGE_CURRENT, MAX_SAFE_MIN_CHARGE_CURRENT, 0.1f, key);
    else if (strcmp(key, "OM_MIN_CHARGE_CURRENT") == 0)
        CHARGE_END_LIMIT_CURRENT = validate_float(f, MIN_SAFE_CHARGE_END_LIMIT_CURRENT, MAX_SAFE_CHARGE_END_LIMIT_CURRENT, 0.1f, key);
    else if (strcmp(key, "OM_ONE_WHEEL_TIMEOUT") == 0)
        ONE_WHEEL_LIFT_EMERGENCY_MILLIS = validate_u32(f, MIN_SAFE_ONE_WHEEL_LIFT_EMERGENCY_MILLIS, MAX_SAFE_ONE_WHEEL_LIFT_EMERGENCY_MILLIS, 10000, key);
    else if (strcmp(key, "OM_BOTH_WHEELS_TIMEOUT") == 0)
        BOTH_WHEELS_LIFT_EMERGENCY_MILLIS = validate_u32(f, MIN_SAFE_BOTH_WHEELS_LIFT_EMERGENCY_MILLIS, MAX_SAFE_BOTH_WHEELS_LIFT_EMERGENCY_MILLIS, 1000, key);
    else if (strcmp(key, "OM_TILT_TIMEOUT") == 0)
        TILT_EMERGENCY_MILLIS = validate_u32(f, MIN_SAFE_TILT_EMERGENCY_MILLIS, MAX_SAFE_TILT_EMERGENCY_MILLIS, 500, key);
    else if (strcmp(key, "OM_STOP_BUTTON_TIMEOUT") == 0)
        STOP_BUTTON_EMERGENCY_MILLIS = validate_u32(f, MIN_SAFE_STOP_BUTTON_EMERGENCY_MILLIS, MAX_SAFE_STOP_BUTTON_EMERGENCY_MILLIS, 100, key);
    else if (strcmp(key, "OM_PLAY_BUTTON_CLEAR_TIMEOUT") == 0)
        PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS = validate_u32(f, MIN_SAFE_PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS, MAX_SAFE_PLAY_BUTTON_CLEAR_EMERGENCY_MILLIS, 2000, key);
    else if (strcmp(key, "OM_IMU_THRESHOLD") == 0)
        IMU_ONBOARD_INCLINATION_THRESHOLD = (uint8_t)f;
    else if (strcmp(key, "OM_MAX_MPS") == 0)
        MAX_MPS = validate_float(f, MIN_SAFE_MAX_MPS, MAX_SAFE_MAX_MPS, 0.5f, key);
    else if (strcmp(key, "OM_PWM_PER_MPS") == 0)
        PWM_PER_MPS = validate_float(f, MIN_SAFE_PWM_PER_MPS, MAX_SAFE_PWM_PER_MPS, 300.0f, key);
    else if (strcmp(key, "OM_TICKS_PER_M") == 0)
        TICKS_PER_M = validate_float(f, MIN_SAFE_TICKS_PER_M, MAX_SAFE_TICKS_PER_M, 300.0f, key);
    else if (strcmp(key, "OM_WHEEL_BASE") == 0)
        WHEEL_BASE = validate_float(f, MIN_SAFE_WHELL_BASE, MAX_SAFE_WHELL_BASE, 0.325f, key);
    else if (strcmp(key, "OM_DISABLE_EMERGENCY") == 0)
        I_DONT_NEED_MY_FINGERS = (f != 0.0f) ? 1 : 0;
    else if (strcmp(key, "OM_OPTION_BUMPER") == 0)
        OPTION_BUMPER = (uint8_t)f;
    else if (strcmp(key, "OM_OPTION_ULTRASONIC") == 0)
        OPTION_ULTRASONIC = (uint8_t)f;
    else if (strcmp(key, "OM_EMERGENCY_DEBUG") == 0)
        EMERGENCY_DEBUG = (uint8_t)f;
    else
        printf("[WARN] Unknown config key: %s\n", key);
}
