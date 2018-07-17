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
 * @file    fusion.c
 * @brief   Definition of Fusion functions.
 */

#include "fusion.h"

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

static bool DISPLAY_OUTPUT = 0;

static void debugOn() {DISPLAY_OUTPUT = 1;};
static void debugOff() {DISPLAY_OUTPUT = 0;};

uint8_t txFusionInternalBuffer[MAXIMUM_TX_BYTES];

void fusionWaitOnReady()
{
	uint8_t SR[2];
	do
	{
		fusionReadSR(SR);
		SPI_Delay(10);
	}
	while(SR[0] & 1);
}

void fusionGlobalProtect()
{
	fusionWriteEnable();
	fusionWriteSRB1(0x7F);
}

void fusionGlobalUnprotect()
{
	fusionWriteEnable();
	fusionWriteSRB1(0x00);
}

void fusionReadArray(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_READ_ARRAY, address);
	SPI_Exchange(txFusionInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void fusionReadArrayLF(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_READ_ARRAY_LF, address);
	SPI_Exchange(txFusionInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void fusionDualOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_DUAL_OUTPUT_READ, address);
	SPI_DualExchange(4, txFusionInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void fusionPageErase(uint32_t address)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_PAGE_ERASE, address);
	SPI_Exchange(txFusionInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, NULL, 0);
	}
}

void fusionBlockErase4K(uint32_t address)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_BLOCK_ERASE_4K, address);
	SPI_Exchange(txFusionInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, NULL, 0);
	}
}

void fusionChipErase()
{
	txFusionInternalBuffer[0] = CMD_FUSION_CHIP_ERASE;
	SPI_Exchange(txFusionInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, NULL, 0);
	}
}

void fusionProgramArray(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_PROGRAM_ARRAY, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txFusionInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txFusionInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, totalBytes, NULL, 0);
	}
}

void fusionWriteEnable()
{
	txFusionInternalBuffer[0] = CMD_FUSION_WRITE_ENABLE;
	SPI_Exchange(txFusionInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 1, NULL, 0);
	}
}

void fusionWriteDisable()
{
	txFusionInternalBuffer[0] = CMD_FUSION_WRITE_DISABLE;
	SPI_Exchange(txFusionInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 1, NULL,  0);
	}
}

void fusionReadSR(uint8_t *rxBuffer)
{
	txFusionInternalBuffer[0] = CMD_FUSION_READ_SR;
	SPI_Exchange(txFusionInternalBuffer, 1, rxBuffer, 2, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 1, rxBuffer, 2);
	}
}

void fusionWriteSRB1(uint8_t data)
{
	txFusionInternalBuffer[0] = CMD_FUSION_WRITE_SRB1;
	txFusionInternalBuffer[1] = data;
	SPI_Exchange(txFusionInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 2, NULL, 0);
	}
}

void fusionWriteSRB2(uint8_t data)
{
	txFusionInternalBuffer[0] = CMD_FUSION_WRITE_SRB2;
	txFusionInternalBuffer[1] = data;
	SPI_Exchange(txFusionInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 2, NULL, 0);
	}
}

void fusionReset()
{
	txFusionInternalBuffer[0] = CMD_FUSION_RESET;
	txFusionInternalBuffer[1] = CMD_FUSION_RESET_CONFIRMATION;
	SPI_Exchange(txFusionInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 2, NULL, 0);
	}
}

void fusionReadMID(uint8_t *rxBuffer)
{
	txFusionInternalBuffer[0] = CMD_FUSION_READ_MID;
	SPI_Exchange(txFusionInternalBuffer, 1, rxBuffer, 4, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 1, rxBuffer, 4);
	}
}

void fusionDeepPowerDown()
{
	txFusionInternalBuffer[0] = CMD_FUSION_DEEP_POWER_DOWN;
	SPI_Exchange(txFusionInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 1, NULL, 0);
	}
}

void fusionResumeFromDeepPowerDown()
{
	txFusionInternalBuffer[0] = CMD_FUSION_RESUME_FROM_DPD;
	SPI_Exchange(txFusionInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 1, NULL, 0);
	}
}

void fusionUDPDMode()
{
	txFusionInternalBuffer[0] = CMD_FUSION_UDPD_MODE;
	SPI_Exchange(txFusionInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 1, NULL, 0);
	}
}


