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

#include <serialport/namespace.hpp>
#if defined(__linux__)
    #include <serialport/linux/properties.hpp>
#elif defined(_WIN32) || defined(_WIN64)
    #include <serialport/windows/properties.hpp>
#else
    #error Platform not supported
#endif // __linux__

BEGIN_NAMESPACE_LIBSERIAL

/**
 * @brief Null character constant
 *
 */
static constexpr unsigned char NUL{0x00};

/**
 * @brief Start of Header constant
 *
 */
static constexpr unsigned char SOH{0x01};

/**
 * @brief Start of Text constant
 *
 */
static constexpr unsigned char STX{0x02};

/**
 * @brief End of Text constant
 *
 */
static constexpr unsigned char ETX{0x03};

/**
 * @brief End of Transmission constant
 *
 */
static constexpr unsigned char EOT{0x04};

/**
 * @brief Enquiry constant
 *
 */
static constexpr unsigned char ENQ{0x05};

/**
 * @brief Acknowledgement constant
 *
 */
static constexpr unsigned char ACK{0x06};

/**
 * @brief Bell constant
 *
 */
static constexpr unsigned char BEL{0x07};

/**
 * @brief Backspace constant
 *
 */
static constexpr unsigned char BS{0x08};

/**
 * @brief Horizontal Tab constant
 *
 */
static constexpr unsigned char TAB{0x09};

/**
 * @brief Line Feed constant
 *
 */
static constexpr unsigned char LF{0x0A};

/**
 * @brief Vertical Tab constant
 *
 */
static constexpr unsigned char VT{0x0B};

/**
 * @brief Form Feed constant
 *
 */
static constexpr unsigned char FF{0x0C};

/**
 * @brief Carriage return constant
 *
 */
static constexpr unsigned char CR{0x0D};

/**
 * @brief Shift Out constant
 *
 */
static constexpr unsigned char SO{0x0E};

/**
 * @brief Shift In constant
 *
 */
static constexpr unsigned char SI{0x0F};

/**
 * @brief Data Link Escape constant
 *
 */
static constexpr unsigned char DLE{0x10};

/**
 * @brief Device Control 1 constant
 *
 */
static constexpr unsigned char DC1{0x11};

/**
 * @brief Software flow control: XON constant
 *
 */
static constexpr unsigned char XON{DC1};

/**
 * @brief Device Control 2 constant
 *
 */
static constexpr unsigned char DC2{0x12};

/**
 * @brief Device Control 3 constant
 *
 */
static constexpr unsigned char DC3{0x13};

/**
 * @brief Software flow control: XOFF constant
 *
 */
static constexpr unsigned char XOFF{DC3};

/**
 * @brief Device Control 4 constant
 *
 */
static constexpr unsigned char DC4{0x14};

/**
 * @brief Negative Acknowledgement constant
 *
 */
static constexpr unsigned char NAK{0x15};

/**
 * @brief Synchronous Idle constant
 *
 */
static constexpr unsigned char SYN{0x16};

/**
 * @brief End of Transmission Block constant
 *
 */
static constexpr unsigned char ETB{0x17};

/**
 * @brief Cancel constant
 *
 */
static constexpr unsigned char CAN{0x18};

/**
 * @brief End of Medium constant
 *
 */
static constexpr unsigned char EM{0x19};

/**
 * @brief Substitute constant
 *
 */
static constexpr unsigned char SUB{0x1A};

/**
 * @brief Escape constant
 *
 */
static constexpr unsigned char ESC{0x1B};

/**
 * @brief File Separator constant
 *
 */
static constexpr unsigned char FS{0x1C};

/**
 * @brief Group Separator constant
 *
 */
static constexpr unsigned char GS{0x1D};

/**
 * @brief Record Separator constant
 *
 */
static constexpr unsigned char RS{0x1E};

/**
 * @brief Unit Separator constant
 *
 */
static constexpr unsigned char US{0x1F};

/**
 * @brief Baud rate
 *
 */
enum class BaudRate : unsigned char
{
    /**
     * @brief Custom baud rate
     *
     */
    BAUD_RATE_CUSTOM  = 0U,

    /**
     * @brief Baud rate of 50
     *
     * @note Supported on Linux
     */
    BAUD_RATE_50 = 1U,

    /**
     * @brief Baud rate of 75
     *
     * @note Supported on Linux
     */
    BAUD_RATE_75 = 2U,

    /**
     * @brief Baud rate of 110
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_110 = 3U,

    /**
     * @brief Baud rate of 134
     *
     * @note Supported on Linux
     */
    BAUD_RATE_134 = 4U,

