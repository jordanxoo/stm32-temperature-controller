#include "ds18b20.h"
#include "tim.h"

static void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(&htim2, 0);
    HAL_TIM_Base_Start(&htim2);
    while(__HAL_TIM_GET_COUNTER(&htim2) < us);
}

static uint8_t DS18B20_Reset(void)
{
    uint8_t presence;
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
    delay_us(480);
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
    delay_us(70);
    presence = HAL_GPIO_ReadPin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(410);
    return presence;
}

static void DS18B20_WriteBit(uint8_t bit)
{
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
    if(bit)
    {
        delay_us(1);
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
        delay_us(59);
    }
    else
    {
        delay_us(60);
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
        delay_us(1);
    }
}

static uint8_t DS18B20_ReadBit(void)
{
    uint8_t bit;
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
    delay_us(1);
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
    delay_us(14);
    bit = HAL_GPIO_ReadPin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(45);
    return bit;
}

static void DS18B20_WriteByte(uint8_t byte)
{
    for(uint8_t i = 0; i < 8; i++)
    {
        DS18B20_WriteBit(byte & (1 << i));
    }
}

static uint8_t DS18B20_ReadByte(void)
{
    uint8_t byte = 0;
    for(uint8_t i = 0; i < 8; i++)
    {
        if(DS18B20_ReadBit())
        {
            byte |= (1 << i);
        }
    }
    return byte;
}

uint8_t DS18B20_Init(void)
{
    return DS18B20_Reset();
}

float DS18B20_ReadTemp(void)
{
    uint8_t byte1, byte2;
    int16_t raw;
    float temp;

    DS18B20_Reset();
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0x44);
    HAL_Delay(750);

    DS18B20_Reset();
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0xBE);

    byte1 = DS18B20_ReadByte();
    byte2 = DS18B20_ReadByte();

    raw = (int16_t)((byte2 << 8) | byte1);
    temp = (float)raw * 0.0625f;

    return temp;
}
