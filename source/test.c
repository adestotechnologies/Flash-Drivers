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
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*!
 * @ingroup TEST_LAYER MAIN
 */
/**
 * @file    test.c
 * @brief   Test layer definitions exist here.
 */

#include "test.h"

#if defined(MONETA_DEVICE)

uint32_t defaultTest(){return monetaTest();};

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

#elif defined(FUSION_DEVICE)

uint32_t defaultTest(){return fusionTest();};

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

#elif defined(DATAFLASH_DEVICE)

uint32_t defaultTest(){return dataflashTest();};

uint32_t dataflashTest()
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

#if PARTNO == AT45DB021E
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x23, 0x00, 0x01, 0x00};
#elif PARTNO == AT45DB041E
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x24, 0x00, 0x01, 0x00};
#elif PARTNO == AT45DB081E
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x25, 0x00, 0x01, 0x00};
#elif PARTNO == AT45DB161E
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x26, 0x00, 0x01, 0x00};
#elif PARTNO == AT45DB321E
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x27, 0x01, 0x01, 0x00};
#elif PARTNO == AT45DB641E
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x28, 0x00, 0x01, 0x00};
#elif PARTNO == AT45DQ161
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x26, 0x00, 0x01, 0x00};
#elif PARTNO == AT45DQ321
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x27, 0x01, 0x01, 0x00};
#elif PARTNO == AT25PE20
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x23, 0x00, 0x01, 0x00};
#elif PARTNO == AT25PE40
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x24, 0x00, 0x01, 0x00};
#elif PARTNO == AT25PE80
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x25, 0x00, 0x01, 0x00};
#elif PARTNO == AT25PE16
	// Store the manufacturing ID for later comparison
	uint8_t MID[5] = {0x1F, 0x26, 0x00, 0x01, 0x00};
#endif


	// Store all Fs in the test buffer for later comparison
	fillArrayConst(dataTest, MAXIMUM_BUFFER_SIZE, 0xFF);
	// Load up dataWrite with a pattern that will be used for testing.
	fillArrayPattern(dataWrite, 100, 0x00);


	/********************************************************************
	 *		    		1. Read manufacturing ID						*
	 ********************************************************************/

	printf("\n\nTesting Read MID -----------------------------------\n\n");

	// Part A: Test that the MID can be read.
	// Read the manufacturing ID and store the returned data in dataRead
	dataflashReadMID(dataRead);
	// Compare the read data to the expected bytes stored in MID. Print
	// the result of the test.
	if(compareByteArrays(dataRead, MID, 5))
	{
		printf("ReadMID Success.\n");
	}
	else
	{
		printf("ReadMID fail.\n");
		errorCount++;
	}


	/********************************************************************
	 *       		2. Test read and write functionality.		        *
	 ********************************************************************/
	/*
	 * This test is comprised of 4 components:
	 * Part A: Test buffer read and buffer write.
	 * Part B: Test buffer to main memory.
	 * Part C: Test buffer to memory without erase.
	 * Part D: Test program through buffer without erase.
	 */

	printf("\n\nTesting Read and Write functionality ---------------\n\n");

	// Test buffer 1:
	// Part A: Test that the buffers can be written to and read.
	// Data will be written to the buffers then read back using both high and low frequency commands
	fillArrayPattern(dataWrite, 100, 0x05);
	dataflashBuffer1Write(0, dataWrite, 100);
	dataflashBuffer1ReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Buffer 1 Write or ReadLF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Buffer 1 Write and ReadLF success.\n");
	}
	dataflashBuffer1ReadHighFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Buffer 1 Write or ReadHF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Buffer 1 Write and ReadHF success.\n");
	}
	// Test buffer 1:
	// Part B: Test that data can be written to main memory.
	// Data will be programmed to the device through the buffer, and then read back with different
	// read commands. Each read command will be tested, and success/failure messages output to the
	// console.
	dataflashBuffer1ToMainMemoryWithErase(0);
	dataflashWaitOnReady();
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Main memory through Buffer 1 r/w failure.\n");
		errorCount++;
	}
	else
	{
		printf("Main memory through Buffer 1 r/w success.\n");
	}

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
	// Test buffer 2:
	// Part A: Test that the buffers can be written to and read.
	// Data will be programmed to the device through the buffer, read back,
	// and success/failure message output to the console.
	fillArrayPattern(dataWrite, 100, 0x25);
	dataflashBuffer2Write(0, dataWrite, 100);
	dataflashBuffer2ReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Buffer 2 Write or ReadLF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Buffer 2 Write and ReadLF success.\n");
	}
	dataflashBuffer2ReadHighFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Buffer 2 Write or ReadHF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Buffer 2 Write and ReadHF success.\n");
	}

	// Test buffer 2:
	// Part B: Test that data can be written to main memory with erase.
	// Data will be programmed to the device through the buffer, read back,
	// and success/failure message output to the console.

	dataflashBuffer2ToMainMemoryWithErase(0);
	dataflashWaitOnReady();
	dataflashArrayReadLowPower(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Main memory through Buffer 2 r/w failure.\n");
		errorCount++;
	}
	else
	{
		printf("Main memory through Buffer 2 r/w success.\n");
	}
