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

#include <memory>
#include <string>
#include <iostream>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport/serialport.hpp>

#ifdef __linux__
    #include <serialport/linux/serialport_impl.hpp>
#elif defined(_WIN32) || defined(_WIN64)
    #include <serialport/windows/serialport_impl.hpp>
#endif // __linux__

BEGIN_NAMESPACE_LIBSERIAL

SerialPort::SerialPort() :
    impl{std::make_unique<SerialPortImpl>()}
{

}

SerialPort::SerialPort(const std::string& portName,
    BaudRate baudRate,
    CharacterSize characterSize,
    FlowControl flowControl,
    Parity parity,
    StopBit stopBit) :
    impl{std::make_unique<SerialPortImpl>(portName, baudRate, characterSize, flowControl, parity, stopBit)}
{

}

SerialPort::~SerialPort() noexcept
{
    impl->close();
    impl.reset();
}

bool SerialPort::isOpen() const
{
    return impl->isOpen();
}

void SerialPort::open(std::ios_base::openmode openMode)
{
    impl->open(openMode);
}

void SerialPort::close()
{
    impl->close();
}

bool SerialPort::setExclusive(bool exclusive)
{
    return impl->setExclusive(exclusive);
}

size_t SerialPort::read(char* buffer, size_t size) const
{
    return impl->read(buffer, size);
}

size_t SerialPort::read(std::string& buffer) const
{
    return impl->read(buffer);
}

bool SerialPort::write(char data) const
{
    return impl->write(data);
}

size_t SerialPort::write(const char* buffer, size_t size) const
{
    return impl->write(buffer, size);
}

size_t SerialPort::write(const std::string& buffer) const
{
    return impl->write(buffer);
}

bool SerialPort::drain() const
{
    return impl->drain();
}

bool SerialPort::flushInput() const
{
    return impl->flushInput();
}

bool SerialPort::flushOutput() const
{
    return impl->flushOutput();
}

bool SerialPort::flushInputOutput() const
{
    return impl->flushInputOutput();
}

size_t SerialPort::getInputQueueCount() const
{
    return impl->getInputQueueCount();
}

size_t SerialPort::getOutputQueueCount() const
{
    return impl->getOutputQueueCount();
}

std::string SerialPort::getPortName() const
{
    return impl->getPortName();
}

void SerialPort::setPortName(const std::string& portName)
{
    impl->setPortName(portName);
}

BaudRate SerialPort::getBaudRate() const
{
    return impl->getBaudRate();
}

void SerialPort::setBaudRate(BaudRate baudRate)
{
    impl->setBaudRate(baudRate);
}

CharacterSize SerialPort::getCharacterSize() const
{
    return impl->getCharacterSize();
}

void SerialPort::setCharacterSize(CharacterSize characterSize)
{
    impl->setCharacterSize(characterSize);
}

FlowControl SerialPort::getFlowControl() const
{
    return impl->getFlowControl();
}

void SerialPort::setFlowControl(FlowControl flowControl)
{
    impl->setFlowControl(flowControl);
}

Parity SerialPort::getParity() const
{
    return impl->getParity();
}

void SerialPort::setParity(Parity parity)
{
    impl->setParity(parity);
}

StopBit SerialPort::getStopBit() const
{
    return impl->getStopBit();
}

void SerialPort::setStopBit(StopBit stopBit)
{
    impl->setStopBit(stopBit);
}

bool SerialPort::getControlLine(ControlLine controlLine) const
{
    return impl->getControlLine(controlLine);
}

bool SerialPort::setControlLine(ControlLine controlLine, bool state) const
{
    return impl->setControlLine(controlLine, state);
}

END_NAMESPACE_LIBSERIAL
