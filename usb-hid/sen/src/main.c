/*
 ******************************************************************************
 * @file    main.c
 * @author  Kailang@DotLab
 * @version V0.0.1
 * @date    20-09-2014
 * @brief   Open DoVision HID Main
 ******************************************************************************
 * @note
 *
 * Happy to see you here!
 *
 * This is the project that can turn the Open DoVision into a customized HID
 * which send and receive data through the HID protocol.
 *
 * odvTypes.h		- all the data types we need to process ODV data;
 * globalVars.h		- all the global variables like data holder and status
 * 						indicator we need to make ODV work;
 * peripherals.h	- all the useful functions for controlling the sensors and
 * 						other things like button and LED.
 *
 * All the files written by DotLab is under GUN GPL V3.0 License, which give
 * you plenty of freedom to do the things you want to do as long as you keep
 * this note.
 *
 * Again, happy to see you here and enjoy!
 *
 ******************************************************************************
 */

/* Pragmas -------------------------------------------------------------------*/
#pragma pack(1)							/* Alien 1 byte */

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>

#include "main.h"

/* Global variables ----------------------------------------------------------*/
uint8_t	feaRawStream[128];				/* EP0 Feature Report Raw Stream */
uint8_t	EP0Buffer[128];					/* EP0 Unknown Data Buffer */
uint8_t	inRawStream[128];				/* EP1 In Report Buffer */
uint8_t	EP1Buffer[128];					/* EP1 Unknown Data Buffer */

OutDataFrameSep			outData;			/* EP1 Out Report Holders */
FeaCheckFrameSep		feaCheck;			/* EP0 Feature Report Holders */
FeaRenewFrameSep		feaRenew;
FeaConfigFrameSep		feaConfig;

__IO uint8_t EP1OutState = 1;				/* EP1 Out Report Send Indicator, Now Sent*/
__IO uint8_t EP1InState = 0;				/* EP1 In Report Read Indicator, Now Haven't Read */
__IO int8_t  EP0FeaState;					/* EP0 Feature Report Indicator -1 No Known Feature Report */

__IO uint8_t senDatReady;				/* Sensor Data Ready Indicator -0 Ready + Not */

__IO uint32_t timeSinceRenew;				/* Timer Count Since Renew Report */
__IO uint32_t timeSinceSend;				/* Timer Count Since Data Sent */
__IO uint32_t timeSinceRead;				/* Timer Count Since Read From Sensors */
__IO uint32_t timerDelay;					/* Timer Count Down To 0 */

float gyrBuffer[3];						/* XYZ Gyroscope Data Buffer in deg/s */
float accBuffer[3];						/* XYZ Accelerometer Data Buffer in 0.01 m/s2 */
float magBuffer[3];						/* XZY Magnet Data Buffer in 0.001 Gauss */
int16_t tempBuffer;						/* Temperature Data Buffer */

__IO uint32_t curDueTime;				/* Current Settings */
__IO uint16_t curMesInterval;
__IO uint16_t curGyrScale;
__IO uint16_t curAccScale;
__IO uint16_t curMagScale;
__IO uint8_t curGyrUnit;
__IO uint8_t curAccUnit;
__IO uint8_t curMagUnit;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;
uint8_t odvState = State_Idle;

/* Private function prototypes -----------------------------------------------*/
void AtoMemCopy(uint8_t *to, __IO uint8_t *from, size_t size);
int32_t FloToInt32(float flo, uint8_t movPoiLeft);

void USB_Config(void);
void ODV_Config(void);
uint8_t ButHandler(void);
uint8_t FeaHandler(void);
/* void FeaPacker(uint8_t repType); Exported*/
uint8_t OutHandler(uint8_t repType);
void OutPacker(uint8_t repType);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main.
* Description    : main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void) {

	/* Setup SysTick event each 1 ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	if (SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000)) {
		return 1;
	}

	/* Configure the USB (will get stuck here with mad lights until USB is plugged in)*/
	LED_Config();
	USB_Config();
	LED_Off();

	/* Initiate peripherals */
	ODV_Config();								/* Restore Default Open DoVision Settings */
	Gyr_Init();
	Acc_Init();
	Mag_Init();

	/* Infinite loop */
	while (1) {

		/* Read from EP1 */
		if (EP1InState) {
			STM_EVAL_LEDToggle(LED5);

			USB_SIL_Read(EP1_OUT, EP1Buffer);
			SetEPRxValid(ENDP1);
			EP1InState = 0;
		}

		/* Handles the Feature Report(If There Is Any) */
		FeaHandler();

		/* User Button Utility */
		ButHandler();

		/* Data Routine */
		switch (odvState) {
		case State_Idle:							/* Due */
			STM_EVAL_LEDOff(LED6);
			STM_EVAL_LEDToggle(LED7);
			Delay(100);

			break;
		case State_Track:
			STM_EVAL_LEDOff(LED7);
			STM_EVAL_LEDToggle(LED6);

			/* Read Data */
			senDatReady = 0;

			Gyr_Read(gyrBuffer);
			Acc_Read(accBuffer);
			Mag_Read(magBuffer);

			/* Send Data */
			if (timeSinceSend > curMesInterval) {
				OutHandler(Out_Data);		/* Form Message_Data */
			}

			/* Check Due */
			if (timeSinceRenew > curDueTime) {
				LED_Dim(LED7);
				Delay(100);
				LED_Off();
				odvState = State_Idle;				/* Due */
			}
			break;
		}
	}

	uint8_t i;
	while (1) {
		for (i=0; i<8; i++) {
			LED_Dim(i);
			Delay(10);
		}
	}
	return 0;
}

