/*
 * The Clear BSD License
 * Copyright (c) 2018 Adesto Technologies Corporation, Inc
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @ingroup ADESTO_LAYER MONETA
 */
/**
 * @file    moneta.h
 * @brief   Declarations of Moneta functions.
 */
#ifndef MONETA_H_
#define MONETA_H_

#include "cmd_defs.h"
#include "spi_driver.h"
#include "helper_functions.h"
#include <stdio.h>
#include <stdlib.h>

#if (PARTNO == RM331x)	|| \
	(ALL == 1)

extern uint8_t txMonetaInternalBuffer[MAXIMUM_TX_BYTES];

/******************************************
 *
 *
 * Moneta Commands
 *
 *
 ******************************************/

/*!
 * @brief: Wait while Read/Busy Status bit in SRB is 1 (device is busy).
 *
 * @retval void
 */
void monetaWaitOnReady();

/*!
 * @brief OPCODE: 0x06 <br>
 * Sends opcode to enable writing.
 *
 * @retval void
 */
void monetaWriteEnable();

/*!
 * @brief OPCODE: 0x04 <br>
 * Sends opcode to disable writing.
 *
 * @retval void
 */
void monetaWriteDisable();

/*!
 * @brief OPCODE: 0x05 <br>
 * Reads the value in the status registers.
 *
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 *  Must have at least 2 elements.
 *
 * @retval void
 */
void monetaReadSR(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0x01 <br>
 * Writes the value in data to status register byte 1.
 *
 * @param data Byte of data to be written into status register byte 1.
 *
 * @retval void
 */
void monetaWriteSRB1(uint8_t data);

/*!
 * @brief OPCODE: 0x31 <br>
 * Writes the value in data to status register byte 2.
 *
 * @param data Byte of data to be written into status register byte 2.
 *
 * @retval void
 */
void monetaWriteSRB2(uint8_t data);

/*!
 * @brief OPCODE: 0x03 <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in the byte array rx.
 *
 * @param address 2 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory. rxBuffer must
 * have a minimum of this many elements.
 *
 * @retval void
 */
void monetaReadArray(uint16_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x02 <br>
 * Writes txNumBytes bytes of data starting at the address indicated by address.
 *
 * @param address The 2 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory.
 * Must have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 *
 * @warning Check datasheet for the maximum number of bytes that can be written,
 * as well as the device behavior if writing goes beyond the page boundary.
 */
void monetaWriteArray(uint16_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x9F <br>
 * Reads the manufacturer ID and stores the data in rxBuffer.
 *
 * @param rxBuffer Pointer to a byte array in which the data will be stored.
 * Minimum of 8 bytes required.
 *
 * @retval void
 */
void monetaReadMID(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0x79 <br>
 * Sends the device into Ultra Deep Power Down Mode 1. Exit UDPD Mode 1 by
 * power cycling the device or calling function monetaEUDPDHardwareReset() which
 * performs a JEDEC reset.
 *
 * @retval void
 */
void monetaUDPDMode1();

/*!
 * @brief Exits Ultra Deep Power Down mode using the hardware reset option.
 *
 * @retval void
 */
void monetaHardwareReset();

#endif

#endif /* ADESTO_LAYER_H_ */
