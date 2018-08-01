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
 * @ingroup TEST_LAYER MAIN
 */
/**
 * @file    test.h
 * @brief   Test layer declarations exist here.
 */
#ifndef TEST_H_
#define TEST_H_

#include "cmd_defs.h"

#if defined(MONETA_DEVICE)
#include "moneta.h"

/**
 * @brief Sample use of the driver for a Moneta device. <br>
 * This function is meant to showcase how the API is
 * used by performing tests on the device, and outputting
 * messages based on performance. Not all functions are used,
 * although most are. This is NOT meant as a standalone reference
 * and should be used in conjunction with the provided documentation
 * and the relevant datasheets. <br>
 *
 * Testing is done in multiple stages. The order matters from
 * each step to then next in this example.
 *
 * Testing commences as follows:
 * 1. Read manufacturing ID, test write enable, test write disable.
 * 2. Test read and write functionality.
 * 3. Test ultra deep power down mode and JEDEC reset.
 *
 * At each stage various messages will confirm that the tests have passed or failed.
 * Failure is expected during power down mode, as the MISO line is not driven, thus
 * causing data read to be unpredictable.
 *
 * @retval int Returns the number of errors in the testbench.
 */
int monetaTest();

#elif defined(FUSION_DEVICE)
#include "fusion.h"

/**
 * @brief Sample use of the driver for a Fusion device. <br>
 * This function is meant to showcase how the API is to
 * be used by performing tests on the device, and outputting
 * messages based on performance. Not all functions are used,
 * although most are. This is NOT meant as a standalone reference
 * and should be used in conjunction with the provided documentation
 * and the relevant datasheets. <br>
 *
 * Testing is done in multiple stages. The order matters from
 * each step to the next in this example.
 *
 * Testing commences as follows:
 * 1. Read manufacturing ID, test write enable, test write disable.
 * 2. Test that sectors can be properly unprotected.
 * 3. Test the page erase functionality.
 * 4. Test the sequential/byte program mode.
 * 5. Test dual read and write.
 * 6. Test deep power down mode
 * 7. Test global protect and global unprotect.
 * 8. Test ultra deep power down mode and JEDEC reset.
 *
 * At each stage various messages will confirm that the tests have passed or failed.
 * Failure is expected during power down modes, as the MISO line is not driven, thus
 * causing data read to be unpredictable.
 *
 * @retval uint32_t Returns the number of errors in the testbench.
 */
uint32_t fusionTest();

#elif defined(DATAFLASH_DEVICE)
#include "dataflash.h"

/**
 * @brief Sample use of the driver for a Dataflash device. <br>
 * This function is meant to showcase how the API is to
 * be used by performing tests on the device, and outputting
 * messages based on performance. Not all functions are used,
 * although most are. This is NOT meant as a standalone reference
 * and should be used in conjunction with the provided documentation
 * and the relevant datasheets. <br>
 *
 * Testing is done in multiple stages. The order matters from
 * each step to the next in this example.
 *
 * Testing commences as follows:
 * 1. Read manufacturing ID.
 * 2. Test read and write functionality.
 * 3. Test deep power down mode.
 * 4. Test ultra deep power down mode and JEDEC reset.
 * 5. Test erase commands.
 * 6. Test memory to buffer commands.
 * 7. Test Read - Modify - Write capabilities.
 *
 * At each stage various messages will confirm that the tests have passed or failed.
 * Failure is expected during power down modes, as the MISO line is not driven, thus
 * causing data read to be unpredictable.
 *
 * @retval uint32_t Returns the number of errors in the testbench.
 */
uint32_t dataflashTest();

#elif defined(STANDARDFLASH_DEVICE)
#include "standardflash.h"

/**
 * @brief Sample use of the driver for a Standardflash device. <br>
 * This function is meant to showcase how the API is to
 * be used by performing tests on the device, and outputting
 * messages based on performance. Not all functions are used,
 * although most are. This is NOT meant as a standalone reference
 * and should be used in conjunction with the provided documentation
 * and the relevant datasheets. <br>
 *
 * Testing is done in multiple stages. The order matters from
 * each step to the next in this example.
 *
 * Testing commences as follows:
 * 1. Read manufacturing ID.
 * 2. Write and read commands.
 * 3. Dual read commands.
 * 4. Quad read commands.
 * 5. Test deep power down mode.
 *
 * If one is using a component with specialized capabilities, the test
 * continues with features exclusive to that device.
 *
 * 1. QPI read manufacturing ID.
 * 2. QPI write and read commands.
 * 3. QPI read commands
 * 4. Dual Input Byte/Page Program
 *
 * At each stage various messages will confirm that the tests have passed or failed.
 * Failure is expected during power down modes, as the inputs lines are not driven, thus
 * causing data read to be unpredictable.
 *
 * @retval uint32_t Returns the number of errors in the testbench.
 */
uint32_t standardflashTest();

#endif

/*!
 * @brief A user defined test function. Calls to the Adesto Layer and any test
 * sequences can be written here.
 *
 * @retval uint32_t The number of errors in a given test.
 */
uint32_t test();

/*!
 * @brief The default test function can be called by the user to perform
 * the default test prepared for each device. The default tests are declared
 * and defined in the test layer (test.h test.c).
 *
 * @retval uint32_t The number of errors in a given test.
 */
uint32_t defaultTest();

#endif /* TEST_H_ */
