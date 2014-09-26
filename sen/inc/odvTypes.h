/*
 ******************************************************************************
 * @file    odvTypes.h
 * @author  Kailang@DotLab
 * @version V0.0.0
 * @date    04-09-2014
 * @brief   Open DoVision Types
 ******************************************************************************
 * @note
 *
 * Happy to see you here!
 *
 * This file defines all the useful type that may or may not be directly or
 * indirectly used by you and me.
 *
 * And there're only some keywords to remember:
 * In	- HID In Report
 * Out	- HID Out Report
 * Fea	- HID Feature Report
 * Rep	- All kinds of HID Reports
 * These report stuff are little tricky... You can google it to get started.
 * And we are working on some detailed descriptions.
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
#ifndef ODVTYPES_H_
#define ODVTYPES_H_

/* Includes ------------------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define OutData_Size			(uint8_t)			40
#define FeaCheck_Size			(uint8_t)			5
#define FeaRenew_Size			(uint8_t)			5
#define FeaConfig_Size			(uint8_t)			16

/* Exported types ------------------------------------------------------------*/
typedef struct OutDataFrame {	/* EP1 Out Report Types */
	uint8_t		messageType;	/* 0x10 */
	uint16_t	count;
	int8_t		temp;
	int32_t		gyrData[3];
	int32_t		accData[3];
	int32_t		magData[3];
} OutDataFrame;

typedef struct FeaCheckFrame {	/* EP0 Feature Report Types */
	uint8_t		reportType;		/* 0x01 */
	uint32_t	magicWord;
} FeaCheckFrame;
typedef struct FeaRenewFrame {
	uint8_t		reportType;		/* 0x02 */
	uint32_t	magicWord;
} FeaRenewFrame;
typedef struct FeaConfigFrame {
	uint8_t		reportType;		/* 0x03 */
	uint32_t 	dueTime;
	uint16_t	mesInterval;
	uint16_t	gyrScale;
	uint16_t	accScale;
	uint16_t	magScale;
	uint8_t		gyrUnit;
	uint8_t		accUnit;
	uint8_t		magUnit;
} FeaConfigFrame;

/* Exported Union ------------------------------------------------------------*/
typedef union OutDataFrameSep {		/* EP1 Out Report Types */
	uint8_t			rawStream[OutData_Size];	/* 4 + 4 X 3 X 3 */
	OutDataFrame	frame;
} OutDataFrameSep;

typedef union FeaCheckFrameSep {	/* EP0 Feature Report Types */
	uint8_t			rawStream[FeaCheck_Size];	/* 1 + 4 */
	FeaCheckFrame	frame;
} FeaCheckFrameSep;
typedef union FeaRenewFrameSep {
	uint8_t			rawStream[FeaRenew_Size];	/* 1 + 4 */
	FeaRenewFrame	frame;
} FeaRenewFrameSep;
typedef union FeaConfigFrameSep {
	uint8_t			rawStream[FeaConfig_Size];	/* 1 + 4 + 2 X 4 + 3 */
	FeaConfigFrame	frame;
} FeaConfigFrameSep;

/* Exported constants --------------------------------------------------------*/
typedef enum RepType {
	Feature_None	= 0x00,
	Feature_Check	= 0x01,		/* EP0 Feature Report Types */
	Feature_Renew	= 0x02,
	Feature_Config	= 0x03,

	Out_None	= 0x10,			/* EP1 Out Report Types */
	Out_Data	= 0x11,
} RepType;

typedef enum State {
	State_Idle		= 0x00,
	State_Track		= 0x01,
} State;

/* Exported functions ------------------------------------------------------- */

#endif /* ODVTYPES_H_ */
