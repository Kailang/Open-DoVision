/*
 ******************************************************************************
 * @file    peripherals.c
 * @author  Kailang@DotLab
 * @version V0.0.1
 * @date    20-09-2014
 * @brief   Open DoVision Peripheral Functions
 ******************************************************************************
 * @note
 *
 * Happy to see you here!
 *
 * This file shouldn't contain any private variables. So...
 *
 * All the global variables have their own descriptions in the globalVars.h;
 * All the functions have their own descriptions above their source code.
 *
 * All the files written by DotLab is under GUN GPL V3.0 License, which give
 * you plenty of freedom to do the things you want to do as long as you keep
 * this note.
 *
 * Again, happy to see you here and enjoy!
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "peripherals.h"

/* Global variables ----------------------------------------------------------*/
__IO uint32_t timeSinceRenew = 0;
__IO uint32_t timeSinceSend = 0;
__IO uint32_t timeSinceRead = 0;
__IO uint32_t timerDelay = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float gyrCorrec[3];					/* XYZ Gyroscope Correction for Current Scale Only */

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : SysTick_Handler.
* Description    : Timer interrupt, called every 1 ms, keeps track of timers.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void SysTick_Handler(void) {
	STM_EVAL_LEDToggle(LED3);

	timeSinceRenew++;
	timeSinceSend++;
	timeSinceRead++;
	if (timerDelay > 0) {
		timerDelay--;
	}
}

/*******************************************************************************
* Function Name  : Delay.
* Description    : Delay.
* Input          : Delay time in ms.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Delay(uint32_t time) {
	timerDelay = time;
	while (timerDelay != 0) {
	}
}

/*******************************************************************************
* Function Name  : Gyr_Init.
* Description    : Configure the Mems to gyroscope application.
* Input          : gyrScale- Gyroscope Scale.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Gyr_Init(void) {
	L3GD20_InitTypeDef L3GD20_InitStructure;
	L3GD20_FilterConfigTypeDef L3GD20_FilterStructure;

	/* Configure Mems L3GD20 */
	L3GD20_InitStructure.Power_Mode = L3GD20_MODE_ACTIVE;
	L3GD20_InitStructure.Output_DataRate = L3GD20_OUTPUT_DATARATE_1;
	L3GD20_InitStructure.Axes_Enable = L3GD20_AXES_ENABLE;
	L3GD20_InitStructure.Band_Width = L3GD20_BANDWIDTH_4;
	L3GD20_InitStructure.BlockData_Update = L3GD20_BlockDataUpdate_Continous;
	L3GD20_InitStructure.Endianness = L3GD20_BLE_LSB;
	L3GD20_InitStructure.Full_Scale = L3GD20_FULLSCALE_500;
	L3GD20_Init(&L3GD20_InitStructure);

	L3GD20_FilterStructure.HighPassFilter_Mode_Selection =
			L3GD20_HPM_NORMAL_MODE_RES;
	L3GD20_FilterStructure.HighPassFilter_CutOff_Frequency = L3GD20_HPFCF_0;
	L3GD20_FilterConfig(&L3GD20_FilterStructure);

	L3GD20_FilterCmd(L3GD20_HIGHPASSFILTER_ENABLE);

	/* Load Calibration Data */
	Fla_Read32(0, (uint32_t*)gyrCorrec, 3);
}

