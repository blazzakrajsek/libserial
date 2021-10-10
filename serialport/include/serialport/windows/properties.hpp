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
#include <string>
#include <windef.h>
#include <winbase.h>
#include <serialport/namespace.hpp>

BEGIN_NAMESPACE_LIBSERIAL

/**
 * @brief Default value for an invalid file descriptor
 *
 */
static const HANDLE INVALID_FILE_DESCRIPTOR{INVALID_HANDLE_VALUE};

/**
 * @brief Default serial port name
 *
 */
static constexpr char DEFAULT_PORT_NAME[]{"COM1"};

/**
 * @brief Default serial port prefix
 *
 */
static constexpr char SERIAL_PORT_PREFIX[]{"\\\\.\\"};

END_NAMESPACE_LIBSERIAL
