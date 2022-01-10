//based heavily on this blog post: https://www.artfulbytes.com/vl53l0x-post
#include "config.h"
#include "warp.h"
#include <stdlib.h>
#include "devVL53.h"
#include "fsl_port_hal.h"
#include "fsl_i2c_master_driver.h"

volatile WarpI2CDeviceState deviceVL53State;
extern volatile uint32_t gWarpI2cBaudRateKbps;
extern volatile uint32_t gWarpI2cTimeoutMilliseconds;
extern volatile uint32_t gWarpSupplySettlingDelayMilliseconds;

uint8_t stop_variable = 0;

bool vl53_init()
{
	deviceVL53State.i2cAddress = 0x29; //TODO: check this is correct

	bool success = false;

	/* Set 2v8 mode */
	uint8_t vhv_config_scl_sda = 0;
	if (!i2c_read_addr8_data8(VL53_REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, &vhv_config_scl_sda))
		return false;

	vhv_config_scl_sda |= 0x01;
	if (!i2c_write_addr8_data8(VL53_REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV, vhv_config_scl_sda))
		return false;

	/* Set I2C standard mode */
	success = i2c_write_addr8_data8(0x88, 0x00);

	success &= i2c_write_addr8_data8(0x80, 0x01);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x00, 0x00);
	success &= i2c_read_addr8_data8(0x91, &stop_variable);
	warpPrint("read stop variable %d", stop_variable);
	success &= i2c_write_addr8_data8(0x00, 0x01);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x80, 0x00);

	if (!success)
		return false;

	if (!vl53_load_default_tuning_settings())
		return false;

	if (!vl53_configure_interrupt())
		return false;

	if (!vl53_set_sequence_steps_enabled(RANGE_SEQUENCE_STEP_DSS +
										 RANGE_SEQUENCE_STEP_PRE_RANGE +
										 RANGE_SEQUENCE_STEP_FINAL_RANGE))
		return false;

	if (!vl53_perform_ref_calibration())
		return false;
	
	return true;
}

bool i2c_read_addr8_data(uint8_t addr, uint8_t *data, uint8_t size)
{

	uint8_t cmdBuf[1] = {addr};
	i2c_status_t status;

	i2c_device_t slave =
		{
			.address = deviceVL53State.i2cAddress,
			.baudRate_kbps = gWarpI2cBaudRateKbps};

	//warpScaleSupplyVoltage(deviceVL53State.operatingVoltageMillivolts);
	cmdBuf[0] = addr;
	warpEnableI2Cpins();

	status = I2C_DRV_MasterReceiveDataBlocking(
		0 /* I2C peripheral instance */,
		&slave,
		cmdBuf,
		1,
		data,
		size,
		gWarpI2cTimeoutMilliseconds);

	if (status != kStatus_I2C_Success)
		return false;

	return true;
}

bool i2c_read_addr8_data8(uint8_t addr, uint8_t *data)
{
	return i2c_read_addr8_data(addr, data, 1);
}

bool i2c_write_addr8_data8(uint8_t addr, uint8_t data)
{
	uint8_t payloadByte[1], commandByte[1];
	i2c_status_t status;

	i2c_device_t slave =
		{
			.address = deviceVL53State.i2cAddress,
			.baudRate_kbps = gWarpI2cBaudRateKbps};

//	warpScaleSupplyVoltage(deviceVL53State.operatingVoltageMillivolts);
	commandByte[0] = addr;
	payloadByte[0] = data;
	warpEnableI2Cpins();

	status = I2C_DRV_MasterSendDataBlocking(
		0 /* I2C instance */,
		&slave,
		commandByte,
		1,
		payloadByte,
		1,
		gWarpI2cTimeoutMilliseconds);
	if (status != kStatus_I2C_Success)
	{
		return false;
	}

	return true;
}

