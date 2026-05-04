#include "thermostat.h"
#include "tim.h"


static ThermoState_t state = THERMO_IDLE;
static uint32_t state_start_ms = 0;
static uint8_t pwm_percent = FAN_PWM_DEFAULT;


static uint32_t PWM_PercentToCCR(uint8_t percent){
	return (uint32_t)(percent * 3199/100);
}

static void Fan_SetPWN(uint8_t percent){
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,PWM_PercentToCCR(percent));
}

static void Fan_Off(void){
	__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
}

void Thermostat_Init(void){
	state = THERMO_IDLE;
	state_start_ms =0;
	pwm_percent = FAN_PWM_DEFAULT;
	Fan_Off();
}

void Thermostat_Update(float temperature, float setpoint){
	uint32_t now = HAL_GetTick();

	switch(state)
	{
	case THERMO_IDLE:
		if(temperature > setpoint)
		{
			state = THERMO_FAN_ON;
			state_start_ms = now;
			Fan_SetPWN(pwm_percent);
		}
	case THERMO_FAN_ON:
		if((now - state_start_ms) >= FAN_ON_TIME_MS){
			state = THERMO_COOLDOWN;
			state_start_ms = now;
			Fan_Off();
		}
		break;

	case THERMO_COOLDOWN:
		if((now - state_start_ms) >= COOLDOWN_TIME_MS){
			state = THERMO_IDLE;
		}
		break;
	}
}

ThermoState_t Thermostat_GetState(void){
	return state;
}

uint8_t Thermostat_GetPWM(void){
	return pwm_percent;
}