#endif
	// Part C: Test buffer to memory without erase.
	// All 0s will be stored in the buffers and then the buffers will be written to main memory
	// without erase. This should reset all bits.

	// Test buffer 1:
	// Load zeroes into the array
	fillArrayConst(dataWrite, 100, 0);
	// Store the values in buffer 1
	dataflashBuffer1Write(0, dataWrite, 100);
	// Write the data to main memory from buffer 1 without erase
	dataflashBuffer1ToMainMemoryWithoutErase(0);
	// Read back and compare
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Buffer 1 to main memory failure.\n");
		errorCount++;
	}
	else
	{
		printf("Buffer 1 to main memory success.\n");
	}

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
	// Test buffer 2:
	// Load zeroes into the array
	fillArrayConst(dataWrite, 100, 0);
	// Store the values in buffer 1
	dataflashBuffer2Write(100, dataWrite, 100);
	// Write the data to main memory from buffer 1 without erase
	dataflashBuffer2ToMainMemoryWithoutErase(0);
	// Read back and compare
	dataflashArrayReadLowFreq(100, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Buffer 2 to main memory failure.\n");
		errorCount++;
	}
	else
	{
		printf("Buffer 2 to main memory success.\n");
	}
#endif
	// Part D: Test program through buffer with erase.
	// Program through buffer 1 and compare the buffer to the input data.
	fillArrayPattern(dataWrite, 100, 0x45);
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();
	dataflashBuffer1ReadHighFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Program through Buffer 1 failure.\n");
		errorCount++;
	}
	else
	{
		printf("Program through Buffer 1 success.\n");
	}
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
	// Program through buffer 2 and compare the buffer to the input data.
	fillArrayPattern(dataWrite, 100, 0x23);
	dataflashMemoryProgramThruBuffer2WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();
	dataflashBuffer2ReadHighFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Program through Buffer 2 failure.\n");
		errorCount++;
	}
	else
	{
		printf("Program through Buffer 2 success.\n");
	}
