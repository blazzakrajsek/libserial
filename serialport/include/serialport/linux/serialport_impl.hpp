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
#include <iostream>
#include <termios.h>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>

BEGIN_NAMESPACE_LIBSERIAL

/**
 * @brief SerialPortImpl class
 *
 */
class SerialPortImpl final
{
public:
    /**
     * @brief Construct a new SerialPortImpl object
     *
     */
    explicit SerialPortImpl();

    /**
     * @brief Construct a new SerialPortImpl object
     *
     * @param portName Serial port name
     * @param baudRate Baud rate
     * @param characterSize Character size
     * @param flowControl Flow control
     * @param parity Parity
     * @param stopBit Stop bit
     */
    explicit SerialPortImpl(const std::string& portName,
        BaudRate baudRate = BaudRate::BAUD_RATE_DEFAULT,
        CharacterSize characterSize = CharacterSize::CHARACTER_SIZE_DEFAULT,
        FlowControl flowControl = FlowControl::FLOW_CONTROL_DEFAULT,
        Parity parity = Parity::PARITY_TYPE_DEFAULT,
        StopBit stopBit = StopBit::STOP_BIT_DEFAULT);

    /**
     * @brief Copy-construct a new SerialPortImpl object
     *
     * @param serialPortImpl Serial port implementation
     */
    SerialPortImpl(const SerialPortImpl& serialPortImpl) = delete;

    /**
     * @brief Move-construct a new SerialPortImpl object
     *
     * @param serialPortImpl Serial port implementation
     */
    SerialPortImpl(SerialPortImpl&& serialPortImpl) = delete;

    /**
     * @brief Copy-assignment operator
     *
     * @param serialPort Serial port implementation to copy-assign
     * @return SerialPortImpl& Assigned serial port implementation
     */
    SerialPortImpl& operator=(const SerialPortImpl& serialPortImpl) = delete;

    /**
     * @brief Move-assignment operator
     *
     * @param serialPort Serial port implementation to move-assign
     * @return SerialPortImpl& Assigned serial port implementation
     */
    SerialPortImpl& operator=(SerialPortImpl&& serialPortImpl) = delete;

    /**
     * @brief Destroy the SerialPortImpl object
     *
     */
    ~SerialPortImpl() noexcept;

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
     * @brief Reopen serial port
     *
     * @throw std::runtime_error Unsupported open mode
     * @throw std::runtime_error Unable to open serial port
     * @throw std::runtime_error Unable to get port settings
     * @throw std::runtime_error Unable to set exclusive mode
     * @throw std::runtime_error Unable to set port settings
     */
    void reopen();

    /**
     * @brief Update serial port settings
     *
     * @throw std::runtime_error Unable to get port settings
     * @throw std::runtime_error Unable to set port settings
     */
    void updatePortSettings() const;

    /**
     * @brief Get the serial port settings
     *
     * @param portSettings Serial port settings
     * @return true Successfully obtained serial port settings
     * @return false Failed to obtain serial port settings
     */
    bool getPortSettings(struct termios& portSettings) const;

    /**
     * @brief Prepare/update serial port settings
     *
     * @param portSettings Serial port settings
     */
    void preparePortSettings(struct termios& portSettings) const;

    /**
     * @brief Set the serial port settings
     *
     * @param portSettings Serial port settings
     * @return true Successfully applied serial port settings
     * @return false Failed to apply serial port settings
     */
    bool setPortSettings(const struct termios& portSettings) const;

    /**
     * @brief Get the native control line value
     *
     * @param controlLine Control line
     * @return int Control line value
     */
    int getNativeControlLine(ControlLine controlLine) const;

    /**
     * @brief Get the native control line status
     *
     * @param controlLine Control line value
     * @param state Control line state
     * @return int Result of the system call
     */
    int getControlLine(int controlLine, bool& state) const;

    /**
     * @brief Set the native control line status
     *
     * @param controlLine Control line value
     * @param state Control line state
     * @return int Result of the system call
     */
    int setControlLine(int controlLine, bool state) const;

    /**
     * @brief Serial port file descriptor
     *
     */
    int fileDescriptor;

    /**
     * @brief Serial port settings
     *
     */
    struct termios portSettings;

    /**
     * @brief Serial port open mode
     *
     */
    std::ios_base::openmode openMode;

    /**
     * @brief Serial port name
     *
     */
    std::string portName;

    /**
     * @brief Baud rate
     *
     */
    BaudRate baudRate;

    /**
     * @brief Character size
     *
     */
    CharacterSize characterSize;

    /**
     * @brief Flow control
     *
     */
    FlowControl flowControl;

    /**
     * @brief Parity
     *
     */
    Parity parity;

    /**
     * @brief Stop bit
     *
     */
    StopBit stopBit;
};

END_NAMESPACE_LIBSERIAL
