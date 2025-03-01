;/*****************************************************************************
; * @file:    startup_MKL25Z4.s
; * @purpose: CMSIS Cortex-M4 Core Device Startup File
; *           MKL25Z4
; * @version: 1.0
; * @date:    2014-4-2
; *----------------------------------------------------------------------------
; *
; * www.beyondcore.net
; *
; ******************************************************************************/


;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.
;
; Cortex-M version
;

        MODULE  ?cstartup

        ;; Forward declaration of sections.
        SECTION CSTACK:DATA:NOROOT(3)
        SECTION .intvec:CODE:NOROOT(2)
        EXTERN  __iar_program_start
        EXTERN  SystemInit        
        PUBLIC  __vector_table
        PUBLIC  __Vectors
        PUBLIC  __Vectors_End
        PUBLIC  __Vectors_Size

        DATA

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler

        DCD     NMI_Handler                                   ;NMI Handler
        DCD     HardFault_Handler                             ;Hard Fault Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
__vector_table_0x1c
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     SVC_Handler                                   ;SVCall Handler
        DCD     0                                             ;Reserved
        DCD     0                                             ;Reserved
        DCD     PendSV_Handler                                ;PendSV Handler
        DCD     SysTick_Handler                               ;SysTick Handler

                                                              ;External Interrupts
        DCD     DMA0_IRQHandler                               ;DMA channel 0 transfer complete and error interrupt
        DCD     DMA1_IRQHandler                               ;DMA channel 1 transfer complete and error interrupt
        DCD     DMA2_IRQHandler                               ;DMA channel 2 transfer complete and error interrupt
        DCD     DMA3_IRQHandler                               ;DMA channel 3 transfer complete and error interrupt
        DCD     Reserved20_IRQHandler                         ;Reserved interrupt
        DCD     FTFA_IRQHandler                               ;FTFA command complete and read collision
        DCD     LVD_LVW_IRQHandler                            ;Low-voltage detect, low-voltage warning
        DCD     LLW_IRQHandler                                ;Low Leakage Wakeup
        DCD     I2C0_IRQHandler                               ;I2C0 interrupt
        DCD     I2C1_IRQHandler                               ;I2C1 interrupt
        DCD     SPI0_IRQHandler                               ;SPI0 single interrupt vector for all sources
        DCD     SPI1_IRQHandler                               ;SPI1 single interrupt vector for all sources
        DCD     UART0_IRQHandler                              ;UART0 status and error
        DCD     UART1_IRQHandler                              ;UART1 status and error
        DCD     UART2_IRQHandler                              ;UART2 status and error
        DCD     ADC0_IRQHandler                               ;ADC0 interrupt
        DCD     CMP0_IRQHandler                               ;CMP0 interrupt
        DCD     TPM0_IRQHandler                               ;TPM0 single interrupt vector for all sources
        DCD     TPM1_IRQHandler                               ;TPM1 single interrupt vector for all sources
        DCD     TPM2_IRQHandler                               ;TPM2 single interrupt vector for all sources
        DCD     RTC_IRQHandler                                ;RTC alarm interrupt
        DCD     RTC_Seconds_IRQHandler                        ;RTC seconds interrupt
        DCD     PIT_IRQHandler                                ;PIT single interrupt vector for all channels
        DCD     I2S0_IRQHandler                               ;I2S0 Single interrupt vector for all sources
        DCD     USB0_IRQHandler                               ;USB0 OTG
        DCD     DAC0_IRQHandler                               ;DAC0 interrupt
        DCD     TSI0_IRQHandler                               ;TSI0 interrupt
        DCD     MCG_IRQHandler                                ;MCG interrupt
        DCD     LPTMR0_IRQHandler                             ;LPTMR0 interrupt
        DCD     LCD_IRQHandler                                ;Segment LCD interrupt
        DCD     PORTA_IRQHandler                              ;PORTA pin detect
        DCD     PORTC_PORTD_IRQHandler                        ;Single interrupt vector for PORTC and PORTD pin detect
__Vectors_End
__FlashConfig
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFF
      	DCD	0xFFFFFFFE
__FlashConfig_End

__Vectors       EQU   __vector_table
__Vectors_Size 	EQU 	__Vectors_End - __Vectors


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Default interrupt handlers.
;;
        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0


        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        PUBWEAK DMA0_IRQHandler
        PUBWEAK DMA1_IRQHandler
        PUBWEAK DMA2_IRQHandler
        PUBWEAK DMA3_IRQHandler
        PUBWEAK Reserved20_IRQHandler
        PUBWEAK FTFA_IRQHandler
        PUBWEAK LVD_LVW_IRQHandler
        PUBWEAK LLW_IRQHandler
        PUBWEAK I2C0_IRQHandler
        PUBWEAK I2C1_IRQHandler
        PUBWEAK SPI0_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK UART0_IRQHandler
        PUBWEAK UART1_IRQHandler
        PUBWEAK UART2_IRQHandler
        PUBWEAK ADC0_IRQHandler
        PUBWEAK CMP0_IRQHandler
        PUBWEAK TPM0_IRQHandler
        PUBWEAK TPM1_IRQHandler
        PUBWEAK TPM2_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK RTC_Seconds_IRQHandler
        PUBWEAK PIT_IRQHandler
        PUBWEAK I2S0_IRQHandler
        PUBWEAK USB0_IRQHandler
        PUBWEAK DAC0_IRQHandler
        PUBWEAK TSI0_IRQHandler
        PUBWEAK MCG_IRQHandler
        PUBWEAK LPTMR0_IRQHandler
        PUBWEAK LCD_IRQHandler
        PUBWEAK PORTA_IRQHandler
        PUBWEAK PORTC_PORTD_IRQHandler
        PUBWEAK DefaultISR
        SECTION .text:CODE:REORDER:NOROOT(2)
DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
Reserved20_IRQHandler
FTFA_IRQHandler
LVD_LVW_IRQHandler
LLW_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
UART0_IRQHandler
UART1_IRQHandler
UART2_IRQHandler
ADC0_IRQHandler
CMP0_IRQHandler
TPM0_IRQHandler
TPM1_IRQHandler
TPM2_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT_IRQHandler
I2S0_IRQHandler
USB0_IRQHandler
DAC0_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler
LPTMR0_IRQHandler
LCD_IRQHandler
PORTA_IRQHandler
PORTC_PORTD_IRQHandler
DefaultISR
        LDR R0, =DefaultISR
        BX R0

        END