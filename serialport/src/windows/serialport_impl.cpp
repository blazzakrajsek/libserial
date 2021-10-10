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
#include <windows.h>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport/windows/serialport_impl.hpp>

BEGIN_NAMESPACE_LIBSERIAL

SerialPortImpl::SerialPortImpl() :
    SerialPortImpl(DEFAULT_PORT_NAME)
{

}

SerialPortImpl::SerialPortImpl(const std::string& portName, BaudRate baudRate,
    CharacterSize characterSize, FlowControl flowControl, Parity parity,
    StopBit stopBit) :
    fileDescriptor{INVALID_FILE_DESCRIPTOR}, openMode(std::ios_base::in | std::ios_base::out),
    portName{portName}, baudRate{baudRate}, characterSize{characterSize},
    flowControl{flowControl}, parity{parity}, stopBit{stopBit}
{

}

SerialPortImpl::~SerialPortImpl() noexcept
{
    try
    {
        close();
    }
    catch(...)
    {
    }
}

bool SerialPortImpl::isOpen() const
{
    return (fileDescriptor != INVALID_FILE_DESCRIPTOR);
}

void SerialPortImpl::open(std::ios_base::openmode openMode)
{
    // Do nothing on an open port
    if (isOpen())
        return;

    // Prepare open mode
    DWORD flags {};
    if (openMode == (std::ios_base::in | std::ios_base::out))
        flags |= GENERIC_READ | GENERIC_WRITE;
    else if (openMode == std::ios_base::in)
        flags |= GENERIC_READ;
    else if (openMode == std::ios_base::out)
        flags |= GENERIC_WRITE;
    else
        throw std::runtime_error("Unsupported open mode");

    // Store open mode
    this->openMode = openMode;

    // Open serial port
    const std::string fileName{std::string(SERIAL_PORT_PREFIX) + portName};
    fileDescriptor = CreateFile(fileName.c_str(), flags, 0, NULL, OPEN_EXISTING, 0, 0);

    // Is serial port open?
    if (!isOpen())
        throw std::runtime_error("Unable to open serial port");

    // Store current port settings
    if (!getPortSettings(portSettings))
    {
        // Close serial port and reset file descriptor
        CloseHandle(fileDescriptor);
        fileDescriptor = INVALID_FILE_DESCRIPTOR;
        throw std::runtime_error("Unable to get port settings");
    }

    // Store current port timeout settings
    if (!getPortTimeoutSettings(timeoutSettings))
    {
        // Close serial port and reset file descriptor
        CloseHandle(fileDescriptor);
        fileDescriptor = INVALID_FILE_DESCRIPTOR;
        throw std::runtime_error("Unable to get port timeout settings");
    }

    // Apply new port settings
    try
    {
        updatePortSettings();
    }
    catch(...)
    {
        close();
        throw;
    }
}

void SerialPortImpl::close()
{
    // Do nothing on a closed port
    if (!isOpen())
        return;

    // Restore previous serial port settings and timeouts
    auto settingsReset = setPortSettings(portSettings);
    settingsReset &= setPortTimeoutSettings(timeoutSettings);

    // Close serial port and reset file descriptor
    CloseHandle(fileDescriptor);
    fileDescriptor = INVALID_FILE_DESCRIPTOR;

    // Throw if restoring previous settings or timeouts failed
    if (!settingsReset)
        throw std::runtime_error("Unable to set port settings");
}

bool SerialPortImpl::setExclusive(bool exclusive)
{
    // Do nothing on a closed port
    if (!isOpen())
        return false;

    // Windows supports only exclusive ports
    return exclusive;
}

size_t SerialPortImpl::read(char* buffer, size_t size) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    DWORD read{0};
    return (ReadFile(fileDescriptor, buffer, size, &read, NULL) ? read : 0);
}

size_t SerialPortImpl::read(std::string& buffer) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    // Initialize result and buffer
    int result{0};
    buffer.clear();

    // Initialize chunk size
    constexpr short chunkSize{64};
    char chunkData[chunkSize];
    DWORD readCount{0};

    do
    {
        // Continue to read data up to a size of a chunk
        if (!ReadFile(fileDescriptor, &chunkData, chunkSize, &readCount, NULL))
            break;

        buffer.append(reinterpret_cast<const char*>(&chunkData), readCount);
        result += readCount;
    }
    while (readCount > 0);

    // Return size of the read data
    return result;
}

