/*
    Copyright (C) 2020-2021  Blaž Zakrajšek

    This file is part of libserial.

    libserial is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    libserial is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with libserial.  If not, see <https://www.gnu.org/licenses/>.

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once
#include <gtest/gtest.h>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport/serialport.hpp>

BEGIN_NAMESPACE_LIBSERIAL

/**
 * @brief SerialPortTest class
 *
 */
class SerialPortTest : public testing::Test
{
protected:
    /**
     * @brief Get the first serial port name
     *
     * @return std::string Serial port name
     */
    static std::string getFirstSerialPort();

    /**
     * @brief Get the second serial port name
     *
     * @return std::string Serial port name
     */
    static std::string getSecondSerialPort();

    /**
     * @brief Get the non-existing serial port name
     *
     * @return std::string Serial port name
     */
    static std::string getNonExistingSerialPort();

    /**
      * @brief Get the first data sample
      *
      * @param characterSize Character size
      * @return std::string Data sample
      */
    static std::string getFirstData(CharacterSize characterSize = CharacterSize::CHARACTER_SIZE_DEFAULT);

    /**
     * @brief Get the second data sample
     *
     * @param characterSize Character size
     * @return std::string Data sample
     */
    static std::string getSecondData(CharacterSize characterSize = CharacterSize::CHARACTER_SIZE_DEFAULT);

    /**
     * @brief Static transmit/receive delay in milli-seconds
     *
     * @note This is an undeterminate "safety" part of the delay
     *   which depends on the hardware and operating system.
     *   If failures are experienced on a ReadWrite tests, this
     *   delay should be increased appropriately.
     */
    static constexpr double staticDelay{100.0};

    /**
     * @brief Set up the test
     *
     */
    virtual void SetUp() override;

    /**
     * @brief Tear down the test
     *
     */
    virtual void TearDown() override;

    /**
     * @brief Perform contructor tests
     *
     */
    void performConstructorTests();

    /**
     * @brief Perform open test on a first test port
     *
     */
    void performFirstPortOpenTest();

    /**
     * @brief Perform open test on a second test port
     *
     */
    void performSecondPortOpenTest();

    /**
     * @brief Perform open and close tests
     *
     */
    void performOpenCloseTests();

    /**
     * @brief Perform open mode tests
     *
     */
    void performOpenModeTests();

    /**
     * @brief Perform function tests on closed port
     *
     */
    void performClosedPortFunctionTests();

    /**
     * @brief Perform exclusive mode tests
     *
     */
    void performExclusiveModeTests();

    /**
     * @brief Perform properties tests
     *
     */
    void performPropertiesTests();

    /**
     * @brief Perform simple read/write tests
     *
     */
    void performSimpleReadWriteTests();

    /**
     * @brief Perform extended read/write std::string tests
     *
     */
    void performExtendedReadWriteStringTests();

    /**
     * @brief Perform extended read/write char[] tests
     *
     */
    void performExtendedReadWriteCharArrayTests();

    /**
     * @brief Perform control line tests
     *
     */
    void performControlLineTests();

    /**
     * @brief Perform control line tests on a pair of ports
     *
     * @param portA First serial port
     * @param portB Second serial port
     */
    void performControlLineTest(SerialPort& portA, SerialPort& portB);

    /**
     * @brief First serial port
     *
     */
    SerialPtrUniquePtr firstPort;

    /**
     * @brief Second serial port
     *
     */
    SerialPtrUniquePtr secondPort;
};

END_NAMESPACE_LIBSERIAL
