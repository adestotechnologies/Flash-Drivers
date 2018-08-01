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
 * @ingroup USER_CONFIG Configuration Layer
 */
/**
 * @file    user_config.h
 * @brief   Project declarations exist here.
 */
#ifndef USER_CONFIG_H_
#define USER_CONFIG_H_


/* \=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\=\
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * ----------------- USER DEFINED PORTION ----------------
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * /=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/=/
 */

/*
 *
 * This portion of the code contains the various user definitions
 * and functions. These include the following:
 *
 * 1. MCU specific include files
 * 2. Part number
 * 3. SPI ports and pins
 * 3. User defined function calls to clear/set/init pins
 * 4. Board initialization function
 */

/*
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * ----------------- MCU Specific Includes ---------------
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

// Included for board initialization (see USER_CONFIG_BoardInit()).
#include "board.h"
// Included for board initialization (see USER_CONFIG_BoardInit()).
#include "peripherals.h"
// Included for board initialization (see USER_CONFIG_BoardInit()).
#include "pin_mux.h"
// Included for GPIO library function calls (see USER_CONFIG_BoardInit()).
#include "fsl_gpio.h"

/*
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * --------------------- Part Number ---------------------
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

/*! Definition of part number. */
#define PARTNO ATxxxx /* <- Replace with the device being used. */
#define ALL 1 /* <- Replace with 0 if selecting a part above. */

/*
 * List of supported parts:
 * RM331x
 * AT25XE512C
 * AT25XE011
 * AT25XE021A
 * AT25XE041B
 * AT25DN256
 * AT25DN512C
 * AT25DN011
 * AT25DF256
 * AT25DF512C
 * AT25DF011
 * AT25DF021A
 * AT25DF041B
 * AT25XV021A
 * AT25XV041B
 * AT45DB021E
 * AT45DB041E
 * AT45DB081E
 * AT45DB161E
 * AT45DB321E
 * AT45DB641E
 * AT45DQ161
 * AT45DQ321
 * AT25PE20
 * AT25PE40
 * AT25PE80
 * AT25PE16
 * AT25SF041
 * AT25SF081
 * AT25SF161
 * AT25SF321
 * AT25SF641
 * AT25SL321
 * AT25SL641
 * AT25SL128A
 * AT25DL081
 * AT25DL161
 * AT25DF081A
 * AT25DF321A
 * AT25DF641A
 * AT25QL321
 * AT25QL641
 * AT25QL128A
 * AT25QF641
 */

/*
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * -------------------- Ports and Pins -------------------
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

/*!
 * @brief Base register used for CSb control.
 */
#define USER_CONFIG_CSB_PORT  GPIO3_BASE
/*!
 * @brief Base register used for TRIGGER control.
 */
#define USER_CONFIG_TRIGGER_PORT  GPIO3_BASE
/*!
 * @brief Base register used for SCK control.
 */
#define USER_CONFIG_SCK_PORT  GPIO3_BASE
/*!
 * @brief Base register used for MOSI control.
 */
#define USER_CONFIG_MOSI_PORT GPIO3_BASE
/*!
 * @brief Base register used for MISO control.
 */
#define USER_CONFIG_MISO_PORT GPIO3_BASE
/*!
 * @brief Base register used for IO2 control.
 */
#define USER_CONFIG_IO2_PORT  GPIO3_BASE
/*!
 * @brief Base register used for IO3 control.
 */
#define USER_CONFIG_IO3_PORT GPIO3_BASE

//! Pin number for CSb
//! @warning Don't forget to update USER_CONFIG_BoardInit()
//! called from main! As is, both CSb pins are configured as outputs
//! so the changes above and below in this file are the only things
//! needed (pin and port numbers for CSb).
#define USER_CONFIG_CSB_PIN  6U
//! Pin number for TRIGGER
#define USER_CONFIG_TRIGGER_PIN  2U
//! Pin number for SCK
#define USER_CONFIG_SCK_PIN  7U
//! Pin number for MOSI
#define USER_CONFIG_MOSI_PIN 8U
//! Pin number for MISO
#define USER_CONFIG_MISO_PIN 9U
//! Pin number for IO2
#define USER_CONFIG_IO2_PIN 10U
//! Pin number for IO3
#define USER_CONFIG_IO3_PIN 11U
//! Half clock period delay interval
#define DELAY 5U


/*
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * ---------------- Read, Write, Init Port/Pins --------------
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

/*!
 * @brief Enumeration used when initializing pins as inputs or outputs.
 * OUTPUT = 0, INPUT = 1.
 * See USER_CONFIG_PinInit().
 */
enum directionIO{OUTPUT, INPUT};

/*!
 * @brief Initializes a given pin as either an input or output. When porting,
 * this function must be redefined.
 *
 * @param port The port number.
 * @param pin The pin number.
 * @param direction INPUT or OUTPUT. See the directionIO enum type.
 *
 * @retval void
 */
void USER_CONFIG_PinInit(uint32_t port, uint32_t pin, enum directionIO direction);

/*!
 * @brief Clears a given pin on a port to LOW.
 *
 * @param port The port number.
 * @param pin The pin number.
 *
 * @retval void
 */
void USER_CONFIG_PinClear(uint32_t port, uint32_t pin);

/*!
 * @brief Sets a given pin on a port to HIGH.
 *
 * @param port The port number.
 * @param pin The pin number.
 *
 * @retval void
 */
void USER_CONFIG_PinSet(uint32_t port, uint32_t pin);

/*!
 * @brief Reads the voltage on a given pin.
 *
 * @param port The port number.
 * @param pin The pin number.
 *
 * @retval uint32_t 0 = LOW, 1 = HIGH.
 */
uint8_t USER_CONFIG_PinRead(uint32_t port, uint32_t pin);


/*
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * ---------------------- Board Init ---------------------
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 */

/*!
 * @brief Configures the board for program usage. Initializes the requisite ports/pins
 * for use as GPIOs. Starts the various clocks, and enables UART via USB connection.
 *
 * @retval void
 */
void USER_CONFIG_BoardInit();

#endif
