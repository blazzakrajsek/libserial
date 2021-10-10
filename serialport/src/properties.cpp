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
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>

#ifdef __linux__
    #include <termios.h>
#elif defined(_WIN32) || defined(_WIN64)
    #include <windef.h>
    #include <winbase.h>
#endif // __linux__

BEGIN_NAMESPACE_LIBSERIAL

bool isBaudRateSupported(BaudRate baudRate)
{
    switch (baudRate)
    {
        case BaudRate::BAUD_RATE_110:
        case BaudRate::BAUD_RATE_300:
        case BaudRate::BAUD_RATE_600:
        case BaudRate::BAUD_RATE_1200:
        case BaudRate::BAUD_RATE_2400:
        case BaudRate::BAUD_RATE_4800:
        case BaudRate::BAUD_RATE_9600:
        case BaudRate::BAUD_RATE_19200:
        case BaudRate::BAUD_RATE_38400:
        case BaudRate::BAUD_RATE_57600:
        case BaudRate::BAUD_RATE_115200:

#ifdef __linux__
        case BaudRate::BAUD_RATE_50:
        case BaudRate::BAUD_RATE_75:
        case BaudRate::BAUD_RATE_134:
        case BaudRate::BAUD_RATE_150:
        case BaudRate::BAUD_RATE_200:
        case BaudRate::BAUD_RATE_1800:
        case BaudRate::BAUD_RATE_230400:
        case BaudRate::BAUD_RATE_460800:
        case BaudRate::BAUD_RATE_500000:
        case BaudRate::BAUD_RATE_576000:
        case BaudRate::BAUD_RATE_921600:
        case BaudRate::BAUD_RATE_1000000:
        case BaudRate::BAUD_RATE_1152000:
        case BaudRate::BAUD_RATE_1500000:
#if __MAX_BAUD > B2000000
        case BaudRate::BAUD_RATE_2000000:
        case BaudRate::BAUD_RATE_2500000:
        case BaudRate::BAUD_RATE_3000000:
        case BaudRate::BAUD_RATE_3500000:
        case BaudRate::BAUD_RATE_4000000:
#endif // __MAX_BAUD
#endif // __linux__

#if defined(_WIN32) || defined(_WIN64)
        case BaudRate::BAUD_RATE_14400:
        case BaudRate::BAUD_RATE_56000:
        case BaudRate::BAUD_RATE_128000:
        case BaudRate::BAUD_RATE_256000:
#endif // _WIN32 || _WIN64

            return true;
            break;

        default:
        case BaudRate::BAUD_RATE_CUSTOM:
            return false;
            break;
    }
}