#endif
	/********************************************************************
	 *        		    3. Test deep power down mode	 	    	    *
	 ********************************************************************/
	/*
	 * This portion tests the deep power down mode of the flash device.
	 */

	printf("\n\nTesting Deep Power Down functionality ---------------\n\n");

	// Part A: Test that the device goes into DPD mode.
	// Send the device into deep power down (DPD) mode. In DPD mode the MISO line is in a high impedance
	// state and the data will be unpredictable. We test for this by reading the device and confirming
	// that the output doesn't match the stored data. The probability that it will is infinitesimal.
	// If any errors are output, the device is in DPD Mode and the test passes.

	// First, program some data to the device.
	fillArrayPattern(dataWrite, 100, 0x90);
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();

	// Send the device to deep power down mode.
	dataflashDPD();
	// Attempt to read from the device. Note, this should not work in DPD Mode.
	dataflashArrayReadLowFreq(0, dataRead, 100);
	// Compare the data as stated above. There should be a data mismatch in DPD mode.
	if(!compareByteArrays(dataRead, dataWrite, 25))
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
	dataflashResumeFromDPD();
	dataflashWaitOnReady();
	// ... and try to read from the device. It should function in its normal state.
	dataflashArrayReadHighFreq0(0, dataRead, 100);
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


	/********************************************************************
	 * 	    4. Test ultra deep power down mode and JEDEC reset.   	    *
	 ********************************************************************/
	/*
	 * This portion tests the ultra deep power down mode and JEDEC reset.
	 */
	printf("\n\nTesting UDPD Mode and JEDEC Reset --------------------\n\n");

	// Part A: Enter UDPD mode and confirm that the device outputs Hi-Z.
	// Set the device into ultra deep power down (UDPD) mode. The tests performed here
	// are the same as the ones performed for the DPD mode, but the device is woken up
	// with a JEDEC reset.
	dataflashUDPDMode();
	// Attempt to read from the device. Note, this should not work in DPD Mode.
	dataflashArrayReadLowFreq(0, dataRead, 100);
	// Compare the data as stated above. There should me a data mismatch in DPD mode.
	if(!compareByteArrays(dataRead, dataWrite, 25))
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
	dataflashHardwareReset();
	printf("JEDEC reset performed, device should be operational.\n");
	// Perform a device read and compare against known data.
	dataflashArrayReadHighFreq1(0, dataRead, 100);
	if(compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("JEDEC reset operation success.\n");
	}
	else
	{
		printf("JEDEC reset operation fail.\n");
		errorCount++;
	}


	/********************************************************************
	 * 	 					   5. Test erase.   					    *
	 ********************************************************************/

	// This section tests all 4 erase commands available for the dataflash part.
	// A page erase will be performed, followed by block, sector, and chip erase.
	// In each test data is programmed to main memory, read back for confirmation,
	// then erased, at which point it's read back and tested against all F's.

	printf("\n\nTesting Erase Commands ------------------------------\n\n");

	// Part A: Test page erase.
	// Erase the page so that data can be written.
	dataflashPageErase(0);
	// Wait for the erase to finish.
	dataflashWaitOnReady();
	// Prep the data to be written and write it.
	fillArrayPattern(dataWrite, 100, 0x56);
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();
	// Make sure the data was properly written
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 25))
	{
		printf("Device r/w fail.\n");
		errorCount++;
	}
	// Erase the page
	dataflashPageErase(0);
	// ... and wait.
	dataflashWaitOnReady();
	// Read it back and test that the erase worked.
	dataflashArrayReadLegacy(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataTest, 25))
	{
		printf("Device page erase fail.\n");
		errorCount++;
	}
	else
	{
		printf("Page erase successful.\n");
	}


	// Part B: Repeat the above operation but perform a block erase instead.
	// Prep the device with some random sequence.
	fillArrayPattern(dataWrite, 100, 0x23);
	// Program the data to memory.
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	// Wait for the operation to complete.
	dataflashWaitOnReady();
	// Read the data back for confirmation.
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 25))
	{
		printf("Device r/w fail.\n");
		errorCount++;
	}
	// Block erase.
	dataflashBlockErase(0);
	// Wait for the erase to complete.
	dataflashWaitOnReady();
	// Confirm the data was erased.
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataTest, 25))
	{
		printf("Device block erase fail.\n");
		errorCount++;
	}
	else
	{
		printf("Block erase successful.\n");
	}

	// Part C: Repeat the above operation but perform a sector erase instead.
	fillArrayPattern(dataWrite, 100, 0x89);
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 25))
	{
		printf("Device r/w fail.\n");
		errorCount++;
	}
	// Sector erase.
	dataflashSectorErase(0);
	dataflashWaitOnReady();
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataTest, 25))
	{
		printf("Device sector erase fail.\n");
		errorCount++;
	}
	else
	{
		printf("Sector erase successful.\n");
	}

	// Part D: Repeat the above operation but perform a chip erase instead.
	fillArrayPattern(dataWrite, 100, 0x76);
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();
	dataflashArrayReadLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 25))
	{
		printf("Device r/w fail.\n");
		errorCount++;
	}
	// Chip erase.
