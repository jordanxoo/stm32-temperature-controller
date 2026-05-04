#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

#include "main.h"

/* Pin DS18B20 - PA0 Open-Drain */
#define DS18B20_GPIO_PORT   GPIOA
#define DS18B20_GPIO_PIN    GPIO_PIN_0

/* Funkcje publiczne */
uint8_t DS18B20_Init(void);
float   DS18B20_ReadTemp(void);

#endif /* INC_DS18B20_H_ */
