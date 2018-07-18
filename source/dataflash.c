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
#endif

#endif
