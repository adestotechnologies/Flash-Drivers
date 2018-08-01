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
 * @ingroup ADESTO_LAYER STANDARDFLASH
 */
/**
 * @file    standardflash.h
 * @brief   Declarations of Standardflash functions.
 */

#ifndef STANDARDFLASH_H_
#define STANDARDFLASH_H_
	
#include "cmd_defs.h"
#include "spi_driver.h"
#include "helper_functions.h"
#include <stdio.h>
#include <stdlib.h>

#if (PARTNO == AT25SF641) 	|| \
	(PARTNO == AT25SF321)	|| \
	(PARTNO == AT25SF161) 	|| \
	(PARTNO == AT25SF081) 	|| \
	(PARTNO == AT25SF041) 	|| \
	(PARTNO == AT25SL128A)  || \
	(PARTNO == AT25SL641) 	|| \
	(PARTNO == AT25SL321) 	|| \
	(PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)  || \
	(PARTNO == AT25QL128A)  || \
 	(PARTNO == AT25QL641) 	|| \
	(PARTNO == AT25QL321) 	|| \
	(PARTNO == AT25QF641)	|| \
	(ALL == 1)

extern uint8_t txStandardflashInternalBuffer[MAXIMUM_TX_BYTES];

/******************************************
 *
 *
 * Standardflash Commands
 *
 *
 ******************************************/

/*!
 * @brief: Wait while Read/Busy Status bit in SRB is 1 (device is busy).
 * This is accomplished by reading the first status register and delaying
 * while the BSY bit is 1.
 *
 * This is a 'derived' command which uses multiple single commands to accomplish
 * a set task.
 *
 * @retval void
 */
void standardflashWaitOnReady();

/*!
 * @brief: Sets the QE bit in status register byte 2. Does not modify any other bits.
 * This is accomplished by first reading both bytes, performing a write enable, then
 * writing the status register data back in with the QE bit set.<br>
 *
 * This is a 'derived' command which uses multiple single commands to accomplish
 * a set task.
 *
 * @retval void
 */
void standardflashSetQEBit();

/*!
 * @brief: Clears the QE bit in status register byte 2. Does not modify any other bits.
 * This is accomplished by first reading both bytes, performing a write enable, then
 * writing the status register data back in with the QE bit cleared.<br>
 *
 * This is a 'derived' command which uses multiple single commands to accomplish
 * a set task.
 *
 * @retval void
 */
void standardflashClearQEBit();

/*!
 * @brief OPCODE: 0x06 <br>
 * Sends opcode to enable writing.
 *
 * @retval void
 */
void standardflashWriteEnable();

/*!
 * @brief OPCODE: 0x04 <br>
 * Sends opcode to disable writing.
 *
 * @retval void
 */
void standardflashWriteDisable();

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
void standardflashReadArrayLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

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
void standardflashReadArrayHighFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

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
 */
void standardflashBytePageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x20 <br>
 * Erases a block of data (4 KBytes) starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the block erase will start.
 *
 * @retval void
 */
void standardflashBlockErase4K(uint32_t address);

/*!
 * @brief OPCODE: 0x52 <br>
 * Erases a block of data (4 KBytes) starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the block erase will start.
 *
 * @retval void
 */
void standardflashBlockErase32K(uint32_t address);

/*!
 * @brief OPCODE: 0xD8 <br>
 * Erases a block of data (4 KBytes) starting from page address 'address.'
 *
 * @param address The 3 byte page address at which the block erase will start.
 *
 * @retval void
 */
void standardflashBlockErase64K(uint32_t address);

/*!
 * @brief OPCODE: 0x60 <br>
 * Erases the entire chip by setting all bits.
 * standardflashChipErase2() is functionally equivalent.
 *
 * @retval void
 */
void standardflashChipErase1();

/*!
 * @brief OPCODE: 0xC7 <br>
 * Erases the entire chip by setting all bits.
 * standardflashChipErase1() is functionally equivalent.
 *
 * @retval void
 */
void standardflashChipErase2();

/*!
 * @brief OPCODE: 0xB9
 * Put the device into deep power down mode. Call dataflashResumeFromDPD() to wake up, or
 * perform a hardware reset dataflashHardwareReset().
 *
 * @retval void
 */
void standardflashDPD();

/*!
 * @brief OPCODE: 0xAB <br>
 * Wake the device from deep power down mode. See dataflashDPD().
 *
 * @retval void
 */
void standardflashResumeFromDPD();

/*!
 * @brief OPCODE: 0x90 <br>
 * Legace Command: Reads the manufacturer ID and stores the data in rxBuffer.
 *
 * @param rxBuffer Pointer to a byte array in which the data will be stored.
 * Minimum of 2 bytes required.
 *
 * @retval void
 */
