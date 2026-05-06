#include "ds18b20.h"
#include "tim.h"
#include <stdio.h>

static void delay_us(uint16_t us)
{
    uint32_t start = DWT->CYCCNT;
    uint32_t cycles = (uint32_t)us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - start) < cycles);
}

static uint8_t DS18B20_Reset(void)
{
    uint8_t presence;
    __disable_irq();
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
    delay_us(480);
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
    delay_us(70);
    presence = HAL_GPIO_ReadPin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(410);
    __enable_irq();
    return presence;
}

static void DS18B20_WriteBit(uint8_t bit)
{
    __disable_irq();
    if(bit)
    {
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
        delay_us(5);
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
        delay_us(58);
    }
    else
    {
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
        delay_us(62);
        HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
        delay_us(2);
    }
    __enable_irq();
}

static uint8_t DS18B20_ReadBit(void)
{
    uint8_t bit;
    __disable_irq();
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_RESET);
    delay_us(2);
    HAL_GPIO_WritePin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN, GPIO_PIN_SET);
    delay_us(10);
    bit = HAL_GPIO_ReadPin(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(50);
    __enable_irq();
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
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    return DS18B20_Reset();
}

float DS18B20_ReadTemp(void)
{
    uint8_t b[4], presence1, presence2;
    int16_t raw;

    presence1 = DS18B20_Reset();
    if(presence1 != 0)
    {
        printf("NOT FOUND\r\n");
        return -999.0f;
    }

    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0x44);
    HAL_Delay(800);

    presence2 = DS18B20_Reset();
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0xBE);

    b[0] = DS18B20_ReadByte();
    b[1] = DS18B20_ReadByte();
    b[2] = DS18B20_ReadByte();
    b[3] = DS18B20_ReadByte();

    (void)presence2;

    raw = (int16_t)((b[1] << 8) | b[0]);
    return (float)raw * 0.0625f;
}