bool SerialPortImpl::write(char data) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return false;

    DWORD written{0};
    return (WriteFile(fileDescriptor, &data, sizeof(data), &written, NULL) ? (written == sizeof(written)) : false);
}

size_t SerialPortImpl::write(const char* buffer, size_t size) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    DWORD written{0};
    return (WriteFile(fileDescriptor, buffer, size, &written, NULL) ? written : 0);
}

size_t SerialPortImpl::write(const std::string& buffer) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    DWORD written{0};
    return (WriteFile(fileDescriptor, buffer.c_str(), buffer.size(), &written, NULL) ? written : 0);
}

bool SerialPortImpl::drain() const
{
    // Not supported on Windows
    return false;
}

bool SerialPortImpl::flushInput() const
{
    return (isOpen() ? (PurgeComm(fileDescriptor, PURGE_RXCLEAR)) : false);
}

bool SerialPortImpl::flushOutput() const
{
    return (isOpen() ? (PurgeComm(fileDescriptor, PURGE_TXCLEAR)) : false);
}

bool SerialPortImpl::flushInputOutput() const
{
    return (isOpen() ? (PurgeComm(fileDescriptor, PURGE_RXCLEAR | PURGE_TXCLEAR)) : false);
}

size_t SerialPortImpl::getInputQueueCount() const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    // Read input queue count
    COMSTAT comStat{};
    DWORD errors{0};
    return (ClearCommError(fileDescriptor, &errors, &comStat) ? comStat.cbInQue : 0);
}

size_t SerialPortImpl::getOutputQueueCount() const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    // Read output queue count
    COMSTAT comStat{};
    DWORD errors{0};
    return (ClearCommError(fileDescriptor, &errors, &comStat) ? comStat.cbOutQue : 0);
}

std::string SerialPortImpl::getPortName() const
{
    return portName;
}

void SerialPortImpl::setPortName(const std::string& portName)
{
    this->portName = portName;
    reopen();
}

BaudRate SerialPortImpl::getBaudRate() const
{
    return baudRate;
}

void SerialPortImpl::setBaudRate(BaudRate baudRate)
{
    // Baud rate supported?
    if (!LibSerial::isBaudRateSupported(baudRate))
        throw std::out_of_range("Baud rate not supported");

    this->baudRate = baudRate;
    updatePortSettings();
}

CharacterSize SerialPortImpl::getCharacterSize() const
{
    return characterSize;
}

void SerialPortImpl::setCharacterSize(CharacterSize characterSize)
{
    // Character size supported?
    if (!LibSerial::isCharacterSizeSupported(characterSize))
        throw std::out_of_range("Character size not supported");

    this->characterSize = characterSize;
    updatePortSettings();
}

FlowControl SerialPortImpl::getFlowControl() const
{
    return flowControl;
}

void SerialPortImpl::setFlowControl(FlowControl flowControl)
{
    // Flow control supported?
    if (!LibSerial::isFlowControlSupported(flowControl))
        throw std::out_of_range("Flow control not supported");

    drain();
    this->flowControl = flowControl;
    updatePortSettings();
}

Parity SerialPortImpl::getParity() const
{
    return parity;
}

void SerialPortImpl::setParity(Parity parity)
{
    // Parity supported?
    if (!LibSerial::isParitySupported(parity))
        throw std::out_of_range("Parity not supported");

    drain();
    this->parity = parity;
    updatePortSettings();
}

StopBit SerialPortImpl::getStopBit() const
{
    return stopBit;
}

void SerialPortImpl::setStopBit(StopBit stopBit)
{
    // Stop bit supported?
    if (!LibSerial::isStopBitSupported(stopBit))
        throw std::out_of_range("Stop bit not supported");

    drain();
    this->stopBit = stopBit;
    updatePortSettings();
}

bool SerialPortImpl::getControlLine(ControlLine controlLine) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return false;

    DWORD nativeValue{0};
    if (!GetCommModemStatus(fileDescriptor, &nativeValue))
        return false;

    bool status{true};
    if ((controlLine & ControlLine::LINE_DCD) == ControlLine::LINE_DCD)
        status &= ((nativeValue & MS_RLSD_ON) == MS_RLSD_ON);
    if ((controlLine & ControlLine::LINE_DSR) == ControlLine::LINE_DSR)
        status &= ((nativeValue & MS_DSR_ON) == MS_DSR_ON);
    if ((controlLine & ControlLine::LINE_CTS) == ControlLine::LINE_CTS)
        status &= ((nativeValue & MS_CTS_ON) == MS_CTS_ON);
    if ((controlLine & ControlLine::LINE_RI) == ControlLine::LINE_RI)
        status &= ((nativeValue & MS_RING_ON) == MS_RING_ON);
    return status;
}

