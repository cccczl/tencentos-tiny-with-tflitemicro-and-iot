/** ----------------------------------------------------------------------------
 *         Nationz Technology Software Support  -  NATIONZ  -
 * -----------------------------------------------------------------------------
 * Copyright (c) 2013��2018, Nationz Corporation  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution. 
 * 
 * Nationz's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission. 
 * 
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY NATIONZ "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
 * -----------------------------------------------------------------------------
 */

/** ****************************************************************************
 * @copyright      Nationz Co.,Ltd 
 *        Copyright (c) 2013��2018 All Rights Reserved 
 *******************************************************************************
 * @file     am_hal_queue.h
 * @author   
 * @date     
 * @version  v1.0
 * @brief    Functions for implementing a queue system.
 *
 * @addtogroup hal Hardware Abstraction Layer (HAL)
 * @addtogroup Miscellaneous Software Features (MISC)
 * @ingroup hal
 * @{
 ******************************************************************************/
#ifndef AM_HAL_QUEUE_H
#define AM_HAL_QUEUE_H

//*****************************************************************************
//
//! @brief A data structure that will operate as a queue.
//!
//! This data structure holds information necessary for operating a thread-safe
//! queue. When declaring a structure of type am_hal_queue_t, you will also need
//! to provide some working memory for the queue to use. For more information on
//! setting up and using the am_hal_queue_t structure, please see the
//! documentation for am_hal_queue_init().
//
//*****************************************************************************
typedef struct
{
    uint32_t ui32WriteIndex;
    uint32_t ui32ReadIndex;
    uint32_t ui32Length;
    uint32_t ui32Capacity;
    uint32_t ui32ItemSize;
    uint8_t *pui8Data;
}
am_hal_queue_t;

//*****************************************************************************
//
// Function-like macros.
//
//*****************************************************************************
#define am_hal_queue_empty(psQueue)                                           \
    ((psQueue)->ui32Length == 0)

#define am_hal_queue_full(psQueue)                                            \
    ((psQueue)->ui32Length == (psQueue)->ui32Capacity)

#define am_hal_queue_space_left(psQueue)                                      \
    ((psQueue)->ui32Capacity - (psQueue)->ui32Length)

#define am_hal_queue_data_left(psQueue)                                       \
    ((psQueue)->ui32Length)

//*****************************************************************************
//
// Use this to make sure you get the size parameters right.
//
//*****************************************************************************
#define am_hal_queue_from_array(queue, array)                                 \
    am_hal_queue_init((queue), (array), sizeof((array)[0]), sizeof(array))

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// External function definitions.
//
//*****************************************************************************
extern void am_hal_queue_init(am_hal_queue_t *psQueue, void *pvData, uint32_t ui32ItemSize, uint32_t ui32ArraySize);
extern bool am_hal_queue_item_add(am_hal_queue_t *psQueue, const void *pvSource, uint32_t ui32NumItems);
extern bool am_hal_queue_item_get(am_hal_queue_t *psQueue, void *pvDest, uint32_t ui32NumItems);

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_QUEUE_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
