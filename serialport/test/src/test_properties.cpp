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

#include <stdexcept>
#include <gtest/gtest.h>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport_test/test_properties.hpp>

#ifdef __linux__
    #include <termios.h>
#elif defined(_WIN32) || defined(_WIN32)
    #include <winbase.h>
#endif // __linux__

BEGIN_NAMESPACE_LIBSERIAL

TEST(PropertiesTest, IsBaudRateSupportedFunctionTest)
{
    SCOPED_TRACE("IsBaudRateSupportedFunctionTest");

    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_CUSTOM));
#ifdef __linux__
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_50));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_75));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_110));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_134));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_150));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_200));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_300));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_600));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_1200));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_1800));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_2400));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_4800));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_9600));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_14400));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_19200));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_38400));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_56000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_57600));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_115200));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_128000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_230400));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_256000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_460800));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_500000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_576000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_921600));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_1000000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_1152000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_1500000));

#if __MAX_BAUD > B2000000
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_2000000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_2500000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_3000000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_3500000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_4000000));
#else
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_2000000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_2500000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_3000000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_3500000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_4000000));
#endif // __MAX_BAUD

#elif defined(_WIN32) || defined(_WIN64)
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_50));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_75));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_110));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_134));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_150));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_200));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_300));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_600));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_1200));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_1800));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_2400));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_4800));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_9600));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_14400));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_19200));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_38400));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_56000));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_57600));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_115200));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_128000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_230400));
    ASSERT_TRUE(isBaudRateSupported(BaudRate::BAUD_RATE_256000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_460800));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_500000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_576000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_921600));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_1000000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_1152000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_1500000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_2000000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_2500000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_3000000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_3500000));
    ASSERT_FALSE(isBaudRateSupported(BaudRate::BAUD_RATE_4000000));
#endif // __linux__
}

TEST(PropertiesTest, GetBaudRateValueFunctionTest)
{
    SCOPED_TRACE("GetBaudRateValueFunctionTest");

    ASSERT_THROW(getBaudRateValue(BaudRate::BAUD_RATE_CUSTOM), std::out_of_range);
#ifdef __linux__
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_50), B50);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_75), B75);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_110), B110);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_134), B134);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_150), B150);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_200), B200);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_300), B300);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_600), B600);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_1200), B1200);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_1800), B1800);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_2400), B2400);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_4800), B4800);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_9600), B9600);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_19200), B19200);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_38400), B38400);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_57600), B57600);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_115200), B115200);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_230400), B230400);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_460800), B460800);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_500000), B500000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_576000), B576000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_921600), B921600);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_1000000), B1000000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_1152000), B1152000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_1500000), B1500000);

#if __MAX_BAUD > B2000000
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_2000000), B2000000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_2500000), B2500000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_3000000), B3000000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_3500000), B3500000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_4000000), B4000000);
#endif // __MAX_BAUD

#elif defined(_WIN32) || defined(_WIN64)
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_110), CBR_110);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_300), CBR_300);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_600), CBR_600);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_1200), CBR_1200);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_2400), CBR_2400);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_4800), CBR_4800);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_9600), CBR_9600);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_14400), CBR_14400);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_19200), CBR_19200);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_38400), CBR_38400);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_56000), CBR_56000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_57600), CBR_57600);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_115200), CBR_115200);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_128000), CBR_128000);
    ASSERT_EQ(getBaudRateValue(BaudRate::BAUD_RATE_256000), CBR_256000);
#endif // _WIN32 | _WIN64
}

