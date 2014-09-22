/*
 ******************************************************************************
 * @file    main.h
 * @author  Kailang@DotLab
 * @version V0.0.1
 * @date    20-09-2014
 * @brief   Open DoVision HID Main Header
 ******************************************************************************
 * @note
 *
 * Happly to see you here!
 *
 * Only connect all the dependencies to the main.c, and...
 *
 * Output a feature report packer function letting the usb_prop.c use it. You
 * can easily search for its name to get an idea.
 *
 * All the files written by DotLab is under GUN GPL V3.0 License, which give
 * you plenty of freedom to do the things you want to do as long as you keep
 * this note.
 *
 * Again, happle to see you here and enjoy!
 *
 ******************************************************************************
 */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x.h"
#include "stm32f30x_it.h"
#include "stm32f3_discovery.h"

#include "stm32f3_discovery_lsm303dlhc.h"
#include "stm32f3_discovery_l3gd20.h"

#include "usb_lib.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "usb_desc.h"

#include "hw_config.h"
#include "platform_config.h"

#include "odvTypes.h"
#include "globalVars.h"
#include "peripherals.h"

/* Exported variables ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void FeaPacker(uint8_t repType);

#endif /* __MAIN_H */
