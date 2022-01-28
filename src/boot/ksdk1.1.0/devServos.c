#include <stdint.h>

/*
 *	config.h needs to come first
 */
#include "config.h"
#include "devServos.h"

#include "fsl_port_hal.h"
#include "fsl_tpm_driver.h"

//running at higher frequency than the default 50Hz seems to reduce jitter and random jumps
#define SERVO_FREQUENCY_SCALING 5 //this seems to be max reliable value

tpm_pwm_param_t pwm_params[2];

uint8_t initServos()
{
	for (int i=0; i<2; i++)
	{
		pwm_params[i].mode = kTpmEdgeAlignedPWM;         /*!< TPM PWM operation mode */
		pwm_params[i].edgeMode = kTpmHighTrue;    /*!< PWM output mode */
		pwm_params[i].uFrequencyHZ = 50u * SERVO_FREQUENCY_SCALING;           /*!< PWM period in Hz */
		pwm_params[i].uDutyCyclePercent = 0u; //this has been modified to be 1/1000 of a percent instead
	}

	//TODO: choose mode?
	//PTB10, alt2 => TPM0_CH1
	//PTA6, alt2 => TPM0_CH0
	//PTA5, alt2 => TPM0_CH1
	//PTB11, alt2 => TPM0_CH0

	//THIS WORKS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	
	PORT_HAL_SetMuxMode(PORTB_BASE, 11, kPortMuxAlt2); //servo 0 = ptb11
	PORT_HAL_SetMuxMode(PORTB_BASE, 10, kPortMuxAlt2); //servo 1 = ptb10
	
	
	tpm_general_config_t driverInfo;
	memset(&driverInfo, 0, sizeof(driverInfo));
	TPM_DRV_Init(0, &driverInfo);
	
	//warpPrint("Setting clock mode");
	TPM_DRV_SetClock(0, kTpmClockSourceModuleClk, kTpmDividedBy32);
	
	return 0;
}

uint8_t moveServo(uint8_t servo, uint8_t angle)
{
	//datasheet: 50Hz, 1-2ms
	//50Hz = 20ms PWM period, 180 points in 1ms => 3600 pwm resolution
	//uint32_t max value = 
	//convert angle to PWM setting
	//we are running at 50Hz = 20ms => 10%=10000 gives a 2ms pulse = 180, 5% = 5000 gives a 1ms pulse = 0
	//arduino testing shows 0.5ms -> 2.5ms yields 180degrees
	//this should be calibrated per servo to get the exact start and end positions
	//0.5ms -> 2.5ms gives 2500 - 12500
	//these values give 180 degrees of range
	uint16_t minpulse = 2800*SERVO_FREQUENCY_SCALING; //2500
	uint16_t maxpulse = 11800*SERVO_FREQUENCY_SCALING; //12500

	if (angle > 180)
		return 1;

	//pwm_params[servo].uDutyCyclePercent = (uint32_t) 5000 + 5000ul*angle/180;
	pwm_params[servo].uDutyCyclePercent = (uint32_t) minpulse + (maxpulse-minpulse)*angle/180;
	//warpPrint("Setting servo %d to %d duty cycle", servo, pwm_params[servo].uDutyCyclePercent);

	return TPM_DRV_PwmStart(0, &pwm_params[servo], servo) ? 0 : 1;
}