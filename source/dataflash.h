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
 * @ingroup ADESTO_LAYER DATAFLASH
 */
/**
 * @file    dataflash.h
 * @brief   Declarations of Dataflash functions.
 */

#ifndef DATAFLASH_H_
#define DATAFLASH_H_
	
#include "cmd_defs.h"
#include "spi_driver.h"
#include "helper_functions.h"
#include <stdio.h>
#include <stdlib.h>

#if (PARTNO == AT45DB021E) || \
	(PARTNO == AT45DB041E) || \
	(PARTNO == AT45DB081E) || \
	(PARTNO == AT45DB161E) || \
	(PARTNO == AT45DB321E) || \
	(PARTNO == AT45DB641E) || \
	(PARTNO == AT45DQ161)  || \
	(PARTNO == AT45DQ321)  || \
	(PARTNO == AT25PE20)   || \
	(PARTNO == AT25PE40)   || \
	(PARTNO == AT25PE80)   || \
	(PARTNO == AT25PE16)   || \
	(ALL == 1)

extern uint8_t txDataflashInternalBuffer[MAXIMUM_TX_BYTES];

/******************************************
 *
 *
 * Dataflash Commands
 *
 *
 ******************************************/
/*!
 * @brief: Wait while Read/Busy Status bit in SRB is 1 (device is busy).
 *
 * @retval void
 */
void dataflashWaitOnReady();

/*!
 * @brief OPCODE: 0x9F <br>
 * Reads the manufacturer ID and stores the data in rxBuffer.
 *
 * @param rxBuffer Pointer to a byte array in which the data will be stored.
 * Minimum of 8 bytes required.
 *
 * @retval void
 */
