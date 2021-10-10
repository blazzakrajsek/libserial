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

BEGIN_NAMESPACE_LIBSERIAL

/**
 * @brief Enum of all control lines
 *
 */
static const ControlLine all{ControlLine::LINE_DCD | ControlLine::LINE_DTR | ControlLine::LINE_DSR | ControlLine::LINE_RTS | ControlLine::LINE_CTS | ControlLine::LINE_RI};

/**
 * @brief Enum of all settable control lines
 *
 */
static const ControlLine settable{ControlLine::LINE_DTR | ControlLine::LINE_RTS};

/**
 * @brief Enum of all gettable control lines
 *
 */
static const ControlLine gettable{ControlLine::LINE_DCD | ControlLine::LINE_DSR | ControlLine::LINE_CTS | ControlLine::LINE_RI};

END_NAMESPACE_LIBSERIAL