int getBaudRateValue(BaudRate baudRate)
{
    switch (baudRate)
    {
#ifdef __linux__
        case BaudRate::BAUD_RATE_50:
            return B50;
            break;

        case BaudRate::BAUD_RATE_75:
            return B75;
            break;

        case BaudRate::BAUD_RATE_110:
            return B110;
            break;

        case BaudRate::BAUD_RATE_134:
            return B134;
            break;

        case BaudRate::BAUD_RATE_150:
            return B150;
            break;

        case BaudRate::BAUD_RATE_200:
            return B200;
            break;

        case BaudRate::BAUD_RATE_300:
            return B300;
            break;

        case BaudRate::BAUD_RATE_600:
            return B600;
            break;

        case BaudRate::BAUD_RATE_1200:
            return B1200;
            break;

        case BaudRate::BAUD_RATE_1800:
            return B1800;
            break;

        case BaudRate::BAUD_RATE_2400:
            return B2400;
            break;

        case BaudRate::BAUD_RATE_4800:
            return B4800;
            break;

        case BaudRate::BAUD_RATE_9600:
            return B9600;
            break;

        case BaudRate::BAUD_RATE_19200:
            return B19200;
            break;

        case BaudRate::BAUD_RATE_38400:
            return B38400;
            break;

        case BaudRate::BAUD_RATE_57600:
            return B57600;
            break;

        case BaudRate::BAUD_RATE_115200:
            return B115200;
            break;

        case BaudRate::BAUD_RATE_230400:
            return B230400;
            break;

        case BaudRate::BAUD_RATE_460800:
            return B460800;
            break;

        case BaudRate::BAUD_RATE_500000:
            return B500000;
            break;

        case BaudRate::BAUD_RATE_576000:
            return B576000;
            break;

        case BaudRate::BAUD_RATE_921600:
            return B921600;
            break;

        case BaudRate::BAUD_RATE_1000000:
            return B1000000;
            break;

        case BaudRate::BAUD_RATE_1152000:
            return B1152000;
            break;

        case BaudRate::BAUD_RATE_1500000:
            return B1500000;
            break;

#if __MAX_BAUD > B2000000
        case BaudRate::BAUD_RATE_2000000:
            return B2000000;
            break;

        case BaudRate::BAUD_RATE_2500000:
            return B2500000;
            break;

        case BaudRate::BAUD_RATE_3000000:
            return B3000000;
            break;

        case BaudRate::BAUD_RATE_3500000:
            return B3500000;
            break;

        case BaudRate::BAUD_RATE_4000000:
            return B4000000;
            break;
#endif // __MAX_BAUD
#endif // __linux__

#if defined(_WIN32) || defined(_WIN64)
        case BaudRate::BAUD_RATE_110:
            return CBR_110;
            break;

        case BaudRate::BAUD_RATE_300:
            return CBR_300;
            break;

        case BaudRate::BAUD_RATE_600:
            return CBR_600;
            break;

        case BaudRate::BAUD_RATE_1200:
            return CBR_1200;
            break;

        case BaudRate::BAUD_RATE_2400:
            return CBR_2400;
            break;

        case BaudRate::BAUD_RATE_4800:
            return CBR_4800;
            break;

        case BaudRate::BAUD_RATE_9600:
            return CBR_9600;
            break;

        case BaudRate::BAUD_RATE_14400:
            return CBR_14400;
            break;

        case BaudRate::BAUD_RATE_19200:
            return CBR_19200;
            break;

        case BaudRate::BAUD_RATE_38400:
            return CBR_38400;
            break;

        case BaudRate::BAUD_RATE_56000:
            return CBR_56000;
            break;

        case BaudRate::BAUD_RATE_57600:
            return CBR_57600;
            break;

        case BaudRate::BAUD_RATE_115200:
            return CBR_115200;
            break;

        case BaudRate::BAUD_RATE_128000:
            return CBR_128000;
            break;

        case BaudRate::BAUD_RATE_256000:
            return CBR_256000;
            break;
#endif // _WIN32 | _WIN64

        default:
            throw std::out_of_range("Baud rate out of range");
            break;
    }
}

unsigned long getBaudRate(BaudRate baudRate)
{
    switch (baudRate)
    {
        case BaudRate::BAUD_RATE_50:
            return 50U;
            break;

        case BaudRate::BAUD_RATE_75:
            return 75U;
            break;

        case BaudRate::BAUD_RATE_110:
            return 110U;
            break;

        case BaudRate::BAUD_RATE_134:
            return 134U;
            break;

        case BaudRate::BAUD_RATE_150:
            return 150U;
            break;

        case BaudRate::BAUD_RATE_200:
            return 200U;
            break;

        case BaudRate::BAUD_RATE_300:
            return 300U;
            break;

        case BaudRate::BAUD_RATE_600:
            return 600U;
            break;

        case BaudRate::BAUD_RATE_1200:
            return 1200U;
            break;

        case BaudRate::BAUD_RATE_1800:
            return 1800U;
            break;

        case BaudRate::BAUD_RATE_2400:
            return 2400U;
            break;

        case BaudRate::BAUD_RATE_4800:
            return 4800U;
            break;

        case BaudRate::BAUD_RATE_9600:
            return 9600U;
            break;

        case BaudRate::BAUD_RATE_14400:
            return 14400U;
            break;

        case BaudRate::BAUD_RATE_19200:
            return 19200U;
            break;

        case BaudRate::BAUD_RATE_38400:
            return 38400U;
            break;

        case BaudRate::BAUD_RATE_56000:
            return 56000U;
            break;

        case BaudRate::BAUD_RATE_57600:
            return 57600U;
            break;

        case BaudRate::BAUD_RATE_115200:
            return 115200U;
            break;

        case BaudRate::BAUD_RATE_128000:
            return 128000U;
            break;

        case BaudRate::BAUD_RATE_230400:
            return 230400U;
            break;

        case BaudRate::BAUD_RATE_256000:
            return 256000U;
            break;

        case BaudRate::BAUD_RATE_460800:
            return 460800U;
            break;

        case BaudRate::BAUD_RATE_500000:
            return 500000U;
            break;

        case BaudRate::BAUD_RATE_576000:
            return 576000U;
            break;

        case BaudRate::BAUD_RATE_921600:
            return 921600U;
            break;

        case BaudRate::BAUD_RATE_1000000:
            return 1000000U;
            break;

        case BaudRate::BAUD_RATE_1152000:
            return 1152000U;
            break;

        case BaudRate::BAUD_RATE_1500000:
            return 1500000U;
            break;

        case BaudRate::BAUD_RATE_2000000:
            return 2000000U;
            break;

        case BaudRate::BAUD_RATE_2500000:
            return 2500000U;
            break;

        case BaudRate::BAUD_RATE_3000000:
            return 3000000U;
            break;

        case BaudRate::BAUD_RATE_3500000:
            return 3500000U;
            break;

        case BaudRate::BAUD_RATE_4000000:
            return 4000000U;
            break;

        default:
            throw std::out_of_range("Baud rate out of range");
            break;
    }
}

