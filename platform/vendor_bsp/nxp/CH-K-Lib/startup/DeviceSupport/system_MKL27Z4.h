/*
** ###################################################################
**     Processors:          MKL27Z256VFM4
**                          MKL27Z128VFM4
**                          MKL27Z256VFT4
**                          MKL27Z128VFT4
**                          MKL27Z256VLH4
**                          MKL27Z128VLH4
**                          MKL27Z256VMP4
**                          MKL27Z128VMP4
**
**     Compilers:           Keil ARM C/C++ Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          GNU C Compiler - CodeSourcery Sourcery G++
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    KL27P64M48SF6RM, Rev.3, Aug 2014
**     Version:             rev. 1.3, 2014-08-21
**     Build:               b140821
**
**     Abstract:
**         CMSIS Peripheral Access Layer for MKL27Z4
**
**     Copyright (c) 1997 - 2014 Freescale Semiconductor, Inc.
**     All rights reserved.
**
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2014-03-27)
**         Initial version.
**     - rev. 1.1 (2014-05-26)
**         I2S registers TCR2/RCR2 and others were changed.
**         FLEXIO register FLEXIO_VERID has now bitfields: FEATURE, MINOR, MAJOR.
**         Names of the bitfields of the FLEXIO_SHIFTBUF have been changed to the appropriate register name e.g.: FLEXIO_SHIFTBUFBBS_SHIFTBUFBBS.
**         Peripheral_BASES macros has been changed to Peripheral_BASE_PTRS, e.g.: ADC_BASES to ADC_BASE_PTRS.
**         Clock configuration for high range external oscillator has been added.
**         RFSYS module access has been added.
**     - rev. 1.2 (2014-07-10)
**         GPIO - Renamed modules PTA,PTB,PTC,PTD,PTE to GPIOA,GPIOB,GPIOC,GPIOD,GPIOE.
**         UART0 - UART0 module renamed to UART2.
**         I2S - removed MDR register.
**     - rev. 1.3 (2014-08-21)
**         UART2 - Removed ED register.
**         UART2 - Removed MODEM register.
**         UART2 - Removed IR register.
**         UART2 - Removed PFIFO register.
**         UART2 - Removed CFIFO register.
**         UART2 - Removed SFIFO register.
**         UART2 - Removed TWFIFO register.
**         UART2 - Removed TCFIFO register.
**         UART2 - Removed RWFIFO register.
**         UART2 - Removed RCFIFO register.
**         USB - Removed bitfield REG_EN in CLK_RECOVER_IRC_EN register.
**         SIM - Changed bitfield value MCGIRCLK to LIRC_CLK of bitfield CLKOUTSEL in SOPT2 register.
**         SIM - Removed bitfield DIEID in SDID register.
**
** ###################################################################
*/

/*!
 * @file MKL27Z4
 * @version 1.3
 * @date 2014-08-21
 * @brief Device specific configuration file for MKL27Z4 (header file)
 *
 * Provides a system configuration function and a global variable that contains
 * the system frequency. It configures the device and initializes the oscillator
 * (PLL) that is part of the microcontroller device.
 */

#ifndef SYSTEM_MKL27Z4_H_
#define SYSTEM_MKL27Z4_H_                        /**< Symbol preventing repeated inclusion */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define DISABLE_WDOG    1

#ifndef CLOCK_SETUP
  #define CLOCK_SETUP   1
