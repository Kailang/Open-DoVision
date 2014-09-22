/*
 ******************************************************************************
 * @file    globalVars.h
 * @author  Kailang@DotLab
 * @version V0.0.0
 * @date    04-09-2014
 * @brief   Open DoVision Global Variables
 ******************************************************************************
 * @note
 *
 * Happy to see you here!
 *
 * We use the odvTypes to initiate some data holder for the application.
 *
 * And of course all the variables have their own description.
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
#ifndef GLOBALVARS_H_
#define GLOBALVARS_H_

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "odvTypes.h"

/* Exported types ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/**
 * @brief EP0: Feature Report		Control Pipe					0x00
 * 		  EP1: In/Out Report		Interrupt In/Out Pipe			0x81/0x01
 * 		  	TX- Send Out
 * 		  	RX- Read In
 * 		  The In/Out in the HID Specification is in the PC's point of view
 * 		  Our In/Out is in the ODV's point of view
 */
extern uint8_t	feaRawStream[128];				/* EP0 Feature Report Raw Stream */
extern uint8_t	EP0Buffer[128];					/* EP0 Unknown Data Buffer */
extern uint8_t	inRawStream[128];				/* EP1 In Report Buffer */
extern uint8_t	EP1Buffer[128];					/* EP1 Unknown Data Buffer */

extern OutDataFrameSep		outData;			/* EP1 Out Report Holders */
extern FeaCheckFrameSep		feaCheck;			/* EP0 Feature Report Holders */
extern FeaRenewFrameSep		feaRenew;
extern FeaConfigFrameSep	feaConfig;

extern __IO uint8_t EP1OutState;				/* EP1 Out Report Send Indicator */
extern __IO uint8_t EP1InState;					/* EP1 In Report Read Indicator */
extern __IO int8_t  EP0FeaState;				/* EP0 Feature Report Indicator -1 No Known Feature Report */

extern __IO uint8_t senDatReady;				/* Sensor Data Ready Indicator -0 Ready -1 Not */

extern __IO uint32_t timeSinceRenew;			/* Timer Count Since Renew Report */
extern __IO uint32_t timeSinceSend;				/* Timer Count Since Data Message Sent */
extern __IO uint32_t timeSinceRead;				/* Timer Count Since Read From Sensors */
extern __IO uint32_t timerDelay;				/* Timer Count Down To 0 */

extern float gyrBuffer[3];					/* XYZ Gyroscope Data Buffer */
extern float accBuffer[3];					/* XYZ Accelerometer Data Buffer */
extern float magBuffer[3];					/* XZY Magnet Data Buffer */
extern int16_t tempBuffer;					/* Temperature Data Buffer */

extern __IO uint32_t curDueTime;				/* Current Configuration */
extern __IO uint16_t curMesInterval;
extern __IO uint16_t curGyrScale;
extern __IO uint16_t curAccScale;
extern __IO uint16_t curMagScale;
extern __IO uint8_t	curGyrUnit;
extern __IO uint8_t	curAccUnit;
extern __IO uint8_t	curMagUnit;

/* Exported constants --------------------------------------------------------*/
#define Magic_Word					(uint32_t)			0x19980317

#define Uint32_Flash_Data_Addr		(uint32_t)			0x08010000

typedef enum {
	X = 0,		Y = 1,		Z = 2
} UniCoordinate;

typedef enum {
	gyrX = 0,	gyrY = 1,	gyrZ = 2
} GyrCoordinate;

typedef enum {
	accX = 0,	accY = 1,	accZ = 2
} AccCoordinate;

typedef enum {
	magX = 0,	magY = 2,	magZ = 1
} MagCoordinate;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif	/* GLOBALVARS_H_ */
