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

#include <chrono>
#include <cmath>
#include <ios>
#include <iostream>
#include <limits>
#include <thread>
#include <stdexcept>
#include <vector>
#include <gtest/gtest.h>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport/serialport.hpp>
#include <serialport_test/test_serialport_impl.hpp>

BEGIN_NAMESPACE_LIBSERIAL

std::string SerialPortTest::getFirstSerialPort()
{
#ifdef __linux__
    return "/dev/ttyUSB0";
#elif defined(_WIN32) || defined(_WIN64)
    return "COM1";
#endif // __linux__
}

std::string SerialPortTest::getSecondSerialPort()
{
#ifdef __linux__
    return "/dev/ttyUSB1";
#elif defined(_WIN32) || defined(_WIN64)
    return "COM2";
#endif // __linux__
}

std::string SerialPortTest::getNonExistingSerialPort()
{
#ifdef __linux__
    return "/dev/ttyUSB-";
#elif defined(_WIN32) || defined(_WIN64)
    return "COM-";
#endif // __linux__
}

std::string SerialPortTest::getFirstData(CharacterSize characterSize)
{
    // Reserve space for data
    std::string result{};
    result.reserve(static_cast<unsigned short>(std::numeric_limits<unsigned char>::max()) + 1);

    // Prepare and return data
    for (unsigned short index{std::numeric_limits<unsigned char>::min()}; index < (std::numeric_limits<unsigned char>::max() + 1); ++index)
    {
        const auto value{static_cast<unsigned char>((index ^ 0xFF) ^ 0x5A)};
        switch (characterSize) {
            case CharacterSize::CHARACTER_SIZE_5:
                result += (value & 0x1F);
                break;

            case CharacterSize::CHARACTER_SIZE_6:
                result += (value & 0x3F);
                break;

            case CharacterSize::CHARACTER_SIZE_7:
                result += (value & 0x7F);
                break;

            case CharacterSize::CHARACTER_SIZE_8:
            default:
                result += value;
                break;
        }
    }
    return result;
}

std::string SerialPortTest::getSecondData(CharacterSize characterSize)
{
    // Reserve space for data
    std::string result{};
    result.reserve(static_cast<unsigned short>(std::numeric_limits<unsigned char>::max()) + 1);

    // Prepare and return data
    for (unsigned short index{std::numeric_limits<unsigned char>::min()}; index < (std::numeric_limits<unsigned char>::max() + 1); ++index)
    {
        const auto value{static_cast<unsigned char>((index ^ 0xFF) ^ 0xA5)};
        switch (characterSize) {
            case CharacterSize::CHARACTER_SIZE_5:
                result += (value & 0x1F);
                break;

            case CharacterSize::CHARACTER_SIZE_6:
                result += (value & 0x3F);
                break;

            case CharacterSize::CHARACTER_SIZE_7:
                result += (value & 0x7F);
                break;

            case CharacterSize::CHARACTER_SIZE_8:
            default:
                result += value;
                break;
        }
    }
    return result;
}

void SerialPortTest::SetUp()
{
    Test::SetUp();
    firstPort = std::make_unique<SerialPort>(getFirstSerialPort());
    secondPort = std::make_unique<SerialPort>(getSecondSerialPort());
}

void SerialPortTest::TearDown()
{
    Test::TearDown();
    firstPort.reset();
    secondPort.reset();
}

void SerialPortTest::performConstructorTests()
{
    SCOPED_TRACE(__func__);

    SerialPort port1{};
    ASSERT_FALSE(port1.isOpen());
    ASSERT_EQ(port1.getPortName(), DEFAULT_PORT_NAME);
    ASSERT_EQ(port1.getBaudRate(), BaudRate::BAUD_RATE_DEFAULT);
    ASSERT_EQ(port1.getCharacterSize(), CharacterSize::CHARACTER_SIZE_DEFAULT);
    ASSERT_EQ(port1.getFlowControl(), FlowControl::FLOW_CONTROL_DEFAULT);
    ASSERT_EQ(port1.getParity(), Parity::PARITY_TYPE_DEFAULT);
    ASSERT_EQ(port1.getStopBit(), StopBit::STOP_BIT_DEFAULT);

    SerialPort port2{getFirstSerialPort(), BaudRate::BAUD_RATE_9600,
        CharacterSize::CHARACTER_SIZE_7, FlowControl::FLOW_CONTROL_HARDWARE,
        Parity::PARITY_TYPE_EVEN, StopBit::STOP_BIT_TWO};
    ASSERT_FALSE(port2.isOpen());
    ASSERT_EQ(port2.getPortName(), getFirstSerialPort());
    ASSERT_EQ(port2.getBaudRate(), BaudRate::BAUD_RATE_9600);
    ASSERT_EQ(port2.getCharacterSize(), CharacterSize::CHARACTER_SIZE_7);
    ASSERT_EQ(port2.getFlowControl(), FlowControl::FLOW_CONTROL_HARDWARE);
    ASSERT_EQ(port2.getParity(), Parity::PARITY_TYPE_EVEN);
    ASSERT_EQ(port2.getStopBit(), StopBit::STOP_BIT_TWO);
}

