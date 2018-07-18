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
 * @ingroup ADESTO_LAYER PHOENIX
 */
/**
 * @file    phoenix.c
 * @brief   Definition of Phoenix functions.
 */
	
#include "phoenix.h"

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
 * See phoenixEnableQPI(), and phoenixDisableQPI().<br>
 * - 0: SPI
 * - 1: QUAD
 */
static uint8_t MCU_SPI_MODE = 0;

static bool DISPLAY_OUTPUT = 0;

static void debugOn() {DISPLAY_OUTPUT = 1;};
static void debugOff() {DISPLAY_OUTPUT = 0;};

uint8_t txPhoenixInternalBuffer[MAXIMUM_TX_BYTES];

void phoenixWaitOnReady()
{
	uint8_t SRArray[2] = {0, 0};
	do
	{
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
		phoenixReadSR(SRArray);
		SPI_Delay(10);
	}
	while(SRArray[1] & (1<<0));
#else
		SRArray[0] = phoenixReadSRB1();
		SPI_Delay(10);
	}
	while(SRArray[0] & (1<<0));
#endif
}

void phoenixSetQEBit()
{
	uint8_t SRArray[2] = {0, 0};
	// Read both status register bytes.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	phoenixReadSR(SRArray);
#else
	SRArray[0] = phoenixReadSRB1();
	SRArray[1] = phoenixReadSRB2();
#endif
	// Store a 1 in the QE bit.
	SRArray[1] |= (1 << 1);
	// Altering the device data, so send over a write enable first.
	phoenixWriteEnable();
	// Store the command and new SRB values. NOTE this needs to be done after
	// the write enable since the internal buffer used is a shared resource.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	phoenixWriteSRB1(SRArray[0]);
	phoenixWriteSRB2(SRArray[1]);
#else
	phoenixWriteSR(SRArray, 2);
#endif
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 3, NULL, 0);
	}
}

void phoenixClearQEBit()
{
	uint8_t SRArray[2] = {0, 0};
	// Read both status register bytes.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	phoenixReadSR(SRArray);
#else
	SRArray[0] = phoenixReadSRB1();
	SRArray[1] = phoenixReadSRB2();
#endif
	// Store a 0 in the QE bit.
	SRArray[1] &= ~(1 << 1);
	// Altering the device data, so send over a write enable first.
	phoenixWriteEnable();
	// Store the command and new SRB values. NOTE this needs to be done after
	// the write enable since the internal buffer used is a shared resource.
#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)
	phoenixWriteSRB1(SRArray[0]);
	phoenixWriteSRB2(SRArray[1]);
#else
	phoenixWriteSR(SRArray, 2);
#endif
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 3, NULL, 0);
	}
}

void phoenixWriteEnable()
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_WRITE_ENABLE;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixWriteDisable()
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_WRITE_DISABLE;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL,  0);
	}
}

void phoenixReadArrayLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_READ_ARRAY_LF, address);
	SPI_Exchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void phoenixReadArrayHighFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_READ_ARRAY_HF, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 2);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void phoenixBytePageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_BYTE_PAGE_PROGRAM, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txPhoenixInternalBuffer[j+4] = txBuffer[j];
	}

	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, totalBytes, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, totalBytes, NULL, 0);
	}
}

void phoenixBlockErase4K(uint32_t address)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_BLOCK_ERASE_4K, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 4, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, NULL, 0);
	}
}

void phoenixBlockErase32K(uint32_t address)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_BLOCK_ERASE_32K, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 4, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, NULL, 0);
	}
}

void phoenixBlockErase64K(uint32_t address)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_BLOCK_ERASE_64K, address);
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 4, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, NULL, 0);
	}
}

void phoenixChipErase1()
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_CHIP_ERASE1;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixChipErase2()
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_CHIP_ERASE2;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixDPD()
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_DEEP_POWER_DOWN;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixResumeFromDPD()
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_RESUME_FROM_DPD;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixReadID(uint8_t *rxBuffer)
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_READ_ID;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, rxBuffer, 2, 3);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, rxBuffer, 2, 3);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, rxBuffer, 2);
	}
}