    /**
     * @brief Baud rate of 150
     *
     * @note Supported on Linux
     */
    BAUD_RATE_150 = 5U,

    /**
     * @brief Baud rate of 200
     *
     * @note Supported on Linux
     */
    BAUD_RATE_200 = 6U,

    /**
     * @brief Baud rate of 300
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_300 = 7U,

    /**
     * @brief Baud rate of 600
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_600 = 8U,

    /**
     * @brief Baud rate of 1200
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_1200 = 9U,

    /**
     * @brief Baud rate of 1800
     *
     * @note Supported on Linux
     */
    BAUD_RATE_1800 = 10U,

    /**
     * @brief Baud rate of 2400
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_2400 = 11U,

    /**
     * @brief Baud rate of 4800
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_4800 = 12U,

    /**
     * @brief Baud rate of 9600
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_9600 = 13U,

    /**
     * @brief Baud rate of 14400
     *
     * @note Supported on Windows
     */
    BAUD_RATE_14400 = 14U,

    /**
     * @brief Baud rate of 19200
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_19200 = 15U,

    /**
     * @brief Baud rate of 38400
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_38400 = 16U,

    /**
     * @brief Baud rate of 56000
     *
     * @note Supported on Windows
     */
    BAUD_RATE_56000 = 17U,

    /**
     * @brief Baud rate of 57600
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_57600 = 18U,

    /**
     * @brief Baud rate of 115200
     *
     * @note Supported on Linux and Windows
     */
    BAUD_RATE_115200 = 19U,

    /**
     * @brief Baud rate of 128000
     *
     * @note Supported on Windows
     */
    BAUD_RATE_128000 = 20U,

    /**
     * @brief Baud rate of 230400
     *
     * @note Supported on Linux
     */
    BAUD_RATE_230400 = 21U,

    /**
     * @brief Baud rate of 256000
     *
     * @note Supported on Windows
     */
    BAUD_RATE_256000 = 22U,

    /**
     * @brief Baud rate of 460800
     *
     * @note Supported on Linux
     */
    BAUD_RATE_460800 = 23U,

    /**
     * @brief Baud rate of 500000
     *
     * @note Supported on Linux
     */
    BAUD_RATE_500000 = 24U,

    /**
     * @brief Baud rate of 576000
     *
     * @note Supported on Linux
     */
    BAUD_RATE_576000 = 25U,

    /**
     * @brief Baud rate of 921600
     *
     * @note Supported on Linux
     */
    BAUD_RATE_921600 = 26U,

    /**
     * @brief Baud rate of 1000000
     *
     * @note Supported on Linux
     */
    BAUD_RATE_1000000 = 27U,

    /**
     * @brief Baud rate of 1152000
     *
     * @note Supported on Linux
     */
    BAUD_RATE_1152000 = 28U,

    /**
     * @brief Baud rate of 1500000
     *
     * @note Supported on Linux
     */
    BAUD_RATE_1500000 = 29U,

    /**
     * @brief Baud rate of 2000000
     *
     * @note Supported on Linux (__MAX_BAUD > B2000000)
     */
    BAUD_RATE_2000000 = 30U,

    /**
     * @brief Baud rate of 2500000
     *
     * @note Supported on Linux (__MAX_BAUD > B2000000)
     */
    BAUD_RATE_2500000 = 31U,

    /**
     * @brief Baud rate of 3000000
     *
     * @note Supported on Linux (__MAX_BAUD > B2000000)
     */
    BAUD_RATE_3000000 = 32U,

    /**
     * @brief Baud rate of 3500000
     *
     * @note Supported on Linux (__MAX_BAUD > B2000000)
     */
    BAUD_RATE_3500000 = 33U,

    /**
     * @brief Baud rate of 4000000
     *
     * @note Supported on Linux (__MAX_BAUD > B2000000)
     */
    BAUD_RATE_4000000 = 34U,

    BAUD_RATE_DEFAULT = BAUD_RATE_115200,
    BAUD_RATE_MIN = BAUD_RATE_CUSTOM,
    BAUD_RATE_MAX = BAUD_RATE_4000000
};

/**
 * @brief Get the baud rate support depending on the platform
 *
 * @param baudRate Baud rate
 * @return true Baud rate is supported
 * @return false Baud rate is not supported
 */
bool isBaudRateSupported(BaudRate baudRate);

/**
 * @brief Get the platform specific baud rate value
 *
 * @param baudRate Baud rate
 * @throw std::out_of_range Unsupported or out-of-range baud rate
 * @return int Baud rate value
 */
