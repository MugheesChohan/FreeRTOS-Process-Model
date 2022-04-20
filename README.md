# FreeRTOS-Process-Model

This repositry holds code for a Processes Framework for FreeRTOS. FreeRTOS is a popular Royalty Free and Open Source Real Time Operating System (RTOS) for use in Embedded Systems. Its low footprint is one of the key features that makes it the ultimate choice for memory constrained Embedded Systems. The aim of this research is to develop a processes framework for FreeRTOS to provide support for dynamic loading and unloading of processes at run-time. Some added benefits this process support will provide are Efficient Utilization of Memory resources, Memory Protection, Kernel Reliability and In-field updating of software. Special care will be taken to keep the footprint of the process framework support as low as possible while still providing basic functionalities of the processes. This framework has been tested on the Texas Instrument's Tiva C Series TM4C123G Launchpad Evaluation Kit. The source can run on other Cortex-M4 and Cortex-M4F based targets too. However, the Demo folder inside this folder only contains code for the TM4C123G Kit.

1. The FreeRTOS folder contains the modified FreeRTOS source code with the Processes Framework. The Demo folder contains a demo for Texas Instrument's Tiva C Series TM4C123G Launchpad Evaluation Kit and a file process.proc.h which contains the ELF image of a sample process to load. Copy this complete FreeRTOS folder into an Eclipse C Project and build it.

2. The Process folder contains a simple process (process.c) that the Demo application would load. It also contains a linker description file process.lds for the process and an include folder with necessary include files. Copy the files and folder inside this Process folder into an Eclipse C Project and build it.