#if	(PARTNO == AT25XE021A)	|| \
	(PARTNO == AT25XE041B)	|| \
	(PARTNO == AT25DF021A)	|| \
	(PARTNO == AT25DF041B)	|| \
	(PARTNO == AT25XV021A)	|| \
	(PARTNO == AT25XV041B)	|| \
	(ALL == 1)
void fusionSequentialProgramMode(uint8_t txBuffer)
{
	txFusionInternalBuffer[0] = CMD_FUSION_SQNTL_PROGRAM_MODE;
	txFusionInternalBuffer[1] = txBuffer;
	SPI_Exchange(txFusionInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 2, NULL, 0);
	}
}

void fusionDualInputProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_DUAL_INPUT_PROGRAM, address);
	// Offset the data bytes by 4; opcode takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txFusionInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_DualExchange(4, txFusionInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, totalBytes, NULL, 0);
	}
}

void fusionProtectSector(uint32_t address)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_PROTECT_SECTOR, address);
	SPI_Exchange(txFusionInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, NULL, 0);
	}
}

void fusionUnprotectSector(uint32_t address)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_UNPROTECT_SECTOR, address);
	SPI_Exchange(txFusionInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, NULL, 0);
	}
}

void fusionReadSectorProtectionRegisters(uint32_t address, uint8_t *rxBuffer)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_PROTECTION_REGISTER, address);
	SPI_Exchange(txFusionInternalBuffer, 4, rxBuffer, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 4, rxBuffer, 1);
	}
}

void fusionSequentialProgramModeEnable(uint32_t address, uint8_t txBuffer)
{
	load4BytesToTxBuffer(txFusionInternalBuffer, CMD_FUSION_SQNTL_PROGRAM_MODE, address);
	txFusionInternalBuffer[4] = txBuffer;
	SPI_Exchange(txFusionInternalBuffer, 5, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txFusionInternalBuffer, 5, NULL, 0);
	}
}
#endif

void fusionHardwareReset()
{
	SPI_JEDECReset();
}

uint32_t fusionTest()
{
    // Sets the various pins as inputs and output
    SPI_ConfigureSingleSPIIOs();

	// Instantiate the arrays needed for testing purposes.
	// dataWrite is used when sending data that will be flashed to the device.
	uint8_t dataWrite[MAXIMUM_BUFFER_SIZE] = {0};
	// dataRead is used as a buffer for received data. Read data will be stored here.
	uint8_t dataRead[MAXIMUM_BUFFER_SIZE] = {0};
	// dataTest is used as a buffer for comparisons. dataRead will be
	// loaded with data and compared against this buffer for equality.
	uint8_t dataTest[MAXIMUM_BUFFER_SIZE] = {0};
	// Count the number of errors, this is output at the end of the testbench.
	uint32_t errorCount = 0;

	// Store all Fs in the test buffer for later comparison
	for(int i = 0; i < MAXIMUM_BUFFER_SIZE; i++)
	{
		dataTest[i] = 0xFF;
	}
#if PARTNO == AT25XE512C
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x65, 0x01, 0x00};
#elif PARTNO == AT25XE011
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x42, 0x00, 0x00};
#elif PARTNO == AT25XE021A
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x43, 0x01, 0x00};
#elif PARTNO == AT25XE041B
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x44, 0x02, 0x00};
#elif PARTNO == AT25DN256
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x40, 0x00, 0x00};
#elif PARTNO == AT25DN512C
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x65, 0x01, 0x00};
#elif PARTNO == AT25DN011
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x42, 0x00, 0x00};
#elif PARTNO == AT25DF256
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x40, 0x00, 0x00};
#elif PARTNO == AT25DF512C
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x64, 0x01, 0x00};
#elif PARTNO == AT25DF011
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x42, 0x00, 0x00};
#elif PARTNO == AT25DF021A
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x43, 0x01, 0x00};
#elif PARTNO == AT25DF041B
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x44, 0x02, 0x00};
#elif PARTNO == AT25XV021A
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x43, 0x01, 0x00};
#elif PARTNO == AT25XV041B
	// Store the manufacturing ID for later comparison
	uint8_t MID[4] = {0x1F, 0x44, 0x02, 0x00};