void SerialPortTest::performFirstPortOpenTest()
{
    SCOPED_TRACE(__func__);

    ASSERT_NO_THROW(firstPort->open()) << "Serial port " << firstPort->getPortName() << " is not available.";
    ASSERT_TRUE(firstPort->isOpen());
    ASSERT_NO_THROW(firstPort->close());
}

void SerialPortTest::performSecondPortOpenTest()
{
    SCOPED_TRACE(__func__);

    ASSERT_NO_THROW(secondPort->open()) << "Serial port " << secondPort->getPortName() << " is not available.";
    ASSERT_TRUE(secondPort->isOpen());
    ASSERT_NO_THROW(secondPort->close());
}

void SerialPortTest::performOpenCloseTests()
{
    SCOPED_TRACE(__func__);

    // First port
    ASSERT_FALSE(firstPort->isOpen());
    ASSERT_NO_THROW(firstPort->close());
    ASSERT_FALSE(firstPort->isOpen());

    ASSERT_NO_THROW(firstPort->open());
    ASSERT_TRUE(firstPort->isOpen());
    ASSERT_NO_THROW(firstPort->open());
    ASSERT_TRUE(firstPort->isOpen());

    ASSERT_NO_THROW(firstPort->close());
    ASSERT_FALSE(firstPort->isOpen());

    // Second port
    ASSERT_FALSE(secondPort->isOpen());
    ASSERT_NO_THROW(secondPort->close());
    ASSERT_FALSE(secondPort->isOpen());

    ASSERT_NO_THROW(secondPort->open());
    ASSERT_TRUE(secondPort->isOpen());
    ASSERT_NO_THROW(secondPort->open());
    ASSERT_TRUE(secondPort->isOpen());

    ASSERT_NO_THROW(secondPort->close());
    ASSERT_FALSE(secondPort->isOpen());

    // Non-existing port
    SerialPort port{getNonExistingSerialPort()};
    ASSERT_FALSE(port.isOpen());
    ASSERT_NO_THROW(port.close());
    ASSERT_FALSE(port.isOpen());

    ASSERT_THROW(port.open(), std::runtime_error);
    ASSERT_FALSE(port.isOpen());
    ASSERT_THROW(port.open(), std::runtime_error);
    ASSERT_FALSE(port.isOpen());

    ASSERT_NO_THROW(port.close());
    ASSERT_FALSE(port.isOpen());
}

void SerialPortTest::performOpenModeTests()
{
    SCOPED_TRACE(__func__);

    // First port
    ASSERT_FALSE(firstPort->isOpen());
    ASSERT_NO_THROW(firstPort->open(std::ios_base::in));
    ASSERT_TRUE(firstPort->isOpen());
    ASSERT_NO_THROW(firstPort->close());

    ASSERT_FALSE(firstPort->isOpen());
    ASSERT_NO_THROW(firstPort->open(std::ios_base::out));
    ASSERT_TRUE(firstPort->isOpen());
    ASSERT_NO_THROW(firstPort->close());

    ASSERT_FALSE(firstPort->isOpen());
    ASSERT_THROW(firstPort->open(std::ios_base::in | std::ios_base::ate), std::runtime_error);
    ASSERT_FALSE(firstPort->isOpen());

    // Second port
    ASSERT_FALSE(secondPort->isOpen());
    ASSERT_NO_THROW(secondPort->open(std::ios_base::in));
    ASSERT_TRUE(secondPort->isOpen());
    ASSERT_NO_THROW(secondPort->close());

    ASSERT_FALSE(secondPort->isOpen());
    ASSERT_NO_THROW(secondPort->open(std::ios_base::out));
    ASSERT_TRUE(secondPort->isOpen());
    ASSERT_NO_THROW(secondPort->close());

    ASSERT_FALSE(secondPort->isOpen());
    ASSERT_THROW(secondPort->open(std::ios_base::out | std::ios_base::app), std::runtime_error);
    ASSERT_FALSE(secondPort->isOpen());
}

