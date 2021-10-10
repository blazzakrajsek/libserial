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
#include <memory>
#include <string>
#include <iostream>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>

BEGIN_NAMESPACE_LIBSERIAL

/**
 * @brief Forward declaration of the SerialPort implementation class
 *
 */
class SerialPortImpl;

/**
 * @brief SerialPort class
 *
 */
class SerialPort final
{
public:
    /**
     * @brief Construct a new SerialPort object
     *
     */
    explicit SerialPort();

    /**
     * @brief Construct a new SerialPort object
     *
     * @param portName Serial port name
     * @param baudRate Baud rate
     * @param characterSize Character size
     * @param flowControl Flow control
     * @param parity Parity
     * @param stopBit Stop bit
     */
    explicit SerialPort(const std::string& portName,
        BaudRate baudRate = BaudRate::BAUD_RATE_DEFAULT,
        CharacterSize characterSize = CharacterSize::CHARACTER_SIZE_DEFAULT,
        FlowControl flowControl = FlowControl::FLOW_CONTROL_DEFAULT,
        Parity parity = Parity::PARITY_TYPE_DEFAULT,
        StopBit stopBit = StopBit::STOP_BIT_DEFAULT);

    /**
     * @brief Copy-construct a new SerialPort object
     *
     * @param serialPort Serial port
     */
    SerialPort(const SerialPort& serialPort) = delete;

    /**
     * @brief Move-construct a new SerialPort object
     *
     * @param serialPort Serial port
     */
    SerialPort(SerialPort&& serialPort) = delete;

    /**
     * @brief Copy-assignment operator
     *
     * @param serialPort Serial port to copy-assign
     * @return SerialPort& Assigned serial port
     */
    SerialPort& operator=(const SerialPort& serialPort) = delete;

    /**
     * @brief Move-assignment operator
     *
     * @param serialPort Serial port to move-assign
     * @return SerialPort& Assigned serial port
     */
    SerialPort& operator=(SerialPort&& serialPort) = delete;

    /**
     * @brief Destroy the SerialPort object
     *
     */
    ~SerialPort() noexcept;

    /**
     * @brief Get serial port open status
     *
     * @return true Serial port is open
     * @return false Serial port is closed
     */
    bool isOpen() const;

    /**
     * @brief Open serial port
     *
     * @param openMode Serial port open mode
     * @throw std::runtime_error Unsupported open mode
     * @throw std::runtime_error Unable to open serial port
     * @throw std::runtime_error Unable to get port settings
     * @throw std::runtime_error Unable to get port timeout settings
     * @throw std::runtime_error Unable to set exclusive mode
     */
    void open(std::ios_base::openmode openMode = std::ios_base::in | std::ios_base::out);

    /**
     * @brief Close serial port
     *
     * @throw std::runtime_error Unable to set port settings
     */
    void close();

    /**
     * @brief Set the exclusive mode of the serial port
     *
     * @param exclusive Exclusive mode
     * @return true Successfully changed the exclusive mode
     * @return false Failed to change the exclusive mode
     */
    bool setExclusive(bool exclusive);

    /**
     * @brief Read data
     *
     * @param buffer Data buffer
     * @param size Size of the data to read
     * @return size_t Size of the data actually read
     */
    size_t read(char* buffer, size_t size) const;

    /**
     * @brief Read data
     *
     * @param buffer Data buffer
     * @return size_t Size of the data actually read
     */
    size_t read(std::string& buffer) const;

    /**
     * @brief Write data
     *
     * @param data Data
     * @return true Data written
     * @return false Data not written
     */
    bool write(char data) const;

    /**
     * @brief Write data
     *
     * @param buffer Data buffer
     * @param size Size of the data to write
     * @return size_t Size of the data actually written
     */
    size_t write(const char* buffer, size_t size) const;

    /**
     * @brief Write data
     *
     * @param buffer Data buffer
     * @return size_t Size of the data actually written
     */
    size_t write(const std::string& buffer) const;

    /**
     * @brief Wait for all the pending data to transmit
     *
     * @return true Successfully drained data on the serial port
     * @return false Failed to drain data on the serial port
     */
    bool drain() const;