void phoenixReadMID(uint8_t *rxBuffer)
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_READ_MID;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, rxBuffer, 3, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, rxBuffer, 3, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, rxBuffer, 3);
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

void phoenixWriteSR(uint8_t *txBuffer, uint8_t txNumBytes)
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_WRITE_SR;
	txPhoenixInternalBuffer[1] = txBuffer[0];
	if(txNumBytes > 1)
	{
		txPhoenixInternalBuffer[2] = txBuffer[1];
	}
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, txNumBytes+1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, txNumBytes+1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, txNumBytes+1, NULL, 0);
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

void phoenixWriteSRB1(uint8_t regVal)
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_WRITE_SRB1;
	txPhoenixInternalBuffer[1] = regVal;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 2, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 2, NULL, 0);
	}
}

void phoenixWriteSRB2(uint8_t regVal)
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_WRITE_SRB2;
	txPhoenixInternalBuffer[1] = regVal;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 2, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 2, NULL, 0);
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
void phoenixWriteEnableVolatileSR()
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_WE_FOR_VOLATILE_SR;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

uint8_t phoenixReadSRB1()
{
	uint8_t regVal = 0;
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_READ_SRB1;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, &regVal, 1, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, &regVal, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, &regVal, 1);
	}
	return regVal;
}

uint8_t phoenixReadSRB2()
{
	uint8_t regVal = 0;
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_READ_SRB2;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, &regVal, 1, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, &regVal, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, &regVal, 1);
	}
	return regVal;
}

void phoenixDualOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_DUAL_OUTPUT_READ, address);
	SPI_DualExchange(4, txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void phoenixDualIORead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes, uint8_t readMode, uint8_t modeByteValue)
{
	// Set to an unused number to ensure a transmission does not take place with invalid input.
	uint8_t transmissionSelect = 0x02;
	switch(readMode)
	{
		// Single Dual Read
		case 0:
			load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_DUAL_IO_READ, address);
			txPhoenixInternalBuffer[4] = 0x00;
			transmissionSelect = 0x00;
			break;
		// Start Continuous Dual Read
		case 1:
			load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_DUAL_IO_READ, address);
			txPhoenixInternalBuffer[4] = modeByteValue;
			transmissionSelect = 0x00;
			break;
		// Continue Continuous Dual Read
		case 2:
			txPhoenixInternalBuffer[0] = (uint8_t) (address >> 16);
			txPhoenixInternalBuffer[1] = (uint8_t) (address >> 8);
			txPhoenixInternalBuffer[2] = (uint8_t) address;
			txPhoenixInternalBuffer[3] = modeByteValue;
			transmissionSelect = 0x01;
			break;
		// End Continuous Dual Read
		case 3:
			txPhoenixInternalBuffer[0] = (uint8_t) (address >> 16);
			txPhoenixInternalBuffer[1] = (uint8_t) (address >> 8);
			txPhoenixInternalBuffer[2] = (uint8_t) address;
			txPhoenixInternalBuffer[3] = 0x00;
			transmissionSelect = 0x01;
			break;
		default:
			printf("Error with phoenixDualIORead.\n");
			printf("\t- Invalid dualReadMode value. Expected 0-3, received %d.\n", readMode);
			printf("\tSee documentation for operating modes.\n");
			break;
	}

	if(transmissionSelect == 0x00)
	{
		SPI_DualExchange(1, txPhoenixInternalBuffer, 5, rxBuffer, rxNumBytes, 0);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txPhoenixInternalBuffer, 5, rxBuffer, rxNumBytes);
		}
	}
	else if(transmissionSelect == 0x01)
	{
		SPI_DualExchange(0, txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes);
		}
	}
}

void phoenixQuadOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_QUAD_OUTPUT_READ, address);
	SPI_QuadExchange(4, txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void phoenixQuadIORead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes, uint8_t readMode, uint8_t modeByteValue)
{
	// Set to an unused number to ensure a transmission does not take place with invalid input.
	uint8_t transmissionSelect = 0x02;
	switch(readMode)
	{
		// Single Quad Read
		case 0:
			load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_QUAD_IO_READ, address);
			txPhoenixInternalBuffer[4] = 0x00;
			transmissionSelect = 0x00;
			break;
		// Start Continuous Quad Read
		case 1:
			load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_QUAD_IO_READ, address);
			txPhoenixInternalBuffer[4] = modeByteValue;
			transmissionSelect = 0x00;
			break;
		// Continue Continuous Quad Read
		case 2:
			txPhoenixInternalBuffer[0] = (uint8_t) (address >> 16);
			txPhoenixInternalBuffer[1] = (uint8_t) (address >> 8);
			txPhoenixInternalBuffer[2] = (uint8_t) address;
			txPhoenixInternalBuffer[3] = modeByteValue;
			transmissionSelect = 0x01;
			break;
		// End Continuous Quad Read
		case 3:
			txPhoenixInternalBuffer[0] = (uint8_t) (address >> 16);
			txPhoenixInternalBuffer[1] = (uint8_t) (address >> 8);
			txPhoenixInternalBuffer[2] = (uint8_t) address;
			txPhoenixInternalBuffer[3] = modeByteValue;
			transmissionSelect = 0x01;
			break;
		default:
			printf("Error with phoenixQuadIORead.\n");
			printf("\t- Invalid quadReadMode value. Expected 0-3, received %d.\n", readMode);
			printf("\tSee documentation for operating modes.\n");
			break;
	}

	if(transmissionSelect == 0x00)
	{
		if(MCU_SPI_MODE == SPI)
			SPI_QuadExchange(1, txPhoenixInternalBuffer, 5, rxBuffer, rxNumBytes, 2);
		else
			SPI_QuadExchange(0, txPhoenixInternalBuffer, 5, rxBuffer, rxNumBytes, 1);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txPhoenixInternalBuffer, 5, rxBuffer, rxNumBytes);
		}
	}
	else if(transmissionSelect == 0x01)
	{
		if(MCU_SPI_MODE == SPI)
			SPI_QuadExchange(0, txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 2);
		else
			SPI_QuadExchange(0, txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
		if(DISPLAY_OUTPUT)
		{
			printSPIExchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes);
		}
	}
}

void phoenixContinuousReadModeDualReset()
{
	txPhoenixInternalBuffer[0] = (uint8_t) (CMD_PHOENIX_CONT_READ_MODE_RST_DUAL >> 8);
	txPhoenixInternalBuffer[1] = (uint8_t) CMD_PHOENIX_CONT_READ_MODE_RST_DUAL;
	SPI_Exchange(txPhoenixInternalBuffer, 2, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 2, NULL, 0);
	}
}

void phoenixContinuousReadModeQuadReset()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_CONT_READ_MODE_RST_QUAD;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixEraseSecurityRegister(uint32_t address)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_ERASE_SECURTIY_REG_PAGE, address);
	SPI_Exchange(txPhoenixInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, NULL, 0);
	}
}

void phoenixProgramSecurityRegisters(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_PROGRAM_SECURITY_REG_PAGE, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txPhoenixInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_Exchange(txPhoenixInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, totalBytes, NULL, 0);
	}
}

void phoenixReadSecurityRegisters(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_READ_SECURITY_REG_PAGE, address);
	SPI_Exchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void phoenixResumeFromDPDReadID(uint8_t *rxBuffer)
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_RESUME_FROM_DPD;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, rxBuffer, 1, 3);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, rxBuffer, 1, 3);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, rxBuffer, 1);
	}
}

void phoenixQuadPageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes, uint8_t mode)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_QUAD_PAGE_PROGRAM, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txPhoenixInternalBuffer[j+4] = txBuffer[j];
	}

	if(MCU_SPI_MODE == SPI)
		SPI_QuadExchange(1, txPhoenixInternalBuffer, totalBytes, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, totalBytes, NULL, 0);
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

