/**
  ******************************************************************************
  * @file    USB_Example/usb_desc.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Descriptors for Demo
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
#include "usb_desc.h"
#include "odvTypes.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB Standard Device Descriptor */
const uint8_t Hid_DeviceDescriptor[HID_SIZ_DEVICE_DESC] =
  {
    0x12,                       /*bLength */
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType*/
    0x00,                       /*bcdUSB */
    0x02,
    0x00,                       /*bDeviceClass*/
    0x00,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    0x40,                       /*bMaxPacketSize 64*/
    0x21,                       /*idVendor DotLab (0x6021)*/
    0x60,
    0x01,                       /*idProduct ODV (0x0001)*/
    0x00,
    0x01,                       /*bcdDevice (or version) 0x0001 */
    0x00,
    1,                          /*Index of string descriptor describing
                                                  manufacturer */
    2,                          /*Index of string descriptor describing
                                                 product*/
    3,                          /*Index of string descriptor describing the
                                                 device serial number */
    0x01                        /*bNumConfigurations*/
  }
  ; /* Hid_DeviceDescriptor */


/* USB Configuration Descriptor */
/*   All Descriptors (Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t Hid_ConfigDescriptor[HID_SIZ_CONFIG_DESC] =
  {
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    HID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x00,         /*iConfiguration: Index of string descriptor describing
                                     the configuration*/
    0xE0,         /*bmAttributes: bus powered */
    0xFA,         /*MaxPower 500 mA: this current is used for detecting Vbus*/

    /************** Descriptor of Hid RIFT interface ****************/
    /* 09 */
    0x09,         /*bLength: Interface Descriptor size*/
    USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
    0x00,         /*bInterfaceNumber: Number of Interface*/
    0x00,         /*bAlternateSetting: Alternate setting*/
    0x02,         /*bNumEndpoints*/
    0x03,         /*bInterfaceClass: HID*/
    0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,            /*iInterface: Index of string descriptor*/
    /******************** Descriptor of Hid RIFT HID ********************/
    /* 18 */
    0x09,         /*bLength: HID Descriptor size*/
    HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
    0x11,         /*bcdHID: HID Class Spec release number*/
    0x01,
    0x00,         /*bCountryCode: Hardware target country*/
    0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
    0x22,         /*bDescriptorType*/
    HID_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
    0x00,
    /******************** Descriptor of Hid RIFT endpoint ********************/
	/* Defines a interrupt in and out endpoint */
    /* 27 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x81,          /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    64,            /*wMaxPacketSize: 64 Byte max */
    0x00,
    0x01,          /*bInterval: Polling Interval (1 ms)*/
    /* 34 */
    0x07,          /*bLength: Endpoint Descriptor size*/
    USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/

    0x01,          /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03,          /*bmAttributes: Interrupt endpoint*/
    USB_PACKET_SIZE,          /*wMaxPacketSize: Byte max */
    0x00,
    0x80,          /*bInterval: Polling Interval (128 ms)*/
    /* 41 */
  }; /* RIFT_ConfigDescriptor */

const uint8_t Hid_ReportDescriptor[HID_SIZ_REPORT_DESC] =
  {
		    0x06, 0x00, 0xff,             	 /* USAGE_PAGE (Vendor Defined Page 1) */
		    0x09, 0x01,                      /* USAGE (Vendor Usage 1) */
		    0xa1, 0x01,                      /* COLLECTION (Application) */
		    /* 7 */
		    0x15, 0x00,                        /* LOGICAL_MINIMUM (0) */
		    0x26, 0xff, 0x00,                  /* LOGICAL_MAXIMUM (255) */
		    0x75, 0x08,                        /* REPORT_SIZE (8) */
		    /* 14 */
		    0x85, 0x01,                        /* REPORT_ID (0x01) */
		    0x95, (FeaCheck_Size-1),           /* REPORT_COUNT */
		    0x09, 0x01,                        /* USAGE (Feature_Check) */
		    0xb2, 0x02, 0x01,                  /* FEATURE (Data,Var,Abs,Buf) */
		    /* 23 */
		    0x85, 0x02,                        /* REPORT_ID (0x02) */
		    0x95, (FeaRenew_Size-1),           /* REPORT_COUNT */
		    0x09, 0x02,                        /* USAGE (Feature_Renew) */
		    0xb2, 0x02, 0x01,                  /* FEATURE (Data,Var,Abs,Buf) */
		    /* 32 */
		    0x85, 0x03,                        /* REPORT_ID (0x03) */
		    0x95, (FeaConfig_Size-1),          /* REPORT_COUNT */
		    0x09, 0x03,                        /* USAGE (Feature_Config) */
		    0xb2, 0x02, 0x01,                  /* FEATURE (Data,Var,Abs,Buf) */
		    /* 41 */
		    0x85, 0x11,                        /* REPORT_ID (0x11) */
		    0x95, (OutData_Size-1),            /* REPORT_COUNT */
		    0x09, 0x11,                        /* USAGE (Out_Data) */
		    0x82, 0x02, 0x01,                  /* INPUT (Data,Var,Abs,Buf) */
		    /* 50 */
		    0xc0                             /* END_COLLECTION */
  }  ; /* Hid_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t Hid_StringLangID[HID_SIZ_STRING_LANGID] =
  {
    HID_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
  }
  ; /* LangID = 0x0409: U.S. English */

const uint8_t Hid_StringVendor[HID_SIZ_STRING_VENDOR] =
  {
    HID_SIZ_STRING_VENDOR, /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,  /* bDescriptorType*/
    /* Manufacturer: DotLab */
    'D', 0, 'o', 0, 't', 0, 'L', 0, 'a', 0, 'b', 0
  };

const uint8_t Hid_StringProduct[HID_SIZ_STRING_PRODUCT] =
  {
    HID_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product: ODV */
    'O', 0, 'D', 0, 'V', 0
  };
uint8_t Hid_StringSerial[HID_SIZ_STRING_SERIAL] =
  {
    HID_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Serial: 0x00 */
    '0', 0, 'X', 0, 'O', 0, 'O', 0
  };


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
