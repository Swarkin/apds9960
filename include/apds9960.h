#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "driver/i2c_master.h"
#include "esp_err.h"

typedef enum {
	APDS9960_ENABLE_PON  = 0b00000001, // Powern on
	APDS9960_ENABLE_AEN  = 0b00000010, // ALS enable
	APDS9960_ENABLE_PEN  = 0b00000100, // Proximity detect enable
	APDS9960_ENABLE_WEN  = 0b00001000, // Wait enable
	APDS9960_ENABLE_AIEN = 0b00010000, // ALS interrupt enable
	APDS9960_ENABLE_PIEN = 0b00100000, // Proximity interrupt enable
	APDS9960_ENABLE_GEN  = 0b01000000, // Gesture enable
	// APDS9960_ENABLE_RESERVED = 0b10000000, // Reserved
} apds9960_enable_t;

typedef enum {
	APDS9960_STATUS_AVALID = 0b00000001, // Clear data valid
	APDS9960_STATUS_PVALID = 0b00000010, // Proximity data valid
	APDS9960_STATUS_GINT   = 0b00000100, // Gesture interrupt
	// APDS9960_STATUS_RESERVED = 0b00001000, // Reserved
	APDS9960_STATUS_AINT   = 0b00010000, // ALS interrupt
	APDS9960_STATUS_PINT   = 0b00100000, // Proximity interrupt
	APDS9960_STATUS_PGSAT  = 0b01000000, // Proximity saturation
	APDS9960_STATUS_CPSAT  = 0b10000000, // Clear data saturation
} apds9960_status_t;

esp_err_t apds9960_i2c_device_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *dev_handle);
esp_err_t apds9960_check_id(i2c_master_dev_handle_t dev_handle, uint8_t *id);

esp_err_t apds9960_set_enable(i2c_master_dev_handle_t dev_handle, uint8_t enable);
esp_err_t apds9960_set_wait_time(i2c_master_dev_handle_t dev_handle, uint8_t wait_time);
esp_err_t apds9960_set_ambient_light_integration_time(i2c_master_dev_handle_t dev_handle, uint8_t atime);
esp_err_t apds9960_set_proximity_threshold(i2c_master_dev_handle_t dev_handle, uint8_t low_int, uint8_t high_int);

esp_err_t apds9960_get_status(i2c_master_dev_handle_t dev_handle, apds9960_status_t status_bit, bool *state);
esp_err_t apds9960_get_proximity(i2c_master_dev_handle_t dev_handle, uint8_t *proximity);
esp_err_t apds9960_get_ambient_light_clear(i2c_master_dev_handle_t dev_handle, uint16_t *clear_data);

esp_err_t apds9960_reset_proximity_interrupt(i2c_master_dev_handle_t dev_handle);