//	dataflashChipErase(0);
//	dataflashWaitOnReady();
//	dataflashArrayReadLowPower(0, dataRead, 100);
//	if(!compareByteArrays(dataRead, dataTest, 25))
//	{
//		printf("Device chip erase fail.\n");
//		errorCount++;
//	}
//	else
//	{
//		printf("Chip erase successful.\n");
//	}

	/********************************************************************
	 * 	 				 6. Test memory to buffer commands		 	 	*
	 ********************************************************************/
	// This section tests 2 memory/buffer transactions; memory to buffer transfer
	// and memory to buffer compare. Data is written to memory through buffer 1 and
	// then the memory to buffer commands store data from memory to buffers 1 and 2.
	// The memory to buffer command opcode is then sent over and the status register
	// read. Bit 6 is checked, and a success/fail message printed to the console.
	printf("\n\nTesting Memory to Buffer Commands -------------------\n\n");

	// Fill the txBuffer with a pattern
	fillArrayPattern(dataWrite, 100, 0x88);
	// Program the data through buffer 1
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	// Wait for the operation to complete.
	dataflashWaitOnReady();
	// Read data to buffer 1
	dataflashMemtoBuffer1Transfer(0);
	dataflashWaitOnReady();
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
	// and read data to buffer 2
	dataflashMemtoBuffer2Transfer(0);
	dataflashWaitOnReady();
#endif
	// Finally, run the compare and read the status register to confirm that the
	// operations were successful.
	dataflashMemtoBuffer1Compare(0);
	dataflashWaitOnReady();
	dataflashReadSR(dataRead);
	if(dataRead[0] & (1<<6))
	{
		printf("Error with ReadSR, Buffer 1 to Memory Compare, or Memory Program.\n");
		errorCount++;
	}
	else
	{
		printf("Memory to Buffer 1 commands successful.\n");
	}
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
	// Run the comparison on buffer 2
	dataflashMemtoBuffer2Compare(0);
	dataflashWaitOnReady();
	dataflashReadSR(dataRead);
	if(dataRead[0] & (1<<6))
	{
		printf("Error with ReadSR, Buffer 2 to Memory Compare, or Memory Program.\n");
		errorCount++;
	}
	else
	{
		printf("Memory to Buffer 2 commands successful.\n");
	}
#endif

	/********************************************************************
	 * 	 			7. Test Read - Modify - Write capabilities.	 	 	*
	 ********************************************************************/
	// This portion tests the RMW capabilities of the flash device by modifying a single byte
	// and storing it in main main memory.

	printf("\n\nTesting RMW Commands --------------------------------\n\n");
	// Write data to main memory with erase
	fillArrayPattern(dataWrite, 100, 0x12);
	dataflashMemoryProgramThruBuffer1WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();
	// Load a new value to dataWrite byte 5.
	if(dataWrite[4] != 0xAA)
	{
		dataWrite[4] = 0xAA;
	}
	else
	{
		dataWrite[4] = 0x55;
	}
	// Store the new byte in main memory.
	dataflashRMWThruBuffer1(4, &dataWrite[4], 1);
	dataflashWaitOnReady();
	// Read back the data and make sure the command worked properly.
	dataflashArrayReadLowPower(0, dataRead, 100);
	if(!compareByteArrays(dataWrite, dataRead, 100))
	{
		printf("Error with RMW through Buffer 1 command.\n");
		errorCount++;
	}
	else
	{
		printf("RMW through Buffer 1 command successful.\n");
	}
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
	// Repeat the process using buffer 2, and write 2 bytes.
	fillArrayPattern(dataWrite, 100, 0x21);
	dataflashMemoryProgramThruBuffer2WithErase(0, dataWrite, 100);
	dataflashWaitOnReady();
	// Load a new value to dataWrite byte 5, and byte 6.
	if(dataWrite[5] != 0xAA)
	{
		dataWrite[5] = 0xAA;
		dataWrite[6] = 0xAB;
	}
	else
	{
		dataWrite[5] = 0x55;
		dataWrite[6] = 0x56;
	}
	// Program it in.
	dataflashRMWThruBuffer1(5, &dataWrite[5], 2);
	dataflashWaitOnReady();
	// Test the data.
	dataflashArrayReadLowPower(0, dataRead, 100);
	if(!compareByteArrays(dataWrite, dataRead, 100))
	{
		printf("Error with RMW through Buffer 2 command.\n");
		errorCount++;
	}
	else
	{
		printf("RMW through Buffer 2 command successful.\n");
	}