#endif
/* Predefined clock setups
   0 ... Multipurpose Clock Generator Lite (MCG_Lite) in Low-frequency Internal Reference Clock 8 MHz (LIRC 8 MHz) mode
         Default part configuration.
         Core clock/Bus clock derived from the internal clock source 8 MHz
         Core clock = 4MHz, BusClock = 2MHz, USB FS clock derived from external clock USB_CLKIN (applicable only for derivatived with USB)
   1 ... Multipurpose Clock Generator Lite (MCG_Lite) in High-frequency Internal Reference Clock (HIRC) mode
         Maximum achievable clock frequency configuration using internal clock.
         Core clock/Bus clock derived from the internal clock source 48MHz
         Core clock = 48MHz, BusClock = 24MHz, USB FS clock derived from external clock USB_CLKIN (applicable only for derivatived with USB)
   2 ... Multipurpose Clock Generator Lite (MCG_Lite) in External Oscillator (EXT) mode
         Core clock/Bus clock derived directly from the external crystal 32.768kHz
         The clock settings is ready for Very Low Power Run mode.
         Core clock = 32.768kHz, BusClock = 32.768kHz, USB FS clock derived from external clock USB_CLKIN (applicable only for derivatived with USB)
   3 ... Multipurpose Clock Generator Lite (MCG_Lite) in Low-frequency Internal Reference Clock 2 MHz (LIRC 2 MHz) mode
         Core clock/Bus clock derived from the internal clock source 2 MHz
         The clock settings is ready for Very Low Power Run mode.
         Core clock = 2MHz, BusClock = 1MHz, USB FS clock derived from external clock USB_CLKIN (applicable only for derivatived with USB)
   4 ... Multipurpose Clock Generator Lite (MCG_Lite) in High-frequency Internal Reference Clock (HIRC) mode
         USB clock setup - for USB to receive internal 48MHz clock derived from HIRC.
         Core clock/Bus clock derived from the internal clock source 48MHz
         Core clock = 48MHz, BusClock = 24MHz, USB FS clock derived from HIRC (MCGPCLK)
   5 ... Multipurpose Clock Generator Lite (MCG_Lite) in External Oscillator (EXT) mode
         Core clock/Bus clock derived directly from the external crystal 8 MHz
         Core clock = 8MHz, BusClock = 4MHz, USB FS clock derived from external clock USB_CLKIN (applicable only for derivatived with USB)
*/

/*----------------------------------------------------------------------------
  Define clock source values
 *----------------------------------------------------------------------------*/
#if (CLOCK_SETUP == 0)
    #define CPU_XTAL_CLK_HZ                 32768u    /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             8000000u  /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             48000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            4000000u  /* Default System clock value */
#elif (CLOCK_SETUP == 1)
    #define CPU_XTAL_CLK_HZ                 32768u    /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             8000000u  /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             48000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            48000000u /* Default System clock value */
#elif (CLOCK_SETUP == 2)
    #define CPU_XTAL_CLK_HZ                 32768u    /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             8000000u  /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             48000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            32768u    /* Default System clock value */
#elif (CLOCK_SETUP == 3)
    #define CPU_XTAL_CLK_HZ                 32768u    /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             2000000u  /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             48000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            2000000u  /* Default System clock value */
#elif (CLOCK_SETUP == 4)
    #define CPU_XTAL_CLK_HZ                 32768u    /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             8000000u  /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             48000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            48000000u /* Default System clock value */
#elif (CLOCK_SETUP == 5)
    #define CPU_XTAL_CLK_HZ                 8000000u  /* Value of the external crystal or oscillator clock frequency in Hz */
    #define CPU_INT_SLOW_CLK_HZ             8000000u  /* Value of the slow internal oscillator clock frequency in Hz  */
    #define CPU_INT_FAST_CLK_HZ             48000000u /* Value of the fast internal oscillator clock frequency in Hz  */
    #define DEFAULT_SYSTEM_CLOCK            8000000u    /* Default System clock value */
#endif /* (CLOCK_SETUP == 5) */


/**
 * @brief System clock frequency (core clock)
 *
 * The system clock frequency supplied to the SysTick timer and the processor
 * core clock. This variable can be used by the user application to setup the
 * SysTick timer or configure other parameters. It may also be used by debugger to
 * query the frequency of the debug timer or configure the trace clock speed
 * SystemCoreClock is initialized with a correct predefined value.
 */
extern uint32_t SystemCoreClock;

/**
 * @brief Setup the microcontroller system.
 *
 * Typically this function configures the oscillator (PLL) that is part of the
 * microcontroller device. For systems with variable clock speed it also updates
 * the variable SystemCoreClock. SystemInit is called from startup_device file.
 */
void SystemInit (void);

/**
 * @brief Updates the SystemCoreClock variable.
 *
 * It must be called whenever the core clock is changed during program
 * execution. SystemCoreClockUpdate() evaluates the clock register settings and calculates
 * the current core clock.
 */
void SystemCoreClockUpdate (void);

#ifdef __cplusplus
}
#endif

#endif  /* #if !defined(SYSTEM_MKL27Z4_H_) */