void SerialPortTest::performClosedPortFunctionTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_FALSE(firstPort->isOpen());
    ASSERT_FALSE(secondPort->isOpen());

    char data1{};
    ASSERT_EQ(firstPort->read(&data1, 1), 0);
    ASSERT_EQ(secondPort->read(&data1, 1), 0);

    std::string data2{'A'};
    ASSERT_EQ(firstPort->read(data2), 0);
    ASSERT_EQ(secondPort->read(data2), 0);

    ASSERT_FALSE(firstPort->write(data1));
    ASSERT_FALSE(secondPort->write(data1));

    ASSERT_EQ(firstPort->write(&data1, 1), 0);
    ASSERT_EQ(secondPort->write(&data1, 1), 0);

    ASSERT_EQ(firstPort->write(data2), 0);
    ASSERT_EQ(secondPort->write(data2), 0);

    ASSERT_FALSE(firstPort->drain());
    ASSERT_FALSE(secondPort->drain());

    ASSERT_FALSE(firstPort->flushInput());
    ASSERT_FALSE(secondPort->flushInput());

    ASSERT_FALSE(firstPort->flushOutput());
    ASSERT_FALSE(secondPort->flushOutput());

    ASSERT_FALSE(firstPort->flushInputOutput());
    ASSERT_FALSE(secondPort->flushInputOutput());

    ASSERT_EQ(firstPort->getInputQueueCount(), 0);
    ASSERT_EQ(secondPort->getInputQueueCount(), 0);

    ASSERT_EQ(firstPort->getOutputQueueCount(), 0);
    ASSERT_EQ(secondPort->getOutputQueueCount(), 0);

    ASSERT_FALSE(firstPort->getControlLine(ControlLine::LINE_DCD));
    ASSERT_FALSE(secondPort->getControlLine(ControlLine::LINE_DCD));

    ASSERT_FALSE(firstPort->setControlLine(ControlLine::LINE_DTR, true));
    ASSERT_FALSE(secondPort->setControlLine(ControlLine::LINE_DTR, true));
}

void SerialPortTest::performExclusiveModeTests()
{
    SCOPED_TRACE(__func__);

#ifdef __linux__
    // First port
    ASSERT_FALSE(firstPort->isOpen());
    ASSERT_FALSE(firstPort->setExclusive(true));
    ASSERT_FALSE(firstPort->setExclusive(false));

    ASSERT_NO_THROW(firstPort->open());
    for (const bool& exclusive : {false, true})
    {
        ASSERT_TRUE(firstPort->setExclusive(exclusive));

        SerialPort exclusivePort{firstPort->getPortName()};
        if (exclusive)
            ASSERT_THROW(exclusivePort.open(), std::runtime_error);
        else
            ASSERT_NO_THROW(exclusivePort.open());
    }
    ASSERT_NO_THROW(firstPort->close());

    // Second port
    ASSERT_FALSE(secondPort->isOpen());
    ASSERT_FALSE(secondPort->setExclusive(true));
    ASSERT_FALSE(secondPort->setExclusive(false));

    ASSERT_NO_THROW(secondPort->open());

    ASSERT_NO_THROW(secondPort->open());
    for (const bool& exclusive : {false, true})
    {
        ASSERT_TRUE(secondPort->setExclusive(exclusive));

        SerialPort exclusivePort{secondPort->getPortName()};
        if (exclusive)
            ASSERT_THROW(exclusivePort.open(), std::runtime_error);
        else
            ASSERT_NO_THROW(exclusivePort.open());
    }
    ASSERT_NO_THROW(secondPort->close());
#elif defined(_WIN32) || defined(_WIN64)
    // First port
    ASSERT_FALSE(firstPort->isOpen());
    ASSERT_FALSE(firstPort->setExclusive(true));
    ASSERT_FALSE(firstPort->setExclusive(false));

    ASSERT_NO_THROW(firstPort->open());

    ASSERT_TRUE(firstPort->setExclusive(true));
    ASSERT_FALSE(firstPort->setExclusive(false));

    ASSERT_NO_THROW(firstPort->close());

    // Second port
    ASSERT_FALSE(secondPort->isOpen());
    ASSERT_FALSE(secondPort->setExclusive(true));
    ASSERT_FALSE(secondPort->setExclusive(false));

    ASSERT_NO_THROW(secondPort->open());

    ASSERT_TRUE(secondPort->setExclusive(true));
    ASSERT_FALSE(secondPort->setExclusive(false));

    ASSERT_NO_THROW(secondPort->close());
#endif // __linux__
}

