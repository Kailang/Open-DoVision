/**
 ******************************************************************************
 * @file    USB_Example/usb_prop.c
 * @author  MCD Application Team
 * @version V1.1.0
 * @date    20-September-2012
 * @brief   All processing related to Demo
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
 *
 * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "globalVars.h"
#include "odvTypes.h"
#include "string.h"
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;						/* Something from the ST */

/* Structures initializations ------------------------------------------------*/
DEVICE Device_Table = { EP_NUM, 1 };

DEVICE_PROP Device_Property = { Hid_init, Hid_Reset, Hid_Status_In,
		Hid_Status_Out, Hid_Data_Setup, Hid_NoData_Setup,
		Hid_Get_Interface_Setting, Hid_GetDeviceDescriptor,
		Hid_GetConfigDescriptor, Hid_GetStringDescriptor, 0,
		USB_PACKET_SIZE /*MAX PACKET SIZE*/
};
USER_STANDARD_REQUESTS User_Standard_Requests = {
Hid_GetConfiguration, Hid_SetConfiguration,
Hid_GetInterface,
Hid_SetInterface,
Hid_GetStatus,
Hid_ClearFeature,
Hid_SetEndPointFeature,
Hid_SetDeviceFeature, Hid_SetDeviceAddress };

ONE_DESCRIPTOR Device_Descriptor = { (uint8_t*) Hid_DeviceDescriptor,
HID_SIZ_DEVICE_DESC };

ONE_DESCRIPTOR Config_Descriptor = { (uint8_t*) Hid_ConfigDescriptor,
HID_SIZ_CONFIG_DESC };

ONE_DESCRIPTOR Hid_Report_Descriptor = { (uint8_t *) Hid_ReportDescriptor,
HID_SIZ_REPORT_DESC };

ONE_DESCRIPTOR Mouse_Hid_Descriptor = { (uint8_t*) Hid_ConfigDescriptor
		+ HID_OFF_HID_DESC,
HID_SIZ_HID_DESC };

ONE_DESCRIPTOR String_Descriptor[4] = { { (uint8_t*) Hid_StringLangID,
HID_SIZ_STRING_LANGID }, { (uint8_t*) Hid_StringVendor,
HID_SIZ_STRING_VENDOR }, { (uint8_t*) Hid_StringProduct,
HID_SIZ_STRING_PRODUCT }, { (uint8_t*) Hid_StringSerial,
HID_SIZ_STRING_SERIAL } };

/* Extern variables ----------------------------------------------------------*/
__IO int8_t EP0FeaState = -1;				/* EP0 Feature Report Indicator -1 No Known Feature Report */

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Hid init routine.
 * @param  None
 * @retval None
 */
void Hid_init(void) {
	/* Update the serial number string descriptor with the data from the unique ID*/
	Get_SerialNum();

	pInformation->Current_Configuration = 0;
	/* Connect the device */
	PowerOn();

	/* Perform basic device initialization operations */
	USB_SIL_Init();

	bDeviceState = UNCONNECTED;
}

/**
 * @brief  Hid reset routine.
 * @param  None
 * @retval None
 */
void Hid_Reset(void) {
	/* Set Hid_DEVICE as not configured */
	pInformation->Current_Configuration = 0;
	pInformation->Current_Interface = 0;/*the default Interface*/

	/* Current Feature initialization */
	pInformation->Current_Feature = Hid_ConfigDescriptor[7];

	SetBTABLE(BTABLE_ADDRESS);

	/* Initialize Endpoint 0 */
	SetEPType(ENDP0, EP_CONTROL);
	SetEPTxStatus(ENDP0, EP_TX_STALL);
	SetEPRxAddr(ENDP0, ENDP0_RXADDR);
	SetEPTxAddr(ENDP0, ENDP0_TXADDR);
	Clear_Status_Out(ENDP0);
	SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
	SetEPRxValid(ENDP0);

	/* Initialize Endpoint 1 */
	SetEPType(ENDP1, EP_INTERRUPT);
	SetEPTxAddr(ENDP1, ENDP1_TXADDR);
	SetEPRxAddr(ENDP1, ENDP1_RXADDR);
	SetEPTxCount(ENDP1, USB_PACKET_SIZE);
	SetEPRxCount(ENDP1, USB_PACKET_SIZE);
	SetEPRxStatus(ENDP1, EP_RX_VALID);
	SetEPTxStatus(ENDP1, EP_TX_NAK);

	/* Set this device to response on default address */
	SetDeviceAddress(0);

	bDeviceState = ATTACHED;
}

