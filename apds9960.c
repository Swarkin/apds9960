#include "apds9960.h"

#include "esp_check.h"

#define APDS9960_I2C_ADDR 0x39

static const char *TAG = "APDS9960";

typedef enum {
	APDS9960_REG_ENABLE = 0x80,
	APDS9960_REG_ATIME  = 0x81, // ALS integration time
	APDS9960_REG_WTIME  = 0x83, // Wait time
	APDS9960_REG_PILT   = 0x89, // Proximity interrupt low threshold
	APDS9960_REG_PIHT   = 0x8B, // Proximity interrupt high threshold
	APDS9960_REG_STATUS = 0x93, // Device status
	APDS9960_REG_CDATAL = 0x94, // Clear data low byte
	APDS9960_REG_CDATAH = 0x95, // Clear data high byte
	APDS9960_REG_PDATA  = 0x9C, // Proximity data
	APDS9960_REG_PICLEAR = 0xE5, // Proximity interrupt clear
} apds9960_reg_t;


esp_err_t write_register(i2c_master_dev_handle_t dev_handle, apds9960_reg_t reg, uint8_t val) {
	uint8_t buf[2] = { reg, val };
	return i2c_master_transmit(dev_handle, buf, sizeof(buf), 1000);
}

esp_err_t read_register(i2c_master_dev_handle_t dev_handle, apds9960_reg_t reg, uint8_t *rx_buf) {
	uint8_t tx_buf[1] = { reg };
	return i2c_master_transmit_receive(dev_handle, tx_buf, sizeof(tx_buf), rx_buf, 1, 1000);
}


esp_err_t apds9960_i2c_device_init(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t *dev_handle) {
	esp_err_t ret = ESP_OK;

	i2c_device_config_t dev_cfg = {
		.dev_addr_length = I2C_ADDR_BIT_LEN_7,
		.device_address = APDS9960_I2C_ADDR,
		.scl_speed_hz = 100000,
	};

	ESP_RETURN_ON_ERROR(
		i2c_master_bus_add_device(bus_handle, &dev_cfg, dev_handle),
		TAG, "Failed to add I2C device"
	);

	return ret;
}

esp_err_t apds9960_set_enable(i2c_master_dev_handle_t dev_handle, uint8_t enable) {
	esp_err_t ret = ESP_OK;

	ESP_RETURN_ON_ERROR(
		write_register(dev_handle, APDS9960_REG_ENABLE, enable),
		TAG, "Failed to set ENABLE register"
	);

	return ret;
}

esp_err_t apds9960_get_proximity(i2c_master_dev_handle_t dev_handle, uint8_t *proximity) {
	esp_err_t ret = ESP_OK;

	uint8_t tx_buf[1] = { APDS9960_REG_PDATA };

	ESP_RETURN_ON_ERROR(
		i2c_master_transmit_receive(dev_handle, tx_buf, sizeof(tx_buf), proximity, 1, 1000),
		TAG, "Failed to read proximity data"
	);

	return ret;
}

esp_err_t apds9960_set_wait_time(i2c_master_dev_handle_t dev_handle, uint8_t wait_time) {
	esp_err_t ret = ESP_OK;

	ESP_RETURN_ON_ERROR(
		write_register(dev_handle, APDS9960_REG_WTIME, wait_time),
		TAG, "Failed to set wait time"
	);

	return ret;
}

esp_err_t apds9960_set_proximity_threshold(i2c_master_dev_handle_t dev_handle, uint8_t low_int, uint8_t high_int) {
	esp_err_t ret = ESP_OK;

	ESP_RETURN_ON_ERROR(
		write_register(dev_handle, APDS9960_REG_PILT, low_int),
		TAG, "Failed to set low threshold"
	);
	ESP_RETURN_ON_ERROR(
		write_register(dev_handle, APDS9960_REG_PIHT, high_int),
		TAG, "Failed to set high threshold"
	);

	return ret;
}

esp_err_t apds9960_reset_proximity_interrupt(i2c_master_dev_handle_t dev_handle) {
	esp_err_t ret = ESP_OK;

	ESP_RETURN_ON_ERROR(
		write_register(dev_handle, APDS9960_REG_PICLEAR, 0),
		TAG, "Failed to clear proximity interrupt"
	);

	return ret;
}

esp_err_t apds9960_get_status(i2c_master_dev_handle_t dev_handle, apds9960_status_t status_bit, bool *state) {
	esp_err_t ret = ESP_OK;

	uint8_t status;
	ESP_RETURN_ON_ERROR(
		read_register(dev_handle, APDS9960_REG_STATUS, &status),
		TAG, "Failed to read status register"
	);

	*state = (status & status_bit) != 0;

	return ret;
}

esp_err_t apds9960_get_ambient_light_clear(i2c_master_dev_handle_t dev_handle, uint16_t *clear_data) {
	esp_err_t ret = ESP_OK;

	uint8_t clear_data_low, clear_data_high;

	ESP_RETURN_ON_ERROR(
		read_register(dev_handle, APDS9960_REG_CDATAL, &clear_data_low),
		TAG, "Failed to read clear data low byte"
	);
	ESP_RETURN_ON_ERROR(
		read_register(dev_handle, APDS9960_REG_CDATAH, &clear_data_high),
		TAG, "Failed to read clear data high byte"
	);

	*clear_data = ((uint16_t)clear_data_high << 8) | clear_data_low;

	return ret;
}

esp_err_t apds9960_set_ambient_light_integration_time(i2c_master_dev_handle_t dev_handle, uint8_t atime) {
	esp_err_t ret = ESP_OK;

	ESP_RETURN_ON_ERROR(
		write_register(dev_handle, APDS9960_REG_ATIME, atime),
		TAG, "Failed to set ambient light integration time"
	);

	return ret;
}
