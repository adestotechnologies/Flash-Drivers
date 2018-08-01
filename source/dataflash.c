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
 * @file    dataflash.c
 * @brief   Definitions of Dataflash functions.
 */

#include "dataflash.h"

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

static bool DISPLAY_OUTPUT = 0;

static void debugOn() {DISPLAY_OUTPUT = 1;};
static void debugOff() {DISPLAY_OUTPUT = 0;};

uint8_t txDataflashInternalBuffer[MAXIMUM_TX_BYTES];

void dataflashWaitOnReady()
{
	uint8_t SR[2];
	do
	{
		dataflashReadSR(SR);
		SPI_Delay(10);
	}
	while(!(SR[0] & (1<<7)));
}

void dataflashReadMID(uint8_t *rxBuffer)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_READ_MID;
	SPI_Exchange(txDataflashInternalBuffer, 1, rxBuffer, 5, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, rxBuffer, 5);
	}
}

void dataflashReadSR(uint8_t *rxBuffer)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_READ_SR;
	SPI_Exchange(txDataflashInternalBuffer, 1, rxBuffer, 2, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, rxBuffer, 2);
	}
}

void dataflashMemoryPageRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_PAGE_READ, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 4);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashArrayReadLowPower(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_ARRAY_READ_LP, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashArrayReadLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_ARRAY_READ_LF, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashArrayReadHighFreq0(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_ARRAY_READ_HF0, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashArrayReadHighFreq1(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_ARRAY_READ_HF1, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 2);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashArrayReadLegacy(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_ARRAY_READ_LEG, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 4);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashBuffer1ReadLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUFFER1_READ_LF, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashBuffer1ReadHighFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUFFER1_READ_HF, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashBuffer1Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUFFER1_WRITE, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashBuffer1ToMainMemoryWithErase(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUF1_2MEM_W_ERASE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashBuffer1ToMainMemoryWithoutErase(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUF1_2MEM_WO_ERASE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashMemoryProgramThruBuffer1WithErase(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_PRGM_BUF1_W_ERASE, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashMemoryProgramThruBuffer1WithoutErase(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_PRGM_BUF1_WO_ERASE, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashPageErase(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_PAGE_ERASE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashBlockErase(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BLOCK_ERASE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashSectorErase(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_SECTOR_ERASE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashChipErase()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_CHIP_ERASE >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_CHIP_ERASE >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_CHIP_ERASE >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_CHIP_ERASE);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashRMWThruBuffer1(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_RD_MOD_WR_THRU_BUF1, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashEnableSectorProtection()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_ENABLE_SECT_PROTECTION >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_ENABLE_SECT_PROTECTION >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_ENABLE_SECT_PROTECTION >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_ENABLE_SECT_PROTECTION);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashDisableSectorProtection()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_DISABLE_SECT_PROTECTION >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_DISABLE_SECT_PROTECTION >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_DISABLE_SECT_PROTECTION >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_DISABLE_SECT_PROTECTION);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashEraseSectorProtectionReg()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_ERASE_SECT_PROT_REG >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_ERASE_SECT_PROT_REG >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_ERASE_SECT_PROT_REG >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_ERASE_SECT_PROT_REG);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashProgSectorProtectionReg(uint8_t *txBuffer, uint32_t txNumBytes)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_PROGRAM_SECT_PROT_REG >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_PROGRAM_SECT_PROT_REG >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_PROGRAM_SECT_PROT_REG >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_PROGRAM_SECT_PROT_REG);
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashReadSectorProtectionReg(uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_READ_SECT_PROT_REG;
	SPI_Exchange(txDataflashInternalBuffer, 1, rxBuffer, rxNumBytes, 3);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, rxBuffer, rxNumBytes);
	}
}

void dataflashReadSecurityReg(uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_READ_SECURITY_REG;
	SPI_Exchange(txDataflashInternalBuffer, 1, rxBuffer, rxNumBytes, 3);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, rxBuffer, rxNumBytes);
	}
}

void dataflashMemtoBuffer1Transfer(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_BUF1_TRANSFER, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashMemtoBuffer1Compare(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_BUF1_COMPARE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashAutoPageRewrite1(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_AUTO_PAGE_REWRITE1, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashDPD()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_DEEP_POWER_DOWN;
	SPI_Exchange(txDataflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, NULL, 0);
	}
}

void dataflashUDPDMode()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_UDPD_MODE;
	SPI_Exchange(txDataflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, NULL, 0);
	}
}

void dataflashHardwareReset()
{
	SPI_JEDECReset();
}

void dataflashResumeFromDPD()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_RESUME_FROM_DPD;
	SPI_Exchange(txDataflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, NULL, 0);
	}
}

void dataflashConfigurePower2PageSize()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_CONFIGURE_P2_PG_SIZE >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_CONFIGURE_P2_PG_SIZE >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_CONFIGURE_P2_PG_SIZE >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_CONFIGURE_P2_PG_SIZE);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashConfigureStandardPageSize()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_CONFIGURE_STD_PG_SIZE >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_CONFIGURE_STD_PG_SIZE >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_CONFIGURE_STD_PG_SIZE >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_CONFIGURE_STD_PG_SIZE);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashSoftwareReset()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_SOFTWARE_RESET >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_SOFTWARE_RESET >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_SOFTWARE_RESET >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_SOFTWARE_RESET);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashBuffer1ReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUF1_READ_LEG, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashMemPageReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_PAGE_READ_LEG, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashContinuousArrayReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_CONTINUOUS_ARRAY_READ_LEG, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashReadSRLegacy(uint8_t *rxBuffer)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_SR_READ_LEG;
	SPI_Exchange(txDataflashInternalBuffer, 1, rxBuffer, 2, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, rxBuffer, 2);
	}
}
#if (PARTNO == AT45DQ161) || \
	(PARTNO == AT45DQ321) || \
	(ALL == 1)
void dataflashDualOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_DUAL_OUTPUT_READ_ARRAY, address);
	SPI_DualExchange(4, txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashQuadOutputRead(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_QUAD_OUTPUT_READ_ARRAY, address);
	SPI_QuadExchange(4, txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashDualInputBuffer1Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_DUAL_INPUT_BUFFER1_WRITE, address);
	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_DualExchange(4, txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, txNumBytes);
	}
}

void dataflashDualInputBuffer2Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_DUAL_INPUT_BUFFER2_WRITE, address);
	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_DualExchange(4, txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, txNumBytes);
	}
}

void dataflashQuadInputBuffer1Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_QUAD_INPUT_BUFFER1_WRITE, address);
	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_QuadExchange(4, txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, txNumBytes);
	}
}

void dataflashQuadInputBuffer2Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_QUAD_INPUT_BUFFER2_WRITE, address);
	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}

	SPI_QuadExchange(4, txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, txNumBytes);
	}
}

void dataflashReadConfigRegister(uint8_t *rxBuffer)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_READ_CONFIG_REGISTER;
	SPI_Exchange(txDataflashInternalBuffer, 1, rxBuffer, 1, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, rxBuffer, 1);
	}
}

void dataflashQuadEnable()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_QUAD_ENABLE >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_QUAD_ENABLE >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_QUAD_ENABLE >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_QUAD_ENABLE);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashQuadDisable()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_QUAD_DISABLE >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_QUAD_DISABLE >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_QUAD_DISABLE >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_QUAD_DISABLE);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

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
void dataflashProgramEraseSuspend()
{
	txDataflashInternalBuffer[3] = CMD_DATAFLASH_PROGRAM_ERASE_SUSPEND;
	SPI_Exchange(txDataflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, NULL, 0);
	}
}

void dataflashProgramEraseResume()
{
	txDataflashInternalBuffer[3] = CMD_DATAFLASH_PROGRAM_ERASE_RESUME;
	SPI_Exchange(txDataflashInternalBuffer, 1, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, NULL, 0);
	}
}

void dataflashSectorLockdown(uint32_t address)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_SECTOR_LOCKDOWN >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_SECTOR_LOCKDOWN >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_SECTOR_LOCKDOWN >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_SECTOR_LOCKDOWN);
	txDataflashInternalBuffer[4] = address >> 16;
	txDataflashInternalBuffer[5] = address >> 8;
	txDataflashInternalBuffer[6] = address;
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashReadSectorLockdownReg(uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_READ_SECT_LOCK_REG;
	SPI_Exchange(txDataflashInternalBuffer, 1, rxBuffer, rxNumBytes, 3);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 1, rxBuffer, rxNumBytes);
	}
}

void dataflashFreezeSectorLockdown()
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_FREEZE_SECTOR_LOCKDOWN >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_FREEZE_SECTOR_LOCKDOWN >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_FREEZE_SECTOR_LOCKDOWN >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_FREEZE_SECTOR_LOCKDOWN);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashProgSecurityReg(uint8_t *txBuffer, uint32_t txNumBytes)
{
	txDataflashInternalBuffer[0] = CMD_DATAFLASH_PROGRAM_SECURITY_REG >> 24;
	txDataflashInternalBuffer[1] = (uint8_t) (CMD_DATAFLASH_PROGRAM_SECURITY_REG >> 16);
	txDataflashInternalBuffer[2] = (uint8_t) (CMD_DATAFLASH_PROGRAM_SECURITY_REG >> 8);
	txDataflashInternalBuffer[3] = (uint8_t) (CMD_DATAFLASH_PROGRAM_SECURITY_REG);
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}
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
void dataflashBuffer2ReadLowFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUFFER2_READ_LF, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashBuffer2ReadHighFreq(uint32_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUFFER2_READ_HF, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 1);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}

void dataflashBuffer2Write(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUFFER2_WRITE, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashBuffer2ToMainMemoryWithErase(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUF2_2MEM_W_ERASE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashBuffer2ToMainMemoryWithoutErase(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUF2_2MEM_WO_ERASE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashMemoryProgramThruBuffer2WithErase(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_PRGM_BUF2_W_ERASE, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashRMWThruBuffer2(uint32_t address, uint8_t *txBuffer, uint32_t txNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_RD_MOD_WR_THRU_BUF2, address);
	// Offset the data bytes by 4; opcode+address takes up the first 4 bytes of a transmission.
	uint32_t totalBytes = txNumBytes + 4;

	for(uint32_t j = 0; j < txNumBytes; j++)
	{
		txDataflashInternalBuffer[j+4] = txBuffer[j];
	}
	SPI_Exchange(txDataflashInternalBuffer, totalBytes, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, totalBytes, NULL, 0);
	}
}

void dataflashMemtoBuffer2Transfer(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_BUF2_TRANSFER, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashMemtoBuffer2Compare(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_MEM_BUF2_COMPARE, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashAutoPageRewrite2(uint32_t address)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_AUTO_PAGE_REWRITE2, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, NULL, 0, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, NULL, 0);
	}
}

void dataflashBuffer2ReadLegacy(uint8_t address, uint8_t *rxBuffer, uint32_t rxNumBytes)
{
	load4BytesToTxBuffer(txDataflashInternalBuffer, CMD_DATAFLASH_BUF2_READ_LEG, address);
	SPI_Exchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes, 0);
	if(DISPLAY_OUTPUT)
	{
		printSPIExchange(txDataflashInternalBuffer, 4, rxBuffer, rxNumBytes);
	}
}
#endif

#endif