/*******************************************************************************
* Function Name  : Acc_Init.
* Description    : Configure the Mems to accelerometer application.
* Input          : accScale- Accelerometer Scale.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Acc_Init(void) {
	LSM303DLHCAcc_InitTypeDef LSM303DLHCAcc_InitStructure;
	LSM303DLHCAcc_FilterConfigTypeDef LSM303DLHCFilter_InitStructure;

	/* Fill the accelerometer structure */
	LSM303DLHCAcc_InitStructure.Power_Mode = LSM303DLHC_NORMAL_MODE;
	LSM303DLHCAcc_InitStructure.AccOutput_DataRate = LSM303DLHC_ODR_50_HZ;
	LSM303DLHCAcc_InitStructure.Axes_Enable = LSM303DLHC_AXES_ENABLE;
	LSM303DLHCAcc_InitStructure.AccFull_Scale = LSM303DLHC_FULLSCALE_2G;
	LSM303DLHCAcc_InitStructure.BlockData_Update =
	LSM303DLHC_BlockUpdate_Continous;
	LSM303DLHCAcc_InitStructure.Endianness = LSM303DLHC_BLE_LSB;
	LSM303DLHCAcc_InitStructure.High_Resolution = LSM303DLHC_HR_ENABLE;
	/* Configure the accelerometer main parameters */
	LSM303DLHC_AccInit(&LSM303DLHCAcc_InitStructure);

	/* Fill the accelerometer LPF structure */
	LSM303DLHCFilter_InitStructure.HighPassFilter_Mode_Selection =
	LSM303DLHC_HPM_NORMAL_MODE;
	LSM303DLHCFilter_InitStructure.HighPassFilter_CutOff_Frequency =
	LSM303DLHC_HPFCF_16;
	LSM303DLHCFilter_InitStructure.HighPassFilter_AOI1 =
	LSM303DLHC_HPF_AOI1_DISABLE;
	LSM303DLHCFilter_InitStructure.HighPassFilter_AOI2 =
	LSM303DLHC_HPF_AOI2_DISABLE;

	/* Configure the accelerometer LPF main parameters */
	LSM303DLHC_AccFilterConfig(&LSM303DLHCFilter_InitStructure);
}

/*******************************************************************************
* Function Name  : Mag_Init.
* Description    : Configure the Mems to compass application.
* Input          : gyrScale- Gyroscope Scale.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Mag_Init(void) {
	LSM303DLHCMag_InitTypeDef LSM303DLHC_InitStructure;

	/* Configure MEMS magnetometer main parameters: temp, working mode, full Scale and Data rate */
	LSM303DLHC_InitStructure.Temperature_Sensor = LSM303DLHC_TEMPSENSOR_DISABLE;
	LSM303DLHC_InitStructure.MagOutput_DataRate = LSM303DLHC_ODR_30_HZ;
	LSM303DLHC_InitStructure.MagFull_Scale = LSM303DLHC_FS_1_9_GA;
	LSM303DLHC_InitStructure.Working_Mode = LSM303DLHC_CONTINUOS_CONVERSION;
	LSM303DLHC_MagInit(&LSM303DLHC_InitStructure);
}

/*******************************************************************************
* Function Name  : Gyr_Read.
* Description    : Calculate the angular Data rate Gyroscope.
* Input          : pfData- Data out pointer.
* Output         : None.
* Return         : 0- Read Success; 1- No Data.
*******************************************************************************/
void Gyr_Read(float* pfData) {
	uint8_t tmpbuffer[6] = { 0 };
	int16_t RawData[3] = { 0 };
	uint8_t tmpreg = 0;
	float sensitivity = 0;
	int i = 0;

	L3GD20_Read(&tmpreg, L3GD20_CTRL_REG4_ADDR, 1);

	L3GD20_Read(tmpbuffer, L3GD20_OUT_X_L_ADDR, 6);

	/* check in the control register 4 the data alignment (Big Endian or Little Endian)*/
	if (!(tmpreg & 0x40)) {
		for (i = 0; i < 3; i++) {
			RawData[i] = (int16_t) (((uint16_t) tmpbuffer[2 * i + 1] << 8)
					+ tmpbuffer[2 * i]);
		}
	} else {
		for (i = 0; i < 3; i++) {
			RawData[i] = (int16_t) (((uint16_t) tmpbuffer[2 * i] << 8)
					+ tmpbuffer[2 * i + 1]);
		}
	}

	/* Switch the sensitivity value set in the CRTL4 */
	switch (tmpreg & 0x30) {
	case 0x00:
		sensitivity = L3G_Sensitivity_250dps;
		break;

	case 0x10:
		sensitivity = L3G_Sensitivity_500dps;
		break;

	case 0x20:
		sensitivity = L3G_Sensitivity_2000dps;
		break;
	}
	/* divide by sensitivity */
	for (i = 0; i < 3; i++) {
		pfData[i] = (float) RawData[i] / sensitivity;
	}
}