/*******************************************************************************
* Function Name  : USB_Config.
* Description    : USB Configuration.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Config(void) {
	uint8_t  i = 0;

	Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

	while (bDeviceState != CONFIGURED) { /* Blink LEDs Until It's Configured */
		if (i < 0xff) {
			i ++;
		} else {
			i = 1;
		}
		LED_Out(i);
		Delay(10);
	}
	LED_Off();
}

/*******************************************************************************
* Function Name  : ODV_Config.
* Description    : Restore Default Open DoVision Settings.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void ODV_Config(void) {
	curDueTime		= 3000;
	curMesInterval	= 100;

}

/*******************************************************************************
* Function Name  : FeaHandler.
* Description    : Copy the report to relevant structure and handle them(If there is any).
* Input          : repType- Feature report type(EP0FeaState).
* Output         : None.
* Return         : 0- Success; 1- No data; 2- Glitch.
*******************************************************************************/
uint8_t FeaHandler(void) {
	if (EP0FeaState < 0) {							/* Check Feature Report from EP0 */
		return 1;									/* No Data */
	} else if (EP0FeaState != feaRawStream[0]) {	/* It Suppose To Be So */
		STM_EVAL_LEDToggle(LED10);
		EP0FeaState = -1;							/* Glitch Happens, Reset Report State */
		return 2;
	}

	STM_EVAL_LEDToggle(LED8);
	uint8_t repType = EP0FeaState;
	EP0FeaState = -1;					/* Reset Report State */

	switch (repType) {
	case Feature_None:
		/* Your Are Joking */
		/* "Report ID 0x00 Should Be Reserved And Not Used." - HID Specification */

		break;
	case Feature_Check:
		AtoMemCopy(feaCheck.rawStream, feaRawStream, FeaCheck_Size);

		break;
	case Feature_Renew:
		AtoMemCopy(feaRenew.rawStream, feaRawStream, FeaRenew_Size);
		outData.frame.count = 0;

		break;
	case Feature_Config:
		AtoMemCopy(feaConfig.rawStream, feaRawStream, FeaConfig_Size);
		curDueTime		= feaConfig.frame.dueTime;
		curMesInterval	= feaConfig.frame.mesInterval;
		curGyrScale		= feaConfig.frame.gyrScale;
		curAccScale		= feaConfig.frame.accScale;
		curMagScale		= feaConfig.frame.magScale;
		curGyrUnit		= feaConfig.frame.gyrUnit;
		curAccUnit		= feaConfig.frame.accUnit;
		curMagUnit		= feaConfig.frame.magUnit;

		break;
	}

	timeSinceRenew = 0;					/* Renew Despite Of The Feature Report Type */
	odvState = State_Track;

	return 0;
}

/*******************************************************************************
* Function Name  : FeaPacker.
* Description    : Pack the buffer data into outData.frame.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void FeaPacker(uint8_t repType) {
	STM_EVAL_LEDOn(LED9);

	switch (repType) {
	case Feature_None:
		/* Your Are Joking */
		/* "Report ID 0x00 Should Be Reserved And Not Used." - HID Specification */

		break;
	case Feature_Check:
		feaCheck.frame.reportType	= 0x01;
		feaCheck.frame.magicWord	= Magic_Word;

		break;
	case Feature_Renew:
		feaRenew.frame.reportType	= 0x02;
		feaRenew.frame.magicWord	= Magic_Word;

		break;
	case Feature_Config:
		feaConfig.frame.reportType	= 0x03;
		feaConfig.frame.dueTime		= curDueTime;
		feaConfig.frame.mesInterval	= curMesInterval;
		feaConfig.frame.gyrScale	= curGyrScale;
		feaConfig.frame.accScale	= curAccScale;
		feaConfig.frame.magScale	= curMagScale;
		feaConfig.frame.gyrUnit		= curGyrUnit;
		feaConfig.frame.accUnit		= curAccUnit;
		feaConfig.frame.magUnit		= curMagUnit;

		break;
	}
}

