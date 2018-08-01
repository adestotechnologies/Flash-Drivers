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
 * @ingroup ADESTO_LAYER STANDARDFLASH
 */
/**
 * @file    standardflash.c
 * @brief   Definition of Standardflash functions.
 */
	
#include <standardflash.h>

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
/*!
 * @brief Communication mode byte. This byte is changed when various commands are executed.
 * See standardflashEnableQPI(), and standardflashDisableQPI().<br>
 * - 0: SPI
 * - 1: QUAD
 */
static uint8_t MCU_SPI_MODE = 0;

static bool DISPLAY_OUTPUT = 0;

static void debugOn() {DISPLAY_OUTPUT = 1;};
static void debugOff() {DISPLAY_OUTPUT = 0;};

uint8_t txStandardflashInternalBuffer[MAXIMUM_TX_BYTES];

void standardflashWaitOnReady()
{
	uint8_t SRArray[2] = {0, 0};
	do
	{
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
		standardflashReadSR(SRArray);
		SPI_Delay(10);
	}
	while(SRArray[1] & (1<<0));
#else
		SRArray[0] = standardflashReadSRB1();
		SPI_Delay(10);
	}
	while(SRArray[0] & (1<<0));
#endif
}

void standardflashSetQEBit()
{
	uint8_t SRArray[2] = {0, 0};
	// Read both status register bytes.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	standardflashReadSR(SRArray);
#else
	SRArray[0] = standardflashReadSRB1();
	SRArray[1] = standardflashReadSRB2();
#endif
	// Store a 1 in the QE bit.
	SRArray[1] |= (1 << 1);
	// Altering the device data, so send over a write enable first.
	standardflashWriteEnable();
	// Store the command and new SRB values. NOTE this needs to be done after
	// the write enable since the internal buffer used is a shared resource.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	standardflashWriteSRB1(SRArray[0]);
	standardflashWriteSRB2(SRArray[1]);
#else
	standardflashWriteSR(SRArray, 2);
#endif
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 3, NULL, 0);
	}
}

void standardflashClearQEBit()
{
	uint8_t SRArray[2] = {0, 0};
	// Read both status register bytes.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	standardflashReadSR(SRArray);
#else
	SRArray[0] = standardflashReadSRB1();
	SRArray[1] = standardflashReadSRB2();
#endif
	// Store a 0 in the QE bit.
	SRArray[1] &= ~(1 << 1);
	// Altering the device data, so send over a write enable first.
	standardflashWriteEnable();
	// Store the command and new SRB values. NOTE this needs to be done after
	// the write enable since the internal buffer used is a shared resource.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	standardflashWriteSRB1(SRArray[0]);
	standardflashWriteSRB2(SRArray[1]);
#else
	standardflashWriteSR(SRArray, 2);
#endif
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 3, NULL, 0);
	}
}

void standardflashWriteEnable()
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_WRITE_ENABLE;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashWriteDisable()
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_WRITE_DISABLE;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL,  0);
	}
}

void standardflashReadArrayLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_READ_ARRAY_LF, address);
	SPI_Exchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void standardflashReadArrayHighFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_READ_ARRAY_HF, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 2);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void standardflashBytePageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_BYTE_PAGE_PROGRAM, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txStandardflashInternalBuffer[j+4] = txBuffer[j];
	}

	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, totalBytes, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void standardflashBlockErase4K(uint32_t address)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_BLOCK_ERASE_4K, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 4, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, NULL, 0);
	}
}

void standardflashBlockErase32K(uint32_t address)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_BLOCK_ERASE_32K, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 4, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, NULL, 0);
	}
}

void standardflashBlockErase64K(uint32_t address)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_BLOCK_ERASE_64K, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 4, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, NULL, 0);
	}
}

void standardflashChipErase1()
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_CHIP_ERASE1;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashChipErase2()
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_CHIP_ERASE2;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashDPD()
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_DEEP_POWER_DOWN;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashResumeFromDPD()
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_RESUME_FROM_DPD;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashReadID(uint8_t *rxBuffer)
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_READ_ID;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, rxBuffer, 2, 3);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, rxBuffer, 2, 3);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, rxBuffer, 2);
	}
}

void standardflashReadMID(uint8_t *rxBuffer)
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_READ_MID;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, rxBuffer, 3, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, rxBuffer, 3, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, rxBuffer, 3);
	}
}
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

void standardflashWriteSR(uint8_t *txBuffer, uint8_t txNumBytes)
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_WRITE_SR;
	txStandardflashInternalBuffer[1] = txBuffer[0];
	if(txNumBytes > 1)
	{
		txStandardflashInternalBuffer[2] = txBuffer[1];
	}
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, txNumBytes+1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, txNumBytes+1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, txNumBytes+1, NULL, 0);
	}
}
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

void standardflashWriteSRB1(uint8_t regVal)
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_WRITE_SRB1;
	txStandardflashInternalBuffer[1] = regVal;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 2, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 2, NULL, 0);
	}
}

