/** ****************************************************************************
 * @copyright                  Nationz Co.Ltd
 *               Copyright (c) 2013��2018 All Rights Reserved
 *******************************************************************************
 * @file     am_hal_itm.c
 * @author   
 * @date     
 * @version  v1.0
 * @brief    Functions for operating the instrumentation trace macrocell
 *
 * @addtogroup hal Hardware Abstraction Layer (HAL)
 * @addtogroup itm Instrumentation Trace Macrocell (ITM)
 * @ingroup hal
 * @{
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "am_mcu_Z32HLP.h"

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************


//*****************************************************************************
//
//! @brief Delays for a desired amount of microseconds.
//!
//! @note - This function is based on the similar function in am_util_delay.c,
//! please see that module for implementation details. It was necessary to
//! duplicate it here to avoid having to update every example to include the
//! am_util_delay.c module in its build.
//!
//! @returns None
//
//*****************************************************************************
void
am_hal_itm_delay_us(uint32_t ui32MicroSeconds)
{
    uint32_t ui32Cycles = ui32MicroSeconds *
                          (am_hal_clkgen_sysclk_get() / 3000000);

    //
    // Call the cycle delay
    //
    am_hal_flash_delay(ui32Cycles);
}

//*****************************************************************************
//
//! @brief Enables the ITM
//!
//! This function enables the ARM ITM by setting the TRCENA bit in the DEMCR
//! register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_enable(void)
{
    if (g_ui32HALflags & AM_HAL_FLAGS_ITMSKIPENABLEDISABLE_M)
    {
        return;
    }

    //
    // To be able to access ITM registers, set the Trace Enable bit
    // in the Debug Exception and Monitor Control Register (DEMCR).
    //
    AM_REG(SYSCTRL, DEMCR) |= AM_REG_SYSCTRL_DEMCR_TRCENA(1);
    while ( !(AM_REG(SYSCTRL, DEMCR) & AM_REG_SYSCTRL_DEMCR_TRCENA(1)) );

    //
    // Write the key to the ITM Lock Access register to unlock the ITM_TCR.
    //
    AM_REGVAL(AM_REG_ITM_LOCKAREG_O) = AM_REG_ITM_LOCKAREG_KEYVAL;

    //
    // Set the enable bits in the ITM trace enable register, and the ITM
    // control registers to enable trace data output.
    //
    AM_REGVAL(AM_REG_ITM_TPR_O) = 0x0000000f;
    AM_REGVAL(AM_REG_ITM_TER_O) = 0xffffffff;

    //
    // Write to the ITM control and status register (don't enable yet).
    //
    AM_REGVAL(AM_REG_ITM_TCR_O) =
        AM_WRITE_SM(AM_REG_ITM_TCR_ATB_ID, 0x15)      |
        AM_WRITE_SM(AM_REG_ITM_TCR_TS_FREQ, 1)        |
        AM_WRITE_SM(AM_REG_ITM_TCR_TS_PRESCALE, 1)    |
        AM_WRITE_SM(AM_REG_ITM_TCR_SWV_ENABLE, 1)     |
        AM_WRITE_SM(AM_REG_ITM_TCR_DWT_ENABLE, 0)     |
        AM_WRITE_SM(AM_REG_ITM_TCR_SYNC_ENABLE, 0)    |
        AM_WRITE_SM(AM_REG_ITM_TCR_TS_ENABLE, 0)      |
        AM_WRITE_SM(AM_REG_ITM_TCR_ITM_ENABLE, 1);
}

//*****************************************************************************
//
//! @brief Disables the ITM
//!
//! This function completely disables the ARM ITM by resetting the TRCENA bit
//! in the DEMCR register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_disable(void)
{

    if (g_ui32HALflags & AM_HAL_FLAGS_ITMSKIPENABLEDISABLE_M)
    {
        return;
    }

    //
    // Make sure the ITM_TCR is unlocked.
    //
    AM_REGVAL(AM_REG_ITM_LOCKAREG_O) = AM_REG_ITM_LOCKAREG_KEYVAL;

    //
    // Make sure the ITM/TPIU is not busy.
    //
    while ( AM_REG(ITM, TCR) & AM_REG_ITM_TCR_BUSY(1) );

    //
    // Disable the ITM.
    //
    for (int ix = 0; ix < 100; ix++)
    {
        AM_REG(ITM, TCR) &= ~AM_REG_ITM_TCR_ITM_ENABLE(1);
        while ( AM_REG(ITM, TCR) & (AM_REG_ITM_TCR_ITM_ENABLE(1) | AM_REG_ITM_TCR_BUSY(1)) );
    }

    //
    // Reset the TRCENA bit in the DEMCR register, which should disable the ITM
    // for operation.
    //
    AM_REG(SYSCTRL, DEMCR) &= ~AM_REG_SYSCTRL_DEMCR_TRCENA(1);

    //
    // Disable the TPIU clock source in MCU control.
    //
    AM_REG(MCUCTRL, TPIUCTRL) = AM_REG_MCUCTRL_TPIUCTRL_CLKSEL_0MHz |
                                AM_REG_MCUCTRL_TPIUCTRL_ENABLE_DIS;
}

//*****************************************************************************
//
//! @brief Checks if itm is busy and provides a delay to fluch the fifo
//!
//! This function disables the ARM ITM by resetting the TRCENA bit in the DEMCR
//! register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_not_busy(void)
{
    //
    // Make sure the ITM/TPIU is not busy.
    //
    while (AM_REG(ITM, TCR) & AM_REG_ITM_TCR_BUSY(1));

    // wait for 50us for the data to flush out
    am_hal_itm_delay_us(50);
}

//*****************************************************************************
//
//! @brief Enables tracing on a given set of ITM ports
//!
//! @param ui8portNum - Set ports to be enabled
//!
//! Enables tracing on the ports referred to by \e ui8portNum by writing the
//! associated bit in the Trace Privilege Register in the ITM. The value for
//! ui8portNum should be the logical OR one or more of the following values:
//!
//! \e ITM_PRIVMASK_0_7 - enable ports 0 through 7
//! \e ITM_PRIVMASK_8_15 - enable ports 8 through 15
//! \e ITM_PRIVMASK_16_23 - enable ports 16 through 23
//! \e ITM_PRIVMASK_24_31 - enable ports 24 through 31
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_trace_port_enable(uint8_t ui8portNum)
{
    AM_REGVAL(AM_REG_ITM_TPR_O) |= (0x00000001 << (ui8portNum>>3));
}

//*****************************************************************************
//
//! @brief Disable tracing on the given ITM stimulus port.
//!
//! @param ui8portNum
//!
//! Disables tracing on the ports referred to by \e ui8portNum by writing the
//! associated bit in the Trace Privilege Register in the ITM. The value for
//! ui8portNum should be the logical OR one or more of the following values:
//!
//! \e ITM_PRIVMASK_0_7 - disable ports 0 through 7
//! \e ITM_PRIVMASK_8_15 - disable ports 8 through 15
//! \e ITM_PRIVMASK_16_23 - disable ports 16 through 23
//! \e ITM_PRIVMASK_24_31 - disable ports 24 through 31
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_trace_port_disable(uint8_t ui8portNum)
{
    AM_REGVAL(AM_REG_ITM_TPR_O) &= ~(0x00000001 << (ui8portNum >> 3));
}

//*****************************************************************************
//
//! @brief Poll the given ITM stimulus register until not busy.
//!
//! @param ui32StimReg - stimulus register
//!
//! @return true if not busy, false if busy (timed out or other error).
//
//*****************************************************************************
bool
am_hal_itm_stimulus_not_busy(uint32_t ui32StimReg)
{
    uint32_t ui32StimAddr = (AM_REG_ITM_STIM0_O + (4 * ui32StimReg));

    //
    // Busy waiting until it is available, non-zero means ready.
    //
    while (!AM_REGVAL(ui32StimAddr));

    return true;
}

//*****************************************************************************
//
//! @brief Writes a 32-bit value to the given ITM stimulus register.
//!
//! @param ui32StimReg - stimulus register
//! @param ui32Value - value to be written.
//!
//! Write a word to the desired stimulus register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_stimulus_reg_word_write(uint32_t ui32StimReg, uint32_t ui32Value)
{
    uint32_t ui32StimAddr;

    ui32StimAddr = (AM_REG_ITM_STIM0_O + (4 * ui32StimReg));

    //
    // Busy waiting until it is available, non-zero means ready
    //
    while (!AM_REGVAL(ui32StimAddr));

    //
    // Write the register.
    //
    AM_REGVAL(ui32StimAddr) = ui32Value;
}

//*****************************************************************************
//
//! @brief Writes a short to the given ITM stimulus register.
//!
//! @param ui32StimReg - stimulus register
//! @param ui16Value - short to be written.
//!
//! Write a short to the desired stimulus register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_stimulus_reg_short_write(uint32_t ui32StimReg, uint16_t ui16Value)
{
    uint32_t ui32StimAddr;

    ui32StimAddr = (AM_REG_ITM_STIM0_O + (4 * ui32StimReg));

    //
    // Busy waiting until it is available non-zero means ready
    //
    while (!AM_REGVAL(ui32StimAddr));

    //
    // Write the register.
    //
    *((volatile uint16_t *) ui32StimAddr) = ui16Value;
}

//*****************************************************************************
//
//! @brief Writes a byte to the given ITM stimulus register.
//!
//! @param ui32StimReg - stimulus register
//! @param ui8Value - byte to be written.
//!
//! Write a byte to the desired stimulus register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_stimulus_reg_byte_write(uint32_t ui32StimReg, uint8_t ui8Value)
{
    uint32_t ui32StimAddr;

    ui32StimAddr = (AM_REG_ITM_STIM0_O + (4 * ui32StimReg));

    //
    // Busy waiting until it is available (non-zero means ready)
    //
    while (!AM_REGVAL(ui32StimAddr));

    //
    // Write the register.
    //
    *((volatile uint8_t *) ui32StimAddr) = ui8Value;
}

//*****************************************************************************
//
//! @brief Sends a Sync Packet.
//!
//! Sends a sync packet. This can be useful for external software should it
//! become out of sync with the ITM stream.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_sync_send(void)
{
    //
    // Write the register.
    //
    am_hal_itm_stimulus_reg_word_write(AM_HAL_ITM_SYNC_REG,
                                       AM_HAL_ITM_SYNC_VAL);
}

//*****************************************************************************
//
//! @brief Poll the print stimulus registers until not busy.
//!
//! @return true if not busy, false if busy (timed out or other error).
//
//*****************************************************************************
bool
am_hal_itm_print_not_busy(void)
{
    //
    // Poll stimulus register allocated for printing.
    //
    am_hal_itm_stimulus_not_busy(0);


    return true;
}

//*****************************************************************************
//
//! @brief Prints a char string out of the ITM.
//!
//! @param pcString pointer to the character sting
//!
//! This function prints a sting out of the ITM.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_itm_print(char *pcString)
{
    uint32_t ui32Message = 0;
    uint32_t ui32Length = 0;
    uint32_t ui32Idx;

    //
    // Determine the length of the string.
    //
    while (*(pcString + ui32Length))
    {
        ui32Length++;
    }

    //
    // If there is no longer a word left, empty out the remaining characters.
    //
    while (ui32Length)
    {
        if (AM_HAL_ITM_PRINT_NUM_BYTES == 4 && ui32Length >= 4)
        {
            for (ui32Idx = 4; ui32Idx < 8; ui32Idx++)
            {
                ui32Message |= *pcString++ << ((ui32Idx - 4) << 3);

                //
                // Subtract from length.
                //
                ui32Length--;
            }

            //
            // Print string out the ITM.
            //
            am_hal_itm_stimulus_reg_word_write(0, ui32Message);

            //
            // Clear Message.
            //
            ui32Message = 0;
        }
        else
        {
            //
            // Print string out the ITM.
            //
            am_hal_itm_stimulus_reg_byte_write(0, (uint8_t)*pcString++);

            //
            // Subtract from length.
            //
            ui32Length--;
        }
    }
}
//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