bool isCharacterSizeSupported(CharacterSize characterSize)
{
    switch (characterSize)
    {
        case CharacterSize::CHARACTER_SIZE_5:
        case CharacterSize::CHARACTER_SIZE_6:
        case CharacterSize::CHARACTER_SIZE_7:
        case CharacterSize::CHARACTER_SIZE_8:
            return true;
            break;

        default:
            return false;
            break;
    }
}

int getCharacterSizeValue(CharacterSize characterSize)
{
    switch (characterSize)
    {
#ifdef __linux__
        case CharacterSize::CHARACTER_SIZE_5:
            return CS5;
            break;

        case CharacterSize::CHARACTER_SIZE_6:
            return CS6;
            break;

        case CharacterSize::CHARACTER_SIZE_7:
            return CS7;
            break;

        case CharacterSize::CHARACTER_SIZE_8:
            return CS8;
            break;
#endif // __linux__

#if defined(_WIN32) || defined(_WIN64)
        case CharacterSize::CHARACTER_SIZE_5:
            return 5U;
            break;

        case CharacterSize::CHARACTER_SIZE_6:
            return 6U;
            break;

        case CharacterSize::CHARACTER_SIZE_7:
            return 7U;
            break;

        case CharacterSize::CHARACTER_SIZE_8:
            return 8U;
            break;
#endif // _WIN32 || _WIN64

        default:
            throw std::out_of_range("Character size out of range");
            break;
    }
}

unsigned char getCharacterSize(CharacterSize characterSize)
{
    switch (characterSize)
    {
        case CharacterSize::CHARACTER_SIZE_5:
            return 5U;
            break;

        case CharacterSize::CHARACTER_SIZE_6:
            return 6U;
            break;

        case CharacterSize::CHARACTER_SIZE_7:
            return 7U;
            break;

        case CharacterSize::CHARACTER_SIZE_8:
            return 8U;
            break;

        default:
            throw std::out_of_range("Character size out of range");
            break;
    }
}

bool isFlowControlSupported(FlowControl flowControl)
{
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_HARDWARE:
        case FlowControl::FLOW_CONTROL_SOFTWARE:
        case FlowControl::FLOW_CONTROL_NONE:
            return true;
            break;

        default:
            return false;
            break;
    }
}

std::string getFlowControlName(FlowControl flowControl)
{
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_HARDWARE:
            return "Hardware";
            break;

        case FlowControl::FLOW_CONTROL_SOFTWARE:
            return "Software";
            break;

        case FlowControl::FLOW_CONTROL_NONE:
            return "None";
            break;

        default:
            throw std::out_of_range("Flow control out of range");
            break;
    }
}

bool isParitySupported(Parity parity)
{
    switch (parity)
    {
        case Parity::PARITY_TYPE_NONE:
        case Parity::PARITY_TYPE_ODD:
        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_MARK:
        case Parity::PARITY_TYPE_SPACE:
            return true;
            break;

        default:
            return false;
            break;
    }
}

