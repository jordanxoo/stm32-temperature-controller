#include "encoder.h"

/* Aktualny setpoint - zmienna globalna chroniona przed race condition */
static volatile float setpoint = 22.0f;

void Encoder_Init(void)
{
    setpoint = 22.0f; /* Domyślny setpoint 22°C */
}

float Encoder_GetSetpoint(void)
{
    return setpoint;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin != GPIO_PIN_8)
        return;

    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8) == GPIO_PIN_RESET)
        return;

    static uint32_t last_tick = 0;
    uint32_t now = HAL_GetTick();
    if((now - last_tick) < 10)
        return;
    last_tick = now;

    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == GPIO_PIN_RESET)
    {
        if(setpoint < SETPOINT_MAX) setpoint += SETPOINT_STEP;
    }
    else
    {
        if(setpoint > SETPOINT_MIN) setpoint -= SETPOINT_STEP;
    }
}
