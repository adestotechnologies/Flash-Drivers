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
 * @file    spi_driver.c
 * @brief   Definitions of spi_driver functions.
 */
#include "spi_driver.h"

void SPI_PinInit(uint32_t port, uint32_t pin, enum directionIO direction)
{
	USER_CONFIG_PinInit(port, pin, direction);
}
void SPI_PinClear(uint32_t port, uint32_t pin)
{
	USER_CONFIG_PinClear(port, pin);
}
void SPI_PinSet(uint32_t port, uint32_t pin)
{
	USER_CONFIG_PinSet(port, pin);
}

uint32_t SPI_PinRead(uint32_t port, uint32_t pin)
{
	return USER_CONFIG_PinRead(port, pin);
}

void SPI_ConfigureSingleSPIIOs()
{
	/* Configure each of the 4 pins needed for testing. */
	// CSb - PTD4 for Moneta shield, PTC1 for Dataflash shield
	SPI_PinInit(SPI_CSB_PORT, SPI_CSB_PIN, OUTPUT);
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
	// SCK - PTD1
	SPI_PinInit(SPI_SCK_PORT, SPI_SCK_PIN, OUTPUT);
	// MOSI - PTD2
	SPI_PinInit(SPI_MOSI_PORT, SPI_MOSI_PIN, OUTPUT);
	// MISO - PTD3
	SPI_PinInit(SPI_MISO_PORT, SPI_MISO_PIN, INPUT);
	// HOLDb - PTD4
	SPI_PinInit(SPI_HOLDB_PORT, SPI_HOLDB_PIN, OUTPUT);
	// WPb - PTC5
	SPI_PinInit(SPI_WPB_PORT, SPI_WPB_PIN, OUTPUT);
	// Trigger - PTC2
	SPI_PinInit(SPI_TRIGGER_PORT, SPI_TRIGGER_PIN, OUTPUT);
	SPI_PinSet(SPI_TRIGGER_PORT, SPI_TRIGGER_PIN);
	// Set both WPb and HOLDb to high.
	SPI_PinSet(SPI_HOLDB_PORT, SPI_HOLDB_PIN);
	SPI_PinSet(SPI_WPB_PORT, SPI_WPB_PIN);
}

void SPI_ReturnToSingleSPIIOs()
{
	// MOSI - PTD2
	SPI_PinInit(SPI_MOSI_PORT, SPI_MOSI_PIN, OUTPUT);
	// MISO - PTD3
	SPI_PinInit(SPI_MISO_PORT, SPI_MISO_PIN, INPUT);
	// HOLDb - PTD4
	SPI_PinInit(SPI_HOLDB_PORT, SPI_HOLDB_PIN, OUTPUT);
	// WPb - PTC5
	SPI_PinInit(SPI_WPB_PORT, SPI_WPB_PIN, OUTPUT);

	// Set both WPb and HOLDb to high.
	SPI_PinSet(SPI_HOLDB_PORT, SPI_HOLDB_PIN);
	SPI_PinSet(SPI_WPB_PORT, SPI_WPB_PIN);
}

void SPI_ConfigureDualSPIIOsInput()
{
	// MOSI - PTD2
	SPI_PinInit(SPI_MOSI_PORT, SPI_MOSI_PIN, INPUT);
	SPI_PinInit(SPI_MISO_PORT, SPI_MISO_PIN, INPUT);
}

void SPI_ConfigureQuadSPIIOsInput()
{
	// MOSI - PTD2
	// MISO - PTD3
	SPI_PinInit(SPI_MISO_PORT, SPI_MISO_PIN, INPUT);
	// MOSI - PTD2
	SPI_PinInit(SPI_MOSI_PORT, SPI_MOSI_PIN, INPUT);
	// HOLDb - PTD4
	SPI_PinInit(SPI_HOLDB_PORT, SPI_HOLDB_PIN, INPUT);
	// WPb - PTC5
	SPI_PinInit(SPI_WPB_PORT, SPI_WPB_PIN, INPUT);
}

void SPI_ConfigureDualSPIIOsOutput()
{
	// MISO - PTD3
	SPI_PinInit(SPI_MISO_PORT, SPI_MISO_PIN, OUTPUT);
	SPI_PinInit(SPI_MOSI_PORT, SPI_MOSI_PIN, OUTPUT);
}