void dataflashReadMID(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0xD7 <br>
 * Reads the value in the device status register (bytes 1 and 2).
 *
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least 2 elements.
 *
 * @retval void
 */
void dataflashReadSR(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0xD2 <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command reads from a page in main memory and
 * bypasses the buffer leaving their contents untouched as specified in the datasheet.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashMemoryPageRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x01 <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command reads from a page in main memory at
 * a lower frequency than other commands in order to minimize power consumption
 * as specified in the datasheet.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashArrayReadLowPower(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x03 <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command reads from a page in main memory at
 * a lower frequency than other commands as specified in the datasheet.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashArrayReadLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x0B <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command reads from a page in main memory at
 * a higher frequency than other commands as specified in the datasheet.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashArrayReadHighFreq0(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x1B <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command reads from a page in main memory at
 * a higher frequency than other commands as specified in the datasheet.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashArrayReadHighFreq1(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0xE8 <br>
 * Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer. This command reads from a page in main memory
 * as specified in the datasheet.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashArrayReadLegacy(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0xD1 <br>
 * Reads rxNumBytes starting from location 'address' in buffer 1
 * and stores the data in the byte array rxBuffer. This command reads
 * from buffer 1 at a lower frequency than other commands as specified
 * in the datasheet.
 *
 * @param address 3 byte address starting from which the data in buffer 1 will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer1ReadLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0xD4 <br>
 */
/*!
 * @brief OPCODE: 0xD4 <br>
 * Reads rxNumBytes starting from location 'address' in buffer 1
 * and stores the data in the byte array rxBuffer. This command reads
 * from buffer 1 at a higher frequency than other commands as specified
 * in the datasheet.
 *
 * @param address 3 byte address starting from which the data in buffer 1 will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer1ReadHighFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x84 <br>
 * Programs 'txNumBytes' bytes of data to buffer 1 starting at the address indicated by address.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 1. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 1. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer1Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x83 <br>
 * Programs the entire buffer 1 page to main memory starting at the address indicated by 'address'
 * with the build in erase feature.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 *
 * @retval void
 */
void dataflashBuffer1ToMainMemoryWithErase(uint32_t address);

/*!
 * @brief OPCODE: 0x88 <br>
 * Programs the entire buffer 1 page to main memory starting at the address indicated by 'address'.
 * The data in the page indicated at address will be not erased before the data in buffer 1 is
 * programmed to main memory. It is therefore recommended that the user erase the page beforehand.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 *
 * @retval void
 */
void dataflashBuffer1ToMainMemoryWithoutErase(uint32_t address);

/*!
 * @brief OPCODE: 0x82 <br>
 * Programs main memory through buffer 1 starting at the address indicated by 'address'
 * with the built in erase feature.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashMemoryProgramThruBuffer1WithErase(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x02 <br>
 *
 */
/*!
 * @brief OPCODE: 0x02 <br>
 * Programs main memory through buffer 1 starting at the address indicated by 'address'.
 * The data in the page indicated at address will be not erased before the data in buffer 2 is
 * programmed to main memory. It is therefore recommended that the user erase the page beforehand.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashMemoryProgramThruBuffer1WithoutErase(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x81 <br>
 * Erases a page of data (256/264 Bytes) starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the page erase will start.
 *
 * @retval void
 */
void dataflashPageErase(uint32_t address);

/*!
 * @brief OPCODE: 0x50 <br>
 * Erases a block of data (8 pages) starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the block erase will start.
 *
 * @retval void
 */
void dataflashBlockErase(uint32_t address);

/*!
 * @brief OPCODE: 0x7C <br>
 */
/*!
 * @brief OPCODE: 0x7C <br>
 * Erases a sector of data starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the block erase will start.
 *
 * @retval void
 */
void dataflashSectorErase(uint32_t address);

/*!
 * @brief OPCODE: 0xC794_809A <br>
 * Erases the entire chip by setting all bits.
 *
 * @retval void
 */
void dataflashChipErase();

/*!
 * @brief OPCODE: 0x58 <br>
 * Reads main memory to buffer 1, modifies it, then writes the data back to main memory
 * with built in erase.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashRMWThruBuffer1(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x3D_2A_7F_A9 <br>
 * Enables sector protection.
 *
 * @retval void
 */
void dataflashEnableSectorProtection();

/*!
 * @brief OPCODE: 0x3D_2A_7F_9A <br>
 * Disables sector protection.
 *
 * @retval void
 */
void dataflashDisableSectorProtection();

/*!
 * @brief OPCODE: 0x3D_2A_7F_CF <br>
 * Erases the sector protection register. Erasing is required for modification/setting
 * bits in the register.
 *
 * @retval void
 */
void dataflashEraseSectorProtectionReg();

/*!
 * @brief OPCODE: 0x3D_2A_7F_FC <br>
 * Program the sector protection register with the data in txBuffer.
 * @warning The user must send 8 bytes of data. Fewer than 8 bytes results in
 * indeterminate data being stored in the SPRs
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements..
 *
 * @retval void
 */
void dataflashProgSectorProtectionReg(uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x32 <br>
 * Reads 'txNumBytes' bytes of data in from the SPR register.
 *
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the register. rxBuffer must have
 * a minimum of this many elements.
 *
 * @retval void
 */
void dataflashReadSectorProtectionReg(uint8_t *rxBuffer, uint32_t rxNumBytes);


/*!
 * @brief OPCODE: 0x77
 * Reads 'rxNumBytes' bytes of data in from the security register.
 *
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the register. rxBuffer must have
 * a minimum of this many elements.
 *
 * @retval void
 */
void dataflashReadSecurityReg(uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x53 <br>
 * Transfer the contents of a page in main memory at the address indicated by
 * 'address' to buffer 1.
 *
 * @param address The page address in main memory from which data will be transferred to buffer 1.
 *
 * @retval void
 */
void dataflashMemtoBuffer1Transfer(uint32_t address);

/*!
 * @brief OPCODE: 0x60 <br>
 * Compare the contents of a page in main memory to the contents of buffer 1.
 * The status register should be read, and the 6th bit of byte 1 examined to determine
 * the result. <br>
 * 0 -> Main memory page data matches buffer data.<br>
 * 1 -> Main memory page data does not match buffer data.
 *
 * @param address The page address in main memory from which data will be transferred to buffer 1.
 *
 * @retval void
 */
void dataflashMemtoBuffer1Compare(uint32_t address);

/*!
 * @brief OPCODE: 0x58
 * Rewrite the page in main memory at address 'address.' This performs a refresh of
 * stored data. Data is stored in buffer 1 from main memory, then written back in.
 *
 * @param address The page address in main memory from which data will be transferred to buffer 1.
 *
 * @retval void
 */
void dataflashAutoPageRewrite1(uint32_t address);

/*!
 * @brief OPCODE: 0xB9
 * Put the device into deep power down mode. Call dataflashResumeFromDPD() to wake up, or
 * perform a hardware reset dataflashHardwareReset().
 *
 * @retval void
 */
void dataflashDPD();

/*!
 * @brief OPCODE: 0xAB <br>
 * Wake the device from deep power down mode. See dataflashDPD().
 *
 * @retval void
 */
void dataflashResumeFromDPD();

/*!
 * @brief OPCODE: 0x79 <br>
 * Put the device into ultra deep power down mode. Wake the device with dataflashHardwareReset().
 *
 * @retval void
 */
void dataflashUDPDMode();

/*!
 * @brief Perform a JEDEC reset on the flash device.
 *
 * @retval void
 */
void dataflashHardwareReset();

/*!
 * @brief OPCODE: 0x3D_2A_80_A6 <br>
 * Configure the device to read and write with pages configured to 256 bytes.
 *
 * @retval void
 */
void dataflashConfigurePower2PageSize();

/*!
 * @brief OPCODE: 0x3D_2A_80_A7 <br>
 * Configure the device to read and write with pages configured to 264 bytes.
 *
 * @retval void
 */
void dataflashConfigureStandardPageSize();

/*!
 * @brief OPCODE: 0xF0_00_00_00 <br>
 * Perform a software reset on the device. This will terminate any read or write
 * operation in progress.
 */
void dataflashSoftwareReset();

/*!
 * @brief OPCODE: 0x54
 * LEGACY: Reads rxNumBytes starting from location 'address' in buffer 2
 * and stores the data in the byte array rxBuffer.
 *
 * @param address 3 byte address starting from which the data in buffer 2 will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer1ReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x52 <br>
 * LEGACY: Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashMemPageReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x68 <br>
 * LEGACY: Reads rxNumBytes starting from location 'address' and stores the data in
 * the byte array rxBuffer.
 *
 * @param address 3 byte address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 *
 */
void dataflashContinuousArrayReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x57 <br>
 * LEGACY: Reads the value in the device status register (bytes 1 and 2).
 *
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least 2 elements.
 *
 * @retval void
 */
void dataflashReadSRLegacy(uint8_t *rxBuffer);
#if (PARTNO == AT45DQ161) || \
	(PARTNO == AT45DQ321) || \
	(ALL == 1)
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
void dataflashDualOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x6B <br>
 * Reads 'txNumBytes' bytes of data in Quad Output Mode starting at 'address'
 * using SI, SO, WPb, and HOLDb as inputs to master.
 *
 * @param address Address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory. rxBuffer must have
 * a minimum of this many elements.
 *
 * @retval void
 */
void dataflashQuadOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x24 <br>
 * Programs 'txNumBytes' bytes of data to buffer 1 starting at the address indicated by address using
 * SI and SO as inputs for dual input.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashDualInputBuffer1Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x24 <br>
 * Programs 'txNumBytes' bytes of data to buffer 2 starting at the address indicated by address using
 * SI and SO as inputs for dual input.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashDualInputBuffer2Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x44 <br>
 * Programs 'txNumBytes' bytes of data to buffer 2 starting at the address indicated by address using
 * SI, SO, WPb, and RESETb as inputs for quad input.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashQuadInputBuffer1Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x47 <br>
 * Programs 'txNumBytes' bytes of data to buffer 2 starting at the address indicated by address using
 * SI, SO, WPb, and RESETb as inputs for quad input.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashQuadInputBuffer2Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x3F <br>
 * Reads the configuration register and returns the 1-byte value in rxBuffer.
 *
 * @param rxBuffer the buffer in which the 1-byte value will be stored.
 *
 * @retval void
 */
void dataflashReadConfigRegister(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0x3D2A8166 <br>
 * Programs the NV-QE bit to a logical 1 to enable the Quad I/O commands.
 *
 * @retval 0
 */
void dataflashQuadEnable();

/*!
 * @brief OPCODE: 0x3D2A8167 <br>
 * Programs the NV-QE bit to a logical 0 to disable the Quad I/O commands.
 *
 * @retval 0
 */
void dataflashQuadDisable();
#endif

#if (PARTNO == AT45DB021E) || \
	(PARTNO == AT45DB041E) || \
	(PARTNO == AT45DB081E) || \
	(PARTNO == AT45DB161E) || \
	(PARTNO == AT45DB321E) || \
	(PARTNO == AT45DB641E) || \
	(PARTNO == AT45DQ161)  || \
	(PARTNO == AT45DQ321)  || \
	(ALL == 1)
/*!
 * @brief OPCODE: 0xB0 <br>
 * Suspends a program or erase operation in progress to a particular 64KB sector
 * of the main memory array so that other device operations can be performed.
 *
 * @retval void
 */
void dataflashProgramEraseSuspend();

/*!
 * @brief OPCODE: 0xD0 <br>
 * Resumes a suspended program or erase operation. See dataflashProgramEraseSuspend().
 *
 * @retval void
 */
void dataflashProgramEraseResume();

/*!
 * @brief OPCODE: 0x3D_2A_7F_30 <br>
 * Permanently a sector to prevent erasing or programming the stored data.
 *
 * @param address The address of the sector to be locked.
 *
 * @warning Once a sector is locked it can never be unlocked!
 */
void dataflashSectorLockdown(uint32_t address);

/*!
 * @brief OPCODE: 0x35
 * Reads 'rxNumBytes' bytes of data in from the SL register.
 *
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the register. rxBuffer must have
 * a minimum of this many elements.
 *
 * @retval void
 */
void dataflashReadSectorLockdownReg(uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x34_55_AA_40 <br>
 * Permanently freeze the sector lockdown register to prevent any future changes and disable
 * the sector lockdown command. The current state will be preserved.
 *
 * @warning This command permanently disables the sector lockdown command.
 */
void dataflashFreezeSectorLockdown();

/*!
 * @brief OPCODE: 0x9B_00_00_00 <br>
 * Program the security register with the data in txBuffer.
 *
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @warning The user must send 64 bytes of data. Fewer than 64 bytes results in
 * indeterminate data being stored in the SPRs. The bits in this register can
 * only be cleared, and never set. As such each bit is one time erasable.
 *
 * @retval void
 */
void dataflashProgSecurityReg(uint8_t *txBuffer, uint32_t txNumBytes);
#endif
#if (PARTNO == AT45DB021E) || \
	(PARTNO == AT45DB041E) || \
	(PARTNO == AT45DB081E) || \
	(PARTNO == AT45DB161E) || \
	(PARTNO == AT45DB321E) || \
	(PARTNO == AT45DB641E) || \
	(PARTNO == AT45DQ161)  || \
	(PARTNO == AT45DQ321)  || \
	(PARTNO == AT25PE40)   || \
	(PARTNO == AT25PE80)   || \
	(PARTNO == AT25PE16)   || \
	(ALL == 1)
/*!
 * @brief OPCODE: 0xD3 <br>
 * Reads rxNumBytes starting from location 'address' in buffer 2
 * and stores the data in the byte array rxBuffer. This command reads
 * from buffer 2 at a lower frequency than other commands as specified
 * in the datasheet.
 *
 * @param address 3 byte address starting from which the data in buffer 2 will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer2ReadLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0xD6 <br>
 * Reads rxNumBytes starting from location 'address' in buffer 2
 * and stores the data in the byte array rxBuffer. This command reads
 * from buffer 2 at a higher frequency than other commands as specified
 * in the datasheet.
 *
 * @param address 3 byte address starting from which the data in buffer 2 will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer2ReadHighFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0x87 <br>
 * Programs 'txNumBytes' bytes of data to buffer 2 starting at the address indicated by address.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer2Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x86 <br>
 * Programs the entire buffer 2 page to main memory starting at the address indicated by 'address'.
 * with the build in erase feature.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 *
 * @retval void
 */
void dataflashBuffer2ToMainMemoryWithErase(uint32_t address);

/*!
 * @brief OPCODE: 0x89 <br>
 * Programs the entire buffer 2 page to main memory starting at the address indicated by 'address'.
 * The data in the page indicated at address will be not erased before the data in buffer 2 is
 * programmed to main memory. It is therefore recommended that the user erase the page beforehand.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 *
 * @retval void
 */
void dataflashBuffer2ToMainMemoryWithoutErase(uint32_t address);

/*!
 * @brief OPCODE: 0x85 <br>
 * Programs main memory through buffer 2 starting at the address indicated by 'address'
 * with the built in erase feature.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * Note page restrictions in manual.
 * @param txBuffer Pointer to the tx bytes that will be stored in buffer 2. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to buffer 2. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashMemoryProgramThruBuffer2WithErase(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x59 <br>
 * Reads main memory to buffer 1, modifies it, then writes the data back to main memory
 * with built in erase.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashRMWThruBuffer2(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x55 <br>
 */
void dataflashMemtoBuffer2Transfer(uint32_t address);

/*!
 * @brief OPCODE: 0x61 <br>
 * Compare the contents of a page in main memory to the contents of buffer 2.
 * The status register should be read, and the 6th bit of byte 1 examined to determine
 * the result. <br>
 * 0 -> Main memory page data matches buffer data.<br>
 * 1 -> Main memory page data does not match buffer data.
 *
 * @param address The page address in main memory from which data will be transferred to buffer 2.
 *
 * @retval void
 */
void dataflashMemtoBuffer2Compare(uint32_t address);

/*!
 * @brief OPCODE: 0x59
 * Rewrite the page in main memory at address 'address.' This performs a refresh of
 * stored data. Data is stored in buffer 2 from main memory, then written back in.
 *
 * @param address The page address in main memory from which data will be transferred to buffer 2.
 *
 * @retval void
 */
void dataflashAutoPageRewrite2(uint32_t address);

/*!
 * @brief OPCODE: 0x56 <br>
 * LEGACY: Reads rxNumBytes starting from location 'address' in buffer 2 and stores
 * the data in the byte array rxBuffer.
 *
 * @param address 3 byte address starting from which the data in buffer 2 will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory.
 * rxBuffer must have a minimum of this many elements.
 *
 * @retval void
 */
void dataflashBuffer2ReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);
#endif

/**
 * @brief Sample use of the driver for a Dataflash device. <br>
 * This function is meant to showcase how the API is to
 * be used by performing tests on the device, and outputting
 * messages based on performance. Not all functions are used,
 * although most are. This is NOT meant as a standalone reference
 * and should be used in conjunction with the provided documentation
 * and the relevant datasheets. <br>
 *
 * Testing is done in multiple stages. The order matters from
 * each step to the next in this example.
 *
 * Testing commences as follows:
 * 1. Read manufacturing ID.
 * 2. Test read and write functionality.
 * 3. Test deep power down mode.
 * 4. Test ultra deep power down mode and JEDEC reset.
 * 5. Test erase commands.
 * 6. Test memory to buffer commands.
 * 7. Test Read - Modify - Write capabilities.
 *
 * At each stage various messages will confirm that the tests have passed or failed.
 * Failure is expected during power down modes, as the MISO line is not driven, thus
 * causing data read to be unpredictable.
 *
 * @retval uint32_t Returns the number of errors in the testbench.
 */
uint32_t dataflashTest();

#endif

#endif /* DATAFLASH_H_ */
