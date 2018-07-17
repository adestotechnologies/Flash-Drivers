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
 * @ingroup SPI_LAYER
 */
/**
 * @file    spi_driver.h
 * @brief   Declarations of spi_driver functions.
 *
 * This is the Adesto SPI driver. It supports standard SPI, dual
 * SPI, and Quad SPI. Functionality is built in for a trigger GPIO
 * as well as a JEDEC reset.
 *
 * Initialization of the driver takes place with the
 * SPI_ConfigureSingleSPIIOs() function which should be called after
 * the pins have been set as GPIOs but before adesto layer commands
 * are run.
 */
#ifndef SPI_DRIVER_H_
#define SPI_DRIVER_H_

#include "user_config.h"

#define SPI 0
#define QPI 1

/*!
 * @brief Initializes a given pin as either an input or output.
 *
 * @param port The port number. The type must be redefined based on the GPIO driver being used.
 * @param pin The pin number.
 * @param direction INPUT or OUTPUT. See the directionIO enum type.
 *
 * @retval void
 */
void SPI_PinInit(uint32_t port, uint32_t pin, enum directionIO direction);

/*!
 * @brief Clears a given pin on a port to LOW.
 *
 * @param port The port number.
 * @param pin The pin number.
 *
 * @retval void
 */
void SPI_PinClear(uint32_t port, uint32_t pin);

/*!
 * @brief Sets a given pin on a port to HIGH.
 *
 * @param port The port number.
 * @param pin The pin number.
 *
 * @retval void
 */
void SPI_PinSet(uint32_t port, uint32_t pin);

/*!
 * @brief Reads the voltage on a given pin.
 *
 * @param port The port number.
 * @param pin The pin number.
 *
 * @retval 32-bit word, 0 = LOW, 1 = HIGH.
 */
uint32_t SPI_PinRead(uint32_t port, uint32_t pin);

/*!
 * @brief Sends and receives bytes based on the function parameters.
 * MISO and MOSI fill their standard SPI roles.
 *
 * @param *txBuffer A pointer to the tx byte array to be transmitted.
 * Should have tx_bytes elements.
 * @param txNumBytes The number of bytes to be transmitted.
 * @param *rxBuffer A pointer to the rx byte array where received data will be stored.
 * Should have rx_bytes elements.
 * @param rxNumBytes The number of bytes to be received.
 * @param dummyNumBytes The number of dummy bytes to be sent.
 *
 * @retval void
 */
void SPI_Exchange(uint8_t *txBuffer,
				  uint32_t txNumBytes,
				  uint8_t *rxBuffer,
				  uint32_t rxNumBytes,
				  uint32_t dummyNumBytes);

/*!
 * @brief Sends and receives bytes based on the function parameters.
 * MOSI is used for the opcode and address, then MISO or MOSI are switched for transmission
 * and receiving such that data is transmitted from/to the flash device along both lines.
 * Their state will then be reverted to original SPI functionality.
 *
 * @param standardSPINumBytes The number of bytes to be sent in standard single SPI mode.
 * 		  Note: Dummy bytes will be sent in standard single SPI mode.
 * @param *txBuffer A pointer to the tx byte array to be transmitted.
 * Should have tx_bytes elements.
 * @param txNumBytes The total number of bytes to be transmitted including those in standard SPI.
 * @param *rxBuffer A pointer to the rx byte array where received data will be stored.
 * Should have rx_bytes elements.
 * @param rxNumBytes The number of bytes to be received.
 * @param dummyNumBytes The number of dummy bytes to be sent.
 *
 * @retval void
 */
void SPI_DualExchange(uint8_t standardSPINumBytes,
					  uint8_t *txBuffer,
					  uint32_t txNumBytes,
					  uint8_t *rxBuffer,
					  uint32_t rxNumBytes,
					  uint32_t dummyNumBytes);

/*!
 * @brief Sends and receives bytes based on the function parameters.
 * MOSI can used for the opcode and address, then MISO, MOSI, HOLDb, WPb are switched for transmission
 * and receiving such that data is transmitted from/to the flash device along all 4 lines.
 * Their state will then be reverted to original SPI functionality.
 *
 * @param standardSPINumBytes The number of bytes to be sent in standard single SPI mode.
 * 		  Note: Dummy bytes will be sent in standard single SPI mode based on the input parameters.
 * @param *txBuffer A pointer to the tx byte array to be transmitted.
 * Should have tx_bytes elements.
 * @param txNumBytes The total number of bytes to be transmitted including those in standard SPI.
 * @param *rxBuffer A pointer to the rx byte array where received data will be stored.
 * Should have rx_bytes elements.
 * @param rxNumBytes The number of bytes to be received.
 * @param dummyNumBytes The number of dummy bytes to be sent.
 *
 * @retval void
 */
