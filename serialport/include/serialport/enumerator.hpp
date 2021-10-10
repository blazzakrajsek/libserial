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
#include <vector>
#include <string>
#include <serialport/namespace.hpp>

BEGIN_NAMESPACE_LIBSERIAL

/**
 * @brief Enumerator class
 *
 */
class Enumerator final
{
public:
    /**
     * @brief Update the serial ports list
     *
     * @param list List of the ports to update
     * @return true Successfully updated the serial port list
     * @return false Failed to update the serial port list
     */
    static bool updateSerialPortList(std::vector<std::string>& list);

    /**
     * @brief Convert serial port name to file name
     *
     * @param serialPort Serial port name
     * @return std::string Serial port file name
     */
    static std::string serialPortToFileName(const std::string& serialPort);

    /**
     * @brief Convert serial port file name to port name
     *
     * @param fileName Serial port file name
     * @return std::string Serial port name
     */
    static std::string fileNameToSerialPort(const std::string& fileName);
};

END_NAMESPACE_LIBSERIAL
