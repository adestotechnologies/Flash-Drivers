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
 * @ingroup ADESTO_LAYER FUSION
 */
/**
 * @file    fusion.h
 * @brief   Declarations of Fusion functions.
 */

#ifndef FUSION_H_
#define FUSION_H_

#include "cmd_defs.h"
#include "spi_driver.h"
#include "helper_functions.h"
#include <stdio.h>
#include <stdlib.h>

#if	(PARTNO == AT25XE512C)	|| \
	(PARTNO == AT25XE011)	|| \
	(PARTNO == AT25XE021A)	|| \
	(PARTNO == AT25XE041B)	|| \
	(PARTNO == AT25DN256)	|| \
	(PARTNO == AT25DN512C)	|| \
	(PARTNO == AT25DN011)	|| \
	(PARTNO == AT25DF256)	|| \
	(PARTNO == AT25DF512C)	|| \
	(PARTNO == AT25DF011)	|| \
	(PARTNO == AT25DF021A)	|| \
	(PARTNO == AT25DF041B)	|| \
	(PARTNO == AT25XV021A)	|| \
	(PARTNO == AT25XV041B)	|| \
	(ALL == 1)

extern uint8_t txFusionInternalBuffer[MAXIMUM_TX_BYTES];

/******************************************
 *
 *
 * Fusion Commands
 *
 *
 ******************************************/

/*!
 * @brief: Wait while Read/Busy Status bit in SRB is 1 (device is busy).
 *
 * @retval void
 */
void fusionWaitOnReady();

/*!
 * @brief Protects all sectors by issuing a WE, then write of 0x7F to status register
 * byte 1. These 2 write are accomplished with fusionWriteEnable() and fusionWriteSRB1().
 *
 * @retval void
 */
void fusionGlobalProtect();

/*!
 * @brief Unprotects all sectors by issuing a WE, then write of 0x00 to status register
 * byte 1. These 2 write are accomplished with fusionWriteEnable() and fusionWriteSRB1().
 *
 * @retval void
 */
void fusionGlobalUnprotect();

/*!
 * @brief OPCODE: 0x0B <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command
 * can read at multiple frequencies but necessitates the use of a dummy cycle.
 *
 * @param address Address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory. rxBuffer must
 * have a minimum of this many elements.
 *
 * @retval void
 */