void SPI_ConfigureQuadSPIIOsOutput()
{
	// MISO - PTD3
	SPI_PinInit(SPI_MISO_PORT, SPI_MISO_PIN, OUTPUT);
	// MOSI - PTD2
	SPI_PinInit(SPI_MOSI_PORT, SPI_MOSI_PIN, OUTPUT);
	// HOLDb - PTD4
	SPI_PinInit(SPI_HOLDB_PORT, SPI_HOLDB_PIN, OUTPUT);
	// WPb - PTC5
	SPI_PinInit(SPI_WPB_PORT, SPI_WPB_PIN, OUTPUT);
}

void SPI_Delay(uint32_t delay_time)
{
    volatile uint32_t i = 0;
    for (i = 0; i < delay_time; ++i)
    {
        __asm("NOP"); /* delay */
    }
}

void SPI_ClockTick()
{
	SPI_Delay(DELAY);
	SPI_PinSet(SPI_SCK_PORT, SPI_SCK_PIN);
	SPI_Delay(DELAY);
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
}

void SPI_SendBit(uint8_t bit_tx)
{
	// Guarantee clock is set to low
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
	// Set MOSI based on bit_tx
	if(bit_tx)
		SPI_PinSet(SPI_MOSI_PORT, SPI_MOSI_PIN);
	else
		SPI_PinClear(SPI_MOSI_PORT, SPI_MOSI_PIN);
	// Toggle clock
	SPI_ClockTick(DELAY);
}

void SPI_SendByte(uint8_t tx)
{
	int32_t i = 0;
	// Send byte
	for(i = 7; i >= 0; i--)
	{
		SPI_SendBit((tx >> i) & 1);
	}
}

void SPI_DualSendByte(uint8_t tx)
{
	int32_t i = 0;
	uint8_t bit_tx = 0;
	// Send byte
	for(i = 7; i >= 1; i-= 2)
	{
		// Guarantee clock is set to low
		SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);

		// Send MSB first
		bit_tx = (tx >> i) & 1;
		// Set MOSI based on bit_tx
		if(bit_tx)
			SPI_PinSet(SPI_MISO_PORT, SPI_MISO_PIN);
		else
			SPI_PinClear(SPI_MISO_PORT, SPI_MISO_PIN);

		// Send lower bit
		bit_tx = (tx >> (i-1)) & 1;
		// Set MOSI based on bit_tx
		if(bit_tx)
			SPI_PinSet(SPI_MOSI_PORT, SPI_MOSI_PIN);
		else
			SPI_PinClear(SPI_MOSI_PORT, SPI_MOSI_PIN);

		// Toggle clock
		SPI_ClockTick(DELAY);
	}
}

// TODO:
void SPI_QuadSendByte(uint8_t tx)
{
	int32_t i = 0;
	uint8_t bit_tx = 0;
	// Send byte
	for(i = 7; i >= 3; i-= 4)
	{
		// Guarantee clock is set to low
		SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);

		// Send MSB first
		bit_tx = (tx >> i) & 1;
		if(bit_tx)
			SPI_PinSet(SPI_HOLDB_PORT, SPI_HOLDB_PIN);
		else
			SPI_PinClear(SPI_HOLDB_PORT, SPI_HOLDB_PIN);

		bit_tx = (tx >> (i-1)) & 1;
		if(bit_tx)
			SPI_PinSet(SPI_WPB_PORT, SPI_WPB_PIN);
		else
			SPI_PinClear(SPI_WPB_PORT, SPI_WPB_PIN);

		bit_tx = (tx >> (i-2)) & 1;
		if(bit_tx)
			SPI_PinSet(SPI_MISO_PORT, SPI_MISO_PIN);
		else
			SPI_PinClear(SPI_MISO_PORT, SPI_MISO_PIN);

		bit_tx = (tx >> (i-3)) & 1;
		if(bit_tx)
			SPI_PinSet(SPI_MOSI_PORT, SPI_MOSI_PIN);
		else
			SPI_PinClear(SPI_MOSI_PORT, SPI_MOSI_PIN);

		// Toggle clock
		SPI_ClockTick(DELAY);
	}
}

uint8_t SPI_ReceiveByte()
{
	int32_t i = 0;
	uint8_t input = 0;

	// Receive byte
	for(i = 7; i >= 0; i--)
	{
		if(SPI_PinRead(SPI_MISO_PORT, SPI_MISO_PIN))
			input |= (1 << i);
		else
			input &= ~(1 << i);
		SPI_ClockTick(DELAY);
	}
	return input;
}

