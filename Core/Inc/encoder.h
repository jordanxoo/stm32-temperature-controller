#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include "main.h"


#define SETPOINT_MIN 15.0f
#define SETPOINT_MAX 40.0f
#define SETPOINT_STEP 0.5f

void Encoder_Init(void);
float Encoder_GetSetpoint(void);


#endif