void SerialPortTest::performPropertiesTests()
{
    SCOPED_TRACE(__func__);

    SerialPort port{};
    port.setPortName(getFirstSerialPort());
    ASSERT_EQ(port.getPortName(), getFirstSerialPort());

    ASSERT_THROW(port.setBaudRate(static_cast<BaudRate>(static_cast<unsigned int>(BaudRate::BAUD_RATE_MAX) + 1)), std::out_of_range);
    ASSERT_NO_THROW(port.setBaudRate(BaudRate::BAUD_RATE_9600));
    ASSERT_EQ(port.getBaudRate(), BaudRate::BAUD_RATE_9600);

    ASSERT_THROW(port.setCharacterSize(static_cast<CharacterSize>(static_cast<unsigned int>(CharacterSize::CHARACTER_SIZE_MAX) + 1)), std::out_of_range);
    ASSERT_NO_THROW(port.setCharacterSize(CharacterSize::CHARACTER_SIZE_6));
    ASSERT_EQ(port.getCharacterSize(), CharacterSize::CHARACTER_SIZE_6);

    ASSERT_THROW(port.setFlowControl(static_cast<FlowControl>(static_cast<unsigned int>(FlowControl::FLOW_CONTROL_MAX) + 1)), std::out_of_range);
    ASSERT_NO_THROW(port.setFlowControl(FlowControl::FLOW_CONTROL_HARDWARE));
    ASSERT_EQ(port.getFlowControl(), FlowControl::FLOW_CONTROL_HARDWARE);

    ASSERT_THROW(port.setParity(static_cast<Parity>(static_cast<unsigned int>(Parity::PARITY_TYPE_MAX) + 1)), std::out_of_range);
    ASSERT_NO_THROW(port.setParity(Parity::PARITY_TYPE_EVEN));
    ASSERT_EQ(port.getParity(), Parity::PARITY_TYPE_EVEN);

    ASSERT_THROW(port.setStopBit(static_cast<StopBit>(static_cast<unsigned int>(StopBit::STOP_BIT_MAX) + 1)), std::out_of_range);
    ASSERT_NO_THROW(port.setStopBit(StopBit::STOP_BIT_TWO));
    ASSERT_EQ(port.getStopBit(), StopBit::STOP_BIT_TWO);

    ASSERT_NO_THROW(port.open());

    port.setPortName(getSecondSerialPort());
    ASSERT_EQ(port.getPortName(), getSecondSerialPort());
    ASSERT_TRUE(port.isOpen());

    ASSERT_NO_THROW(port.setBaudRate(BaudRate::BAUD_RATE_38400));
    ASSERT_EQ(port.getBaudRate(), BaudRate::BAUD_RATE_38400);

    ASSERT_NO_THROW(port.setCharacterSize(CharacterSize::CHARACTER_SIZE_7));
    ASSERT_EQ(port.getCharacterSize(), CharacterSize::CHARACTER_SIZE_7);

    ASSERT_NO_THROW(port.setFlowControl(FlowControl::FLOW_CONTROL_SOFTWARE));
    ASSERT_EQ(port.getFlowControl(), FlowControl::FLOW_CONTROL_SOFTWARE);

    ASSERT_NO_THROW(port.setParity(Parity::PARITY_TYPE_ODD));
    ASSERT_EQ(port.getParity(), Parity::PARITY_TYPE_ODD);
    ASSERT_NO_THROW(port.setParity(Parity::PARITY_TYPE_MARK));
    ASSERT_EQ(port.getParity(), Parity::PARITY_TYPE_MARK);

    ASSERT_NO_THROW(port.setStopBit(StopBit::STOP_BIT_ONE));
    ASSERT_EQ(port.getStopBit(), StopBit::STOP_BIT_ONE);

    ASSERT_NO_THROW(port.close());
}

