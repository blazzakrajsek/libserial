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

#include <string>
#include <vector>
#include <gtest/gtest.h>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport/enumerator.hpp>

BEGIN_NAMESPACE_LIBSERIAL

TEST(EnumeratorTest, UpdateSerialPortListFunctionTest)
{
    SCOPED_TRACE("UpdateSerialPortListFunctionTest");

    std::vector<std::string> list{};
    ASSERT_EQ(list.size(), 0);
    ASSERT_TRUE(Enumerator::updateSerialPortList(list));
    ASSERT_GE(list.size(), 2);
}

TEST(EnumeratorTest, SerialPortToFileNameFunctionTest)
{
    SCOPED_TRACE("SerialPortToFileNameFunctionTest");

#ifdef __linux__
    const std::string serialPort{"ttyUSB0"};
    const std::string serialPortFileName{std::string(SERIAL_PORT_PREFIX) + serialPort};
#elif defined(_WIN32) || defined(_WIN64)
    const std::string serialPort{"COM1"};
    const std::string serialPortFileName{std::string(SERIAL_PORT_PREFIX) + serialPort};
#endif // __linux__
    ASSERT_EQ(Enumerator::serialPortToFileName(serialPort), serialPortFileName);
    ASSERT_EQ(Enumerator::serialPortToFileName(serialPortFileName), serialPortFileName);
}

TEST(EnumeratorTest, FileNameToSerialPortFunctionTest)
{
    SCOPED_TRACE("FileNameToSerialPortFunctionTest");

#ifdef __linux__
    const std::string serialPort{"ttyUSB0"};
    const std::string serialPortFileName{std::string(SERIAL_PORT_PREFIX) + serialPort};
#elif defined(_WIN32) || defined(_WIN64)
    const std::string serialPort{"COM1"};
    const std::string serialPortFileName{std::string(SERIAL_PORT_PREFIX) + serialPort};
#endif // __linux__
    ASSERT_EQ(Enumerator::fileNameToSerialPort(serialPortFileName), serialPort);
    ASSERT_EQ(Enumerator::fileNameToSerialPort(serialPort), serialPort);
}

END_NAMESPACE_LIBSERIAL