bool SerialPortImpl::setControlLine(ControlLine controlLine, bool state) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return false;

    auto result{true};
    if ((controlLine & ControlLine::LINE_DTR) == ControlLine::LINE_DTR)
        result &= EscapeCommFunction(fileDescriptor, (state ? SETDTR : CLRDTR));
    if ((controlLine & ControlLine::LINE_RTS) == ControlLine::LINE_RTS)
        result &= EscapeCommFunction(fileDescriptor, (state ? SETRTS : CLRRTS));
    return result;
}

void SerialPortImpl::reopen()
{
    // Do nothing on a closed port
    if (!isOpen())
        return;

    // Close and open back the port
    close();
    open(openMode);
}

void SerialPortImpl::updatePortSettings() const
{
    // Do nothing on a closed port
    if (!isOpen())
        return;

    // Get port settings
    struct _DCB portSettings{};
    if (!getPortSettings(portSettings))
        throw std::runtime_error("Unable to get port settings");

    // Update port settings
    preparePortSettings(portSettings);

    // Apply port settings
    if (!setPortSettings(portSettings))
        throw std::runtime_error("Unable to set port settings");

    // Get port timeouts
    COMMTIMEOUTS timeouts{};
    if (!getPortTimeoutSettings(timeouts))
        throw std::runtime_error("Unable to get port timeout settings");

    // Apply port timeouts
    preparePortTimeoutSettings(timeouts);

    // Appy port timeouts
    if (!setPortTimeoutSettings(timeouts))
        throw std::runtime_error("Unable to set port timeout settings");
}

bool SerialPortImpl::getPortSettings(DCB& portSettings) const
{
    return GetCommState(fileDescriptor, &portSettings);
}