void fusionReadArray(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x03 <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command uses a low frequency read as specified in the datasheet.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void fusionReadArrayLF(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x3B <br>
 * Reads 'txNumBytes' bytes of data in Dual Output Mode starting at 'address'
 * using both SI and SO as inputs to master.
 *
 * @param address Address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory. rxBuffer must have
 * a minimum of this many elements.
 *
 * @retval void
 */
void fusionDualOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x81 <br>
 * Erases a page of data (256 Bytes) starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the page erase will start.
 *
 * @retval void
 */
void fusionPageErase(uint32_t address);

/*!
 * @brief OPCODE: 0x20 <br>
 * Erases a block of data (4 KBytes) starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the block erase will start.
 *
 * @retval void
 */
void fusionBlockErase4K(uint32_t address);

/*!
 * @brief OPCODE: 0x60 <br>
 * Erases the entire chip by setting all bits.
 *
 * @retval void
 */
void fusionChipErase();

/*!
 * @brief OPCODE: 0x02 <br>
 * Programs 'txNumBytes' bytes of data starting at the address indicated by address.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 *
 * @warning Check datasheet for the maximum number of bytes that can be written,
 * as well as the device behavior if writing goes beyond the page boundary.
 */
void fusionProgramArray(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x06 <br>
 * Sends opcode to enable writing.
 *
 * @retval void
 */
void fusionWriteEnable();

/*!
 * @brief OPCODE: 0x04 <br>
 * Sends opcode to disable writing.
 *
 * @retval void
 */
void fusionWriteDisable();

/*!
 * @brief OPCODE: 0x05 <br>
 * Reads the value in the status register (bytes 1 and 2).
 *
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least 2 elements.
 *
 * @retval void
 */
void fusionReadSR(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0x01 <br>
 * Writes the value in data to status register byte 1.
 *
 * @param data Byte of data to be written into status register byte 1.
 *
 * @retval void
 */
void fusionWriteSRB1(uint8_t data);

/*!
 * @brief OPCODE: 0x31 <br>
 * Writes the value in data to status register byte 2.
 *
 * @param data Byte of data to be written into status register byte 2.
 *
 * @retval void
 */
void fusionWriteSRB2(uint8_t data);

/*!
 * @brief OPCODE: 0xF0 <br>
 * Resets the flash device for immediately, regardless of what current operation is being
 * performed.
 *
 * @retval void
 */
void fusionReset();

/*!
 * @brief OPCODE: 0x9F <br>
 * Reads the manufacturer ID and stores the data in rxBuffer.
 *
 * @param rxBuffer Pointer to a byte array in which the data will be stored.
 * Minimum of 4 bytes required.
 *
 * @retval void
 */
void fusionReadMID(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0xB9 <br>
 * Enters the device into Deep Power Down mode. fusionResumeFromDeepPowerDown() will wake
 * it back up again.
 *
 * @retval void
 */
void fusionDeepPowerDown();

/*!
 * @brief OPCODE: 0xAB <br>
 * Wakes the device up from Deep Power Down mode (fusionDeepPowerDown()).
 *
 * @retval void
 */
void fusionResumeFromDeepPowerDown();

/*!
 * @brief OPCODE: 0x79 <br>
 * Sends the device into Ultra Deep Power Down Mode. Exit UDPD Mode by
 * toggling CS with command ___ or using a JEDEC reset with command fusionHardwareReset().
 *
 * @retval void
 */
void fusionUDPDMode();
#endif

#if	(PARTNO == AT25XE021A)	|| \
	(PARTNO == AT25XE041B)	|| \
	(PARTNO == AT25DF021A)	|| \
	(PARTNO == AT25DF041B)	|| \
	(PARTNO == AT25XV021A)	|| \
	(PARTNO == AT25XV041B)	|| \
	(ALL == 1)
/*!
 * @brief OPCODE: 0xAD <br>
 *  Sends the byte in txBuffer to the flash device for programming. This command should only
 * be called after fusionSequentialProgramModeEnbale().
 *
 * @param txBuffer The byte of data to be programmed.
 *
 * @retval void
 */
void fusionSequentialProgramMode(uint8_t txBuffer);

/*!
 * @brief OPCODE: 0xA2 <br>
 * Programs 'txNumBytes' bytes of data in Dual Input Mode starting at 'address'
 * using both SI and SO as outputs from master.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must have
 * a minimum of txNumBytes elements.
 * @param txNumBytes Number of non dummy bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void fusionDualInputProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x36 <br>
 * Sends opcode to disable writing.
 *
 * @param address 3 byte address for the sector that will be protected.
 *
 * @retval void
 */
void fusionProtectSector(uint32_t address);

/*!
 * @brief OPCODE: 0x39 <br>
 * Sends opcode to enable writing.
 *
 * @param address 3 byte address for the sector that will be unprotected.
 * @retval void
 */
void fusionUnprotectSector(uint32_t address);

/*!
 * @brief OPCODE: 0x3C <br>
 * Reads the sector protection register at address 'address.'
 * 0x00 -> Sector is unprotected. 0xFF -> Sector is protected.
 *
 * @param address 3 byte address for the sector of interest.
 * @param rxBuffer The byte buffer in which the value of the protection
 * register will be stored.
 *
 * @retval void
 */
void fusionReadSectorProtectionRegisters(uint32_t address, uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0xAD <br>
 * Enables sequential program mode and loads the starting address into the device.
 * The first byte of data is also stored
 *
 * @param address The address from which the sequential programming will begin.
 * @param txBuffer The first byte of data to be stored in flash memory.
 *
 * @retval void
 */
void fusionSequentialProgramModeEnable(uint32_t address, uint8_t txBuffer);

/*!
 * @brief Performs a hardware (JEDEC) reset on the device.
 *
 * @retval void
 */
void fusionHardwareReset();

#endif

#endif /* DATAFLASH_H_ */
