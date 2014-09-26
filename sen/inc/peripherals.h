/*
 ******************************************************************************
 * @file    peripherals.h
 * @author  Kailang@DotLab
 * @version V0.0.1
 * @date    20-09-2014
 * @brief   Open DoVision Peripheral Functions Header
 ******************************************************************************
 * @note
 *
 * Happy to see you here!
 *
 * Only the header file for exporting the functions, but...
 *
 * This file also contain the sensitivity for gyroscope and accelerometer,
 * the sensitivity for magnetometer is in the stm32f3_discovery_lsm303dlhc.h
 * For gyroscope and magnetometer you should divide the sensitibity, but for
 * accelerometer you should multiply the sensitivity.
 *
 * All the files written by DotLab is under GUN GPL V3.0 License, which give
 * you plenty of freedom to do the things you want to do as long as you keep
 * this note.
 *
 * Again, happy to see you here and enjoy!
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef PERIPHERALS_H_
#define PERIPHERALS_H_

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "main.h"
#include "globalVars.h"

#include "stm32f30x_it.h"

#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define L3G_Sensitivity_250dps     (float)   114.285f			/* gyroscope sensitivity with 250 dps full scale [LSB/dps] */
#define L3G_Sensitivity_500dps     (float)    57.1429f			/* gyroscope sensitivity with 500 dps full scale [LSB/dps] */
#define L3G_Sensitivity_2000dps    (float)    14.285f			/* gyroscope sensitivity with 2000 dps full scale [LSB/dps] */

#define LSM_Acc_Sensitivity_2g     (float)     1.0f				/* accelerometer sensitivity with 2 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_4g     (float)     2.0f				/* accelerometer sensitivity with 4 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_8g     (float)     4.0f				/* accelerometer sensitivity with 8 g full scale [LSB/mg] */
#define LSM_Acc_Sensitivity_16g    (float)    12.0f				/* accelerometer sensitivity with 12 g full scale [LSB/mg] */

#define PI                         (float)     3.14159265f
#define g						   (float)     9.80665f

/* Exported macro ------------------------------------------------------------*/
#define ABS(x)			(x < 0) ? (-x) : x
#define MAX(x, y)		(x < y) ? (y) : x

/* Exported functions ------------------------------------------------------- */
void SysTick_Handler(void);
void Delay(uint32_t time);

void Gyr_Init(/*uint16_t gyrScale*/);
void Acc_Init(/*uint16_t accScale*/);
void Mag_Init(/*uint16_t magScale*/);
void Gyr_Read(float* pfData);
void Acc_Read(float* pfData);
void Mag_Read(float* pfData);
void Gyr_Cali(uint8_t sample);
void Gyr_Corr(float* pfData);

void LED_Config(void);
void LED_Off(void);
void LED_On(void);
void LED_Ign(Led_TypeDef LED);
void LED_Dim(Led_TypeDef LED);
void LED_Out(uint8_t byte);
void LED_Read(uint64_t data, uint8_t count);

uint8_t ButPreHandler(void);

void Fla_Read32(uint8_t id, uint32_t* pdata, uint8_t length);
uint8_t Fla_Writ32(uint8_t id, uint32_t* pdata, uint8_t length);

#endif /* PERIPHERALS_H_ */
