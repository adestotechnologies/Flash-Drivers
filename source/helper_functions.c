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
 * @ingroup ADESTO_LAYER
 */
/**
 * @file    helper_functions.c
 * @brief   Definitions of helper functions.
 */

#include "helper_functions.h"

void displayByteArray(uint8_t *byteArray, uint32_t numBytes)
{
	printSPIExchange(NULL, 0, byteArray, numBytes);
}

void fillArrayPattern(uint8_t * byteArray, uint32_t numBytes, int seedNumber)
{
	for(int i = 0; i < numBytes; i++)
	{
		byteArray[i] = (uint8_t)(seedNumber+i);
	}
}

void fillArrayConst(uint8_t * byteArray, uint32_t numBytes, int constantNum)
{
	for(int i = 0; i < numBytes; i++)
	{
		byteArray[i] = (uint8_t)(constantNum);
	}
}

bool compareByteArrays(uint8_t *arr1, uint8_t *arr2, uint32_t arrLength)
{
	uint32_t numErrors = 0;
	for(int i = 0; i < arrLength; i++)
	{
		if(arr1[i] != arr2[i])
		{
			printf("Mismatch @ index: %d - Array 1: 0x%02X | Array 2: 0x%02X\n", i, arr1[i], arr2[i]);
			numErrors++;
		}
	}
	printf("Byte comparison total mismatches: %d\n", numErrors);
	return (numErrors == 0) ? 1 : 0;
}

void load4BytesToTxBuffer(uint8_t *txBuffer, uint8_t opcode, uint32_t address)
{
	txBuffer[0] = opcode;
	txBuffer[1] = (uint8_t) (address >> 16);
	txBuffer[2] = (uint8_t) (address >> 8);
	txBuffer[3] = (uint8_t) address;
}

// TODO: Make this faster.
void printSPIExchange(uint8_t *txBuffer,
							 uint32_t txNumBytes,
							 uint8_t *rxBuffer,
							 uint32_t rxNumBytes)
{
	if(txNumBytes != 0)
	{
		printf("\nSent bytes (0x):");
		for(uint32_t i = 0; i < txNumBytes; i++)
		{
			// %4 to print every 4 byte word
			if(i%1 == 0)
			{
				printf(" ");
			}
			if(i%16 == 0)
			{
				printf("\n");
			}
			printf("%02X", txBuffer[i]);
		}
	}
	if(rxNumBytes != 0)
	{
		printf("\nReceived bytes (0x):");
		for(uint32_t i = 0; i < rxNumBytes; i++)
		{
			// %4 to print every word (4 bytes)
			if(i%1 == 0)
			{
				printf(" ");
			}
			if(i%16 == 0)
			{
				printf("\n");
			}
			printf("%02X", rxBuffer[i]);
		}
	}
	printf("\n");
}
