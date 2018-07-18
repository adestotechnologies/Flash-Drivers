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
 * @ingroup ADESTO_LAYER MAIN
 */
/**
 * @file    user_config.c
 * @brief   Project definitions exist here.
 */

#include "user_config.h"

void USER_CONFIG_PinInit(uint32_t port, uint32_t pin, enum directionIO direction)
{
	if(direction == OUTPUT)
	{
		gpio_pin_config_t spi_config_out = {kGPIO_DigitalOutput, 0U};
		GPIO_PinInit((GPIO_Type *)port, pin, &spi_config_out);
	}
	else if(direction == INPUT)
	{
		gpio_pin_config_t spi_config_in = {kGPIO_DigitalInput, 0U};
		GPIO_PinInit((GPIO_Type *)port, pin, &spi_config_in);
	}
}

void USER_CONFIG_PinClear(uint32_t port, uint32_t pin)
{
	GPIO_PinWrite((GPIO_Type *)port, pin, 0);
}

void USER_CONFIG_PinSet(uint32_t port, uint32_t pin)
{
	GPIO_PinWrite((GPIO_Type *)port, pin, 1);
}

uint8_t USER_CONFIG_PinRead(uint32_t port, uint32_t pin)
{
	return (uint8_t) GPIO_PinRead((GPIO_Type *)port, pin);
}

void USER_CONFIG_BoardInit()
{
  	/* Init board hardware. */
	// Set pinmuxes and clock
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
  	/* Init FSL debug console. */
    BOARD_InitDebugConsole();
}