#endif

#if (PARTNO == AT45DQ161) || \
	(PARTNO == AT45DQ321) || \
	(ALL == 1)
	dataflashQuadEnable();
	dataflashWaitOnReady();
	// Read data on 4 IO lines
	dataflashQuadOutputRead(0, dataRead, 100);
	if(!compareByteArrays(dataWrite, dataRead, 100))
	{
		printf("Error with Quad Output Read command.\n");
		errorCount++;
	}
	else
	{
		printf("Quad Output Read command successful.\n");
	}
	dataflashQuadDisable();
	dataflashWaitOnReady();
#endif
	// Test complete. Print messages and exit.
	printf("\n\n#############################################\n\n");
	printf("Testing complete.\n");
	printf("Total errors detected: %d\n", errorCount);
	printf("Terminating testbench.\n");
	printf("\n#############################################\n\n");

	return errorCount;
}

#elif defined(STANDARDFLASH_DEVICE)

uint32_t defaultTest(){return standardflashTest();};

uint32_t standardflashTest()
{
#if PARTNO == AT25SF641
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x32, 0x17};
#elif PARTNO == AT25SF321
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x87, 0x01};
#elif PARTNO == AT25SF161
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x86, 0x01};
#elif PARTNO == AT25SF081
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x85, 0x01};
#elif PARTNO == AT25SF041
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x84, 0x01};
#elif PARTNO == AT25SL321
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x42, 0x16};
#elif PARTNO == AT25SL641
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x43, 0x17};
#elif PARTNO == AT25SL128A
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x42, 0x18};
#elif PARTNO == AT25DL081
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x45, 0x02};
#elif PARTNO == AT25DL161
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x46, 0x03};
#elif PARTNO == AT25DF081A
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x45, 0x01};
#elif PARTNO == AT25DF321A
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x47, 0x01};
#elif PARTNO == AT25DF641A
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x48, 0x00};
#elif PARTNO == AT25QL321
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x42, 0x16};
#elif PARTNO == AT25QL641
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x43, 0x17};
#elif PARTNO == AT25QL128A
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x42, 0x18};
#elif PARTNO == AT25QF641
	// Sets the modeByteValue used for instruction standardflashQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x32, 0x17};
#endif

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
	fillArrayConst(dataTest, MAXIMUM_BUFFER_SIZE, 0xFF);

	// Load up dataWrite with a pattern that will be used for testing.
	fillArrayPattern(dataWrite, 100, 0x05);


	/********************************************************************
	 *		    		1. Read manufacturing ID						*
	 ********************************************************************/

	printf("\n\nTesting Read MID -----------------------------------\n\n");

	// Part A: Test that the MID can be read.
	// Read the manufacturing ID and store the returned data in dataRead.
	standardflashReadMID(dataRead);
	// Compare the read data to the expected bytes stored in MID. Print
	// the result of the test.
	if(compareByteArrays(dataRead, MID, 3))
	{
		printf("ReadMID Success.\n");
	}
	else
	{
		printf("ReadMID fail.\n");
		errorCount++;
	}

	/********************************************************************
	 *		    		2. Write and read commands						*
	 ********************************************************************/
	/*
	 * This test performs a 4K erase followed by 2 array programs at different addresses.
	 * Data is read back using the both high and low frequency reads, and compared against
	 * the expected values.
	 */

	printf("\n\nTesting Standard Write and Read Commands -----------\n\n");
	// Part A: Test read.
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x10);
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)	|| \
	(ALL == 1)
	// Write enable the device and un-protect the first sector.
	standardflashWriteEnable();
	standardflashUnprotectSector(0);