TEST(PropertiesTest, GetBaudRateFunctionTest)
{
    SCOPED_TRACE("GetBaudRateFunctionTest");

    ASSERT_THROW(getBaudRate(BaudRate::BAUD_RATE_CUSTOM), std::out_of_range);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_50), 50U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_75), 75U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_110), 110U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_134), 134U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_150), 150U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_200), 200U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_300), 300U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_600), 600U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_1200), 1200U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_1800), 1800U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_2400), 2400U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_4800), 4800U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_9600), 9600U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_14400), 14400U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_19200), 19200U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_38400), 38400U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_56000), 56000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_57600), 57600U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_115200), 115200U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_128000), 128000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_230400), 230400U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_256000), 256000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_460800), 460800U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_500000), 500000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_576000), 576000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_921600), 921600U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_1000000), 1000000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_1152000), 1152000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_1500000), 1500000U);

#if __MAX_BAUD > B2000000
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_2000000), 2000000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_2500000), 2500000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_3000000), 3000000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_3500000), 3500000U);
    ASSERT_EQ(getBaudRate(BaudRate::BAUD_RATE_4000000), 4000000U);
#endif // __MAX_BAUD
}

TEST(PropertiesTest, IsCharacterSizeSupportedFunctionTest)
{
    SCOPED_TRACE("IsCharacterSizeSupportedFunctionTest");

    ASSERT_FALSE(isCharacterSizeSupported(static_cast<CharacterSize>(static_cast<unsigned char>(CharacterSize::CHARACTER_SIZE_MAX) + 1)));
    ASSERT_TRUE(isCharacterSizeSupported(CharacterSize::CHARACTER_SIZE_5));
    ASSERT_TRUE(isCharacterSizeSupported(CharacterSize::CHARACTER_SIZE_6));
    ASSERT_TRUE(isCharacterSizeSupported(CharacterSize::CHARACTER_SIZE_7));
    ASSERT_TRUE(isCharacterSizeSupported(CharacterSize::CHARACTER_SIZE_8));
}

TEST(PropertiesTest, GetCharacterSizeValueFunctionTest)
{
    SCOPED_TRACE("GetCharacterSizeValueFunctionTest");

    ASSERT_THROW(getCharacterSizeValue(static_cast<CharacterSize>(static_cast<unsigned char>(CharacterSize::CHARACTER_SIZE_MAX) + 1)), std::out_of_range);
#ifdef __linux__
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_5), CS5);
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_6), CS6);
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_7), CS7);
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_8), CS8);
#elif defined(_WIN32) || defined(_WIN64)
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_5), 5U);
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_6), 6U);
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_7), 7U);
    ASSERT_EQ(getCharacterSizeValue(CharacterSize::CHARACTER_SIZE_8), 8U);
#endif // _WIN32 || _WIN64
}

TEST(PropertiesTest, GetCharacterSizeFunctionTest)
{
    SCOPED_TRACE("GetCharacterSizeFunctionTest");

    ASSERT_THROW(getCharacterSize(static_cast<CharacterSize>(static_cast<unsigned char>(CharacterSize::CHARACTER_SIZE_MAX) + 1)), std::out_of_range);
    ASSERT_EQ(getCharacterSize(CharacterSize::CHARACTER_SIZE_5), 5U);
    ASSERT_EQ(getCharacterSize(CharacterSize::CHARACTER_SIZE_6), 6U);
    ASSERT_EQ(getCharacterSize(CharacterSize::CHARACTER_SIZE_7), 7U);
    ASSERT_EQ(getCharacterSize(CharacterSize::CHARACTER_SIZE_8), 8U);
}

TEST(PropertiesTest, IsFlowControlSupportedFunctionTest)
{
    SCOPED_TRACE("IsFlowControlSupportedFunctionTest");

    ASSERT_FALSE(isFlowControlSupported(static_cast<FlowControl>(static_cast<unsigned char>(FlowControl::FLOW_CONTROL_MAX) + 1)));
    ASSERT_TRUE(isFlowControlSupported(FlowControl::FLOW_CONTROL_HARDWARE));
    ASSERT_TRUE(isFlowControlSupported(FlowControl::FLOW_CONTROL_SOFTWARE));
    ASSERT_TRUE(isFlowControlSupported(FlowControl::FLOW_CONTROL_NONE));
}

