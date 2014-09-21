/*
 ******************************************************************************
 * @file    main.c
 * @author  Kailang@DotLab
 * @version V0.0.0
 * @date    06-10-2014
 * @brief   Open DoVision USB Mouse Main
 ******************************************************************************
 * @note
 *
 * Happy to see you here!
 *
 * This is the main of the Open DoVision USB Mouse implementation.
 *
 * All the functions here are supposed to be used only by this project.
 * So please see the peripherals.c for more infos about fucntions.
 *
 * Again, happy to see you here and enjoy!
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Global variables ----------------------------------------------------------*/
__IO uint8_t EP1OutState = 1;				/* EP1 Out Report Send Indicator */

float gyrBuffer[3];					/* XYZ Gyroscope Data Buffer */
float accBuffer[3];					/* XYZ Accelerometer Data Buffer */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RCC_ClocksTypeDef RCC_Clocks;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main
* Description    : Init stuffs, get data, send mouse position.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
int main(void) {
	uint8_t HIDBuffer[4] = { 0 };

	/* Set the SysTick callback interval to 1ms */
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

	/* Initiate the peripherals */
	Acc_Init();
	Gyr_Init();
	LED_Config();

	/* Configure the USB */
	Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

	LED_Out(0xff);
	while (bDeviceState != CONFIGURED) {
		/* Wait until the USB is configured */
	}
	LED_Off();

	/* Infinite loop */
	while (1) {
		/* Button Calibration */
		if (ButPreHandler() > 0) {
			LED_Out(0xff);
			Delay(800);
			LED_Out(0x0f);
			Gyr_Cali(20);
			LED_Out(0x00);
		}

		/* Wait for the data ready */
		Delay(5);

		/* Get data */
		Acc_Read(accBuffer);
		Gyr_Read(gyrBuffer);
		Gyr_Corr(gyrBuffer);

		/* Calculate the mouse position */
//		HIDBuffer[1] = (int8_t)(accBuffer[accY] / 10);		/* ACC */
//		HIDBuffer[2] = (int8_t)(accBuffer[accX] / 10);

		HIDBuffer[1] = -(int8_t)(gyrBuffer[gyrZ] / 1);		/* GYR */
		HIDBuffer[2] = -(int8_t)(gyrBuffer[gyrX] / 1);

		/* Sent mouse position */
		if (EP1OutState == 1) {
			EP1OutState = 0;
			USB_SIL_Write(EP1_IN, HIDBuffer, 4);
			SetEPTxValid(ENDP1);
		}
	}

	return 0;
}