#endif
	// Write enable the device then erase the first 4K block.
	standardflashWriteEnable();
	standardflashBlockErase4K(0);
	standardflashWaitOnReady();
	// Program the device from address 0-100.
	standardflashWriteEnable();
	standardflashBytePageProgram(0, dataWrite, 100);
	standardflashWaitOnReady();
	// Now read back the data in order to confirm that erase/program/read all work.
	standardflashReadArrayLowFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Write or ReadLF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and ReadLF success.\n");
	}
	// Part B: Test high frequency read.
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x15);
	// Program the device from address 100-200.
	standardflashWriteEnable();
	standardflashBytePageProgram(100, dataWrite, 100);
	standardflashWaitOnReady();
	// Read back and test the programmed data to confirm proper functionality.
	standardflashReadArrayHighFreq(100, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Write or ReadHF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and ReadHF success.\n");
	}
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

	/********************************************************************
	 *		    			3. Dual read commands						*
	 ********************************************************************/
	/*
	 * This test reads data from the device in dual read mode. Data is first read in
	 * dual output mode, then read in dual IO mode. 2 ways to terminate the dual
	 * continuous read mode are demonstrated.
	 * The components of the test are broken down as follows:
	 * Part A: Read data with dual output read.
	 * Part B: Read data with dual IO read and terminate using dual IO read.
	 * Part C: Read data with dual IO read and terminate using dual mode reset.
	 */
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x20);
	// Write the new data to the device.
	standardflashWriteEnable();
	standardflashBlockErase4K(0);
	standardflashWaitOnReady();
	standardflashWriteEnable();
	standardflashBytePageProgram(0, dataWrite, 200);
	standardflashWaitOnReady();

	 // Part A: Read data with dual output read and test it.
	standardflashDualOutputRead(0, dataRead, 200);
	if(!compareByteArrays(dataRead, dataWrite, 200))
	{
		printf("Write or Dual Output Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and Dual Output Read success.\n");
	}

	// Part B: Read and terminate with dual IO read command.
	standardflashDualIORead(0, dataRead, 10, 0, modeByteValue);
	standardflashDualIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	standardflashDualIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	standardflashDualIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	standardflashDualIORead(40, &(dataRead[40]), 10, 3, modeByteValue);
	if(!compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Write or Dual IO Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and Dual IO Read success.\n");
	}

	// Part C: Read and terminate with dual reset command.
	standardflashDualIORead(0, dataRead, 10, 0, modeByteValue);
	standardflashDualIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	standardflashDualIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	standardflashDualIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	standardflashDualIORead(40, &(dataRead[40]), 10, 2, modeByteValue);
	standardflashContinuousReadModeDualReset();

	/********************************************************************
	 *		    			4. Quad read commands						*
	 ********************************************************************/
	/*
	 * This test reads data from the device in quad read mode. Data is first read in
	 * quad output mode, then read in quad IO mode. 2 ways to terminate the quad
	 * continuous read mode are demonstrated.
	 * The components of the test are broken down as follows:
	 * Part A: Read data with quad output read.
	 * Part B: Read data with quad IO read and terminate using quad IO read.
	 * Part C: Read data with quad IO read and terminate using quad mode reset.
	 *
	 * Note: The QE bit in status register byte 2 must be set in order to enable quad
	 * commands. See standardflashSetQEBit().
	 */
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x25);
	// Write the new data to the device.
	standardflashWriteEnable();
	standardflashBlockErase4K(0);
	standardflashWaitOnReady();
	standardflashWriteEnable();
	standardflashBytePageProgram(0, dataWrite, 200);
	standardflashWaitOnReady();
	standardflashSetQEBit();
	standardflashWaitOnReady();
	 // Part A: Read data with quad output read and test it.
	standardflashQuadOutputRead(0, dataRead, 200);
	if(!compareByteArrays(dataRead, dataWrite, 200))
	{
		printf("Write or Quad Output Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and Quad Output Read success.\n");
	}

	// Part B: Read and terminate with quad IO read command.
	standardflashQuadIORead(0, dataRead, 10, 0, modeByteValue);
	standardflashQuadIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	standardflashQuadIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	standardflashQuadIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	standardflashQuadIORead(40, &(dataRead[40]), 10, 3, modeByteValue);
	if(!compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Write or Quad IO Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and Quad IO Read success.\n");
	}

	// Part C: Read and terminate with quad reset command.
	standardflashQuadIORead(0, dataRead, 10, 0, modeByteValue);
	standardflashQuadIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	standardflashQuadIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	standardflashQuadIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	standardflashQuadIORead(40, &(dataRead[40]), 10, 2, modeByteValue);
	standardflashContinuousReadModeQuadReset();

	standardflashClearQEBit();

	standardflashWaitOnReady();

	// Ensure that the device is no longer in quad mode.
	standardflashReadArrayHighFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Leave Quad IO failure.\n");
		errorCount++;
	}
	else
	{
		printf("Leave Quad IO success.\n");
	}