#endif

	// Load up dataWrite with a pattern that will be stored in flash memory.
	for(uint32_t a = 0; a < 100; a++)
	{
		dataWrite[a] = (uint8_t) 0xA+a;
	}

	/********************************************************************
	 * 1. Read manufacturing ID, test write enable, test write disable. *
	 ********************************************************************/

	printf("\n\nTesting Read MID, WEnable, and WDisable ----------\n\n");

	// Part A: Test that the MID can be read.
	// Read the manufacturing ID and store the returned data in dataRead
	fusionReadMID(dataRead);
	// Compare the read data to the expected bytes stored in MID. Print
	// the result of the test.
	if(compareByteArrays(dataRead, MID, 4))
	{
		printf("ReadMID Success.\n");
	}
	else
	{
		printf("ReadMID fail.\n");
		errorCount++;
	}


	// Part B: Ensure that the device can be write enabled.
	// The device is write enabled...
	fusionWriteEnable();
	// ... the status register is then read ..
	fusionReadSR(dataRead);
	// ... and the write enable bit checked.
	// WE should be 1 if the command and setup are operational.
	if(!((dataRead[0] >> 1) & 1))
	{
		printf("Error with WE or Read SR");
		errorCount++;
	}
	else
	{
		printf("WE success.\n");
	}


	// Part C: Now attempt to disable write.
	// The command is sent to disable write...
	fusionWriteDisable();
	// ... the status register is then read...
	fusionReadSR(dataRead);
	// ... and the write enable bit checked.
	// WE should be 0 if the command and setup are operational.
	if(((dataRead[0] >> 1) & 1))
	{
		printf("Error with WD or Read SR");
		errorCount++;
	}
	else
	{
		printf("WD success.\n");
	}

#if	(PARTNO == AT25XE021A)	|| \
	(PARTNO == AT25XE041B)	|| \
	(PARTNO == AT25DF021A)	|| \
	(PARTNO == AT25DF041B)	|| \
	(PARTNO == AT25XV021A)	|| \
	(PARTNO == AT25XV041B)	|| \
	(ALL == 1)
	/********************************************************************
	 *        2. Test that sectors can be properly unprotected.         *
	 ********************************************************************/


	printf("\n\nTesting Un/protect Sector ------------------------\n\n");

	// Part A: Guarantee sector 0 is protected.
	// To start with, ensure that sector 0 is protected. It should be if
	// the test bench has been run before this and completed.
	fusionReadSectorProtectionRegisters(0, dataRead);
	// The register should read 0xFF is protected.
	if(dataRead[0] != 0xFF)
	{
		printf("Error with protection SR of sector 0");
		errorCount++;
	}
	else
	{
		printf("Protect success.\n");
	}

	// Part B: Unprotect sector 0.
	// Write enable the device since we'll be writing to it...
	fusionWriteEnable();
	// ... and unprotect sector 0.
	fusionUnprotectSector(0);
	// Follow up by reading the status register which should be 0x00
	// if the sector was unprotected properly.
	fusionReadSectorProtectionRegisters(0, dataRead);
	// Sector 0 should be unprotected (0x00)
	if(dataRead[0] != 0x00)
	{
		printf("Error unprotecting sector 0");
		errorCount++;
	}
	else
	{
		printf("Unprotect success.\n");
	}
#endif
	/********************************************************************
	 *           	 3. Test the page erase functionality.   	        *
	 ********************************************************************/

	printf("\n\nTesting Erase ------------------------------------\n\n");

	// Test by erasing the device and confirming all bits are set.
	// Write enable the device since we'll be issuing a command that will modify stored data.
	fusionWriteEnable();
	// Issue a page erase command starting from address 0 (page 0).
	fusionPageErase(0);
	// Wait for the operation to complete.
	fusionWaitOnReady();
	// Now read the flash device. All bits should be set -> FF for each byte.
	fusionReadArray(0, dataRead, 100);
	// If any of the first 100 bytes are not 0xFF, print an error message.
	if(compareByteArrays(dataRead, dataTest, 100))
	{
		printf("PageErase success.\n");
	}
	else
	{
		printf("PageErase fail.\n");
		errorCount++;
	}