bool vl53_load_default_tuning_settings()
{
	bool success = i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x00, 0x00);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x09, 0x00);
	success &= i2c_write_addr8_data8(0x10, 0x00);
	success &= i2c_write_addr8_data8(0x11, 0x00);
	success &= i2c_write_addr8_data8(0x24, 0x01);
	success &= i2c_write_addr8_data8(0x25, 0xFF);
	success &= i2c_write_addr8_data8(0x75, 0x00);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x4E, 0x2C);
	success &= i2c_write_addr8_data8(0x48, 0x00);
	success &= i2c_write_addr8_data8(0x30, 0x20);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x30, 0x09);
	success &= i2c_write_addr8_data8(0x54, 0x00);
	success &= i2c_write_addr8_data8(0x31, 0x04);
	success &= i2c_write_addr8_data8(0x32, 0x03);
	success &= i2c_write_addr8_data8(0x40, 0x83);
	success &= i2c_write_addr8_data8(0x46, 0x25);
	success &= i2c_write_addr8_data8(0x60, 0x00);
	success &= i2c_write_addr8_data8(0x27, 0x00);
	success &= i2c_write_addr8_data8(0x50, 0x06);
	success &= i2c_write_addr8_data8(0x51, 0x00);
	success &= i2c_write_addr8_data8(0x52, 0x96);
	success &= i2c_write_addr8_data8(0x56, 0x08);
	success &= i2c_write_addr8_data8(0x57, 0x30);
	success &= i2c_write_addr8_data8(0x61, 0x00);
	success &= i2c_write_addr8_data8(0x62, 0x00);
	success &= i2c_write_addr8_data8(0x64, 0x00);
	success &= i2c_write_addr8_data8(0x65, 0x00);
	success &= i2c_write_addr8_data8(0x66, 0xA0);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x22, 0x32);
	success &= i2c_write_addr8_data8(0x47, 0x14);
	success &= i2c_write_addr8_data8(0x49, 0xFF);
	success &= i2c_write_addr8_data8(0x4A, 0x00);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x7A, 0x0A);
	success &= i2c_write_addr8_data8(0x7B, 0x00);
	success &= i2c_write_addr8_data8(0x78, 0x21);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x23, 0x34);
	success &= i2c_write_addr8_data8(0x42, 0x00);
	success &= i2c_write_addr8_data8(0x44, 0xFF);
	success &= i2c_write_addr8_data8(0x45, 0x26);
	success &= i2c_write_addr8_data8(0x46, 0x05);
	success &= i2c_write_addr8_data8(0x40, 0x40);
	success &= i2c_write_addr8_data8(0x0E, 0x06);
	success &= i2c_write_addr8_data8(0x20, 0x1A);
	success &= i2c_write_addr8_data8(0x43, 0x40);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x34, 0x03);
	success &= i2c_write_addr8_data8(0x35, 0x44);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x31, 0x04);
	success &= i2c_write_addr8_data8(0x4B, 0x09);
	success &= i2c_write_addr8_data8(0x4C, 0x05);
	success &= i2c_write_addr8_data8(0x4D, 0x04);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x44, 0x00);
	success &= i2c_write_addr8_data8(0x45, 0x20);
	success &= i2c_write_addr8_data8(0x47, 0x08);
	success &= i2c_write_addr8_data8(0x48, 0x28);
	success &= i2c_write_addr8_data8(0x67, 0x00);
	success &= i2c_write_addr8_data8(0x70, 0x04);
	success &= i2c_write_addr8_data8(0x71, 0x01);
	success &= i2c_write_addr8_data8(0x72, 0xFE);
	success &= i2c_write_addr8_data8(0x76, 0x00);
	success &= i2c_write_addr8_data8(0x77, 0x00);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x0D, 0x01);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x80, 0x01);
	success &= i2c_write_addr8_data8(0x01, 0xF8);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x8E, 0x01);
	success &= i2c_write_addr8_data8(0x00, 0x01);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x80, 0x00);
	return success;
}

bool vl53_configure_interrupt()
{
	/* Interrupt on new sample ready */
	if (!i2c_write_addr8_data8(VL53_REG_SYSTEM_INTERRUPT_CONFIG_GPIO, 0x04))
	{
		return false;
	}

	/* Configure active low since the pin is pulled-up on most breakout boards */
	uint8_t gpio_hv_mux_active_high = 0;
	if (!i2c_read_addr8_data8(VL53_REG_GPIO_HV_MUX_ACTIVE_HIGH, &gpio_hv_mux_active_high))
		return false;

	gpio_hv_mux_active_high &= ~0x10;
	if (!i2c_write_addr8_data8(VL53_REG_GPIO_HV_MUX_ACTIVE_HIGH, gpio_hv_mux_active_high))
		return false;

	if (!i2c_write_addr8_data8(VL53_REG_SYSTEM_INTERRUPT_CLEAR, 0x01))
		return false;

	return true;
}

