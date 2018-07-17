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
 * @file    helper_functions.h
 * @brief   Declarations of helper functions.
 */

#ifndef HELPER_FUNCTIONS_H_
#define HELPER_FUNCTIONS_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
/*!
 * @brief Helper function to display a byte array/buffer. This function uses the
 * printSPIExchange() function internally, but only displays one byte array.
 *
 * @param byteArray The byte array to be printed.
 * @param numBytes The number of bytes in the array to be printed.
 *
 * @retval void
 */
void displayByteArray(uint8_t *byteArray, uint32_t numBytes);

/*!
 * @brief Helper function to display fill a byte array/buffer with a pattern based
 * on a seedNumber. The seed will be incremented by the index of the array.
 *
 * @param byteArray The byte array to be filled.
 * @param numBytes The number of bytes in the array to be filled.
 * @param seedNumber The byte stored at first index.
 *
 * @retval void
 */
void fillArrayPattern(uint8_t * byteArray, uint32_t numBytes, int seedNumber);

/*!
 * @brief Helper function to display fill a byte array/buffer with a constant number.
 *
 * @param byteArray The byte array to be filled.
 * @param numBytes The number of bytes in the array to be filled.
 * @param constantNum The value to be stored at each element of the array.
 *
 * @retval void
 */
void fillArrayConst(uint8_t * byteArray, uint32_t numBytes, int constantNum);

/**
 * @brief Helper function to compare 2 byte arrays and print an error message
 * if they do not match.
 *
 * @param arr1 The first of the 2 arrays to be compared.
 * @param arr2 The second array to be compared.
 * @param arrLength The length of both arrays.
 *
 * @retval bool Returns 1 if equivalent, 0 if non equivalent
 *
 * @warning The 2 arrays must both be a minimum of arrLength bytes.
 */
bool compareByteArrays(uint8_t *arr1, uint8_t *arr2, uint32_t arrLength);

/*!
 * @brief Loads 1 byte of opcode followed by 3 address bytes into the txBuffer.
 * The data is stored at the first 4 bytes.
 *
 * @retval void
 */
void load4BytesToTxBuffer(uint8_t *txBuffer, uint8_t opcode, uint32_t address);

/*!
 * @brief Prints the byte array in hexadecimal with a formatted output.
 * Indicates what bytes were sent, what was received, and outputs the data in a grid.
 *
 * @param txBuffer Pointer to the txBuffer byte array that will be printed.
 *  Must have at least bytesTx elements.
 * @param txNumBytes The number of txBuffer bytes to be printed. The txBuffer
 * byte array must have at least this many elements.
 * @param rxBuffer Pointer to the rx byte array that will be printed.
 * Must have at least bytesRx elements.
 * @param rxNumBytes The number of rx bytes to be printed.
 * The rx byte array must have at least this many elements.
 *
 * @retval void
 */
void printSPIExchange(uint8_t *txBuffer,
							 uint32_t txNumBytes,
							 uint8_t *rxBuffer,
							 uint32_t rxNumBytes);

#endif /* HELPER_FUNCTIONS_H_ */
