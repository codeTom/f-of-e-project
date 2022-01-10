#ifndef VL53_H
#define VL53_H

#define VL53_REG_IDENTIFICATION_MODEL_ID (0xC0)
#define VL53_REG_VHV_CONFIG_PAD_SCL_SDA_EXTSUP_HV (0x89)
#define VL53_REG_MSRC_CONFIG_CONTROL (0x60)
#define VL53_REG_FINAL_RANGE_CONFIG_MIN_COUNT_RATE_RTN_LIMIT (0x44)
#define VL53_REG_SYSTEM_SEQUENCE_CONFIG (0x01)
#define VL53_REG_DYNAMIC_SPAD_REF_EN_START_OFFSET (0x4F)
#define VL53_REG_DYNAMIC_SPAD_NUM_REQUESTED_REF_SPAD (0x4E)
#define VL53_REG_GLOBAL_CONFIG_REF_EN_START_SELECT (0xB6)
#define VL53_REG_SYSTEM_INTERRUPT_CONFIG_GPIO (0x0A)
#define VL53_REG_GPIO_HV_MUX_ACTIVE_HIGH (0x84)
#define VL53_REG_SYSTEM_INTERRUPT_CLEAR (0x0B)
#define VL53_REG_RESULT_INTERRUPT_STATUS (0x13)
#define VL53_REG_SYSRANGE_START (0x00)
#define VL53_REG_GLOBAL_CONFIG_SPAD_ENABLES_REF_0 (0xB0)
#define VL53_REG_RESULT_RANGE_STATUS (0x14)

#define RANGE_SEQUENCE_STEP_TCC (0x10) /* Target CentreCheck */
#define RANGE_SEQUENCE_STEP_MSRC (0x04) /* Minimum Signal Rate Check */
#define RANGE_SEQUENCE_STEP_DSS (0x28) /* Dynamic SPAD selection */
#define RANGE_SEQUENCE_STEP_PRE_RANGE (0x40)
#define RANGE_SEQUENCE_STEP_FINAL_RANGE (0x80)

#define VL53L0X_OUT_OF_RANGE 8190
#define VL53L0X_READ_FAIL 8191


#include <stdlib.h>

bool i2c_write_addr8_data8(uint8_t addr, uint8_t data);
bool i2c_read_addr8_data8(uint8_t reg, uint8_t * data);

bool vl53_load_default_tuning_settings();

bool vl53_configure_interrupt();
bool vl53_init();
bool vl53_set_sequence_steps_enabled(uint8_t sequence_step);
bool vl53_perform_ref_calibration();

uint16_t vl53_read();

#endif