#endif

	/********************************************************************
	 *        		    5. Test deep power down mode	 	    	    *
	 ********************************************************************/
	/*
	 * This portion tests the deep power down mode of the flash device.
	 */

	printf("\n\nTesting Deep Power Down functionality ---------------\n\n");

	// Part A: Test that the device goes into DPD mode.
	// Send the device into deep power down (DPD) mode. In DPD mode the MISO line is in a high impedance
	// state and the data will be unpredictable. We test for this by reading the device and confirming
	// that the output doesn't match the stored data. The probability that it will is infinitesimal.
	// If any errors are output, the device is in DPD Mode and the test passes.

	// First, program some data to the device.
	fillArrayPattern(dataWrite, 100, 0x30);
	standardflashWriteEnable();
	standardflashBlockErase4K(0);
	standardflashWaitOnReady();
	standardflashWriteEnable();
	standardflashBytePageProgram(0, dataWrite, 100);
	standardflashWaitOnReady();

	// Send the device to deep power down mode.
	standardflashDPD();
	// Attempt to read from the device. Note, this should not work in DPD Mode.
	standardflashReadArrayLowFreq(0, dataRead, 100);
	// Compare the data as stated above. There should be a data mismatch in DPD mode.
	if(!compareByteArrays(dataRead, dataWrite, 25))
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
	standardflashResumeFromDPD();
	standardflashWaitOnReady();
	// ... and try to read from the device. It should function in its normal state.
	standardflashReadArrayHighFreq(0, dataRead, 100);
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



