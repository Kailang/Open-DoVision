Open-DoVision
=============

Open Source Virtual Reality for Everyone

**sen** - Embedded C Project

***

###General Descriptions:

This non-project pieces of code are offered to give you an overview of the key components of the Open DoVision's other real embedded C projects.

In order to start developing the other real projects, you have to download the **lib** folder for it **will not show** in other branshes because of its complexity.

You should include:

**lib/CMSIS/Include**

**lib/CMSIS/Device/ST/STM32F30x/Include**

**lib/STM32_USB-FS-Device_Driver/inc**

**lib/STM32F30x_StdPeriph_Driver/inc**

**lib/STM32F3_Discovery**

The **lib** has all the dependencies the Open DoVision need to work. The licenses of these files are different from the other files written by DotLab. Please go into **lib** for more detailed info.

The **inc** has all the headers define the Open DoVision's data types and runtime data holders. See following sections for more detailed info.

The **src** has all the source files to power the Open DoVision. See following sections for more detailed info.

***

###Folders:

**lib** Dependencies 

**inc** Headers 

**src** Source

***

###Key files:

**odvTypes.h** Defines all the useful type that may or may not be directly or indirectly used by you and me.

**globalVars.h** Use the odvTypes to initiate some data holder for the application.

**peripherals.h/.c** Useful functions for Open DoVision
