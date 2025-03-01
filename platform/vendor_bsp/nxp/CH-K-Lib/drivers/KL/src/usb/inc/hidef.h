/******************************************************/
/**
* @file hidef.h
* Machine/compiler dependent declarations.
*/
/*----------------------------------------------------
   Copyright (c) Freescale DevTech
               All rights reserved
                  Do not modify!
 *****************************************************/

#ifndef _H_HIDEF_
#define _H_HIDEF_

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(__CWCC__)
	#pragma gcc_extensions on

	/**** Version for ColFire V1 */
	#include <stddef.h>
	#include "types.h"

	/*!< Macro to enable all interrupts. */
	#define EnableInterrupts asm ("CPSIE  i")
  
	/*!< Macro to disable all interrupts. */
	#define DisableInterrupts asm ("CPSID  i")
  
#elif defined(__IAR_SYSTEMS_ICC__)
	#include <intrinsics.h>

	#define EnableInterrupts  __enable_interrupt();
	#define DisableInterrupts __disable_interrupt();

#elif defined (__CC_ARM)
    #ifndef EnableInterrupts
	#define EnableInterrupts  __enable_irq();
    #endif
    #ifndef DisableInterrupts
	#define DisableInterrupts __disable_irq();
    #endif
#elif defined (__GNUC__)
	#include <stddef.h>
	#include "types.h"
	
	/*!< Macro to enable all interrupts. */
	#define EnableInterrupts asm ("CPSIE  i")
	
	/*!< Macro to disable all interrupts. */
	#define DisableInterrupts asm ("CPSID  i")
#endif

#ifdef __cplusplus
	}
#endif

#endif

/*****************************************************/
/* end hidef.h */