/*******************************************************************************
* Function Name  : Acc_Read.
* Description    : LSM303DLHC output register, and calculate the acceleration ACC=(1/SENSITIVITY)* (out_h*256+out_l)/16 (12 bit rappresentation).
* Input          : pfData- Pointer to float buffer where to store data.
* Output         : None.
* Return         : 0- Read Success; 1- No Data.
*******************************************************************************/
void Acc_Read(float* pfData) {
	int16_t pnRawData[3];
	uint8_t ctrlx[2];
	uint8_t buffer[6], cDivider;
	uint8_t i = 0;
	float LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;

	/* Read the register content */
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, ctrlx, 2);
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_OUT_X_L_A, buffer, 6);

	if (ctrlx[1] & 0x40)
		cDivider = 64;
	else
		cDivider = 16;

	/* check in the control register4 the data alignment*/
	if (!(ctrlx[0] & 0x40) || (ctrlx[1] & 0x40)) /* Little Endian Mode or FIFO mode */
	{
		for (i = 0; i < 3; i++) {
			pnRawData[i] = ((int16_t) ((uint16_t) buffer[2 * i + 1] << 8)
					+ buffer[2 * i]) / cDivider;
		}
	} else /* Big Endian Mode */
	{
		for (i = 0; i < 3; i++)
			pnRawData[i] = ((int16_t) ((uint16_t) buffer[2 * i] << 8)
					+ buffer[2 * i + 1]) / cDivider;
	}
	/* Read the register content */
	LSM303DLHC_Read(ACC_I2C_ADDRESS, LSM303DLHC_CTRL_REG4_A, ctrlx, 2);

	if (ctrlx[1] & 0x40) {
		/* FIFO mode */
		LSM_Acc_Sensitivity = 4.0f;
	} else {
		/* normal mode */
		/* switch the sensitivity value set in the CRTL4*/
		switch (ctrlx[0] & 0x30) {
		case LSM303DLHC_FULLSCALE_2G:
			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_2g;
			break;
		case LSM303DLHC_FULLSCALE_4G:
			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_4g;
			break;
		case LSM303DLHC_FULLSCALE_8G:
			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_8g;
			break;
		case LSM303DLHC_FULLSCALE_16G:
			LSM_Acc_Sensitivity = LSM_Acc_Sensitivity_16g;
			break;
		}
	}

	/* Obtain the mg value for the three axis */
	for (i = 0; i < 3; i++) {
		pfData[i] = (float) pnRawData[i] * LSM_Acc_Sensitivity;
	}
}

/*******************************************************************************
* Function Name  : Mag_Read.
* Description    : Calculate the magnetic field.
* Input          : pfData- Pointer to float buffer where to store data.
* Output         : None.
* Return         : 0- Read Success; 1- No Data.
*******************************************************************************/
void Mag_Read(float* pfData) {
	static uint8_t buffer[6] = { 0 };
	uint8_t CTRLB = 0;
	uint16_t Magn_Sensitivity_XY = 0, Magn_Sensitivity_Z = 0;
	uint8_t i = 0;
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_CRB_REG_M, &CTRLB, 1);

	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_H_M, buffer, 1);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_X_L_M, buffer + 1, 1);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_H_M, buffer + 2, 1);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Y_L_M, buffer + 3, 1);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_H_M, buffer + 4, 1);
	LSM303DLHC_Read(MAG_I2C_ADDRESS, LSM303DLHC_OUT_Z_L_M, buffer + 5, 1);
	/* Switch the sensitivity set in the CRTLB*/
	switch (CTRLB & 0xE0) {
	case LSM303DLHC_FS_1_3_GA:
		Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_3Ga;
		Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_3Ga;
		break;
	case LSM303DLHC_FS_1_9_GA:
		Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_1_9Ga;
		Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_1_9Ga;
		break;
	case LSM303DLHC_FS_2_5_GA:
		Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_2_5Ga;
		Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_2_5Ga;
		break;
	case LSM303DLHC_FS_4_0_GA:
		Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4Ga;
		Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4Ga;
		break;
	case LSM303DLHC_FS_4_7_GA:
		Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_4_7Ga;
		Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_4_7Ga;
		break;
	case LSM303DLHC_FS_5_6_GA:
		Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_5_6Ga;
		Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_5_6Ga;
		break;
	case LSM303DLHC_FS_8_1_GA:
		Magn_Sensitivity_XY = LSM303DLHC_M_SENSITIVITY_XY_8_1Ga;
		Magn_Sensitivity_Z = LSM303DLHC_M_SENSITIVITY_Z_8_1Ga;
		break;
	}

	for (i = 0; i < 2; i++) {
		pfData[i] = (float) ((int16_t) (((uint16_t) buffer[2 * i] << 8)
				+ buffer[2 * i + 1]) * 1000) / Magn_Sensitivity_XY;
	}
	pfData[2] = (float) ((int16_t) (((uint16_t) buffer[4] << 8) + buffer[5])
			* 1000) / Magn_Sensitivity_Z;
}