void SerialPortTest::performSimpleReadWriteTests()
{
    SCOPED_TRACE(__func__);

    std::string firstData{"The quick brown fox jumps over a lazy dog."};
    std::string secondData{"Pack my box with five dozen liquor jugs."};

    // Setup port defaults
    ASSERT_NO_THROW(firstPort->setBaudRate(BaudRate::BAUD_RATE_DEFAULT));
    ASSERT_NO_THROW(firstPort->setCharacterSize(CharacterSize::CHARACTER_SIZE_DEFAULT));
    ASSERT_NO_THROW(firstPort->setFlowControl(FlowControl::FLOW_CONTROL_DEFAULT));
    ASSERT_NO_THROW(firstPort->setParity(Parity::PARITY_TYPE_DEFAULT));
    ASSERT_NO_THROW(firstPort->setStopBit(StopBit::STOP_BIT_DEFAULT));

    ASSERT_NO_THROW(secondPort->setBaudRate(BaudRate::BAUD_RATE_DEFAULT));
    ASSERT_NO_THROW(secondPort->setCharacterSize(CharacterSize::CHARACTER_SIZE_DEFAULT));
    ASSERT_NO_THROW(secondPort->setFlowControl(FlowControl::FLOW_CONTROL_DEFAULT));
    ASSERT_NO_THROW(secondPort->setParity(Parity::PARITY_TYPE_DEFAULT));
    ASSERT_NO_THROW(secondPort->setStopBit(StopBit::STOP_BIT_DEFAULT));

    // Open ports
    ASSERT_NO_THROW(firstPort->open());
    ASSERT_NO_THROW(secondPort->open());

    ASSERT_TRUE(firstPort->flushOutput());
    ASSERT_TRUE(secondPort->flushOutput());
    ASSERT_TRUE(firstPort->flushInput());
    ASSERT_TRUE(secondPort->flushInput());

    // Simple char read/write test
    char firstChar{static_cast<char>(0x5A)}, secondChar{static_cast<char>(0xA5)};
    ASSERT_EQ(firstPort->write(firstChar), 1);
    ASSERT_EQ(secondPort->write(secondChar), 1);

    // Sleep for some delay
    std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1000>>(staticDelay));

    char readChar{};
    ASSERT_EQ(firstPort->read(&readChar, 1), 1);
    ASSERT_EQ(readChar, secondChar);
    ASSERT_EQ(secondPort->read(&readChar, 1), 1);
    ASSERT_EQ(readChar, firstChar);

    // Simple read/write string test
    ASSERT_EQ(firstPort->write(firstData), firstData.size());
    ASSERT_EQ(secondPort->write(secondData), secondData.size());

    // Calculate delay according to data sizes and settings
    const auto delay{(std::max(firstData.size(), secondData.size()) * LibSerial::calculateTime(BaudRate::BAUD_RATE_DEFAULT)) + staticDelay};

    // Sleep for a calculated delay
    std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1000>>(delay));

    std::string data{};
    ASSERT_EQ(firstPort->read(data), secondData.size());
    ASSERT_EQ(data, secondData);
    ASSERT_EQ(secondPort->read(data), firstData.size());
    ASSERT_EQ(data, firstData);

    // Close ports
    ASSERT_NO_THROW(firstPort->close());
    ASSERT_NO_THROW(secondPort->close());
}