void standardflashReadID(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0x9F <br>
 * Reads the manufacturer ID and stores the data in rxBuffer.
 *
 * @param rxBuffer Pointer to a byte array in which the data will be stored.
 *
 * @retval void
 */
void standardflashReadMID(uint8_t *rxBuffer);
#endif
#if (PARTNO == AT25SF641) 	|| \
	(PARTNO == AT25SF321)	|| \
	(PARTNO == AT25SF161) 	|| \
	(PARTNO == AT25SF081) 	|| \
	(PARTNO == AT25SF041) 	|| \
	(PARTNO == AT25SL128A) 	|| \
	(PARTNO == AT25SL641) 	|| \
	(PARTNO == AT25SL321) 	|| \
	(PARTNO == AT25QL128A) 	|| \
	(PARTNO == AT25QL641) 	|| \
	(PARTNO == AT25QL321) 	|| \
	(PARTNO == AT25QF641)   || \
	(ALL == 1)
/*!
 * @brief OPCODE: 0x01 <br>
 * Writes the value in data to the status register byte 1, or bytes 1 and 2.
 * At least 1 byte needs to be written.
 *
 * @param txBuffer Byte(s) of data to be written into the status register.
 * @param txNumBytes The number of bytes to be transmitted, either 1 or 2.
 *
 * @retval void
 */
void standardflashWriteSR(uint8_t *txBuffer, uint8_t txNumBytes);
#endif

#if (PARTNO == AT25SF641) 	|| \
	(PARTNO == AT25SL128A) 	|| \
	(PARTNO == AT25SL641) 	|| \
	(PARTNO == AT25SL321) 	|| \
	(PARTNO == AT25QL128A) 	|| \
 	(PARTNO == AT25QL641) 	|| \
	(PARTNO == AT25QL321) 	|| \
	(PARTNO == AT25QF641)   || \
	(PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)	|| \
	(ALL == 1)
void standardflashWriteSRB1(uint8_t regVal);
void standardflashWriteSRB2(uint8_t regVal);
#endif

#if (PARTNO == AT25SF641) 	|| \
	(PARTNO == AT25SF321)	|| \
	(PARTNO == AT25SF161) 	|| \
	(PARTNO == AT25SF081) 	|| \
	(PARTNO == AT25SF041) 	|| \
	(PARTNO == AT25SL128A)  || \
	(PARTNO == AT25SL641) 	|| \
	(PARTNO == AT25SL321) 	|| \
	(PARTNO == AT25QL128A)  || \
 	(PARTNO == AT25QL641) 	|| \
	(PARTNO == AT25QL321) 	|| \
	(PARTNO == AT25QF641)	|| \
	(ALL == 1)
/*!
 * @brief OPCODE: 0x50 <br>
 * Sends opcode to enable writing for the volatile status register.
 *
 * @retval void
 */
void standardflashWriteEnableVolatileSR();

/*!
 * @brief OPCODE: 0x05 <br>
 * Reads the value in the device status register (byte 1).
 *
 * @retval Byte stored in the status register.
 */
uint8_t standardflashReadSRB1();

/*!
 * @brief OPCODE: 0x35 <br>
 * Reads the value in the device status register (byte 2).
 *
 * @retval Byte stored in the status register.
 */
uint8_t standardflashReadSRB2();

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
void standardflashDualOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0xBB <br>
 * Reads 'txNumBytes' bytes of data in Dual IO Mode starting at 'address'
 * using both SI and SO as inputs to master. Depending on the readMode parameter
 * the user can eliminate the need to send the opcode over if a continuous read
 * is being performed. See the comments for readMode.
 *
 * @param address Address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory. rxBuffer must have
 * a minimum of this many elements.
 * @param readMode The read mode which selects between 4 different modes.<br>
 * 	- 0: M5/4 =/= 10b Do not perform a continuous read. This results in a one-off dual read.
 * 	- 1: M5/4 = 10b Start a continuous dual read. This removes the need for an opcode the next time
 * 	dual read is issued.
 * 	- 2: M5/4 = 10b Continue dual read. When this is used, the opcode is not sent in the transfer.
 * 	- 3: M5/4 =/= 10b End dual read. This command stops the continuous dual read mode, but does not send
 * 	an opcode.
 * @param modeByteValue This value should be set to the M7-0 byte listed in the datasheet for a continuous operation.
 * The overhead of setting up the opcode, address, and mode byte is taken care of, but different devices within
 * the same family will use a different mode value.<br>
 *  Please note, this is taken care of in the sample code when defining which part is being used.
 *  See main() in standardflash.c.
 *
 * @retval void
 */
void standardflashDualIORead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes, uint8_t readMode, uint8_t modeByteValue);

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
void standardflashQuadOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0xEB <br>
 * Reads 'txNumBytes' bytes of data in Quad IO Mode starting at 'address'
 * using SI, SO, WPb, and HOLDb as inputs to master. Depending on the readMode parameter
 * the user can eliminate the need to send the opcode over if a continuous read
 * is being performed. See the comments for readMode.
 *
 * @param address Address starting from which the data in memory will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory. rxBuffer must have
 * a minimum of this many elements.
 * @param readMode The read mode which selects between 4 different modes.<br>
 * 	- 0: M5/4 =/= 10b Do not perform a continuous read. This results in a one-off quad read.
 * 	- 1: M5/4 = 10b Start a continuous quad read. This removes the need for an opcode the next time
 * 	- 2: M5/4 = 10b Continue quad read. When this is used, the opcode is not sent in the transfer.
 * 	- 3: M5/4 =/= 10b End quad read. This command stops the continuous quad read mode, but does not send
 * 	an opcode.
 * @param modeByteValue This value should be set to the M7-0 byte listed in the datasheet for a continuous operation.
 * The overhead of setting up the opcode, address, and mode byte is taken care of, but different devices within
 * the same family will use a different mode value.<br>
 *  Please note, this is taken care of in the sample code when defining which part is being used.
 *  See main() in standardflash.c.
 *
 * @retval void
 */