void standardflashWriteSRB2(uint8_t regVal)
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_WRITE_SRB2;
	txStandardflashInternalBuffer[1] = regVal;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 2, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 2, NULL, 0);
	}
}
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
void standardflashWriteEnableVolatileSR()
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_WE_FOR_VOLATILE_SR;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

uint8_t standardflashReadSRB1()
{
	uint8_t regVal = 0;
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_READ_SRB1;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, &regVal, 1, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, &regVal, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, &regVal, 1);
	}
	return regVal;
}

uint8_t standardflashReadSRB2()
{
	uint8_t regVal = 0;
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_READ_SRB2;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, &regVal, 1, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, &regVal, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, &regVal, 1);
	}
	return regVal;
}

void standardflashDualOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_DUAL_OUTPUT_READ, address);
	SPI_DualExchange(4, txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void standardflashDualIORead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes, uint8_t readMode, uint8_t modeByteValue)
{
	// Set to an unused number to ensure a transmission does not take place with invalid input.
	uint8_t transmissionSelect = 0x02;
	switch(readMode)
	{
		// Single Dual Read
		case 0:
			load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_DUAL_IO_READ, address);
			txStandardflashInternalBuffer[4] = 0x00;
			transmissionSelect = 0x00;
			break;
		// Start Continuous Dual Read
		case 1:
			load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_DUAL_IO_READ, address);
			txStandardflashInternalBuffer[4] = modeByteValue;
			transmissionSelect = 0x00;
			break;
		// Continue Continuous Dual Read
		case 2:
			txStandardflashInternalBuffer[0] = (uint8_t) (address >> 16);
			txStandardflashInternalBuffer[1] = (uint8_t) (address >> 8);
			txStandardflashInternalBuffer[2] = (uint8_t) address;
			txStandardflashInternalBuffer[3] = modeByteValue;
			transmissionSelect = 0x01;
			break;
		// End Continuous Dual Read
		case 3:
			txStandardflashInternalBuffer[0] = (uint8_t) (address >> 16);
			txStandardflashInternalBuffer[1] = (uint8_t) (address >> 8);
			txStandardflashInternalBuffer[2] = (uint8_t) address;
			txStandardflashInternalBuffer[3] = 0x00;
			transmissionSelect = 0x01;
			break;
		default:
			printf("Error with standardflashDualIORead.\n");
			printf("\t- Invalid dualReadMode value. Expected 0-3, received %d.\n", readMode);
			printf("\tSee documentation for operating modes.\n");
			break;
	}

	if(transmissionSelect == 0x00)
	{
		SPI_DualExchange(1, txStandardflashInternalBuffer, 5, rxBuffer, rxNumBytes, 0);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txStandardflashInternalBuffer, 5, rxBuffer, rxNumBytes);
		}
	}
	else if(transmissionSelect == 0x01)
	{
		SPI_DualExchange(0, txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes);
		}
	}
}

void standardflashQuadOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_QUAD_OUTPUT_READ, address);
	SPI_QuadExchange(4, txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void standardflashQuadIORead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes, uint8_t readMode, uint8_t modeByteValue)
{
	// Set to an unused number to ensure a transmission does not take place with invalid input.
	uint8_t transmissionSelect = 0x02;
	switch(readMode)
	{
		// Single Quad Read
		case 0:
			load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_QUAD_IO_READ, address);
			txStandardflashInternalBuffer[4] = 0x00;
			transmissionSelect = 0x00;
			break;
		// Start Continuous Quad Read
		case 1:
			load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_QUAD_IO_READ, address);
			txStandardflashInternalBuffer[4] = modeByteValue;
			transmissionSelect = 0x00;
			break;
		// Continue Continuous Quad Read
		case 2:
			txStandardflashInternalBuffer[0] = (uint8_t) (address >> 16);
			txStandardflashInternalBuffer[1] = (uint8_t) (address >> 8);
			txStandardflashInternalBuffer[2] = (uint8_t) address;
			txStandardflashInternalBuffer[3] = modeByteValue;
			transmissionSelect = 0x01;
			break;
		// End Continuous Quad Read
		case 3:
			txStandardflashInternalBuffer[0] = (uint8_t) (address >> 16);
			txStandardflashInternalBuffer[1] = (uint8_t) (address >> 8);
			txStandardflashInternalBuffer[2] = (uint8_t) address;
			txStandardflashInternalBuffer[3] = modeByteValue;
			transmissionSelect = 0x01;
			break;
		default:
			printf("Error with standardflashQuadIORead.\n");
			printf("\t- Invalid quadReadMode value. Expected 0-3, received %d.\n", readMode);
			printf("\tSee documentation for operating modes.\n");
			break;
	}

	if(transmissionSelect == 0x00)
	{
		if(MCU_SPI_MODE == SPI)
			SPI_QuadExchange(1, txStandardflashInternalBuffer, 5, rxBuffer, rxNumBytes, 2);
		else
			SPI_QuadExchange(0, txStandardflashInternalBuffer, 5, rxBuffer, rxNumBytes, 1);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txStandardflashInternalBuffer, 5, rxBuffer, rxNumBytes);
		}
	}
	else if(transmissionSelect == 0x01)
	{
		if(MCU_SPI_MODE == SPI)
			SPI_QuadExchange(0, txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 2);
		else
			SPI_QuadExchange(0, txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes);
		}
	}
}

