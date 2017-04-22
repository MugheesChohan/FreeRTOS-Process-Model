# FreeRTOS-Process-Model

This repositry holds code for a Processes Framework for FreeRTOS. This framework has been tested on the Texas Instrument's Tiva C Series TM4C123G Launchpad Evaluation Kit. The source can run on other Cortex-M4 and Cortex-M4F based targets too. However, the Demo folder only contains code for the TM4C123G Kit.

1. The FreeRTOS folder contains the modified FreeRTOS source code with the Processes Framework. The Demo folder contains a demo for Texas Instrument's Tiva C Series TM4C123G Launchpad Evaluation Kit and a file process.proc.h which contains the ELF image of a sample process to load. Copy this complete FreeRTOS folder into an Eclipse C Project and build it.

2. The Process folder contains a simple process (process.c) that the Demo application would load. It also contains a linker description file process.lds for the process and an include folder with necessary include files. Copy the files and folder inside this Process folder into an Eclipse C Project and build it.