void standardflashQuadIORead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes, uint8_t readMode, uint8_t modeByteValue);

/*!
 * @brief OPCODE: 0xFFFF <br>
 * Resets the dual continuous read mode that was entered with standardflashDualIORead() when readMode
 * 2 was used. Data from and to the device will be in standard SPI after using this command.
 *
 * @retval void
 */
void standardflashContinuousReadModeDualReset();

/*!
 * @brief OPCODE: 0xFF <br>
 * Resets the quad continuous read mode that was entered with standardflashQuadIORead() when readMode
 * 2 was used. Data from and to the device will be in standard SPI after using this command.
 *
 * @retval void
 */
void standardflashContinuousReadModeQuadReset();

/*!
 * @brief OPCODE: 0x44 <br>
 * Sends opcode to erase the security registers.
 *
 * @retval void
 */
void standardflashEraseSecurityRegister(uint32_t address);

/*!
 * @brief OPCODE: 0x42 <br>
 * Programs the security registers with the contents of txBuffer.
 *
 * @param address Address starting from which the data in txBuffer will be stored within
 * the security register.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void standardflashProgramSecurityRegisters(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0x48 <br>
 * Reads the contents of the security registers and stores it in the rxBuffer.
 *
 * @param address Address starting from which the data in the status register will be read.
 * @param rxBuffer Pointer to the byte array in which the read data will be stored.
 * Must have at least rxNumBytes elements.
 * @param rxNumBytes Number of bytes to be read from the memory. rxBuffer must have
 * a minimum of this many elements.
 *
 * @retval void
 */
void standardflashReadSecurityRegisters(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes);

/*!
 * @brief OPCODE: 0xAB <br>
 * Wake the device from deep power down mode and read the 1 byte ID. See standardflashDPD().
 *
 * @param rxBuffer A byte array in which the device ID (single byte) will be stored.
 *
 * @retval void
 */
void standardflashResumeFromDPDReadID(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0x33 <br>
 * Programs 'txNumBytes' bytes of data starting at the address indicated by address using Quad IO.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 * @param mode Determines which mode to use when sending data over. If mode is 0, the command will use
 * SPI mode at detailed in the datasheet. If mode is 1, Quad mode will be used as detailed in the datasheet.
 *
 * @retval void
 */
void standardflashQuadPageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes, uint8_t mode);
#endif

#if (PARTNO == AT25SF641) 	|| \
	(PARTNO == AT25SF321)	|| \
	(PARTNO == AT25SF161) 	|| \
	(PARTNO == AT25SL128A)  || \
	(PARTNO == AT25SL641) 	|| \
	(PARTNO == AT25SL321) 	|| \
	(PARTNO == AT25QL128A)  || \
 	(PARTNO == AT25QL641) 	|| \
	(PARTNO == AT25QL321) 	|| \
	(PARTNO == AT25QF641)	|| \
	(ALL == 1)
/*!
 * @brief OPCODE: 0x75 <br>
 * Suspends an erase or program mid operation.
 *
 * @retval void
 */
void standardflashEraseProgramSuspend();

/*!
 * @brief OPCODE: 0x7A <br>
 * Resumes a previously suspended erase or program.
 *
 * @retval void
 */
void standardflashEraseProgramResume();
#endif