void SerialPortImpl::preparePortSettings(DCB& portSettings) const
{
    /*  The length of the structure, in bytes.
        The caller must set this member to sizeof(DCB).  */
    portSettings.DCBlength = sizeof(portSettings);

    /*  The baud rate at which the communications device operates. This member
        can be an actual baud rate value, or one of the following indexes.  */
    portSettings.BaudRate = static_cast<DWORD>(LibSerial::getBaudRateValue(baudRate));

    /*  If this member is TRUE, binary mode is enabled. Windows does not
        support nonbinary mode transfers, so this member must be TRUE.  */
    portSettings.fBinary = true;

    /* If this member is TRUE, parity checking is performed and errors are reported.  */
    switch (parity)
    {
        case Parity::PARITY_TYPE_ODD:
        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_MARK:
        case Parity::PARITY_TYPE_SPACE:
            portSettings.fParity = true;
            break;

        case Parity::PARITY_TYPE_NONE:
        default:
            portSettings.fParity = false;
            break;
    }

    /*  If this member is TRUE, the CTS (clear-to-send) signal is monitored for
        output flow control. If this member is TRUE and CTS is turned off, output
        is suspended until CTS is sent again.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_HARDWARE:
            portSettings.fOutxCtsFlow = true;
            break;

        case FlowControl::FLOW_CONTROL_SOFTWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.fOutxCtsFlow = false;
            break;
    }

    /*  If this member is TRUE, the DSR (data-set-ready) signal is monitored
        for output flow control. If this member is TRUE and DSR is turned off,
        output is suspended until DSR is sent again.  */
    portSettings.fOutxDsrFlow = false;

    /*  The DTR (data-terminal-ready) flow control. This member can be one of
        the following values:
        DTR_CONTROL_DISABLE:
            Disables the DTR line when the device is opened and leaves it disabled.
        DTR_CONTROL_ENABLE:
            Enables the DTR line when the device is opened and leaves it on.
        DTR_CONTROL_HANDSHAKE:
            Enables DTR handshaking. If handshaking is enabled, it is an error
            for the application to adjust the line by using the
            EscapeCommFunction function.  */
    portSettings.fDtrControl = DTR_CONTROL_DISABLE;

    /*  If this member is TRUE, the communications driver is sensitive to the
        state of the DSR signal. The driver ignores any bytes received, unless
        the DSR modem input line is high.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_HARDWARE:
            portSettings.fDsrSensitivity = true;
            break;

        case FlowControl::FLOW_CONTROL_SOFTWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.fDsrSensitivity = false;
            break;
    }

    /*  If this member is TRUE, transmission continues after the input buffer
        has come within XoffLim bytes of being full and the driver has transmitted
        the XoffChar character to stop receiving bytes. If this member is FALSE,
        transmission does not continue until the input buffer is within XonLim
        bytes of being empty and the driver has transmitted the XonChar character
        to resume reception.  */
    portSettings.fTXContinueOnXoff = false;

    /*  Indicates whether XON/XOFF flow control is used during transmission.
        If this member is TRUE, transmission stops when the XoffChar character is
        received and starts again when the XonChar character is received.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_SOFTWARE:
            portSettings.fOutX = true;
            break;

        case FlowControl::FLOW_CONTROL_HARDWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.fOutX = false;
            break;
    }

    /*  Indicates whether XON/XOFF flow control is used during reception.
        If this member is TRUE, the XoffChar character is sent when the input
        buffer comes within XoffLim bytes of being full, and the XonChar character
        is sent when the input buffer comes within XonLim bytes of being empty.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_SOFTWARE:
            portSettings.fInX = true;
            break;

        case FlowControl::FLOW_CONTROL_HARDWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.fInX = false;
            break;
    }

    /*  Indicates whether bytes received with parity errors are replaced with
        the character specified by the ErrorChar member. If this member is TRUE
        and the fParity member is TRUE, replacement occurs.  */
    portSettings.fErrorChar = false;

    /*  If this member is TRUE, null bytes are discarded when received.  */
    portSettings.fNull = false;

    /*  The RTS (request-to-send) flow control. This member can be one of
        the following values:
        RTS_CONTROL_DISABLE
            Disables the RTS line when the device is opened and leaves it disabled.
        RTS_CONTROL_ENABLE
            Enables the RTS line when the device is opened and leaves it on.
        RTS_CONTROL_HANDSHAKE
            Enables RTS handshaking. The driver raises the RTS line when the
            "type-ahead" (input) buffer is less than one-half full and lowers
            the RTS line when the buffer is more than three-quarters full.
            If handshaking is enabled, it is an error for the application to
            adjust the line by using the EscapeCommFunction function.
        RTS_CONTROL_TOGGLE
            Specifies that the RTS line will be high if bytes are available
            for transmission. After all buffered bytes have been sent, the
            RTS line will be low.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_HARDWARE:
            portSettings.fRtsControl = RTS_CONTROL_HANDSHAKE;
            break;

        case FlowControl::FLOW_CONTROL_SOFTWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.fRtsControl = RTS_CONTROL_DISABLE;
            break;
    }

    /*  If this member is TRUE, the driver terminates all read and write
        operations with an error status if an error occurs. The driver will
        not accept any further communications operations until the application
        has acknowledged the error by calling the ClearCommError function.  */
    portSettings.fAbortOnError = false;

    /*  Reserved; do not use.  */
    //portSettings.fDummy2

    /*  Reserved; must be zero.  */
    portSettings.wReserved = 0;

    /*  The minimum number of bytes in use allowed in the input buffer before
        flow control is activated to allow transmission by the sender. This
        assumes that either XON/XOFF, RTS, or DTR input flow control is
        specified in the fInX, fRtsControl, or fDtrControl members.  */
    //portSettings.XonLim = 64;

    /*  The minimum number of free bytes allowed in the input buffer before
        flow control is activated to inhibit the sender. Note that the sender
        may transmit characters after the flow control signal has been
        activated, so this value should never be zero. This assumes that
        either XON/XOFF, RTS, or DTR input flow control is specified in the
        fInX, fRtsControl, or fDtrControl members. The maximum number of bytes
        in use allowed is calculated by subtracting this value from the size,
        in bytes, of the input buffer.  */
    //portSettings.XoffLim = 64;

    /*  The number of bits in the bytes transmitted and received.  */
    portSettings.ByteSize = static_cast<BYTE>(LibSerial::getCharacterSizeValue(characterSize));

    /*  The parity scheme to be used. This member can be one of the
        following values:
        EVENPARITY
            Even parity.
        MARKPARITY
            Mark parity.
        NOPARITY
            No parity.
        ODDPARITY
            Odd parity.
        SPACEPARITY
            Space parity.
    */
    switch (parity)
    {
        case Parity::PARITY_TYPE_ODD:
            portSettings.Parity = ODDPARITY;
            break;

        case Parity::PARITY_TYPE_EVEN:
            portSettings.Parity = EVENPARITY;
            break;

        case Parity::PARITY_TYPE_MARK:
            portSettings.Parity = MARKPARITY;
            break;

        case Parity::PARITY_TYPE_SPACE:
            portSettings.Parity = SPACEPARITY;
            break;

        case Parity::PARITY_TYPE_NONE:
        default:
            portSettings.Parity = NOPARITY;
            break;
    }

    /*  The number of stop bits to be used. This member can be one
        of the following values:
        ONESTOPBIT
            1 stop bit.
        ONE5STOPBITS
            1.5 stop bits.
        TWOSTOPBITS
            2 stop bits.  */
    switch (stopBit)
    {
        case StopBit::STOP_BIT_ONE_HALF:
            portSettings.StopBits = ONE5STOPBITS;
            break;

        case StopBit::STOP_BIT_TWO:
            portSettings.StopBits = TWOSTOPBITS;
            break;

        case StopBit::STOP_BIT_ONE:
        default:
            portSettings.StopBits = ONESTOPBIT;
            break;

    }

    /*  The value of the XON character for both transmission and reception.  */
    portSettings.XonChar = XON;

    /*  The value of the XOFF character for both transmission and reception.  */
    portSettings.XoffChar = XOFF;

    /*  The value of the character used to replace bytes received with a parity error.  */
    portSettings.ErrorChar = NUL;

    /*  The value of the character used to signal the end of data.  */
    portSettings.EofChar = NUL;

    /*  The value of the character used to signal an event.  */
    portSettings.EvtChar = NUL;

    /*  Reserved; do not use.  */
    //portSettings.wReserved1
}