/*******************************************************************************
* Function Name  : Gyr_Cali.
* Description    : Calibrate the gyroscope. Please re-calibrate if the scale changes
* Input          : sample- Amount of samples.
* Output         : float gyrCorrect[3].
* Return         : None.
*******************************************************************************/
void Gyr_Cali(uint8_t sample) {
	uint8_t i, j;
	double stake[3] = { 0 };

	for (i=0; i<3; i++) {
		for (j=0; j<sample; j++) {
			Delay(20);
			Gyr_Read(gyrBuffer);
			stake[i] += gyrBuffer[i];
		}
		gyrCorrec[i] = stake[i] / sample;
	}

	Fla_Writ32(0, (uint32_t*)gyrCorrec, 3);
}

/*******************************************************************************
* Function Name  : Gyr_Corr.
* Description    : Correct the gyroscope.
* Input          : pfData- Pointer to float buffer where to store data.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Gyr_Corr(float* pfData) {
	uint8_t i;
	for (i=0; i<3; i++) {
		pfData[i] -= gyrCorrec[i];
	}
}

/*******************************************************************************
* Function Name  : LED_Config.
* Description    : Initiate the LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LED_Config(void) {
	uint8_t i;
	for (i = LED3; i <= LED10; i++) {
		STM_EVAL_LEDInit(i);
	}
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
}

/*******************************************************************************
* Function Name  : LED_Off.
* Description    : Turn off the LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LED_Off(void) {
	uint8_t i;
	for (i = LED3; i <= LED10; i++) {
		STM_EVAL_LEDOff(i);
	}
}

/*******************************************************************************
* Function Name  : LED_On.
* Description    : Turn on the LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LED_On(void) {
	uint8_t i;
	for (i = LED3; i <= LED10; i++) {
		STM_EVAL_LEDOn(i);
	}
}

/*******************************************************************************
* Function Name  : LED_Ign.
* Description    : Ignite one LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LED_Ign(Led_TypeDef LED) {
	uint8_t i;
	for (i = LED3; i <= LED10; i++) {
		if (i == LED) {
			STM_EVAL_LEDOn(i);
		} else {
			STM_EVAL_LEDOff(i);
		}
	}
}

/*******************************************************************************
* Function Name  : LED_Dim.
* Description    : Ignite one LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LED_Dim(Led_TypeDef LED) {
	uint8_t i;
	for (i = LED3; i <= LED10; i++) {
		if (i != LED) {
			STM_EVAL_LEDOn(i);
		} else {
			STM_EVAL_LEDOff(i);
		}
	}
}

/*******************************************************************************
* Function Name  : LED_Out.
* Description    : Display a byte on the 8 LEDs, LSB on LD3 and MSB on LD10.
* Input          : byte.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LED_Out(uint8_t byte) {
	uint8_t i, tmp = 1;
	for (i = LED3; i <= LED10; i++) {
		if (byte & tmp) {
			STM_EVAL_LEDOn(i);
		} else {
			STM_EVAL_LEDOff(i);
		}
		tmp <<= 1;
	}
}

/*******************************************************************************
* Function Name  : LED_Read.
* Description    : Read out large data.
* Input          : data- Data; count- How many byte? longDelay- Time between out.
* Output         : None.
* Return         : None.
*******************************************************************************/
void LED_Read(uint64_t data, uint8_t count) {
	uint8_t i, j;
	uint64_t temp;
	for (j=0; j<count; j++) {
		temp = data;
		for (i=0; i<count; i++) {
			LED_Out(0xff);
			Delay(500);
			LED_Out((uint8_t)temp);
			Delay(2000);
			temp >>= 8;
		}

		for (i=0; i<3; i++) {
			LED_Out(0xff);
			Delay(50);
			LED_Out(0x00);
			Delay(50);
		}
	}
	for (i=0; i<count; i++) {
		LED_Out(0xff);
		Delay(5);
		LED_Out((uint8_t)data);
		Delay(20);
		data >>= 8;
	}
}