void phoenixEraseProgramSuspend()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_ERASE_PROGRAM_SUSPEND;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixEraseProgramResume()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_ERASE_PROGRAM_RESUME;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
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

void phoenixEnableQPI()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_ENABLE_QPI;
	SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
	// Set the MCU_SPI_MODE byte to QPI.
	MCU_SPI_MODE = QPI;
}

void phoenixDisableQPI()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_DISABLE_QPI;
	SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
	// Set the MCU_SPI_MODE byte to SPI.
	MCU_SPI_MODE = SPI;
}

void phoenixEnableReset()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_ENABLE_RESET;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixReset()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_RESET;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixEnterSecureOTP()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_ENTER_SECURED_OTP;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixExitSecuredOTP()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_EXIT_SECURED_OTP;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

#endif


#if (PARTNO == AT25DL081) 	|| \
	(PARTNO == AT25DL161) 	|| \
	(PARTNO == AT25DF081A)  || \
	(PARTNO == AT25DF321A)  || \
	(PARTNO == AT25DF641A)	|| \
	(ALL == 1)

void phoenixReadSR(uint8_t *rxBuffer)
{
	txPhoenixInternalBuffer[0] = CMD_PHOENIX_READ_SR;
	if(MCU_SPI_MODE == SPI)
		SPI_Exchange(txPhoenixInternalBuffer, 1, rxBuffer, 2, 0);
	else
		SPI_QuadExchange(0, txPhoenixInternalBuffer, 1, rxBuffer, 2, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, rxBuffer, 2);
	}
}

void phoenixDualInputBytePageProgram(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_DUAL_BYTE_PAGE_PROGRAM, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txPhoenixInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_DualExchange(4, txPhoenixInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, totalBytes, NULL, 0);
	}
}

void phoenixProgramEraseSuspend()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_PROGRAM_ERASE_SUSPEND;
	SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixProgramEraseResume()
{
	txPhoenixInternalBuffer[0] = (uint8_t) CMD_PHOENIX_PROGRAM_ERASE_RESUME;
	SPI_Exchange(txPhoenixInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 1, NULL, 0);
	}
}

void phoenixProtectSector(uint32_t address)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_PROTECT_SECTOR, address);
	SPI_Exchange(txPhoenixInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, NULL, 0);
	}
}
void phoenixUnprotectSector(uint32_t address)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_UNPROTECT_SECTOR, address);
	SPI_Exchange(txPhoenixInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, NULL, 0);
	}
}

uint8_t phoenixReadSectorProtectionReg(uint32_t address)
{
	uint8_t registerVal = 0;
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_READ_SECT_PROT_REG, address);
	SPI_Exchange(txPhoenixInternalBuffer, 4, &registerVal, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, &registerVal, 1);
	}
	return registerVal;
}

void phoenixFreezeLockdownState()
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_FREEZE_LOCKDOWN_STATE, (uint32_t) 0x0055AA40);
	txPhoenixInternalBuffer[4] = 0xD0;
	SPI_Exchange(txPhoenixInternalBuffer, 5, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 5, NULL, 0);
	}
}

uint8_t phoenixReadLockdownReg(uint32_t address)
{
	uint8_t registerVal = 0;
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_READ_LOCKDOWN_REG, address);
	SPI_Exchange(txPhoenixInternalBuffer, 4, &registerVal, 1, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, 4, &registerVal, 1);
	}
	return registerVal;
}