/**
 * @brief  Update the device state to configured.
 * @param  None
 * @retval None
 */
void Hid_SetConfiguration(void) {
	DEVICE_INFO *pInfo = &Device_Info;

	if (pInfo->Current_Configuration != 0) {
		/* Device configured */
		bDeviceState = CONFIGURED;
	}
}

/**
 * @brief  Update the device state to addressed.
 * @param  None
 * @retval None
 */
void Hid_SetDeviceAddress(void) {
	bDeviceState = ADDRESSED;
}

/*******************************************************************************
 * Function Name  : Hid_Status_In.
 * Description    : Called during CONFIGURATION REQUESTS only.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Hid_Status_In(void) {
}

/*******************************************************************************
 * Function Name  : Hid_Status_In.
 * Description    : Called on every Control_IN transfer (when data goes to the HOST)
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void Hid_Status_Out(void) {
}

/**
 * @brief  Handle the data class specific requests.
 * @param  RequestNo: Request Nb.
 * @retval USB_UNSUPPORT or USB_SUCCESS.
 */
RESULT Hid_Data_Setup(uint8_t RequestNo) {
	uint8_t *(*CopyRoutine)(uint16_t);
	CopyRoutine = NULL;
	/* Handle GET_DESCRIPTOR Request on Control Endpoint 0 */
	if ((Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
			&& (RequestNo == GET_DESCRIPTOR)
			&& (pInformation->USBwIndex0 == 0)) {
		if (pInformation->USBwValue1 == REPORT_DESCRIPTOR) {
			CopyRoutine = Hid_GetReportDescriptor;
		} else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE) {
			CopyRoutine = Hid_GetHIDDescriptor;
		}
	} /* End of GET_DESCRIPTOR */


	/* Handle All HID Class Request on Endpoint 0 */

	else if ((pInformation->USBbmRequestType & 0x60) == CLASS_REQUEST) {//| INTERFACE_RECIPIENT
		switch (RequestNo) {
		case GET_PROTOCOL:
			CopyRoutine = Hid_GetProtocolValue;
			break;
		case GET_REPORT:								/*<!The Set and Get Is on the PC Perspective!>*/
			/* Send Feature Report to PC */
			CopyRoutine = HID_GetReport_Feature;
			break;
		case SET_REPORT:								/*<!The Set and Get Is on the PC Perspective!>*/
			/* Receive Feature Report from PC */
			CopyRoutine = HID_SetReport_Feature;
			break;
		}
	}

	if (CopyRoutine == NULL) {
		return USB_UNSUPPORT;
	}

	pInformation->Ctrl_Info.CopyData = CopyRoutine;
	pInformation->Ctrl_Info.Usb_wOffset = 0;
	(*CopyRoutine)(0);
	return USB_SUCCESS;
}

/*******************************************************************************
 * Function Name  : HID_SetReport_Feature
 * Description    : Called when a USB packet has been received completely.
 * Input          : Length.
 * Output         : None.
 * Return         : Buffer.
 *******************************************************************************/
uint8_t *HID_SetReport_Feature(uint16_t Length) {
	uint8_t nBytesInThisRequest = 0, requestwValueL;
	__IO uint8_t *pointerToBuffer;

	requestwValueL = pInformation->USBwValue0;				/* The low byte of wValue of a setup packet */

	switch (requestwValueL) {								/* Recognize the Feature Report */
	case Feature_Check:
	case Feature_Renew:
	case Feature_Config:
		pointerToBuffer = feaRawStream;
		EP0FeaState = requestwValueL;						/* Renew the Feature Report State */

		nBytesInThisRequest = 128;
		break;
	default:												/* Unknown Feature Report */
		pointerToBuffer = EP0Buffer;
		EP0FeaState = -1;									/* Ignore the Unknown */

		nBytesInThisRequest = 128;
	}

	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = nBytesInThisRequest;
		return NULL;
	} else {
		return (uint8_t*) pointerToBuffer;
	}
}