uint8_t SPI_DualReceiveByte()
{
	int32_t i = 0;
	uint8_t input = 0;

	// Receive dual byte
	for(i = 7; i >= 1; i-=2)
	{
		// Store MSB first, then lower bit
		if(SPI_PinRead(SPI_MISO_PORT, SPI_MISO_PIN))
			input |= (1 << i);
		else
			input &= ~(1 << i);

		if(SPI_PinRead(SPI_MOSI_PORT, SPI_MOSI_PIN))
			input |= (1 << (i-1));
		else
			input &= ~(1 << (i-1));

		SPI_ClockTick(DELAY);
	}
	return input;
}

uint8_t SPI_QuadReceiveByte()
{
	int32_t i = 0;
	uint8_t input = 0;

	// Receive dual byte
	for(i = 7; i >= 3; i-=4)
	{
		// Store MSB first, then lower bit
		if(SPI_PinRead(SPI_HOLDB_PORT, SPI_HOLDB_PIN))
			input |= (1 << i);
		else
			input &= ~(1 << i);

		if(SPI_PinRead(SPI_WPB_PORT, SPI_WPB_PIN))
			input |= (1 << (i-1));
		else
			input &= ~(1 << (i-1));

		if(SPI_PinRead(SPI_MISO_PORT, SPI_MISO_PIN))
			input |= (1 << (i-2));
		else
			input &= ~(1 << (i-2));

		if(SPI_PinRead(SPI_MOSI_PORT, SPI_MOSI_PIN))
			input |= (1 << (i-3));
		else
			input &= ~(1 << (i-3));
		SPI_ClockTick(DELAY);
	}
	return input;
}

void SPI_Exchange(uint8_t *txBuffer,
				  uint32_t txNumBytes,
				  uint8_t *rxBuffer,
				  uint32_t rxNumBytes,
				  uint32_t dummyNumBytes)
{
	uint32_t i = 0;
	// Begin data exchange
	// Set clock to low
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
	// Select chip
	SPI_PinClear(SPI_CSB_PORT, SPI_CSB_PIN);

	// Send each byte
	for(i = 0; i < txNumBytes; i = i+1)
		SPI_SendByte(txBuffer[i]);
	// Receive each byte
	for(i = 0; i < dummyNumBytes; i = i+1)
		SPI_ReceiveByte();
	// Receive each byte
	for(i = 0; i < rxNumBytes; i = i+1)
		rxBuffer[i] = SPI_ReceiveByte();

	// End data exchange
	// Set clock to low
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
	// Deselect chip
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
}

void SPI_DualExchange(uint8_t standardSPINumBytes,
					  uint8_t *txBuffer,
					  uint32_t txNumBytes,
					  uint8_t *rxBuffer,
					  uint32_t rxNumBytes,
					  uint32_t dummyNumBytes)
{
	uint32_t i = 0;
	// Begin data exchange
	// Set clock to low
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
	// Select chip
	SPI_PinClear(SPI_CSB_PORT, SPI_CSB_PIN);

	// Transmit bytes in standard single SPI
	for(i = 0; i < standardSPINumBytes; i = i+1)
		SPI_SendByte(txBuffer[i]);
	// Transmit dummy bytes in standard single SPI mode
	for(i = 0; i < dummyNumBytes; i = i+1)
		SPI_ReceiveByte();
	// If we're sending data to the flash device then configure for dual tx
	// and transmit the remaining data in dual SPI mode.
	if(standardSPINumBytes <= txNumBytes)
	{
		SPI_ConfigureDualSPIIOsOutput();
		for(i = standardSPINumBytes; i < txNumBytes; i = i+1)
		{
			SPI_DualSendByte(txBuffer[i]);
		}
	}
	// Receive each byte
	// If we're receiving at least 1 byte, then configure the outputs for dual rx
	// and receive the data in dual SPI mode.
	if(rxNumBytes > 0)
	{
		SPI_ConfigureDualSPIIOsInput();
		for(i = 0; i < rxNumBytes; i = i+1)
		{
			rxBuffer[i] = SPI_DualReceiveByte();
		}
	}

	// End data exchange
	// Set clock to low
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
	// Deselect chip
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
	// Reconfigure device for standard SPI operation
	SPI_ReturnToSingleSPIIOs();
}