void phoenixProgramOTPReg(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txPhoenixInternalBuffer, CMD_PHOENIX_PROGRAM_OTP_REG, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txPhoenixInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_Exchange(txPhoenixInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txPhoenixInternalBuffer, totalBytes, NULL, 0);
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

uint32_t phoenixTest()
{
#if PARTNO == AT25SF641
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x32, 0x17};
#elif PARTNO == AT25SF321
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x87, 0x01};
#elif PARTNO == AT25SF161
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x86, 0x01};
#elif PARTNO == AT25SF081
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x85, 0x01};
#elif PARTNO == AT25SF041
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0x20;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x84, 0x01};
#elif PARTNO == AT25SL321
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x42, 0x16};
#elif PARTNO == AT25SL641
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x43, 0x17};
#elif PARTNO == AT25SL128A
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
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
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x42, 0x16};
#elif PARTNO == AT25QL641
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x43, 0x17};
#elif PARTNO == AT25QL128A
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
	// The hex value is determined by the datasheet. Certain bits in the byte need to be set/cleared.
	uint8_t modeByteValue = 0xA0;
	// Store the manufacturer ID for later comparison
	uint8_t MID[3] = {0x1F, 0x42, 0x18};
#elif PARTNO == AT25QF641
	// Sets the modeByteValue used for instruction phoenixQuadIORead(). Set once then forget.
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
	phoenixReadMID(dataRead);
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
	phoenixWriteEnable();
	phoenixUnprotectSector(0);