TEST(PropertiesTest, GetFlowControlNameFunctionTest)
{
    SCOPED_TRACE("GetFlowControlNameFunctionTest");

    ASSERT_THROW(getFlowControlName(static_cast<FlowControl>(static_cast<unsigned char>(FlowControl::FLOW_CONTROL_MAX) + 1)), std::out_of_range);
    ASSERT_EQ(getFlowControlName(FlowControl::FLOW_CONTROL_HARDWARE), "Hardware");
    ASSERT_EQ(getFlowControlName(FlowControl::FLOW_CONTROL_SOFTWARE), "Software");
    ASSERT_EQ(getFlowControlName(FlowControl::FLOW_CONTROL_NONE), "None");
}

TEST(PropertiesTest, IsParitySupportedFunctionTest)
{
    SCOPED_TRACE("IsParitySupportedFunctionTest");

    ASSERT_FALSE(isParitySupported(static_cast<Parity>(static_cast<unsigned char>(Parity::PARITY_TYPE_MAX) + 1)));
    ASSERT_TRUE(isParitySupported(Parity::PARITY_TYPE_NONE));
    ASSERT_TRUE(isParitySupported(Parity::PARITY_TYPE_ODD));
    ASSERT_TRUE(isParitySupported(Parity::PARITY_TYPE_EVEN));
    ASSERT_TRUE(isParitySupported(Parity::PARITY_TYPE_MARK));
    ASSERT_TRUE(isParitySupported(Parity::PARITY_TYPE_SPACE));
}

TEST(PropertiesTest, GetParityNameFunctionTest)
{
    SCOPED_TRACE("GetParityNameFunctionTest");

    ASSERT_THROW(getParityName(static_cast<Parity>(static_cast<unsigned char>(Parity::PARITY_TYPE_MAX) + 1)), std::out_of_range);
    ASSERT_EQ(getParityName(Parity::PARITY_TYPE_NONE), "None");
    ASSERT_EQ(getParityName(Parity::PARITY_TYPE_ODD), "Odd");
    ASSERT_EQ(getParityName(Parity::PARITY_TYPE_EVEN), "Even");
    ASSERT_EQ(getParityName(Parity::PARITY_TYPE_MARK), "Mark");
    ASSERT_EQ(getParityName(Parity::PARITY_TYPE_SPACE), "Space");
}

TEST(PropertiesTest, IsStopBitSupportedFunctionTest)
{
    SCOPED_TRACE("IsStopBitSupportedFunctionTest");

    ASSERT_FALSE(isStopBitSupported(static_cast<StopBit>(static_cast<unsigned char>(StopBit::STOP_BIT_MAX) + 1)));
    ASSERT_TRUE(isStopBitSupported(StopBit::STOP_BIT_ONE));

#ifdef __linux__
    ASSERT_FALSE(isStopBitSupported(StopBit::STOP_BIT_ONE_HALF));
#elif defined(_WIN32) | defined(_WIN64)
    ASSERT_TRUE(isStopBitSupported(StopBit::STOP_BIT_ONE_HALF));
#endif // __linux__

    ASSERT_TRUE(isStopBitSupported(StopBit::STOP_BIT_TWO));
}

TEST(PropertiesTest, GetStopBitNameFunctionTest)
{
    SCOPED_TRACE("GetStopBitNameFunctionTest");

    ASSERT_THROW(getStopBitName(static_cast<StopBit>(static_cast<unsigned char>(StopBit::STOP_BIT_MAX) + 1)), std::out_of_range);
    ASSERT_EQ(getStopBitName(StopBit::STOP_BIT_ONE), "One");
    ASSERT_EQ(getStopBitName(StopBit::STOP_BIT_ONE_HALF), "One and a half");
    ASSERT_EQ(getStopBitName(StopBit::STOP_BIT_TWO), "Two");
}