    /**
     * @brief Flush all pending received data
     *
     * @return true Successfully flushed all pending received data
     * @return false Failed to flush all pending received data
     */
    bool flushInput() const;

    /**
     * @brief Flush all pending transmit data
     *
     * @return true Successfully flushed all pending transmit data
     * @return false Failed to flush all pending transmit data
     */
    bool flushOutput() const;

    /**
     * @brief Flush all pending transmit and receive data
     *
     * @return true Successfully flushed all pending transmit and receive data
     * @return false Failed to flush all pending transmit and receive data
     */
    bool flushInputOutput() const;

    /**
     * @brief Get the input queue count
     *
     * @return size_t Input queue count
     */
    size_t getInputQueueCount() const;

    /**
     * @brief Get the output queue count
     *
     * @return size_t Output queue count
     */
    size_t getOutputQueueCount() const;

    /**
     * @brief Get the port name
     *
     * @return std::string Port name
     */
    std::string getPortName() const;

    /**
     * @brief Set the port name
     *
     * @param portName Port name
     * @throw std::runtime_error Unsupported open mode
     * @throw std::runtime_error Unable to open serial port
     * @throw std::runtime_error Unable to get port settings
     * @throw std::runtime_error Unable to set exclusive mode
     * @throw std::runtime_error Unable to set port settings
     */
    void setPortName(const std::string& portName);

    /**
     * @brief Get the baud rate
     *
     * @return BaudRate Baud rate
     */
    BaudRate getBaudRate() const;

    /**
     * @brief Set the baud rate
     *
     * @param baudRate Baud rate
     * @throw std::out_of_range Baud rate is invalid or not supported
     */
    void setBaudRate(BaudRate baudRate);

    /**
     * @brief Get the character size
     *
     * @return CharacterSize Character size
     */
    CharacterSize getCharacterSize() const;

    /**
     * @brief Set the character size
     *
     * @param characterSize Character size
     * @throw std::out_of_range Character size is invalid or not supported
     */
    void setCharacterSize(CharacterSize characterSize);

    /**
     * @brief Get the flow control
     *
     * @return FlowControl Flow control
     */
    FlowControl getFlowControl() const;

    /**
     * @brief Set the flow control
     *
     * @param flowControl Flow control
     * @throw std::out_of_range Flow control is invalid or not supported
     */
    void setFlowControl(FlowControl flowControl);

    /**
     * @brief Get the parity
     *
     * @return Parity Parity
     */
    Parity getParity() const;

    /**
     * @brief Set the parity
     *
     * @param parity Parity
     * @throw std::out_of_range Parity is invalid or not supported
     */
    void setParity(Parity parity);

    /**
     * @brief Get the stop bit
     *
     * @return StopBit Stop bit
     */
    StopBit getStopBit() const;

    /**
     * @brief Set the stop bit
     *
     * @param stopBit Stop bit
     * @throw std::out_of_range Stop bit is invalid or not supported
     */
    void setStopBit(StopBit stopBit);

    /**
     * @brief Get the control line status
     *
     * @param controlLine Control line
     * @return true Control line is enabled
     * @return false Control line is disabled
     */
    bool getControlLine(ControlLine controlLine) const;

    /**
     * @brief Set the control line status
     *
     * @param controlLine Control line
     * @param state Control line status
     * @return true Successfully set control line status
     * @return false Failed to set control line status
     */
    bool setControlLine(ControlLine controlLine, bool state) const;
protected:
    /**
     * @brief Unique pointer of the SerialPortImpl class
     *
     */
    typedef std::unique_ptr<SerialPortImpl> SerialPortImplUniquePtr;

    /**
     * @brief Pointer to the implementation class
     *
     */
    SerialPortImplUniquePtr impl;
};

/**
 * @brief Shared pointer of the SerialPort class
 *
 */
typedef std::shared_ptr<SerialPort> SerialPortPtr;

/**
 * @brief Unique pointer of the SerialPort class
 *
 */
typedef std::unique_ptr<SerialPort> SerialPtrUniquePtr;

END_NAMESPACE_LIBSERIAL