void standardflashContinuousReadModeDualReset()
{
	txStandardflashInternalBuffer[0] = (uint8_t) (CMD_STANDARDFLASH_CONT_READ_MODE_RST_DUAL >> 8);
	txStandardflashInternalBuffer[1] = (uint8_t) CMD_STANDARDFLASH_CONT_READ_MODE_RST_DUAL;
	SPI_Exchange(txStandardflashInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 2, NULL, 0);
	}
}

void standardflashContinuousReadModeQuadReset()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_CONT_READ_MODE_RST_QUAD;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashEraseSecurityRegister(uint32_t address)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_ERASE_SECURTIY_REG_PAGE, address);
	SPI_Exchange(txStandardflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, NULL, 0);
	}
}

void standardflashProgramSecurityRegisters(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_PROGRAM_SECURITY_REG_PAGE, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txStandardflashInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_Exchange(txStandardflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void standardflashReadSecurityRegisters(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_READ_SECURITY_REG_PAGE, address);
	SPI_Exchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void standardflashResumeFromDPDReadID(uint8_t *rxBuffer)
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_RESUME_FROM_DPD;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, rxBuffer, 1, 3);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, rxBuffer, 1, 3);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, rxBuffer, 1);
	}
}

void standardflashQuadPageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes, uint8_t mode)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_QUAD_PAGE_PROGRAM, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txStandardflashInternalBuffer[j+4] = txBuffer[j];
	}

	if(MCU_SPI_MODE == SPI)
		SPI_QuadExchange(1, txStandardflashInternalBuffer, totalBytes, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, totalBytes, NULL, 0);
	}
}
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

void standardflashEraseProgramSuspend()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_ERASE_PROGRAM_SUSPEND;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashEraseProgramResume()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_ERASE_PROGRAM_RESUME;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

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

void standardflashEnableQPI()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_ENABLE_QPI;
	SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
	// Set the MCU_SPI_MODE byte to QPI.
	MCU_SPI_MODE = QPI;
}

void standardflashDisableQPI()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_DISABLE_QPI;
	SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
	// Set the MCU_SPI_MODE byte to SPI.
	MCU_SPI_MODE = SPI;
}

void standardflashEnableReset()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_ENABLE_RESET;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashReset()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_RESET;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashEnterSecureOTP()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_ENTER_SECURED_OTP;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashExitSecuredOTP()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_EXIT_SECURED_OTP;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

#endif


#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)	|| \
	(ALL == 1)

void standardflashReadSR(uint8_t *rxBuffer)
{
	txStandardflashInternalBuffer[0] = CMD_STANDARDFLASH_READ_SR;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txStandardflashInternalBuffer, 1, rxBuffer, 2, 0);
	else
		SPI_QuadExchange(0, txStandardflashInternalBuffer, 1, rxBuffer, 2, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, rxBuffer, 2);
	}
}

void standardflashDualInputBytePageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_DUAL_BYTE_PAGE_PROGRAM, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txStandardflashInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_DualExchange(4, txStandardflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void standardflashProgramEraseSuspend()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_PROGRAM_ERASE_SUSPEND;
	SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashProgramEraseResume()
{
	txStandardflashInternalBuffer[0] = (uint8_t) CMD_STANDARDFLASH_PROGRAM_ERASE_RESUME;
	SPI_Exchange(txStandardflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 1, NULL, 0);
	}
}

void standardflashProtectSector(uint32_t address)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_PROTECT_SECTOR, address);
	SPI_Exchange(txStandardflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, NULL, 0);
	}
}
void standardflashUnprotectSector(uint32_t address)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_UNPROTECT_SECTOR, address);
	SPI_Exchange(txStandardflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, NULL, 0);
	}
}

uint8_t standardflashReadSectorProtectionReg(uint32_t address)
{
	uint8_t registerVal = 0;
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_READ_SECT_PROT_REG, address);
	SPI_Exchange(txStandardflashInternalBuffer, 4, &registerVal, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, &registerVal, 1);
	}
	return registerVal;
}

void standardflashFreezeLockdownState()
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_FREEZE_LOCKDOWN_STATE, (uint32_t) 0x0055AA40);
	txStandardflashInternalBuffer[4] = 0xD0;
	SPI_Exchange(txStandardflashInternalBuffer, 5, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 5, NULL, 0);
	}
}

uint8_t standardflashReadLockdownReg(uint32_t address)
{
	uint8_t registerVal = 0;
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_READ_LOCKDOWN_REG, address);
	SPI_Exchange(txStandardflashInternalBuffer, 4, &registerVal, 1, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, 4, &registerVal, 1);
	}
	return registerVal;
}

void standardflashProgramOTPReg(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txStandardflashInternalBuffer, CMD_STANDARDFLASH_PROGRAM_OTP_REG, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txStandardflashInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_Exchange(txStandardflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txStandardflashInternalBuffer, totalBytes, NULL, 0);
	}
}
#endif