TEST(PropertiesTest, ControlLineSetTests)
{
    SCOPED_TRACE("ControlLineSetTests");

    // All control lines represent the ALL mask
    ASSERT_EQ(all, ControlLine::LINE_ALL);
}

TEST(PropertiesTest, ControlLineNotOperatorTests)
{
    SCOPED_TRACE("ControlLineNotOperatorTests");

    // All control lines NOT-ed represent NOT-ed ALL mask
    ASSERT_EQ(~all, ~ControlLine::LINE_ALL);

    // All control lines NOT-ed represent no control lines
    ASSERT_EQ(~all, ControlLine::LINE_NONE);

    // All gettable control lines NOT-ed represent settable control lines
    ASSERT_EQ(~gettable, settable);

    // All settable control lines NOT-ed represent gettable control lines
    ASSERT_EQ(~settable, gettable);
}

TEST(PropertiesTest, ControlLineAndOperatorTests)
{
    SCOPED_TRACE("ControlLineAndOperatorTests");

    // All control lines AND-ed with the ALL mask represent the ALL mask
    ASSERT_EQ((all & ControlLine::LINE_ALL), ControlLine::LINE_ALL);

    // All gettable control lines AND-ed with settable represent no control lines
    ASSERT_EQ((gettable & settable), ControlLine::LINE_NONE);

    // All gettable control lines AND-ed with the ALL mask represent gettable control lines
    ASSERT_EQ((gettable & ControlLine::LINE_ALL), gettable);

    // All settable control lines AND-ed with the ALL mask represent settable control lines
    ASSERT_EQ((settable & ControlLine::LINE_ALL), settable);

    // Test AND compound assignment operator
    ControlLine line{all};
    ASSERT_EQ(line &= gettable, gettable);
    ASSERT_EQ(line &= settable, ControlLine::LINE_NONE);
}

TEST(PropertiesTest, ControlLineOrOperatorTests)
{
    SCOPED_TRACE("ControlLineOrOperatorTests");

    // All control lines OR-ed with the ALL mask represent the ALL mask
    ASSERT_EQ((all | ControlLine::LINE_ALL), ControlLine::LINE_ALL);

    // All gettable control lines OR-ed with settable represent the ALL mask
    ASSERT_EQ((gettable | settable), ControlLine::LINE_ALL);

    // All gettable control lines OR-ed with the ALL mask represent the ALL mask
    ASSERT_EQ((gettable | ControlLine::LINE_ALL), ControlLine::LINE_ALL);

    // All settable control lines OR-ed with the ALL mask represent the ALL mask
    ASSERT_EQ((settable | ControlLine::LINE_ALL), ControlLine::LINE_ALL);

    // Test OR compound assignment operator
    ControlLine line{ControlLine::LINE_NONE};
    ASSERT_EQ(line |= gettable, gettable);
    ASSERT_EQ(line |= settable, all);
}

TEST(PropertiesTest, ControlLineXorOperatorTests)
{
    SCOPED_TRACE("ControlLineXorOperatorTests");

    // All control lines XOR-ed with the ALL mask represent no control lines
    ASSERT_EQ((all ^ ControlLine::LINE_ALL), ControlLine::LINE_NONE);

    // All gettable control lines XOR-ed with settable represent the ALL mask
    ASSERT_EQ((gettable ^ settable), ControlLine::LINE_ALL);

    // All gettable control lines XOR-ed with the ALL mask represent all settable control lines
    ASSERT_EQ((gettable ^ ControlLine::LINE_ALL), settable);

    // All settable control lines XOR-ed with the ALL mask represent all gettable control lines
    ASSERT_EQ((settable ^ ControlLine::LINE_ALL), gettable);

    // Test XOR compound assignment operator
    ControlLine line{gettable};
    ASSERT_EQ(line ^= settable, all);
    ASSERT_EQ(line ^= gettable, settable);
}

END_NAMESPACE_LIBSERIAL