#if (PARTNO == AT25SF641) 	|| \
	(PARTNO == AT25SL128A)  || \
	(PARTNO == AT25SL641) 	|| \
	(PARTNO == AT25SL321) 	|| \
	(PARTNO == AT25QL128A)  || \
 	(PARTNO == AT25QL641) 	|| \
	(PARTNO == AT25QL321) 	|| \
	(PARTNO == AT25QF641)	|| \
	(ALL == 1)

	printf("\n\nThis device has Quad IO Capabilities.\n");

	printf("\n\nTesting Read MID Quad IO ----------------------------\n\n");

	/********************************************************************
	 *		    		1. Read manufacturing ID						*
	 ********************************************************************/

	standardflashWriteEnable();
	standardflashSetQEBit();
	standardflashWaitOnReady();
	standardflashWriteEnable();
	standardflashEnableQPI();
	// Part A: Test that the MID can be read.
	// Read the manufacturing ID and store the returned data in dataRead.
	standardflashReadMID(dataRead);
	// Compare the read data to the expected bytes stored in MID. Print
	// the result of the test.
	if(compareByteArrays(dataRead, MID, 3))
	{
		printf("ReadMID in Quad IO Success.\n");
	}
	else
	{
		printf("ReadMID Quad IO fail.\n");
		errorCount++;
	}

	standardflashWriteEnable();
	standardflashDisableQPI();
	standardflashWaitOnReady();
	standardflashClearQEBit();
	standardflashWaitOnReady();

	standardflashReadMID(dataRead);
	// Compare the read data to the expected bytes stored in MID. Print
	// the result of the test.
	if(compareByteArrays(dataRead, MID, 3))
	{
		printf("ReadMID Success.\nLeft Quad IO successfully.\n");
	}
	else
	{
		printf("ReadMID fail.\nFailed to Leave Quad IO.\n");
		errorCount++;
	}

	/********************************************************************
	 *		    		2. Write and read commands						*
	 ********************************************************************/
	/*
	 * This test performs a 4K erase followed by 2 array programs at different addresses.
	 * Data is read back using the both high and low frequency reads, and compared against
	 * the expected values.
	 */
	standardflashWriteEnable();
	standardflashSetQEBit();
	standardflashWaitOnReady();
	standardflashWriteEnable();
	standardflashEnableQPI();

	printf("\n\nTesting Write and Read Quad IO Commands --------------\n\n");

	// Part A: Test low frequency read.
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x30);
	// Write enable the device then erase the first 4K block.
	standardflashWriteEnable();
	standardflashBlockErase4K(0);
	standardflashWaitOnReady();
	// Program the device from address 0-100.
	standardflashWriteEnable();
	standardflashBytePageProgram(0, dataWrite, 100);
	standardflashWaitOnReady();
	// Now read back the data in order to confirm that erase/program/read all work.
	standardflashReadArrayHighFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Write or ReadHF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and ReadHF success.\n");
	}
	// Part B: Test high frequency read.
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x35);
	// Program the device from address 100-200.
	standardflashWriteEnable();
	standardflashBytePageProgram(100, dataWrite, 100);
	standardflashWaitOnReady();
	// Read back and test the programmed data to confirm proper functionality.
	standardflashReadArrayHighFreq(100, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Write or ReadHF failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and ReadHF success.\n");
	}



	/********************************************************************
	 *		    			3. Quad read commands						*
	 ********************************************************************/
	/*
	 * This test reads data from the device in quad read mode.
	 * The components of the test are broken down as follows:
	 * Part A: Read data with quad output read.
	 * Part B: Read data with quad IO read and terminate using quad IO read.
	 */
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x40);
	// Write the new data to the device.
	standardflashWriteEnable();
	standardflashBlockErase4K(0);
	standardflashWaitOnReady();
	standardflashWriteEnable();
	standardflashBytePageProgram(0, dataWrite, 200);
	standardflashWaitOnReady();
	 // Part A: Read data with quad output read and test it.
	standardflashReadArrayHighFreq(0, dataRead, 200);
	if(!compareByteArrays(dataRead, dataWrite, 200))
	{
		printf("Write or Quad Output Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and Quad Output Read success.\n");
	}

	// Part B: Read and terminate with quad IO read command.
	standardflashQuadIORead(0, dataRead, 10, 0, modeByteValue);
	standardflashQuadIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	standardflashQuadIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	standardflashQuadIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	// Device won't be in QPI mode after this command.
	standardflashQuadIORead(40, &(dataRead[40]), 10, 3, modeByteValue);
	// For the purpose of this test, return the flash device to QPI mode.
	// If one wishes to continue in SPI, call standardflashDisableQPI() to return
	// the MCU_SPI_MODE variable (part of the driver) to 0.
	standardflashEnableQPI();
	if(!compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Write or Quad IO Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and Quad IO Read success.\n");
	}

	standardflashWriteEnable();
	standardflashDisableQPI();
	standardflashWaitOnReady();
	standardflashClearQEBit();
	standardflashWaitOnReady();

#endif

#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)	|| \
	(ALL == 1)

	/********************************************************************
	 *		    			4. Dual-Input Program						*
	 ********************************************************************/

	printf("\nThis device has the ability to program using 2 IOs.\n");
	fillArrayPattern(dataWrite, 100, 0x26);
	// Write enable the device for an erase and erase.
	standardflashWriteEnable();
	standardflashBlockErase4K(0);
	standardflashWaitOnReady();
	// Write enable for a program and then program starting at address 0x0.
	standardflashWriteEnable();
	standardflashDualInputBytePageProgram(0, dataWrite, 100);
	standardflashWaitOnReady();
	// Read it back and confirm the program worked.
	standardflashReadArrayHighFreq(0, dataRead, 100);
	if(!compareByteArrays(dataRead, dataWrite, 100))
	{
		printf("Dual Write or HF Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Dual Write and HF Read success.\n");
	}

#endif

	// Test complete. Print messages and exit.
	printf("\n\n#############################################\n\n");
	printf("Testing complete.\n");
	printf("Total errors detected: %d\n", errorCount);
	printf("Terminating testbench.\n");
	printf("\n#############################################\n\n");

	return errorCount;
}

#endif