#endif
	// Write enable the device then erase the first 4K block.
	phoenixWriteEnable();
	phoenixBlockErase4K(0);
	phoenixWaitOnReady();
	// Program the device from address 0-100.
	phoenixWriteEnable();
	phoenixBytePageProgram(0, dataWrite, 100);
	phoenixWaitOnReady();
	// Now read back the data in order to confirm that erase/program/read all work.
	phoenixReadArrayLowFreq(0, dataRead, 100);
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
	phoenixWriteEnable();
	phoenixBytePageProgram(100, dataWrite, 100);
	phoenixWaitOnReady();
	// Read back and test the programmed data to confirm proper functionality.
	phoenixReadArrayHighFreq(100, dataRead, 100);
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
	phoenixWriteEnable();
	phoenixBlockErase4K(0);
	phoenixWaitOnReady();
	phoenixWriteEnable();
	phoenixBytePageProgram(0, dataWrite, 200);
	phoenixWaitOnReady();

	 // Part A: Read data with dual output read and test it.
	phoenixDualOutputRead(0, dataRead, 200);
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
	phoenixDualIORead(0, dataRead, 10, 0, modeByteValue);
	phoenixDualIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	phoenixDualIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	phoenixDualIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	phoenixDualIORead(40, &(dataRead[40]), 10, 3, modeByteValue);
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
	phoenixDualIORead(0, dataRead, 10, 0, modeByteValue);
	phoenixDualIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	phoenixDualIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	phoenixDualIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	phoenixDualIORead(40, &(dataRead[40]), 10, 2, modeByteValue);
	phoenixContinuousReadModeDualReset();

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
	 * commands. See phoenixSetQEBit().
	 */
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x25);
	// Write the new data to the device.
	phoenixWriteEnable();
	phoenixBlockErase4K(0);
	phoenixWaitOnReady();
	phoenixWriteEnable();
	phoenixBytePageProgram(0, dataWrite, 200);
	phoenixWaitOnReady();
	phoenixSetQEBit();
	phoenixWaitOnReady();
	 // Part A: Read data with quad output read and test it.
	phoenixQuadOutputRead(0, dataRead, 200);
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
	phoenixQuadIORead(0, dataRead, 10, 0, modeByteValue);
	phoenixQuadIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	phoenixQuadIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	phoenixQuadIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	phoenixQuadIORead(40, &(dataRead[40]), 10, 3, modeByteValue);
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
	phoenixQuadIORead(0, dataRead, 10, 0, modeByteValue);
	phoenixQuadIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	phoenixQuadIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	phoenixQuadIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	phoenixQuadIORead(40, &(dataRead[40]), 10, 2, modeByteValue);
	phoenixContinuousReadModeQuadReset();

	phoenixClearQEBit();

	phoenixWaitOnReady();

	// Ensure that the device is no longer in quad mode.
	phoenixReadArrayHighFreq(0, dataRead, 100);
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
	phoenixWriteEnable();
	phoenixBlockErase4K(0);
	phoenixWaitOnReady();
	phoenixWriteEnable();
	phoenixBytePageProgram(0, dataWrite, 100);
	phoenixWaitOnReady();

	// Send the device to deep power down mode.
	phoenixDPD();
	// Attempt to read from the device. Note, this should not work in DPD Mode.
	phoenixReadArrayLowFreq(0, dataRead, 100);
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
	phoenixResumeFromDPD();
	phoenixWaitOnReady();
	// ... and try to read from the device. It should function in its normal state.
	phoenixReadArrayHighFreq(0, dataRead, 100);
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

	phoenixWriteEnable();
	phoenixSetQEBit();
	phoenixWaitOnReady();
	phoenixWriteEnable();
	phoenixEnableQPI();
	// Part A: Test that the MID can be read.
	// Read the manufacturing ID and store the returned data in dataRead.
	phoenixReadMID(dataRead);
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

	phoenixWriteEnable();
	phoenixDisableQPI();
	phoenixWaitOnReady();
	phoenixClearQEBit();
	phoenixWaitOnReady();

	phoenixReadMID(dataRead);
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
	phoenixWriteEnable();
	phoenixSetQEBit();
	phoenixWaitOnReady();
	phoenixWriteEnable();
	phoenixEnableQPI();

	printf("\n\nTesting Write and Read Quad IO Commands --------------\n\n");

	// Part A: Test low frequency read.
	// Get data to be programmed.
	fillArrayPattern(dataWrite, 100, 0x30);
	// Write enable the device then erase the first 4K block.
	phoenixWriteEnable();
	phoenixBlockErase4K(0);
	phoenixWaitOnReady();
	// Program the device from address 0-100.
	phoenixWriteEnable();
	phoenixBytePageProgram(0, dataWrite, 100);
	phoenixWaitOnReady();
	// Now read back the data in order to confirm that erase/program/read all work.
	phoenixReadArrayHighFreq(0, dataRead, 100);
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
	phoenixWriteEnable();
	phoenixBytePageProgram(100, dataWrite, 100);
	phoenixWaitOnReady();
	// Read back and test the programmed data to confirm proper functionality.
	phoenixReadArrayHighFreq(100, dataRead, 100);
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
	phoenixWriteEnable();
	phoenixBlockErase4K(0);
	phoenixWaitOnReady();
	phoenixWriteEnable();
	phoenixBytePageProgram(0, dataWrite, 200);
	phoenixWaitOnReady();
	 // Part A: Read data with quad output read and test it.
	phoenixReadArrayHighFreq(0, dataRead, 200);
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
	phoenixQuadIORead(0, dataRead, 10, 0, modeByteValue);
	phoenixQuadIORead(10, &(dataRead[10]), 10, 1, modeByteValue);
	phoenixQuadIORead(20, &(dataRead[20]), 10, 2, modeByteValue);
	phoenixQuadIORead(30, &(dataRead[30]), 10, 2, modeByteValue);
	// Device won't be in QPI mode after this command.
	phoenixQuadIORead(40, &(dataRead[40]), 10, 3, modeByteValue);
	// For the purpose of this test, return the flash device to QPI mode.
	// If one wishes to continue in SPI, call phoenixDisableQPI() to return
	// the MCU_SPI_MODE variable (part of the driver) to 0.
	phoenixEnableQPI();
	if(!compareByteArrays(dataRead, dataWrite, 50))
	{
		printf("Write or Quad IO Read failure.\n");
		errorCount++;
	}
	else
	{
		printf("Write and Quad IO Read success.\n");
	}

	phoenixWriteEnable();
	phoenixDisableQPI();
	phoenixWaitOnReady();
	phoenixClearQEBit();
	phoenixWaitOnReady();

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
	phoenixWriteEnable();
	phoenixBlockErase4K(0);
	phoenixWaitOnReady();
	// Write enable for a program and then program starting at address 0x0.
	phoenixWriteEnable();
	phoenixDualInputBytePageProgram(0, dataWrite, 100);
	phoenixWaitOnReady();
	// Read it back and confirm the program worked.
	phoenixReadArrayHighFreq(0, dataRead, 100);
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