void SPI_QuadExchange(uint8_t standardSPINumBytes,
					  uint8_t *txBuffer,
					  uint32_t txNumBytes,
					  uint8_t *rxBuffer,
					  uint32_t rxNumBytes,
					  uint32_t dummyNumBytes);

/*!
 * @brief Receives a byte along MISO and returns the value received.
 *
 * @retval uint8_t The byte received in Little Endian format.
 */
uint8_t SPI_ReceiveByte();

/*!
 * @brief Receives a byte along both MOSI and MISO and returns the value received.
 *
 * @retval uint8_t The byte received in Little Endian format.
 */
uint8_t SPI_DualReceiveByte();

/*!
 * @brief Sends a byte along MOSI.
 *
 * @param tx Byte to be sent.
 * @retval void
 */
void SPI_SendByte(uint8_t tx);

/*!
 * @brief Sends a byte along both MOSI and MISO.
 *
 * @param tx Byte to be sent.
 * @retval void
 */
void SPI_DualSendByte(uint8_t tx);

/*!
 * @brief Toggles the clock: current_state->high->low
 *
 * @retval void
 */
void SPI_ClockTick();

/*!
 * @brief Sends a single bit along MOSI while toggling the clock.
 *
 * @param bit_tx Bit to be sent, either 1 or 0.
 * @retval void
 */
void SPI_SendBit(uint8_t bit_tx);

/*!
 * @brief Performs a delay_time number of NOPs
 *
 * @param delay_time The number of NOPs to be run.
 * @retval void
 */
void SPI_Delay(uint32_t delay_time);

/*!
 * @brief Configure the IOs for SPI bit banging usage. 4 pins are
 * needed: CSb, SCK, MOSI, MISO.
 *
 * @retval void
 *
 * @warning Moneta and Dataflash shields differ. Moneta uses PTD4 for CSb,
 * while Dataflash uses PTC2-8.
 * @warning Only call this function once at the beginning of the program to
 * configure the IOs. Calling this function a second time will cause CSb to
 * go low temporarily if it was high. Use SPI_ReturnToSingleSPIIOs() if a return
 * to single SPI is required mid program.
 */
void SPI_ConfigureSingleSPIIOs();

/*!
 * @brief Returns MISO and MOSI pins to their standard SPI state as an input and output.
 *
 * @retval void
 */
void SPI_ReturnToSingleSPIIOs();

/*!
 * @brief Configures SPIO IOs for dual input. This changes the
 * MOSI pin from an output to an input
 * so that the slave can drive both SI and SO.
 *
 * @retval void
 *
 * @warning Make sure to reconfigure the SPIIOs using SPI_ReturnToSingleSPIIOs()
 *  when this is no longer needed.
 */
void SPI_ConfigureDualSPIIOsInput();

/*!
 * @brief Configures SPIO IOs for dual output. This changes the
 * MISO pin from an input to an output so that the master can
 * drive both SI and SO.
 *
 * @retval void
 *
 * @warning Make sure to reconfigure the SPIIOs using SPI_ReturnToSingleSPIIOs()
 *  when this is no longer needed.
 */
void SPI_ConfigureDualSPIIOsOutput();

/*!
 * @brief Configures SPIO IOs for quad input. This changes the
 * MOSI, WPb, and HOLDb pins to inputs so that the slave can
 * drive all 4 IOs.
 *
 * @retval void
 *
 * @warning Make sure to reconfigure the SPIIOs using SPI_ReturnToSingleSPIIOs()
 *  when this is no longer needed.
 */
void SPI_ConfigureQuadSPIIOsInput();

/*!
 * @brief Configures SPIO IOs for quad output. This changes the
 * MISO pin to an output so that the master can  drive all 4 IOs.
 *
 * @retval void
 *
 * @warning Make sure to reconfigure the SPIIOs using SPI_ReturnToSingleSPIIOs()
 *  when this is no longer needed.
 */
void SPI_ConfigureQuadSPIIOsOutput();

/*!
 * @brief Sends a byte along MISO, MOSI, WPb, and HOLDb.
 *
 * @param tx Byte to be sent.
 * @retval void
 */
void SPI_QuadSendByte(uint8_t tx);

/*!
 * @brief Receives a byte along MOSI, MISO, HOLDb, and WPb
 * and returns the value received.
 *
 * @retval uint8_t The byte received in Little Endian format.
 */
uint8_t SPI_QuadReceiveByte();

/*!
 * @brief Triggers a falling edge on the SPI_TRIGGER_PORT/PIN output.
 *
 * @retval void
 */
void SPI_Trigger();

/*!
 * @brief Performs a JEDEC reset on the SPI device.
 *
 * @retval void
 *
 * @warning There must be a short, user inserted, delay after resetting with a JEDEC reset.
 */
void SPI_JEDECReset();

#endif /* SPI_DRIVER_H_ */