/*******************************************************************************
 * Function Name  : HID_GetReport_Feature
 * Description    : Send a number of _bytesToReturn_ bytes over USB.
 * Input          : Length.
 * Output         : None.
 * Return         : Buffer.
 *******************************************************************************/
uint8_t *HID_GetReport_Feature(uint16_t Length) {
	uint8_t nBytesInThisRequest = 0, requestwValueL;
	__IO uint8_t *pointerToBuffer;

	requestwValueL = pInformation->USBwValue0;				/* The low byte of wValue of a setup packet */

	switch (requestwValueL) {
	case Feature_Check:
		FeaPacker(Feature_Check);
		pointerToBuffer = feaCheck.rawStream;
		nBytesInThisRequest = sizeof(feaCheck) - 1;
		break;
	case Feature_Renew:
		FeaPacker(Feature_Renew);
		pointerToBuffer = feaRenew.rawStream;
		nBytesInThisRequest = sizeof(feaRenew) - 1;
		break;
	case Feature_Config:
		FeaPacker(Feature_Config);
		pointerToBuffer = feaConfig.rawStream;
		nBytesInThisRequest = sizeof(feaConfig) - 1;
		break;
	default:
		pointerToBuffer = EP0Buffer;						/* Unknown To Unknown */
		nBytesInThisRequest = sizeof(EP0Buffer) - 1;
	}

	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = nBytesInThisRequest;
		return NULL;
	} else {
		return (uint8_t*) pointerToBuffer;
	}
}

/**
 * @brief  Handle the no data class specific requests
 * @param  RequestNo: Request Nb.
 * @retval USB_UNSUPPORT or USB_SUCCESS.
 */
RESULT Hid_NoData_Setup(uint8_t RequestNo) {
	if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
			&& (RequestNo == SET_PROTOCOL)) {
		return Hid_SetProtocol();
	}

	else {
		return USB_UNSUPPORT;
	}
}

/**
 * @brief  Gets the device descriptor.
 * @param  Length: Length.
 * @retval The address of the device descriptor.
 */
uint8_t *Hid_GetDeviceDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/**
 * @brief  Gets the configuration descriptor.
 * @param  Length: Length.
 * @retval The address of the configuration descriptor.
 */
uint8_t *Hid_GetConfigDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/**
 * @brief  Gets the string descriptors according to the needed index
 * @param  Length: Length.
 * @retval The address of the string descriptors.
 */
uint8_t *Hid_GetStringDescriptor(uint16_t Length) {
	uint8_t wValue0 = pInformation->USBwValue0;
	if (wValue0 > 4) {
		return NULL;
	} else {
		return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
	}
}

/**
 * @brief  Gets the HID report descriptor.
 * @param  Length: Length.
 * @retval The address of the configuration descriptor.
 */
uint8_t *Hid_GetReportDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Hid_Report_Descriptor);
}

/**
 * @brief  Gets the HID descriptor.
 * @param  Length: Length.
 * @retval The address of the configuration descriptor.
 */
uint8_t *Hid_GetHIDDescriptor(uint16_t Length) {
	return Standard_GetDescriptorData(Length, &Mouse_Hid_Descriptor);
}

/**
 * @brief  tests the interface and the alternate setting according to the supported one.
 * @param  Interface: interface number.
 * @param  AlternateSetting : Alternate Setting number.
 * @retval USB_SUCCESS or USB_UNSUPPORT.
 */
RESULT Hid_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting) {
	if (AlternateSetting > 0) {
		return USB_UNSUPPORT;
	} else if (Interface > 0) {
		return USB_UNSUPPORT;
	}
	return USB_SUCCESS;
}

/**
 * @brief  Hid Set Protocol request routine.
 * @param  None.
 * @retval USB_SUCCESS
 */
RESULT Hid_SetProtocol(void) {
	uint8_t wValue0 = pInformation->USBwValue0;
	ProtocolValue = wValue0;
	return USB_SUCCESS;
}

/**
 * @brief  Get the protocol value
 * @param  Length: length
 * @retval address of the protocol value.
 */
uint8_t *Hid_GetProtocolValue(uint16_t Length) {
	if (Length == 0) {
		pInformation->Ctrl_Info.Usb_wLength = 1;
		return NULL;
	} else {
		return (uint8_t *) (&ProtocolValue);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