void SerialPortTest::performExtendedReadWriteStringTests()
{
    SCOPED_TRACE(__func__);

    // Open ports
    ASSERT_NO_THROW(firstPort->open());
    ASSERT_NO_THROW(secondPort->open());

    // Baud rates to test
    const std::vector<BaudRate> baudRates{
        BaudRate::BAUD_RATE_1200,
        BaudRate::BAUD_RATE_4800,
        BaudRate::BAUD_RATE_9600,
        BaudRate::BAUD_RATE_19200,
        BaudRate::BAUD_RATE_38400,
        BaudRate::BAUD_RATE_57600,
        BaudRate::BAUD_RATE_115200
    };

    // Character sizes to test
    const std::vector<CharacterSize> characterSizes{
        CharacterSize::CHARACTER_SIZE_5,
        CharacterSize::CHARACTER_SIZE_6,
        CharacterSize::CHARACTER_SIZE_7,
        CharacterSize::CHARACTER_SIZE_8
    };

    // Parities to test
    const std::vector<Parity> parities{
        Parity::PARITY_TYPE_NONE,
        Parity::PARITY_TYPE_ODD,
        Parity::PARITY_TYPE_EVEN
    };

    // Stop bits to test
    const std::vector<StopBit> stopBits{
        StopBit::STOP_BIT_ONE,
        StopBit::STOP_BIT_TWO
    };

    // Prepare data
    std::string firstData{getFirstData(CharacterSize::CHARACTER_SIZE_8)};
    std::string secondData{getSecondData(CharacterSize::CHARACTER_SIZE_8)};
    size_t firstDataSize{firstData.size()};
    size_t secondDataSize{secondData.size()};

    // Loop through all options
    for (const auto& baudRate : baudRates)
        for (const auto& characterSize : characterSizes)
            for (const auto& parity : parities)
                for (const auto& stopBit : stopBits)
    {
#if defined(_WIN32) || defined(_W64)
        /*  When a DCB structure is used to configure the 8250, the following restrictions
            apply to the values specified for the ByteSize and StopBits members:
            - The number of data bits must be 5 to 8 bits.
            - The use of 5 data bits with 2 stop bits is an invalid combination,
              as is 6, 7, or 8 data bits with 1.5 stop bits.

            https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-dcb#remarks  */

        if ((characterSize == CharacterSize::CHARACTER_SIZE_5) && (stopBit == StopBit::STOP_BIT_TWO))
            continue;

        if ((firstPort->getStopBit() == StopBit::STOP_BIT_TWO) && (characterSize == CharacterSize::CHARACTER_SIZE_5))
        {
            ASSERT_NO_THROW(firstPort->setStopBit(StopBit::STOP_BIT_ONE));
            ASSERT_NO_THROW(secondPort->setStopBit(StopBit::STOP_BIT_ONE));
        }
#endif // _WIN32 || _WIN64

        // Flush active received/transmit data
        ASSERT_TRUE(firstPort->flushInputOutput());
        ASSERT_TRUE(secondPort->flushInputOutput());

        // Set up first port
        ASSERT_NO_THROW(firstPort->setBaudRate(baudRate));
        ASSERT_NO_THROW(firstPort->setCharacterSize(characterSize));
        ASSERT_NO_THROW(firstPort->setParity(parity));
        ASSERT_NO_THROW(firstPort->setStopBit(stopBit));

        // Set up second port
        ASSERT_NO_THROW(secondPort->setBaudRate(baudRate));
        ASSERT_NO_THROW(secondPort->setCharacterSize(characterSize));
        ASSERT_NO_THROW(secondPort->setParity(parity));
        ASSERT_NO_THROW(secondPort->setStopBit(stopBit));

        // Write data
        ASSERT_EQ(firstPort->write(firstData), firstDataSize);
        ASSERT_EQ(secondPort->write(secondData), secondDataSize);

        EXPECT_GE(firstPort->getOutputQueueCount(), 0);
        EXPECT_GE(secondPort->getOutputQueueCount(), 0);

#ifdef __linux__
        ASSERT_TRUE(firstPort->drain());
        ASSERT_TRUE(secondPort->drain());
#endif // __linux__

        // Calculate delay according to data sizes and settings
        const auto delay{(std::max(firstDataSize, secondDataSize) * LibSerial::calculateTime(baudRate,
            characterSize, parity, stopBit)) + staticDelay};

        // Print debug information about a test
        std::cerr << "Baud rate = " << LibSerial::getBaudRate(baudRate) << " Bd"
            << ", Character size = " << static_cast<unsigned short>(LibSerial::getCharacterSize(characterSize)) << " b"
            << ", Parity = " << getParityName(parity)
            << ", Stop bit = " << getStopBitName(stopBit)
            << ", Data size = " << std::max(firstDataSize, secondDataSize)
            << ", Delay = " << delay << " ms" << std::endl;

        // Sleep for a calculated delay
        std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1000>>(delay));

        // Check for receive and transmit queue counts
        ASSERT_EQ(firstPort->getOutputQueueCount(), 0);
        ASSERT_EQ(secondPort->getOutputQueueCount(), 0);
        ASSERT_EQ(firstPort->getInputQueueCount(), secondDataSize);
        ASSERT_EQ(secondPort->getInputQueueCount(), firstDataSize);

        // Check received data length and data
        std::string data{};
        ASSERT_EQ(firstPort->read(data), secondDataSize);
        ASSERT_EQ(data, getSecondData(characterSize));
        ASSERT_EQ(secondPort->read(data), firstDataSize);
        ASSERT_EQ(data, getFirstData(characterSize));

        // Check receive queue counts
        ASSERT_EQ(firstPort->getInputQueueCount(), 0);
        ASSERT_EQ(secondPort->getInputQueueCount(), 0);
    }

    // Close ports
    ASSERT_NO_THROW(firstPort->close());
    ASSERT_NO_THROW(secondPort->close());
}