void SPI_QuadExchange(uint8_t standardSPINumBytes,
					  uint8_t *txBuffer,
					  uint32_t txNumBytes,
					  uint8_t *rxBuffer,
					  uint32_t rxNumBytes,
					  uint32_t dummyNumBytes)
{
	uint32_t i = 0;
	// Determines if dummy bytes should be sent in standard SPI, or another mode.
	uint8_t dummyBytesSSPI = 1;
	// Begin data exchange
	// Set clock to low
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
	// Select chip
	SPI_PinClear(SPI_CSB_PORT, SPI_CSB_PIN);

	// Transmit standardSPINumBytes bytes in standard single SPI.
	// Unless in QPI mode, a transmission will take the 1-4-4 or
	// 1-1-4 format, the 1 indicating SPI and the 4 indicating Quad.
	// -- This assumes that the device is configured in single SPI.
	for(i = 0; i < standardSPINumBytes; i = i+1)
		SPI_SendByte(txBuffer[i]);
	// If we're sending data to the flash device then configure for quad tx
	// and transmit the remaining data in quad SPI mode.
	if(standardSPINumBytes < txNumBytes)
	{
		dummyBytesSSPI = 0;
		SPI_ConfigureQuadSPIIOsOutput();
		for(i = standardSPINumBytes; i < txNumBytes; i = i+1)
		{
			SPI_QuadSendByte(txBuffer[i]);
		}
	}
	// Transmit the dummy bytes.
	if(dummyNumBytes > 0)
	{
		// Need this in case it was all standardSPI. This puts the WP, HOLD, SO, SI pins in
		// a high impedance state which is necessary for quad IO.
		// Quad output just needs HOLD, WP, and SO to be Hi-Z.
		// Quad IO needs all 4 to be Hi-Z.
		SPI_ConfigureQuadSPIIOsInput();
		if(dummyBytesSSPI == 1)
		{
			// Transmit dummy bytes in SSPI. (Really just clock toggling)
			for(i = 0; i < dummyNumBytes; i = i+1)
			{
				SPI_ReceiveByte();
			}
		}
		else
		{
			// Transmit dummy bytes in QSPI.
			for(i = 0; i < dummyNumBytes; i = i+1)
			{
				SPI_QuadReceiveByte();
			}
		}
	}
	// Receive each byte
	// If we're receiving at least 1 byte, then configure the outputs for quad rx
	// and receive the data in quad SPI mode.
	if(rxNumBytes > 0)
	{
		SPI_ConfigureQuadSPIIOsInput();
		for(i = 0; i < rxNumBytes; i = i+1)
		{
			rxBuffer[i] = SPI_QuadReceiveByte();
		}
	}

	// End data exchange
	// Set clock to low
	SPI_PinClear(SPI_SCK_PORT, SPI_SCK_PIN);
	// Deselect chip
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
	// Reconfigure device for standard SPI operation
	SPI_ReturnToSingleSPIIOs();
}

void SPI_Trigger()
{
	SPI_PinSet(SPI_TRIGGER_PORT, SPI_TRIGGER_PIN);
	SPI_Delay(DELAY);
	SPI_PinClear(SPI_TRIGGER_PORT, SPI_TRIGGER_PIN);
}

void SPI_JEDECReset()
{
	// Clear CSb
	SPI_PinClear(SPI_CSB_PORT, SPI_CSB_PIN);
	// Clear MOSI
	SPI_PinClear(SPI_MOSI_PORT, SPI_MOSI_PIN);
	SPI_Delay(DELAY);

	// Set CSb
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
	// Set MOSI
	SPI_PinSet(SPI_MOSI_PORT, SPI_MOSI_PIN);
	SPI_Delay(DELAY);

	// Clear CSb
	SPI_PinClear(SPI_CSB_PORT, SPI_CSB_PIN);
	SPI_Delay(DELAY);

	// Set CSb
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
	// Clear MOSI
	SPI_PinClear(SPI_MOSI_PORT, SPI_MOSI_PIN);
	SPI_Delay(DELAY);

	// Clear CSb
	SPI_PinClear(SPI_CSB_PORT, SPI_CSB_PIN);
	SPI_Delay(DELAY);

	// Set CSb
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
	// Set MOSI
	SPI_PinSet(SPI_MOSI_PORT, SPI_MOSI_PIN);
	SPI_Delay(DELAY);

	// Clear CSb
	SPI_PinClear(SPI_CSB_PORT, SPI_CSB_PIN);
	SPI_Delay(DELAY);

	// Set CSb
	SPI_PinSet(SPI_CSB_PORT, SPI_CSB_PIN);
	SPI_Delay(DELAY);
}