bool SerialPortImpl::setPortSettings(DCB& portSettings) const
{
    return SetCommState(fileDescriptor, &portSettings);
}

bool SerialPortImpl::getPortTimeoutSettings(COMMTIMEOUTS& timeoutSettings) const
{
    return GetCommTimeouts(fileDescriptor, &timeoutSettings);
}

void SerialPortImpl::preparePortTimeoutSettings(COMMTIMEOUTS& timeoutSettings) const
{
    /*  The maximum time allowed to elapse before the arrival of the next
        byte on the communications line, in milliseconds. If the interval
        between the arrival of any two bytes exceeds this amount, the
        ReadFile operation is completed and any buffered data is returned.
        A value of zero indicates that interval time-outs are not used.
        A value of MAXDWORD, combined with zero values for both the
        ReadTotalTimeoutConstant and ReadTotalTimeoutMultiplier members,
        specifies that the read operation is to return immediately with
        the bytes that have already been received, even if no bytes have
        been received.  */
    timeoutSettings.ReadIntervalTimeout = MAXDWORD;

    /*  The multiplier used to calculate the total time-out period for
        read operations, in milliseconds. For each read operation, this
        value is multiplied by the requested number of bytes to be read.  */
    timeoutSettings.ReadTotalTimeoutMultiplier = 0;

    /*  A constant used to calculate the total time-out period for read
        operations, in milliseconds. For each read operation, this value
        is added to the product of the ReadTotalTimeoutMultiplier member
        and the requested number of bytes. A value of zero for both the
        ReadTotalTimeoutMultiplier and ReadTotalTimeoutConstant members
        indicates that total time-outs are not used for read operations.  */
    timeoutSettings.ReadTotalTimeoutConstant = 0;

    /*  The multiplier used to calculate the total time-out period for
        write operations, in milliseconds. For each write operation,
        this value is multiplied by the number of bytes to be written.  */
    timeoutSettings.WriteTotalTimeoutMultiplier = 100;

    /*  A constant used to calculate the total time-out period for write
        operations, in milliseconds. For each write operation, this value
        is added to the product of the WriteTotalTimeoutMultiplier member
        and the number of bytes to be written. A value of zero for both
        the WriteTotalTimeoutMultiplier and WriteTotalTimeoutConstant
        members indicates that total time-outs are not used for write
        operations.  */
    timeoutSettings.WriteTotalTimeoutConstant = 1000;
}

bool SerialPortImpl::setPortTimeoutSettings(COMMTIMEOUTS& timeoutSettings) const
{
    return SetCommTimeouts(fileDescriptor, &timeoutSettings);
}

END_NAMESPACE_LIBSERIAL