void SerialPortTest::performExtendedReadWriteCharArrayTests()
{
    SCOPED_TRACE(__func__);

    // Open ports
    ASSERT_NO_THROW(firstPort->open());
    ASSERT_NO_THROW(secondPort->open());

    // Baud rates to test
    const std::vector<BaudRate> baudRates{
        BaudRate::BAUD_RATE_1200,
        BaudRate::BAUD_RATE_4800,
        BaudRate::BAUD_RATE_9600,
        BaudRate::BAUD_RATE_19200,
        BaudRate::BAUD_RATE_38400,
        BaudRate::BAUD_RATE_57600,
        BaudRate::BAUD_RATE_115200
    };

    // Character sizes to test
    const std::vector<CharacterSize> characterSizes{
        CharacterSize::CHARACTER_SIZE_5,
        CharacterSize::CHARACTER_SIZE_6,
        CharacterSize::CHARACTER_SIZE_7,
        CharacterSize::CHARACTER_SIZE_8
    };

    // Parities to test
    const std::vector<Parity> parities{
        Parity::PARITY_TYPE_NONE,
        Parity::PARITY_TYPE_ODD,
        Parity::PARITY_TYPE_EVEN
    };

    // Stop bits to test
    const std::vector<StopBit> stopBits{
        StopBit::STOP_BIT_ONE,
        StopBit::STOP_BIT_TWO
    };

    // Prepare data
    std::string firstData{getFirstData(CharacterSize::CHARACTER_SIZE_8)};
    std::string secondData{getSecondData(CharacterSize::CHARACTER_SIZE_8)};
    size_t firstDataSize{firstData.size()};
    size_t secondDataSize{secondData.size()};

    // Loop through all options
    for (const auto& baudRate : baudRates)
        for (const auto& characterSize : characterSizes)
            for (const auto& parity : parities)
                for (const auto& stopBit : stopBits)
    {
#if defined(_WIN32) || defined(_W64)
        /*  When a DCB structure is used to configure the 8250, the following restrictions
            apply to the values specified for the ByteSize and StopBits members:
            - The number of data bits must be 5 to 8 bits.
            - The use of 5 data bits with 2 stop bits is an invalid combination,
              as is 6, 7, or 8 data bits with 1.5 stop bits.

            https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-dcb#remarks  */

        if ((characterSize == CharacterSize::CHARACTER_SIZE_5) && (stopBit == StopBit::STOP_BIT_TWO))
            continue;

        if ((firstPort->getStopBit() == StopBit::STOP_BIT_TWO) && (characterSize == CharacterSize::CHARACTER_SIZE_5))
        {
            ASSERT_NO_THROW(firstPort->setStopBit(StopBit::STOP_BIT_ONE));
            ASSERT_NO_THROW(secondPort->setStopBit(StopBit::STOP_BIT_ONE));
        }
#endif // _WIN32 || _WIN64

        // Flush active received/transmit data
        ASSERT_TRUE(firstPort->flushInputOutput());
        ASSERT_TRUE(secondPort->flushInputOutput());

        // Set up first port
        ASSERT_NO_THROW(firstPort->setBaudRate(baudRate));
        ASSERT_NO_THROW(firstPort->setCharacterSize(characterSize));
        ASSERT_NO_THROW(firstPort->setParity(parity));
        ASSERT_NO_THROW(firstPort->setStopBit(stopBit));

        // Set up second port
        ASSERT_NO_THROW(secondPort->setBaudRate(baudRate));
        ASSERT_NO_THROW(secondPort->setCharacterSize(characterSize));
        ASSERT_NO_THROW(secondPort->setParity(parity));
        ASSERT_NO_THROW(secondPort->setStopBit(stopBit));

        // Write data
        ASSERT_EQ(firstPort->write(firstData.c_str(), firstDataSize), firstDataSize);
        ASSERT_EQ(secondPort->write(secondData.c_str(), secondDataSize), secondDataSize);

        EXPECT_GE(firstPort->getOutputQueueCount(), 0);
        EXPECT_GE(secondPort->getOutputQueueCount(), 0);

#ifdef __linux__
        ASSERT_TRUE(firstPort->drain());
        ASSERT_TRUE(secondPort->drain());
#endif // __linux__

        // Calculate delay according to data sizes and settings
        const auto delay{(std::max(firstDataSize, secondDataSize) * LibSerial::calculateTime(baudRate,
            characterSize, parity, stopBit)) + staticDelay};

        // Print debug information about a test
        std::cerr << "Baud rate = " << LibSerial::getBaudRate(baudRate) << " Bd"
            << ", Character size = " << static_cast<unsigned short>(LibSerial::getCharacterSize(characterSize)) << " b"
            << ", Parity = " << getParityName(parity)
            << ", Stop bit = " << getStopBitName(stopBit)
            << ", Data size = " << std::max(firstDataSize, secondDataSize)
            << ", Delay = " << delay << " ms" << std::endl;

        // Sleep for a calculated delay
        std::this_thread::sleep_for(std::chrono::duration<double, std::ratio<1, 1000>>(delay));

        // Check for receive and transmit queue counts
        ASSERT_EQ(firstPort->getOutputQueueCount(), 0);
        ASSERT_EQ(secondPort->getOutputQueueCount(), 0);
        ASSERT_EQ(firstPort->getInputQueueCount(), secondDataSize);
        ASSERT_EQ(secondPort->getInputQueueCount(), firstDataSize);

        // Check received data length and data
        char* data2 = new char[secondDataSize];
        EXPECT_EQ(firstPort->read(data2, secondDataSize), secondDataSize);
        ASSERT_STREQ(data2, getSecondData(characterSize).c_str());
        delete [] data2;

        char* data1 = new char[firstDataSize];
        EXPECT_EQ(secondPort->read(data1, firstDataSize), firstDataSize);
        ASSERT_STREQ(data1, getFirstData(characterSize).c_str());
        delete [] data1;

        // Check receive queue counts
        ASSERT_EQ(firstPort->getInputQueueCount(), 0);
        ASSERT_EQ(secondPort->getInputQueueCount(), 0);
    }

    // Close ports
    ASSERT_NO_THROW(firstPort->close());
    ASSERT_NO_THROW(secondPort->close());
}

