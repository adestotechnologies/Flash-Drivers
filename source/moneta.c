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
 * @file    moneta.c
 * @brief   Definition of Moneta functions.
 */
#include "moneta.h"

#if (PARTNO == RM331x)	|| \
	(ALL == 1)

static bool DISPLAY_OUTPUT = 0;

static void debugOn() {DISPLAY_OUTPUT = 1;};
static void debugOff() {DISPLAY_OUTPUT = 0;};

uint8_t txMonetaInternalBuffer[MAXIMUM_TX_BYTES];

void monetaWaitOnReady()
{
	uint8_t SR[2];
	do
	{
		monetaReadSR(SR);
		SPI_Delay(10);
	}
	while(SR[0] & 1);
}

void monetaWriteEnable()
{
	txMonetaInternalBuffer[0] = CMD_MONETA_WRITE_ENABLE;
	SPI_Exchange(txMonetaInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 1, NULL, 0);
	}
}


void monetaWriteDisable()
{
	txMonetaInternalBuffer[0] = CMD_MONETA_WRITE_DISABLE;
	SPI_Exchange(txMonetaInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 1, NULL,  0);
	}
}


void monetaReadSR(uint8_t *rxBuffer)
{
	txMonetaInternalBuffer[0] = CMD_MONETA_READ_SRB1;
	SPI_Exchange(txMonetaInternalBuffer, 1, rxBuffer, 2, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 1, rxBuffer, 2);
	}
}


void monetaWriteSRB1(uint8_t data)
{
	txMonetaInternalBuffer[0] = CMD_MONETA_WRITE_SRB1;
	txMonetaInternalBuffer[1] = data;
	SPI_Exchange(txMonetaInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 2, NULL, 0);
	}
}


void monetaWriteSRB2(uint8_t data)
{
	txMonetaInternalBuffer[0] = CMD_MONETA_WRITE_SRB2;
	txMonetaInternalBuffer[1] = data;
	SPI_Exchange(txMonetaInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 2, NULL, 0);
	}
}


void monetaReadArray(uint16_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	txMonetaInternalBuffer[0] = CMD_MONETA_READ_ARRAY;
	txMonetaInternalBuffer[1] = (uint8_t) (address >> 8);
	txMonetaInternalBuffer[2] = (uint8_t) address;
	SPI_Exchange(txMonetaInternalBuffer, 3, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 3, rxBuffer, rxNumBytes);
	}
}


void monetaWriteArray(uint16_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	// TODO: Eliminate this copy. I think this needs to be done at the lower level,
	// possibly by passing in the opcode and data separately.
	txMonetaInternalBuffer[0] = CMD_MONETA_WRITE_ARRAY;
	txMonetaInternalBuffer[1] = (uint8_t) (address >> 8);
	txMonetaInternalBuffer[2] = (uint8_t) address;
	// Offset the data bytes by 3; opcode+address takes up the first 3 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 3;
	uint32_t i;
	uint32_t j;
	for(i = 3, j = 0; i < totalBytes; i++, j++)
	{
		txMonetaInternalBuffer[i] = txBuffer[j];
	}
	SPI_Exchange(txMonetaInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, totalBytes, NULL, 0);
	}
}


void monetaReadMID(uint8_t *rxBuffer)
{
	txMonetaInternalBuffer[0] = CMD_MONETA_READ_MID;
	SPI_Exchange(txMonetaInternalBuffer, 1, rxBuffer, 8, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 1, rxBuffer, 8);
	}
}

void monetaUDPDMode1()
{
	txMonetaInternalBuffer[0] = CMD_MONETA_UDPD_MODE1;
	SPI_Exchange(txMonetaInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txMonetaInternalBuffer, 1, NULL, 0);
	}
}

void monetaHardwareReset()
{
	SPI_JEDECReset();
	if(DISPLAY_OUTPUT)
	{
		printf("\n\n\nPerformed hardware reset.\n");
	}
}

int monetaTest()
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
	int errorCount = 0;

	// Store all Fs in the test buffer for later comparison
	for(int i = 0; i < MAXIMUM_BUFFER_SIZE; i++)
	{
		dataTest[i] = 0xFF;
	}

	// Store the manufacturing ID for later comparison
	uint8_t MID[8] = {0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x43};

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
	monetaReadMID(dataRead);
	// Compare the read data to the expected bytes stored in MID. Print
	// the result of the test.
	if(compareByteArrays(dataRead, MID, 8))
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
	monetaWriteEnable();
	// ... the status register is then read ..
	monetaReadSR(dataRead);
	// ... and the write enable bit checked.
	// WE should be 1 if the command and setup are operational.
	if(!((dataRead[0] >> 1) & 1))
	{
		printf("Error with WE or Read SR\n");
		errorCount++;
	}
	else
	{
		printf("WE success.\n");
	}

	// Part C: Now attempt to disable write.
	// The command is sent to disable write...
	monetaWriteDisable();
	monetaWaitOnReady();

	/********************************************************************
	 *        		   	 2. Test read and write.		 	    	    *
	 ********************************************************************/

	printf("\n\nTesting read and write -----------------------------\n\n");

	// Part A: Program the device.
	// Write enable the device since we'll be modifying data.
	monetaWriteEnable();
	monetaWaitOnReady();
	// Program the first 50 bytes.
	monetaWriteArray(0, dataWrite, 50);
	monetaWaitOnReady();

	// Part B: Check that the data is correct by reading and comparing to a known value.
	monetaReadArray(0, dataRead, 100);
	// Compare against known data.
	if(compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Program and read operations successful.\n");
	}
	else
	{
		printf("Program and read operations fail.\n");
		errorCount++;
	}

	/********************************************************************
	 * 	    3. Test ultra deep power down mode and JEDEC reset.   	    *
	 ********************************************************************/

	printf("\n\nTesting UDPD Mode and JEDEC Reset --------------------\n\n");

	// Part A: Enter UDPD mode and confirm that the device outputs Hi-Z.
	// Set the device into ultra deep power down (UDPD) mode.
	monetaUDPDMode1();
	// Attempt to read from the device. Note, this should not work in DPD Mode.
	monetaReadArray(0, dataRead, 100);
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
	monetaHardwareReset();
	printf("JEDEC reset performed, device should be operational.\n");
	// Perform a device read and compare against known data.
	monetaReadArray(0, dataRead, 50);
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
