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

#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport/enumerator.hpp>

#ifdef __linux__
    #include <fcntl.h>
    #include <termios.h>
    #include <sys/ioctl.h>
    #include <linux/serial.h>
    #include <unistd.h>
    #include <cstring>
#elif defined(_WIN32) || defined(_WIN64)
    #include <fileapi.h>
    #include <winnt.h>
    #include <handleapi.h>
#endif // __linux__

BEGIN_NAMESPACE_LIBSERIAL

bool Enumerator::updateSerialPortList(std::vector<std::string>& list)
{
    bool result{false};
    const size_t maxSerialPortIndex{64};
#if defined(__linux__)
    const char* serialPorts[]{"ttyUSB", "ttyACM", "ttyS"};
    for (const auto& serialPortName: serialPorts)
    {
        for (size_t index{0}; index < maxSerialPortIndex; ++index)
        {
            const auto serialPort{std::string(serialPortName) + std::to_string(index)};
            const auto fileName{serialPortToFileName(serialPort)};

            const auto fileDescriptor{systemCall(::open, fileName.c_str(), O_NOCTTY | O_NONBLOCK | O_RDWR)};
            if (fileDescriptor != INVALID_FILE_DESCRIPTOR)
            {
                serial_struct serialPortInfo{};
                if (systemCall(::ioctl, fileDescriptor, TIOCGSERIAL, &serialPortInfo) != INVALID_FILE_DESCRIPTOR)
                {
                    result = true;
                    list.push_back(serialPort);
                    systemCall(::close, fileDescriptor);
                }
            }
        }
    }
#elif defined(_WIN32) || defined(_WIN64)
    for (size_t index{1}; index < maxSerialPortIndex; ++index)
    {
        const auto serialPort{std::string("COM") + std::to_string(index)};
        const auto fileName{serialPortToFileName(serialPort)};

        const auto fileDescriptor{CreateFile(fileName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0)};
        if (fileDescriptor != INVALID_FILE_DESCRIPTOR)
        {
            result = true;
            list.push_back(serialPort);
            CloseHandle(fileDescriptor);
        }
    }
#endif // __linux__
    return result;
}

std::string Enumerator::serialPortToFileName(const std::string& serialPort)
{
    const auto position{serialPort.find(SERIAL_PORT_PREFIX)};
    if (position != std::string::npos)
        return serialPort;
    else
        return (std::string(SERIAL_PORT_PREFIX) + serialPort);
}

std::string Enumerator::fileNameToSerialPort(const std::string& fileName)
{
    std::string serialPort{fileName};
    const auto position{serialPort.find(SERIAL_PORT_PREFIX)};
    if (position != std::string::npos)
        serialPort.replace(position, strlen(SERIAL_PORT_PREFIX), "");
    return serialPort;
}

END_NAMESPACE_LIBSERIAL