/*******************************************************************************
* Function Name  : But_Pres.
* Description    : Check if button is pressed and how long it is pressed.
* Input          : None.
* Output         : None.
* Return         : Press time in second.
*******************************************************************************/
uint8_t But_Pres(void) {
	uint8_t ButPreHandler(void) {
		timerDelay = 0xffffff;
		uint8_t counter = 0;
		volatile uint32_t timerCheckPoint = timerDelay;

		if (!STM_EVAL_PBGetState(BUTTON_USER)) {
			return 0;							/* Fluck, Ignore */
		}

		LED_Off();
		while (STM_EVAL_PBGetState(BUTTON_USER) && (timerDelay > 0x00)) {
			if ((timerCheckPoint-timerDelay) > 1000) {
				counter ++;
				timerCheckPoint = timerDelay;
				LED_Out(counter);
			}
		}
		LED_Off();

		return counter;
	}
}

/*******************************************************************************
* Function Name  : Fla_Read.
* Description    : Read length data to pdata from id.
* Input          : id- Data id; data- Data; length- Length of the data in 32-bit.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Fla_Read32(uint8_t id, uint32_t* pdata, uint8_t length) {
	uint8_t i;
	uint32_t staAddr = Uint32_Flash_Data_Addr + id * sizeof(uint32_t);

	for (i=0; i<length; i++) {
		memcpy(&pdata[i], (uint32_t*)(Uint32_Flash_Data_Addr + (id + i) * sizeof(uint32_t)), sizeof(uint32_t));
		memcpy(&pdata[i], (uint32_t*)(staAddr + i * sizeof(uint32_t)), sizeof(uint32_t));
	}
}

/*******************************************************************************
* Function Name  : Fla_Write.
* Description    : Write length data pdata to id.
* Input          : id- Data id; pdata- Pointer of data; length- Length of the data.
* Output         : None.
* Return         : FLASH Status.
*******************************************************************************/
uint8_t Fla_Writ32(uint8_t id, uint32_t* pdata, uint8_t length) {
	uint8_t i;
	FLASH_Status res;
	uint32_t staAddr = Uint32_Flash_Data_Addr + id * sizeof(uint32_t);

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
	if (id == 0) {
		FLASH_ErasePage(Uint32_Flash_Data_Addr);
	}

	for (i=0; i<length; i++) {
		res = FLASH_ProgramWord(staAddr + i * sizeof(uint32_t), pdata[i]);
	}


	FLASH_Lock();

	return res;
}

/*******************************************************************************
* Function Name  : L3GD20_TIMEOUT_UserCallback
* Description    : L3GD20 timeout user defined callback.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint32_t L3GD20_TIMEOUT_UserCallback(void) {
	return 0;
}

/*******************************************************************************
* Function Name  : LSM303DLHC_TIMEOUT_UserCallback
* Description    : LSM303DLHC timeout user defined callback.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
uint32_t LSM303DLHC_TIMEOUT_UserCallback(void) {
	return 0;
}