int getBaudRateValue(BaudRate baudRate);

/**
 * @brief Get the baud rate number
 *
 * @param baudRate Baud rate
 * @return unsigned long Baud rate number
 */
unsigned long getBaudRate(BaudRate baudRate);

/**
 * @brief Character size
 *
 */
enum class CharacterSize : unsigned char
{
    /**
     * @brief Character size of 5 bits
     *
     * @note Supported on Linux and Windows
     */
    CHARACTER_SIZE_5 = 0U,

    /**
     * @brief Character size of 6 bits
     *
     * @note Supported on Linux and Windows
     */
    CHARACTER_SIZE_6 = 1U,

    /**
     * @brief Character size of 7 bits
     *
     * @note Supported on Linux and Windows
     */
    CHARACTER_SIZE_7 = 2U,

    /**
     * @brief Character size of 8 bits
     *
     * @note Supported on Linux and Windows
     */
    CHARACTER_SIZE_8 = 3U,

    CHARACTER_SIZE_DEFAULT = CHARACTER_SIZE_8,
    CHARACTER_SIZE_MIN = CHARACTER_SIZE_5,
    CHARACTER_SIZE_MAX = CHARACTER_SIZE_8
};

/**
 * @brief Get the character size support depending on the platform
 *
 * @param characterSize Character size
 * @return true Character size is supported
 * @return false Character size is not supported
 */
bool isCharacterSizeSupported(CharacterSize characterSize);

/**
 * @brief Get the platform specific character size value
 *
 * @param characterSize Character size
 * @throw std::out_of_range Unsupported or out-of-range character size
 * @return int Character size value
 */
int getCharacterSizeValue(CharacterSize characterSize);

/**
 * @brief Get the character size
 *
 * @param characterSize Character size
 * @throw std::out_of_range Unsupported or out-of-range character size
 * @return unsigned char Character bit size
 */
unsigned char getCharacterSize(CharacterSize characterSize);

/**
 * @brief Flow control
 *
 */
enum class FlowControl : unsigned char
{
    /**
     * @brief Hardware Flow control
     *
     * @note Supported on Linux and Windows
     */
    FLOW_CONTROL_HARDWARE = 0U,

    /**
     * @brief Software Flow control
     *
     * @note Supported on Linux and Windows
     */
    FLOW_CONTROL_SOFTWARE = 1U,

    /**
     * @brief No Flow control
     *
     * @note Supported on Linux and Windows
     */
    FLOW_CONTROL_NONE = 2U,

    FLOW_CONTROL_DEFAULT = FLOW_CONTROL_NONE,
    FLOW_CONTROL_MIN = FLOW_CONTROL_HARDWARE,
    FLOW_CONTROL_MAX = FLOW_CONTROL_NONE
};

/**
 * @brief Get the flow control support depending on the platform
 *
 * @param flowControl Flow control
 * @return true Flow control is supported
 * @return false Flow control is not supported
 */
bool isFlowControlSupported(FlowControl flowControl);

/**
 * @brief Get the flow control name
 *
 * @param flowControl Flow control
 * @throw std::out_of_range Unsupported or out-of-range flow control
 * @return std::string Name
 */
std::string getFlowControlName(FlowControl flowControl);

/**
 * @brief Parity
 *
 */
enum class Parity : unsigned char
{
    /**
     * @brief No parity
     *
     * @note Supported on Linux and Windows
     */
    PARITY_TYPE_NONE = 0U,

    /**
     * @brief Odd parity
     *
     * @note Supported on Linux and Windows
     */
    PARITY_TYPE_ODD = 1U,

    /**
     * @brief Even parity
     *
     * @note Supported on Linux and Windows
     */
    PARITY_TYPE_EVEN = 2U,

    /**
     * @brief Mark parity
     *
     * @note Supported on Linux and Windows
     */
    PARITY_TYPE_MARK = 3U,

    /**
     * @brief Space parity
     *
     * @note Supported on Linux and Windows
     */
    PARITY_TYPE_SPACE = 4U,

    PARITY_TYPE_DEFAULT = PARITY_TYPE_NONE,
    PARITY_TYPE_MIN = PARITY_TYPE_NONE,
    PARITY_TYPE_MAX = PARITY_TYPE_SPACE
};

/**
 * @brief Get the parity support depending on the platform
 *
 * @param parity Parity
 * @return true Parity is supported
 * @return false Parity is not supported
 */
bool isParitySupported(Parity parity);

/**
 * @brief Get the parity name
 *
 * @param parity Parity
 * @throw std::out_of_range Unsupported or out-of-range parity
 * @return std::string Name
 */