#if (PARTNO == AT25SF641) 	|| \
	(PARTNO == AT25SL128A)  || \
	(PARTNO == AT25SL641) 	|| \
	(PARTNO == AT25SL321) 	|| \
	(PARTNO == AT25QL128A)  || \
 	(PARTNO == AT25QL641) 	|| \
	(PARTNO == AT25QL321) 	|| \
	(PARTNO == AT25QF641)	|| \
	(ALL == 1)

/*!
 * @brief OPCODE: 0x38 <br>
 * Switches the device from SPI mode to QPI mode, and sets a variable to quad mode.
 *
 * @warning Must first call standardflashSetQEBit() to set the QE bit!
 *
 * @retval void
 */
void standardflashEnableQPI();

/*!
 * @brief OPCODE: 0xFF <br>
 * Switches the device from QPI mode to SPI mode, and sets a variable to spi mode.
 *
 * @warning Must call standardflashClearQEBit() afterwards to reset the QE bit!
 *
 * @retval void
 */
void standardflashDisableQPI();

/*!
 * @brief OPCODE: 0x66 <br>
 * Enable software reset of the flash device.
 *
 * @warning must follow this by standardflashReset() in order to software reset the device.
 *
 * @retval void
 */
void standardflashEnableReset();

/*!
 * @brief OPCODE: 0x99 <br>
 * Software reset the flash device.
 *
 * @warning Must have first issued standardflashEnableReset() in order to perform a software reset.
 *
 * @retval void
 */
void standardflashReset();

/*!
 * @brief OPCODE: 0xB1 <br>
 * Enter secured OTP region.
 *
 * @retval void
 */
void standardflashEnterSecureOTP();

/*!
 * @brief OPCODE: 0xC1 <br>
 * Exit secured OTP region.
 *
 * @retval void
 */
void standardflashExitSecuredOTP();
#endif

#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)	|| \
	(ALL == 1)
/*!
 * @brief OPCODE: 0x05 <br>
 * Reads the value in the device status register (byte 1 and 2).
 *
 * @retval void
 */
void standardflashReadSR(uint8_t *rxBuffer);

/*!
 * @brief OPCODE: 0xA2 <br>
 * Programs 'txNumBytes' bytes of data starting at the address indicated by address
 * using both the MISO and MOSI lines is inputs to the device.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void standardflashDualInputBytePageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

/*!
 * @brief OPCODE: 0xB0 <br>
 * Suspends an erase or program mid operation.
 *
 * @retval void
 */
void standardflashProgramEraseSuspend();

/*!
 * @brief OPCODE: 0xD0 <br>
 * Resumes a previously suspended erase or program.
 *
 * @retval void
 */
void standardflashProgramEraseResume();

/*!
 * @brief OPCODE: 0x36
 * Protects a sector containing the address address.
 *
 * @retval void
 */
void standardflashProtectSector(uint32_t address);

/*!
 * @brief OPCODE: 0x39 <br>
 * Un-protects a sector containing the address address.
 *
 * @retval void
 */
void standardflashUnprotectSector(uint32_t address);

/*!
 * @brief OPCODE: 0x3C <br>
 * Reads the sector protection register for address address.
 *
 * @retval The byte indicating whether or not the sector is protected.<br>
 * 0xFF indicates the sector is protected, 0x00 indicates the sector is unprotected.
 */
uint8_t standardflashReadSectorProtectionReg(uint32_t address);

/*!
 * @brief OPCODE: 0x34 <br>
 * Freezes the current state of the sector lockdown registers.
 * @warning This operation is permanent, and disables the functionality to change the
 * lockdown registers.
 *
 * @retval void
 */
void standardflashFreezeLockdownState();

/*!
 * @brief OPCODE: 0x35 <br>
 * Reads the sector lockdown register for address address.
 *
 * @retval The byte indicating whether or not the sector is locked-down.<br>
 * 0xFF indicates the sector is locked-down, 0x00 indicates the sector is not locked-down.
 */
uint8_t standardflashReadLockdownReg(uint32_t address);

/*!
 * @brief OPCODE: 0x9B <br>
 * Programs 'txNumBytes' bytes of data to the OTP register starting at the address indicated
 * by address.
 *
 * @warning This is a one time operation, and special rules apply. Read the datasheet.
 *
 * @param address The 3 bytes address indicating the first location to be written to.
 * @param txBuffer Pointer to the tx bytes that will be stored in memory. Must
 * have a minimum of txNumBytes elements.
 * @param txNumBytes Number of bytes to be written to the device. txBuffer
 * must have a minimum of this many elements.
 *
 * @retval void
 */
void standardflashProgramOTPReg(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes);

#endif

#endif /* STANDARDFLASH_H_ */