bool vl53_set_sequence_steps_enabled(uint8_t sequence_step)
{
	return i2c_write_addr8_data8(VL53_REG_SYSTEM_SEQUENCE_CONFIG, sequence_step);
}
typedef enum
{
	CALIBRATION_TYPE_VHV,
	CALIBRATION_TYPE_PHASE
} calibration_type_t;

bool vl53_perform_single_ref_calibration(calibration_type_t calib_type)
{
	uint8_t sysrange_start = 0;
	uint8_t sequence_config = 0;
	switch (calib_type)
	{
	case CALIBRATION_TYPE_VHV:
		sequence_config = 0x01;
		sysrange_start = 0x01 | 0x40;
		break;
	case CALIBRATION_TYPE_PHASE:
		sequence_config = 0x02;
		sysrange_start = 0x01 | 0x00;
		break;
	}
	if (!i2c_write_addr8_data8(VL53_REG_SYSTEM_SEQUENCE_CONFIG, sequence_config))
	{
		return false;
	}
	if (!i2c_write_addr8_data8(VL53_REG_SYSRANGE_START, sysrange_start))
	{
		return false;
	}
	/* Wait for interrupt */
	uint8_t interrupt_status = 0;
	bool success = false;
	do
	{
		success = i2c_read_addr8_data8(VL53_REG_RESULT_INTERRUPT_STATUS, &interrupt_status);
	} while (success && ((interrupt_status & 0x07) == 0));
	if (!success)
	{
		return false;
	}
	if (!i2c_write_addr8_data8(VL53_REG_SYSTEM_INTERRUPT_CLEAR, 0x01))
	{
		return false;
	}

	if (!i2c_write_addr8_data8(VL53_REG_SYSRANGE_START, 0x00))
	{
		return false;
	}
	return true;
}

bool vl53_perform_ref_calibration()
{
	if (!vl53_perform_single_ref_calibration(CALIBRATION_TYPE_VHV))
		return false;

	if (!vl53_perform_single_ref_calibration(CALIBRATION_TYPE_PHASE))
		return false;

	/* Restore sequence steps enabled */
	if (!vl53_set_sequence_steps_enabled(RANGE_SEQUENCE_STEP_DSS +
										 RANGE_SEQUENCE_STEP_PRE_RANGE +
										 RANGE_SEQUENCE_STEP_FINAL_RANGE))
	{
		return false;
	}

	return true;
}

uint16_t vl53_read()
{
	uint8_t range[2];
	bool success = i2c_write_addr8_data8(0x80, 0x01);
	success &= i2c_write_addr8_data8(0xFF, 0x01);
	success &= i2c_write_addr8_data8(0x00, 0x00);
	success &= i2c_write_addr8_data8(0x91, stop_variable);
	success &= i2c_write_addr8_data8(0x00, 0x01);
	success &= i2c_write_addr8_data8(0xFF, 0x00);
	success &= i2c_write_addr8_data8(0x80, 0x00);

	if (!success)
		return VL53L0X_READ_FAIL;

	if (!i2c_write_addr8_data8(VL53_REG_SYSRANGE_START, 0x01))
		return VL53L0X_READ_FAIL;

	uint8_t sysrange_start = 0;
	do
	{
		success = i2c_read_addr8_data8(VL53_REG_SYSRANGE_START, &sysrange_start);
	} while (success && (sysrange_start & 0x01));

	if (!success)
		return VL53L0X_READ_FAIL;

	uint8_t interrupt_status = 0;
	do
	{
		success = i2c_read_addr8_data8(VL53_REG_RESULT_INTERRUPT_STATUS, &interrupt_status);
	} while (success && ((interrupt_status & 0x07) == 0));

	if (!success)
		return VL53L0X_READ_FAIL;

	if (!i2c_read_addr8_data(VL53_REG_RESULT_RANGE_STATUS + 10, range, 2))
		return VL53L0X_READ_FAIL;

	if (!i2c_write_addr8_data8(VL53_REG_SYSTEM_INTERRUPT_CLEAR, 0x01))
		return VL53L0X_READ_FAIL;

	uint16_t converted = (range[0] << 8) | range[1]; //could also do pointer magic

	/* 8190 or 8191 may be returned when obstacle is out of range. */
	if (converted == 8190 || converted == 8191)
		return VL53L0X_OUT_OF_RANGE;

	return converted;
}