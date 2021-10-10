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

#include <gtest/gtest.h>
#include <serialport/namespace.hpp>
#include <serialport_test/test_serialport_impl.hpp>

BEGIN_NAMESPACE_LIBSERIAL

TEST_F(SerialPortTest, ContructorTests)
{
    SCOPED_TRACE("ContructorTests");
    performConstructorTests();
}

TEST_F(SerialPortTest, FirstPortOpenTest)
{
    SCOPED_TRACE("FirstPortOpenTest");
    performFirstPortOpenTest();
}

TEST_F(SerialPortTest, SecondPortOpenTest)
{
    SCOPED_TRACE("SecondPortOpenTest");
    performSecondPortOpenTest();
}

TEST_F(SerialPortTest, OpenCloseTests)
{
    SCOPED_TRACE("OpenCloseTests");
    performOpenCloseTests();
}

TEST_F(SerialPortTest, OpenModeTests)
{
    SCOPED_TRACE("OpenModeTests");
    performOpenModeTests();
}

TEST_F(SerialPortTest, ClosedPortFunctionTests)
{
    SCOPED_TRACE("ClosedPortFunctionTests");
    performClosedPortFunctionTests();
}

TEST_F(SerialPortTest, ExclusiveModeTests)
{
    SCOPED_TRACE("ExclusiveModeTests");
    performExclusiveModeTests();
}

TEST_F(SerialPortTest, PropertiesTests)
{
    SCOPED_TRACE("PropertesTests");
    performPropertiesTests();
}

TEST_F(SerialPortTest, SimpleReadWriteTests)
{
    SCOPED_TRACE("SimpleReadWriteTests");
    performSimpleReadWriteTests();
}

TEST_F(SerialPortTest, ExtendedReadWriteStringTests)
{
    SCOPED_TRACE("ExtendedReadWriteStringTests");
    performExtendedReadWriteStringTests();
}

TEST_F(SerialPortTest, ExtendedReadWriteCharArrayTests)
{
    SCOPED_TRACE("ExtendedReadWriteCharArrayTests");
    performExtendedReadWriteCharArrayTests();
}

TEST_F(SerialPortTest, SerialPortControlLineTests)
{
    SCOPED_TRACE("ControLineTests");
    performControlLineTests();
}

END_NAMESPACE_LIBSERIAL