void SerialPortTest::performControlLineTests()
{
    SCOPED_TRACE(__func__);

    ASSERT_NO_THROW(firstPort->open());
    ASSERT_NO_THROW(secondPort->open());

    performControlLineTest(*firstPort, *secondPort);
    performControlLineTest(*secondPort, *firstPort);

    ASSERT_NO_THROW(firstPort->close());
    ASSERT_NO_THROW(secondPort->close());
}

void SerialPortTest::performControlLineTest(SerialPort& portA, SerialPort& portB)
{
    SCOPED_TRACE(__func__);
    SCOPED_TRACE("Hint: Check if you are using full null modem cable.");
    /*
        DB9 full null modem cable pinout:
        Left end (DB9)        Right end (DB9)
        DCD (1) + DSR (6) <-> DTR (4)
        RXD (2)           <-> TXD (3)
        TXD (3)           <-> RXD (2)
        DTR (4)           <-> DSR (6) + DCD (1)
        GND (5)           <-> GND (5)
        RTS (7)           <-> CTS (8)
        CTS (8)           <-> RTS (7)
        RI (9)            NC  RI (9)
    */

    for (const auto& lineState : {false, true})
    {
        portA.setControlLine(ControlLine::LINE_DTR, lineState);
        portA.setControlLine(ControlLine::LINE_RTS, false);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_DCD), lineState);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_DSR), lineState);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_DCD | ControlLine::LINE_DSR), lineState);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_CTS), false);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_RI), false);
    };

    for (const auto& lineState : {false, true})
    {
        portA.setControlLine(ControlLine::LINE_DTR, false);
        portA.setControlLine(ControlLine::LINE_RTS, lineState);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_DCD), false);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_DSR), false);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_DCD | ControlLine::LINE_DSR), false);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_CTS), lineState);
        EXPECT_EQ(portB.getControlLine(ControlLine::LINE_RI), false);
    };
}

END_NAMESPACE_LIBSERIAL