#if	(PARTNO == AT25XE021A)	|| \
	(PARTNO == AT25XE041B)	|| \
	(PARTNO == AT25DF021A)	|| \
	(PARTNO == AT25DF041B)	|| \
	(PARTNO == AT25XV021A)	|| \
	(PARTNO == AT25XV041B)	|| \
	(ALL == 1)
	/********************************************************************
	 *         	 4. Test the sequential/byte program mode.  	        *
	 ********************************************************************/

	printf("\n\nTesting Sequential Program Mode -------------------\n\n");

	// Part A: Sequentially program the device to confirm sequential program works.
	// Write enable the device since we'll be issuing a command that will modify stored data.
	fusionWriteEnable();
	// Begin sequentially programming the device byte by byte from address 0.
	// The first byte programmed is included in the initial program command.
	fusionSequentialProgramModeEnable(0, 0);
	// Set the first element of the comparison buffer to zero, the byte programmed above.
	dataTest[0] = 0x00;
	// Now program 48 other bytes, and store the values in the test buffer. Note the offset
	// (j=1) since the first byte was already programmed.
	for(int j = 1; j < 50; j++)
	{
		dataTest[j] = (uint8_t) j;
		fusionSequentialProgramMode(j);
	}

	// Part B: Leave sequential program mode.
	// Exit sequential program mode by issuing a write disable command.
	fusionWriteDisable();

	// Part C: Test that the data was written properly.
	// For the sake of testing that we can no longer write in sequential program mode, try
	// writing one last byte. Since we erase this memory location in a prior operation, a
	// 'failed' write in sequential mode means it should still be FF. FF is stored in the buffer
	// for this element so an error will be printed if they do not match.
	fusionSequentialProgramMode(0x0F);
	// Read the array, and compare against known data.
	fusionReadArray(0, dataRead, 100);
	if(compareByteArrays(dataRead, dataTest, 51))
	{
		printf("Sequential operation success.\n");
	}
	else
	{
		printf("Sequential operation fail.\n");
		errorCount++;
	}
	// We checked the 'failed' sequential program mode above, but if indeed it still programmed,
	// This will print out a directed message.
	if(dataRead[0x50] != 0xFF)
	{
		printf("Error, data programmed when write disabled.");
		errorCount++;
	}

	/********************************************************************
	 *        		    5. Test dual read and write.	 	    	    *
	 ********************************************************************/

	printf("\n\nTesting Dual Read/Write Mode -------------------------\n\n");

	// Part A: Program the device in dual mode.
	// Write enable the device since we'll be modifying data.
	fusionWriteEnable();
	// Perform a block erase to setup the test.
	fusionBlockErase4K(0);
	// Wait for the block erase to finish.
	fusionWaitOnReady();
	// Reissue the write enable command since we'll be modifying the data a second time.
	fusionWriteEnable();
	// Program the first 50 bytes in dual input program mode.
	fusionDualInputProgram(0, dataWrite, 50);
	// Wait for the operation to complete. (data is sent to a buffer and the write to flash occurs
	// after CSb is deselected).
	fusionWaitOnReady();

	// Part B: Confirm dual mode write worked.
	// Now perform a standard read. This ensures that the data is readable in both formats
	// and that there wasn't some sort of odd segmentation.
	fusionReadArray(0, dataRead, 100);
	// Compare against known data.
	if(compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Dual input program operation success.\n");
	}
	else
	{
		printf("Dual input program operation fail.\n");
		errorCount++;
	}

	// Part C: Test that dual output read works.
	// Now test dual output read...
	fusionDualOutputRead(0, dataRead, 100);
	// ... and compare against known data.
	if(compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Dual output read operation success.\n");
	}
	else
	{
		printf("Dual output read operation fail.\n");
		errorCount++;
	}
#endif
	/********************************************************************
	 *        		    6. Test deep power down mode	 	    	    *
	 ********************************************************************/

	printf("\n\nTesting Deep Power Down functionality -----------------\n\n");

	// Part A: Test that the device goes into DPD mode.
	// Send the device into deep power down (DPD) mode. In DPD mode the MISO line is in a high impedance
	// state and the data will be unpredictable. We test for this by reading the device and confirming
	// that the output doesn't match the stored data. The probability that it will is infinitesimal.
	// If any errors are output, the device is in DPD Mode and the test passes.
	fusionDeepPowerDown();
	// Attempt to read from the device. Note, this should not work in DPD Mode.
	fusionReadArray(0, dataRead, 100);
	// Compare the data as stated above. There should me a data mismatch in DPD mode.
	if(!compareByteArrays(dataRead, dataTest, 25))
	{
		printf("DEVICE IS IN DPD MODE. DATA MISMATCH EXPECTED. TEST PASSED.\n");
	}
	else
	{
		printf("DEVICE FAILED DPD TEST. The device is reading stored data when the line should be in Hi-Z state.\n");
		errorCount++;
	}

	// Part B: Test that the device can exit DPD mode.
	printf("Resuming from Deep Power Down Mode...\n");

	// Now resume from DPD mode...
	fusionResumeFromDeepPowerDown();
	// ... and try to read from the device. It should function in its normal state.
	fusionReadArray(0, dataRead, 50);
	// A comparison of read data to expected data should pass.
	if(compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Resume from DPD operation success.\n");
	}
	else
	{
		printf("Resume from DPD operation fail.\n");
		errorCount++;
	}

