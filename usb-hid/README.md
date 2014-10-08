Open-DoVision
=============

Open Source Virtual Reality for Everyone

**usb-hid** - The hid project written for Open DoVision

***

###General Descriptions:

**Embedded C Project** Embedded C project which makes the Open DoVision a HID

**Application** N/A

**SDK** N/A

With a little work with the usb, we change the Open DoVision to a HID which can sent out data to the PC

The libraries that are not developed by DotLab are provided under their own licence


***

###Folders:

**sen** Embedded C Project => **lib** Dependencies **inc** Headers **src** source

**app** PC Application

**sdk** SDK

***

###Key files:

**odvTypes.h** Defines all the useful type that may or may not be directly or indirectly used

**globalVars.h** Use the odvTypes to initiate some data holder for the application

**peripherals.h/.c** Useful functions for Open DoVision

**main.h/.c** Init device, get data, pack data! send data