/*******************************************************************************
* Function Name  : OutHandler.
* Description    : Form and send the requested massage.
* Input          : repType- Out report type..
* Output         : None.
* Return         : 0- Success; 1- Transmission is not terminated; 2- Data not ready
*******************************************************************************/
uint8_t OutHandler(uint8_t repType) {
	if (EP1OutState == 0) {
		STM_EVAL_LEDToggle(LED10);
		return 1;
	} else if (senDatReady != 0) {
		STM_EVAL_LEDToggle(LED10);
		return 2;
	}

	EP1OutState = 0;				/* New Transmission */
	switch (repType) {
	case Out_None:
		/* Your Are Joking */
		/* We Do Not Have That... Yet */

		break;
	case Out_Data:
		OutPacker(Out_Data);

		USB_SIL_Write(EP1_IN, outData.rawStream, OutData_Size);

		break;
	}

	SetEPTxValid(ENDP1);			/* Valid the EP1 Tx Status */
	timeSinceSend = 0;

	return 0;
}

/*******************************************************************************
* Function Name  : OutPacker.
* Description    : Pack the buffer data into outData.frame.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void OutPacker(uint8_t repType) {

	switch (repType) {
	case Out_None:
		/* Your Are Joking */
		/* We Do Not Have That... Yet */

		break;
	case Out_Data:
		outData.frame.messageType	= 0x11;

		if (outData.frame.count < 65535) {
			outData.frame.count		++;
		}
		outData.frame.temp			= 0;

		/* The difference between real sensor coordinate and buffer coordinate
		 * is that the buffers are not always given out by the form of XYZ
		 * it might be XZY for instance.
		 * The difference between the out coordinate and real sensor coordinate
		 * is that each sensor has its own unique way of aligning with the
		 * coordinate we want.
		 * Please see the video walk-through for more detailed explanation. */
		outData.frame.gyrData[X]	= FloToInt32( gyrBuffer[gyrX], 4);
		outData.frame.gyrData[Y]	= FloToInt32( gyrBuffer[gyrY], 4);
		outData.frame.gyrData[Z]	= FloToInt32( gyrBuffer[gyrZ], 4);

		outData.frame.accData[X]	= FloToInt32( accBuffer[accY], 4);
		outData.frame.accData[Y]	= FloToInt32(-accBuffer[accX], 4);
		outData.frame.accData[Z]	= FloToInt32( accBuffer[accZ], 4);

		outData.frame.magData[X]	= FloToInt32( magBuffer[magY], 4);
		outData.frame.magData[Y]	= FloToInt32(-magBuffer[magX], 4);
		outData.frame.magData[Z]	= FloToInt32( magBuffer[magZ], 4);

		break;
	}
}

/*******************************************************************************
* Function Name  : ButHandler.
* Description    : Handles the user button events.
* Input          : None.
* Output         : None.
* Return         : 0- Success; 1- No press.
*******************************************************************************/
uint8_t ButHandler(void) {
	if (But_Pres() == 0) {
		return 1;
	}

	return 0;
}

/*******************************************************************************
* Function Name  : AtoMemCopy.
* Description    : Copy data form the memory buffer used by the USB interrupt.
* Input          : to- Destination; from- Origin; size- Data length.
* Output         : None.
* Return         : None.
*******************************************************************************/
void AtoMemCopy(uint8_t *to, __IO uint8_t *from, size_t size) {
	size_t counter;
	for (counter=0; counter<size; counter++) {
		*to = *from;
		to++;
		from++;
	}
}

/*******************************************************************************
* Function Name  : FloToInt32.
* Description    : Pack the buffer data into mesData.frame.
* Input          : flo- Float want to convert; movPoiLeft- Move the
* 					floating point left how far.
* Output         : None.
* Return         : Int32 value of float left moved its floating point.
*******************************************************************************/
int32_t FloToInt32(float flo, uint8_t movPoiLeft) {
	uint8_t i;
	for (i=0; i<movPoiLeft; i++) {
		flo *= 10;
	}

	return (int32_t)flo;
}
