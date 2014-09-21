/*
 ******************************************************************************
 * @file    main.c
 * @author  Kailang@DotLab
 * @version V0.0.0
 * @date    06-10-2014
 * @brief   Open DoVision USB Mouse Main Header
 ******************************************************************************
 * @note
 *
 * Happy to see you here!
 *
 * Just look for dependencies...
 *
 * Again, happy to see you here and enjoy!
 *
 ******************************************************************************
 */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"

#include "hw_config.h"
#include "platform_config.h"

/* USB Lib */
#include "usb_lib.h"

/* USB */
#include "usb_desc.h"
#include "usb_istr.h"
#include "usb_pwr.h"

/* STM32F3 */
#include "stm32f30x.h"
#include "stm32f30x_it.h"
#include "stm32f3_discovery.h"

/* Sensors */
#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"

/* Open DoVision */
#include "odvTypes.h"
#include "globalVars.h"
#include "peripherals.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
