#ifndef INC_THERMOSTAT_H_
#define INC_THERMOSTAT_H_


#include "main.h"

#define FAN_ON_TIME_MS 30000U
#define COOLDOWN_TIME_MS 60000U


#define FAN_PWM_DEFAULT 75U

typedef enum {
	THERMO_IDLE = 0,
	THERMO_FAN_ON,
	THERMO_COOLDOWN
} ThermoState_t;

void Thermostat_Init(void);
void Thermostat_Update(float temperature, float setpoint);
ThermoState_t Thermostat_GetState(void);
uint8_t Thermostat_GetPWM(void);
void Thermostat_SetPWM(uint8_t percent);

#endif