std::string getParityName(Parity parity)
{
    switch (parity)
    {
        case Parity::PARITY_TYPE_ODD:
            return "Odd";
            break;

        case Parity::PARITY_TYPE_EVEN:
            return "Even";
            break;

        case Parity::PARITY_TYPE_MARK:
            return "Mark";
            break;

        case Parity::PARITY_TYPE_SPACE:
            return "Space";
            break;

        case Parity::PARITY_TYPE_NONE:
            return "None";
            break;

        default:
            throw std::out_of_range("Parity out of range");
            break;
    }
}

bool isStopBitSupported(StopBit stopBit)
{
    switch (stopBit)
    {
        case StopBit::STOP_BIT_ONE:
        case StopBit::STOP_BIT_TWO:
            return true;
            break;

#if defined(_WIN32) || defined(_WIN64)
        case StopBit::STOP_BIT_ONE_HALF:
            return true;
            break;
#endif // _WIN32 || _WIN64

        default:
            return false;
            break;
    }
}

std::string getStopBitName(StopBit stopBit)
{
    switch (stopBit)
    {
        case StopBit::STOP_BIT_ONE_HALF:
            return "One and a half";
            break;

        case StopBit::STOP_BIT_TWO:
            return "Two";
            break;

        case StopBit::STOP_BIT_ONE:
            return "One";
            break;

        default:
            throw std::out_of_range("Stop bit out of range");
            break;
    }
}

double calculateTime(BaudRate baudRate, CharacterSize characterSize, Parity parity, StopBit stopBit)
{
    // | Idle | Start | 5-8 data bits | <Parity bit> | Stop bit | <Half/second stop bit> | Idle |

    // One start, stop and idle bit by default
    size_t bits{3};

    // Add additional bit for parity if enabled
    switch (parity) {
        case Parity::PARITY_TYPE_ODD:
        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_MARK:
        case Parity::PARITY_TYPE_SPACE:
            bits += 1;
            break;

        case Parity::PARITY_TYPE_NONE:
        default:
            break;
    }

    // Add additional bit with more stop bits
    switch (stopBit)
    {
        case StopBit::STOP_BIT_TWO:
        case StopBit::STOP_BIT_ONE_HALF:
            bits += 1;
            break;

        case StopBit::STOP_BIT_ONE:
        default:
            break;
    }

    // Add bits according to the character size
    switch (characterSize)
    {
        case CharacterSize::CHARACTER_SIZE_5:
            bits += 5;
            break;

        case CharacterSize::CHARACTER_SIZE_6:
            bits += 6;
            break;

        case CharacterSize::CHARACTER_SIZE_7:
            bits += 7;
            break;

        case CharacterSize::CHARACTER_SIZE_8:
        default:
            bits += 8;
            break;
    }

    // Return result in milli-seconds
    return ((static_cast<double>(bits) * 1000) / LibSerial::getBaudRate(baudRate));
}

ControlLine operator~(const ControlLine& l)
{
    return (static_cast<ControlLine>((~static_cast<unsigned char>(l)) & static_cast<unsigned char>(ControlLine::LINE_ALL)));
}

ControlLine operator&(const ControlLine& l, const ControlLine& r)
{
    return (static_cast<ControlLine>(static_cast<unsigned char>(l) & static_cast<unsigned char>(r)));
}

ControlLine operator|(const ControlLine& l, const ControlLine& r)
{
    return (static_cast<ControlLine>(static_cast<unsigned char>(l) | static_cast<unsigned char>(r)));
}

ControlLine operator^(const ControlLine& l, const ControlLine& r)
{
    return (static_cast<ControlLine>((static_cast<unsigned char>(l) ^ static_cast<unsigned char>(r)) & static_cast<unsigned char>(ControlLine::LINE_ALL)));
}

ControlLine& operator&=(ControlLine& l, const ControlLine& r)
{
    return (l = (l & r));
}

ControlLine& operator|=(ControlLine& l, const ControlLine& r)
{
    return (l = (l | r));
}

ControlLine& operator^=(ControlLine& l, const ControlLine& r)
{
    return (l = (l ^ r));
}

END_NAMESPACE_LIBSERIAL