std::string getParityName(Parity parity);

/**
 * @brief Stop bit
 *
 */
enum class StopBit : unsigned char
{
    /**
     * @brief One stop bit
     *
     * @note Supported on Linux and Windows
     */
    STOP_BIT_ONE = 0U,

    /**
     * @brief One and a half stop bit
     *
     * @note Supported on Windows
     */
    STOP_BIT_ONE_HALF = 1U,

    /**
     * @brief Two stop bits
     *
     * @note Supported on Linux and Windows
     */
    STOP_BIT_TWO = 2U,

    STOP_BIT_DEFAULT = STOP_BIT_ONE,
    STOP_BIT_MIN = STOP_BIT_ONE,
    STOP_BIT_MAX = STOP_BIT_TWO
};

/**
 * @brief Get the stop bit support depending on the platform
 *
 * @param stopBit Stop bit
 * @return true Stop bit is supported
 * @return false Stop bit is not supported
 */
bool isStopBitSupported(StopBit stopBit);

/**
 * @brief Get the stop bit name
 *
 * @param stopBit Stop bit
 * @throw std::out_of_range Unsupported or out-of-range stop bit
 * @return std::string Name
 */
std::string getStopBitName(StopBit stopBit);

/**
 * @brief Control line
 *
 */
enum class ControlLine : unsigned char
{
    /**
     * @brief No control line active
     *
     */
    LINE_NONE = 0x00,

    /**
     * @brief Data Carrier Detect control line active
     *
     * @note Control line is an input
     */
    LINE_DCD = 0x01,

    /**
     * @brief Data Terminal Ready control line active
     *
     * @note Control line is an output
     */
    LINE_DTR = 0x02,

    /**
     * @brief Data Set Ready control line active
     *
     * @note Control line is an input
     */
    LINE_DSR = 0x04,

    /**
     * @brief Request To Send control line active
     *
     * @note Control line is an output
     */
    LINE_RTS = 0x08,

    /**
     * @brief Clear To Send control line active
     *
     * @note Control line is an input
     */
    LINE_CTS = 0x10,

    /**
     * @brief Ring Indicator control line active
     *
     * @note Control line is an input
     */
    LINE_RI = 0x20,

    /**
     * @brief All control lines active
     *
     */
    LINE_ALL = 0x3F,
};

/**
 * @brief Calculate transmit/receive time for a single byte
 *
 * @param baudRate Baud rate
 * @throw std::out_of_range Baud rate is out of range
 * @param characterSize Character size
 * @param parity Parity
 * @param stopBit Stop bit
 * @return double Timeout in milli-seconds
 */
double calculateTime(BaudRate baudRate,
    CharacterSize characterSize = CharacterSize::CHARACTER_SIZE_DEFAULT,
    Parity parity = Parity::PARITY_TYPE_DEFAULT,
    StopBit stopBit = StopBit::STOP_BIT_DEFAULT);

/**
 * @brief ControlLine NOT operator
 *
 * @param l Input parameter
 * @return ControlLine Output
 */
ControlLine operator~(const ControlLine& l);

/**
 * @brief ControlLine AND operator
 *
 * @param l First input parameter
 * @param r Second input parameter
 * @return ControlLine Output
 */
ControlLine operator&(const ControlLine& l, const ControlLine& r);

/**
 * @brief ControlLine OR operator
 *
 * @param l First input parameter
 * @param r Second input parameter
 * @return ControlLine Output
 */
ControlLine operator|(const ControlLine& l, const ControlLine& r);

/**
 * @brief ControlLine XOR operator
 *
 * @param l First input parameter
 * @param r Second input parameter
 * @return ControlLine Output
 */
ControlLine operator^(const ControlLine& l, const ControlLine& r);

/**
 * @brief ControlLine AND compound assignment operator
 *
 * @param l First input parameter
 * @param r Second input parameter
 * @return ControlLine& Output
 */
ControlLine& operator&=(ControlLine& l, const ControlLine& r);

/**
 * @brief ControlLine OR compound assignment operator
 *
 * @param l First input parameter
 * @param r Second input parameter
 * @return ControlLine& Output
 */
ControlLine& operator|=(ControlLine& l, const ControlLine& r);

/**
 * @brief ControlLine XOR compound assignment operator
 *
 * @param l First input parameter
 * @param r Second input parameter
 * @return ControlLine& Output
 */
ControlLine& operator^=(ControlLine& l, const ControlLine& r);

END_NAMESPACE_LIBSERIAL
