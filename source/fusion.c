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

#endif