#if	(PARTNO == AT25XE021A)	|| \
	(PARTNO == AT25XE041B)	|| \
	(PARTNO == AT25DF021A)	|| \
	(PARTNO == AT25DF041B)	|| \
	(PARTNO == AT25XV021A)	|| \
	(PARTNO == AT25XV041B)	|| \
	(ALL == 1)
	/********************************************************************
	 * 		    7. Test global protect and global unprotect.	   	    *
	 ********************************************************************/

	printf("\n\nTesting Global Un/Protect -----------------------------\n\n");

	// Part A: Confirm the cuttenr state of the protection registers.
	// This is not a comprehensive test. We will examine sector zero protect and unprotect
	// to determine that the protection register does indeed change based on the commands issued.
	// From previous steps, sector zero was unprotected, so confirm that this is indeed the case
	// first.
	// Read sector 0's protection register
	fusionReadSectorProtectionRegisters(0, dataRead);
	// An unprotected sector should read 0x00.
	if(dataRead[0] != 0x00)
	{
		printf("Error with sector 0 protection");
		errorCount++;
	}

	// Part B: Protect the sectors.
	// Now attempt to protect all sectors.
	fusionGlobalProtect();
	// Read the sector protection register for sector zero. It should read 0xFF for protected.
	fusionReadSectorProtectionRegisters(0, dataRead);
	// Confirm that the sector is protected.
	if(dataRead[0] != 0xFF)
	{
		printf("Error with global protect");
		errorCount++;
	}
	else
	{
		printf("Global protect success.\n");
	}

	// Part C: Unprotect the sectors.
	// Now perform a global unprotect to confirm we can leave the protected state through
	// this command.
	fusionGlobalUnprotect();
	// Read the SPR again and confirm sector 0 is unprotected.
	fusionReadSectorProtectionRegisters(0, dataRead);
	if(dataRead[0] != 0x00)
	{
		printf("Error with global unprotect");
		errorCount++;
	}
	else
	{
		printf("Global unprotect success.\n");
	}
#endif
	/********************************************************************
	 * 	    8. Test ultra deep power down mode and JEDEC reset.   	    *
	 ********************************************************************/

	printf("\n\nTesting UDPD Mode and JEDEC Reset --------------------\n\n");

	// Part A: Enter UDPD mode and confirm that the device outputs Hi-Z.
	// Set the device into ultra deep power down (UDPD) mode. The tests performed here
	// are the same as the ones performed for the DPD mode, but the device is woken up
	// with a JEDEC reset.
	fusionUDPDMode();
	// Attempt to read from the device. Note, this should not work in DPD Mode.
	fusionReadArray(0, dataRead, 100);
	// Compare the data as stated above. There should me a data mismatch in DPD mode.
	if(!compareByteArrays(dataRead, dataTest, 25))
	{
		printf("DEVICE IS IN UDPD MODE. DATA MISMATCH EXPECTED. TEST PASSED.\n");
	}
	else
	{
		printf("DEVICE FAILED UDPD TEST. The device is reading stored data when the line should be in Hi-Z state.\n");
		errorCount++;
	}

	// Part B: Test that the device can recover from UDPD mode.
	// Exit UDPD mode by performing a hardware reset.
	fusionHardwareReset();
	printf("JEDEC reset performed, device should be operational.\n");
	// Perform a device read and compare against known data.
	fusionReadArray(0, dataRead, 50);
	if(compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("JEDEC reset operation success.\n");
	}
	else
	{
		printf("JEDEC reset operation fail.\n");
		errorCount++;
	}

	// Test complete, exit and print messages.
	printf("\n\n#############################################\n\n");
	printf("Testing complete.\n");
	printf("Total errors detected: %d\n", errorCount);
	printf("Terminating testbench...\n");
	printf("\n#############################################\n\n");

    return errorCount;
}
#endif
