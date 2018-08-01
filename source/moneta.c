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

#endif
