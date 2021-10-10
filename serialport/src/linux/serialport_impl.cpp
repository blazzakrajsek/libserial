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

#include <iostream>
#include <stdexcept>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <serialport/namespace.hpp>
#include <serialport/properties.hpp>
#include <serialport/linux/serialport_impl.hpp>

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
    int descriptorFlags{O_NOCTTY | O_NONBLOCK};
    if (openMode == (std::ios_base::in | std::ios_base::out))
        descriptorFlags |= O_RDWR;
    else if (openMode == std::ios_base::in)
        descriptorFlags |= O_RDONLY;
    else if (openMode == std::ios_base::out)
        descriptorFlags |= O_WRONLY;
    else
        throw std::runtime_error("Unsupported open mode");

    // Store open mode
    this->openMode = openMode;

    // Open serial port
    fileDescriptor = systemCall(::open, portName.c_str(), descriptorFlags);

    // Is serial port open?
    if (!isOpen())
        throw std::runtime_error("Unable to open serial port");

    // Store current port settings
    if (!getPortSettings(portSettings))
    {
        // Close serial port and reset file descriptor
        systemCall(::close, fileDescriptor);
        fileDescriptor = INVALID_FILE_DESCRIPTOR;
        throw std::runtime_error("Unable to get port settings");
    }

    // Set exclusive mode
    if (!setExclusive(true))
        throw std::runtime_error("Unable to set exclusive mode");

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

    // Restore previous serial port settings
    auto settingsReset = setPortSettings(portSettings);

    // Close serial port and reset file descriptor
    systemCall(::close, fileDescriptor);
    fileDescriptor = INVALID_FILE_DESCRIPTOR;

    // Throw if restoring previous settings failed
    if (!settingsReset)
        throw std::runtime_error("Unable to set port settings");
}

bool SerialPortImpl::setExclusive(bool exclusive)
{
    // Do nothing on a closed port
    if (!isOpen())
        return false;

    // Update exclusive mode
    if (exclusive)
        return (systemCall(ioctl, fileDescriptor, TIOCEXCL) == 0);
    else
        return (systemCall(ioctl, fileDescriptor, TIOCNXCL) == 0);
}

size_t SerialPortImpl::read(char* buffer, size_t size) const
{
    return (isOpen() ? systemCall(::read, fileDescriptor, buffer, size) : 0);
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
    int readCount{0};

    do
    {
        // Continue to read data up to a size of a chunk
        readCount = systemCall(::read, fileDescriptor, &chunkData, chunkSize);
        buffer.append((const char*)&chunkData, readCount);
        result += readCount;
    }
    while (readCount > 0);

    // Return size of the read data
    return result;
}

bool SerialPortImpl::write(char data) const
{
    return (isOpen() ? (systemCall(::write, fileDescriptor, &data, sizeof(data)) == sizeof(data)) : false);
}

size_t SerialPortImpl::write(const char* buffer, size_t size) const
{
    return (isOpen() ? systemCall(::write, fileDescriptor, buffer, size) : 0);
}

size_t SerialPortImpl::write(const std::string& buffer) const
{
    return (isOpen() ? systemCall(::write, fileDescriptor, buffer.c_str(), buffer.size()) : 0);
}

bool SerialPortImpl::drain() const
{
    return (isOpen() ? (systemCall(tcdrain, fileDescriptor) == 0) : false);
}

bool SerialPortImpl::flushInput() const
{
    return (isOpen() ? (systemCall(tcflush, fileDescriptor, TCIFLUSH) == 0) : false);
}

bool SerialPortImpl::flushOutput() const
{
    return (isOpen() ? (systemCall(tcflush, fileDescriptor, TCOFLUSH) == 0) : false);
}

bool SerialPortImpl::flushInputOutput() const
{
    return (isOpen() ? (systemCall(tcflush, fileDescriptor, TCIOFLUSH) == 0) : false);
}

size_t SerialPortImpl::getInputQueueCount() const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    // Read input queue count
    size_t value{0};
    const auto result = systemCall(ioctl, fileDescriptor, TIOCINQ, &value);
    return ((result == 0) ? value : 0);
}

size_t SerialPortImpl::getOutputQueueCount() const
{
    // Do nothing on a closed port
    if (!isOpen())
        return 0;

    // Read output queue count
    size_t value{0};
    const auto result = systemCall(ioctl, fileDescriptor, TIOCOUTQ, &value);
    return ((result == 0) ? value : 0);
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

    bool status{false};
    getControlLine(getNativeControlLine(controlLine), status);
    return status;
}

bool SerialPortImpl::setControlLine(ControlLine controlLine, bool state) const
{
    // Do nothing on a closed port
    if (!isOpen())
        return false;

    return (setControlLine(getNativeControlLine(controlLine), state) == 0);
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
    struct termios portSettings{};
    if (!getPortSettings(portSettings))
        throw std::runtime_error("Unable to get port settings");

    // Update port settings
    preparePortSettings(portSettings);

    // Apply port settings
    if (!setPortSettings(portSettings))
        throw std::runtime_error("Unable to set port settings");
}

bool SerialPortImpl::getPortSettings(struct termios& portSettings) const
{
    return (systemCall(tcgetattr, fileDescriptor, &portSettings) == 0);
}

void SerialPortImpl::preparePortSettings(struct termios& portSettings) const
{
    /*  termios structure is described at: https://man7.org/linux/man-pages/man3/termios.3.html  */


    /*  Input mode flags  */


    /*  IGNBRK: Ignore BREAK condition on input.  */
    portSettings.c_iflag &= ~IGNBRK;

    /*  BRKINT: If IGNBRK is set, a BREAK is ignored.  If it is not set but
        BRKINT is set, then a BREAK causes the input and output queues
        to be flushed, and if the terminal is the controlling terminal
        of a foreground process group, it will cause a SIGINT to be
        sent to this foreground process group.  When neither IGNBRK
        nor BRKINT are set, a BREAK reads as a null byte ('\0'),
        except when PARMRK is set, in which case it reads as the
        sequence \377 \0 \0.  */
    portSettings.c_iflag &= ~BRKINT;

    /*  IGNPAR: Ignore framing errors and parity errors.  */
    switch (parity)
    {
        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_ODD:
            portSettings.c_iflag &= ~IGNPAR;
            break;

        case Parity::PARITY_TYPE_NONE:
        default:
            portSettings.c_iflag |= IGNPAR;
            break;
    }

    /*  PARMRK: If this bit is set, input bytes with parity or framing errors
        are marked when passed to the program.  This bit is meaningful
        only when INPCK is set and IGNPAR is not set.  The way erro‐
        neous bytes are marked is with two preceding bytes, \377 and
        \0.  Thus, the program actually reads three bytes for one
        erroneous byte received from the terminal.  If a valid byte
        has the value \377, and ISTRIP (see below) is not set, the
        program might confuse it with the prefix that marks a parity
        error.  Therefore, a valid byte \377 is passed to the program
        as two bytes, \377 \377, in this case.

        If neither IGNPAR nor PARMRK is set, read a character with a
        parity error or framing error as \0.  */
    portSettings.c_iflag &= ~PARMRK;

    /*  INPCK: Enable input parity checking.  */
    switch (parity)
    {
        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_ODD:
            portSettings.c_iflag |= INPCK;
            break;

        case Parity::PARITY_TYPE_NONE:
        default:
            portSettings.c_iflag &= ~INPCK;
            break;
    }

    /*  ISTRIP: Strip off eighth bit.  */
    portSettings.c_iflag &= ~ISTRIP;

    /*  INLCR: Translate NL to CR on input.  */
    portSettings.c_iflag &= ~INLCR;

    /*  IGNCR: Ignore carriage return on input.  */
    portSettings.c_iflag &= ~IGNCR;

    /*  ICRNL: Translate carriage return to newline on input (unless IGNCR is set).  */
    portSettings.c_iflag &= ~ICRNL;

    /*  IUCLC: (not in POSIX) Map uppercase characters to lowercase on input.  */
    portSettings.c_iflag &= ~IUCLC;

    /*  IXON: Enable XON/XOFF flow control on output.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_SOFTWARE:
            portSettings.c_iflag |= IXON;
            break;

        case FlowControl::FLOW_CONTROL_HARDWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.c_iflag &= ~IXON;
            break;
    }

    /*  IXANY: (XSI) Typing any character will restart stopped output. (The
        default is to allow just the START character to restart output.)  */
    portSettings.c_iflag &= ~IXANY;

    /*  IXOFF: Enable XON/XOFF flow control on input.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_SOFTWARE:
            portSettings.c_iflag |= IXOFF;
            break;

        case FlowControl::FLOW_CONTROL_HARDWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.c_iflag &= ~IXOFF;
            break;
    }

    /*  IMAXBEL: (not in POSIX) Ring bell when input queue is full. Linux does not
        implement this bit, and acts as if it is always set.  */
    portSettings.c_iflag &= ~IMAXBEL;

    /*  IUTF8: (since Linux 2.6.4) (not in POSIX) Input is UTF8; this allows character-erase to
        be correctly performed in cooked mode.  */
    portSettings.c_iflag &= ~IUTF8;


    /*  Output mode flags  */


    /*  OPOST: Enable implementation-defined output processing.  */
    portSettings.c_oflag &= ~OPOST;

    /*  OLCUC: (not in POSIX) Map lowercase characters to uppercase on output.  */
    portSettings.c_oflag &= ~OLCUC;

    /*  ONLCR: (XSI) Map NL to CR-NL on output.  */
    portSettings.c_oflag &= ~ONLCR;

    /*  OCRNL: Map CR to NL on output.  */
    portSettings.c_oflag &= ~OCRNL;

    /*  ONOCR: Don't output CR at column 0.  */
    portSettings.c_oflag &= ~ONOCR;

    /*  ONLRET: Don't output CR.  */
    portSettings.c_oflag &= ~ONLRET;

    /*  OFILL: Send fill characters for a delay, rather than using a timed delay.  */
    portSettings.c_oflag &= ~OFILL;

    /*  OFDEL: Fill character is ASCII DEL (0177). If unset, fill character is
        ASCII NUL ('\0').  (Not implemented on Linux.)  */
    portSettings.c_oflag &= ~OFDEL;

    /*  NLDLY: Newline delay mask. Values are NL0 and NL1. [requires _BSD_SOURCE
        or _SVID_SOURCE or _XOPEN_SOURCE]  */
    portSettings.c_oflag &= ~NLDLY;
    //portSettings.c_oflag |= NL0;
    //portSettings.c_oflag |= NL1;

    /*  CRDLY: Carriage return delay mask. Values are CR0, CR1, CR2, or CR3.
        [requires _BSD_SOURCE or _SVID_SOURCE or _XOPEN_SOURCE]  */
    portSettings.c_oflag &= ~CRDLY;
    //portSettings.c_oflag |= CR0;
    //portSettings.c_oflag |= CR1;
    //portSettings.c_oflag |= CR2;
    //portSettings.c_oflag |= CR3;

    /*  TABDLY: Horizontal tab delay mask. Values are TAB0, TAB1, TAB2, TAB3
        (or XTABS, but see the BUGS section).  A value of TAB3, that
        is, XTABS, expands tabs to spaces (with tab stops every eight
        columns).  [requires _BSD_SOURCE or _SVID_SOURCE or _XOPEN_SOURCE]  */
    portSettings.c_oflag &= ~TABDLY;
    //portSettings.c_oflag |= TAB0;
    //portSettings.c_oflag |= TAB1;
    //portSettings.c_oflag |= TAB2;
    //portSettings.c_oflag |= TAB3;

    /*  BSDLY: Backspace delay mask. Values are BS0 or BS1. (Has never been implemented.)
        [requires _BSD_SOURCE or _SVID_SOURCE or _XOPEN_SOURCE]  */
    portSettings.c_oflag &= ~BSDLY;
    //portSettings.c_oflag |= BS0;
    //portSettings.c_oflag |= BS1;

    /*  VTDLY: Vertical tab delay mask. Values are VT0 or VT1.  */
    portSettings.c_oflag &= ~VTDLY;
    //portSettings.c_oflag |= VT0;
    //portSettings.c_oflag |= VT1;

    /*  FFDLY: Form feed delay mask. Values are FF0 or FF1. [requires _BSD_SOURCE or
        _SVID_SOURCE or _XOPEN_SOURCE]  */
    portSettings.c_oflag &= ~FFDLY;
    //portSettings.c_oflag |= FF0;
    //portSettings.c_oflag |= FF1;


    /*  Control mode flags  */


    /*  CBAUD: (not in POSIX) Baud speed mask (4+1 bits). [requires _BSD_SOURCE or _SVID_SOURCE]  */
    cfsetspeed(&portSettings, static_cast<speed_t>(LibSerial::getBaudRateValue(baudRate)));

    /*  CBAUDEX: (not in POSIX) Extra baud speed mask (1 bit), included in
        CBAUD. [requires _BSD_SOURCE or _SVID_SOURCE]

        (POSIX says that the baud speed is stored in the termios
        structure without specifying where precisely, and provides
        cfgetispeed() and cfsetispeed() for getting at it.  Some sys‐
        tems use bits selected by CBAUD in c_cflag, other systems use
        separate fields, for example, sg_ispeed and sg_ospeed.)  */

    /*  CSIZE: Character size mask. Values are CS5, CS6, CS7, or CS8.  */
    portSettings.c_cflag &= (~CSIZE);
    portSettings.c_cflag |= (CSIZE & static_cast<tcflag_t>(LibSerial::getCharacterSizeValue(characterSize)));

    /*  CSTOPB: Set two stop bits, rather than one.  */
    switch (stopBit)
    {
        case StopBit::STOP_BIT_TWO:
            portSettings.c_cflag |= CSTOPB;
            break;

        case StopBit::STOP_BIT_ONE:
        default:
            portSettings.c_cflag &= ~CSTOPB;
            break;
    }

    /*  CREAD: Enable receiver.  */
    portSettings.c_cflag |= CREAD;

    /*  PARENB: Enable parity generation on output and parity checking for input.  */
    switch (parity)
    {
        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_ODD:
        case Parity::PARITY_TYPE_MARK:
        case Parity::PARITY_TYPE_SPACE:
            portSettings.c_cflag |= PARENB;
            break;

        case Parity::PARITY_TYPE_NONE:
        default:
            portSettings.c_cflag &= ~PARENB;
            break;
    }

    /*  PARODD: If set, then parity for input and output is odd; otherwise
        even parity is used.  */
    switch (parity)
    {
        case Parity::PARITY_TYPE_ODD:
        case Parity::PARITY_TYPE_MARK:
            portSettings.c_cflag |= PARODD;
            break;

        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_NONE:
        case Parity::PARITY_TYPE_SPACE:
        default:
            portSettings.c_cflag &= ~PARODD;
            break;
    }

    /*  HUPCL: Lower modem control lines after last process closes the device (hang up).  */
    portSettings.c_cflag &= ~HUPCL;

    /*  CLOCAL: Ignore modem control lines.  */
    portSettings.c_cflag |= CLOCAL;

    /*  LOBLK: (not in POSIX) Block output from a noncurrent shell layer.
        For use by shl (shell layers). (Not implemented on Linux.)  */
    //portSettings.c_cflag &= ~LOBLK;

    /*  CIBAUD: (not in POSIX) Mask for input speeds. The values for the
        CIBAUD bits are the same as the values for the CBAUD bits,
        shifted left IBSHIFT bits.  [requires _BSD_SOURCE or
        _SVID_SOURCE] (Not implemented on Linux.)  */

    /*  CMSPAR: (not in POSIX) Use "stick" (mark/space) parity (supported on
        certain serial devices): if PARODD is set, the parity bit is
        always 1; if PARODD is not set, then the parity bit is always
        0. [requires _BSD_SOURCE or _SVID_SOURCE]  */
    switch (parity)
    {
        case Parity::PARITY_TYPE_MARK:
        case Parity::PARITY_TYPE_SPACE:
            portSettings.c_cflag |= CMSPAR;
            break;

        case Parity::PARITY_TYPE_EVEN:
        case Parity::PARITY_TYPE_ODD:
        case Parity::PARITY_TYPE_NONE:
        default:
            portSettings.c_cflag &= ~CMSPAR;
            break;
    }

    /*  CRTSCTS: (not in POSIX) Enable RTS/CTS (hardware) flow control.
        [requires _BSD_SOURCE or _SVID_SOURCE]  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_HARDWARE:
            portSettings.c_cflag |= CRTSCTS;
            break;

        case FlowControl::FLOW_CONTROL_SOFTWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.c_cflag &= ~CRTSCTS;
            break;
    }


    /*  Local modes flags  */


    /*  ISIG: When any of the characters INTR, QUIT, SUSP, or DSUSP are
        received, generate the corresponding signal.  */
    portSettings.c_lflag &= ~ISIG;

    /*  ICANON: Enable canonical mode (described below).  */
    portSettings.c_lflag &= ~ICANON;

    /*  XCASE: (not in POSIX; not supported under Linux) If ICANON is also
        set, terminal is uppercase only. Input is converted to lower‐
        case, except for characters preceded by \. On output, upper‐
        case characters are preceded by \ and lowercase characters are
        converted to uppercase. [requires _BSD_SOURCE or _SVID_SOURCE
        or _XOPEN_SOURCE]  */
    portSettings.c_lflag &= ~XCASE;

    /*  ECHO: Echo input characters.  */
    portSettings.c_lflag &= ~ECHO;

    /*  ECHOE: If ICANON is also set, the ERASE character erases the preced‐
        ing input character, and WERASE erases the preceding word.  */
    portSettings.c_lflag &= ~ECHOE;

    /*  ECHOK: If ICANON is also set, the KILL character erases the current line.  */
    portSettings.c_lflag &= ~ECHOK;

    /*  ECHONL: If ICANON is also set, echo the NL character even if ECHO is not set.  */
    portSettings.c_lflag &= ~ECHONL;

    /*  NOFLSH: Disable flushing the input and output queues when generating
        signals for the INT, QUIT, and SUSP characters.  */
    portSettings.c_lflag &= ~NOFLSH;

    /*  TOSTOP: Send the SIGTTOU signal to the process group of a background
        process which tries to write to its controlling terminal.  */
    portSettings.c_lflag &= ~TOSTOP;

    /*  ECHOCTL: (not in POSIX) If ECHO is also set, terminal special charac‐
        ters other than TAB, NL, START, and STOP are echoed as ^X,
        where X is the character with ASCII code 0x40 greater than the
        special character. For example, character 0x08 (BS) is echoed
        as ^H. [requires _BSD_SOURCE or _SVID_SOURCE]  */
    portSettings.c_lflag &= ~ECHOCTL;

    /*  ECHOPRT: (not in POSIX) If ICANON and ECHO are also set, characters are
        printed as they are being erased.  [requires _BSD_SOURCE or _SVID_SOURCE]  */
    portSettings.c_lflag &= ~ECHOPRT;

    /*  ECHOKE: (not in POSIX) If ICANON is also set, KILL is echoed by eras‐
        ing each character on the line, as specified by ECHOE and
        ECHOPRT.  [requires _BSD_SOURCE or _SVID_SOURCE]  */
    portSettings.c_lflag &= ~ECHOKE;

    /*  FLUSHO: (not in POSIX; not supported under Linux) Output is being
        flushed. This flag is toggled by typing the DISCARD charac‐
        ter. [requires _BSD_SOURCE or _SVID_SOURCE]  */
    portSettings.c_lflag &= ~FLUSHO;

    /*  PENDIN: (not in POSIX; not supported under Linux) All characters in
        the input queue are reprinted when the next character is read.
        (bash(1) handles typeahead this way.) [requires _BSD_SOURCE or _SVID_SOURCE]  */
    portSettings.c_lflag &= ~PENDIN;

    /*  DEFECHO: (not in POSIX) Echo only when a process is reading.  (Not implemented on Linux.)  */
    //portSettings.c_lflag &= ~DEFECHO;

    /*  IEXTEN: Enable implementation-defined input processing. This flag, as
        well as ICANON must be enabled for the special characters
        EOL2, LNEXT, REPRINT, WERASE to be interpreted, and for the
        IUCLC flag to be effective.  */
    portSettings.c_lflag &= ~IEXTEN;


    /*  Terminal special characters  */


    /*  An individual terminal special character can be disabled by setting
        the value of the corresponding c_cc element to _POSIX_VDISABLE.

        The above symbolic subscript values are all different, except that
        VTIME, VMIN may have the same value as VEOL, VEOF, respectively.  In
        noncanonical mode the special character meaning is replaced by the
        timeout meaning.  For an explanation of VMIN and VTIME, see the
        description of noncanonical mode below.  */

    /*  VDISCARD: (not in POSIX; not supported under Linux; 017, SI, Ctrl-O)
        Toggle: start/stop discarding pending output. Recognized when
        IEXTEN is set, and then not passed as input.  */
    portSettings.c_cc[VDISCARD] = _POSIX_VDISABLE;

    /*  VDSUSP: (not in POSIX; not supported under Linux; 031, EM, Ctrl-Y)
        Delayed suspend character (DSUSP): send SIGTSTP signal when
        the character is read by the user program. Recognized when
        IEXTEN and ISIG are set, and the system supports job control,
        and then not passed as input.  */
    //portSettings.c_cc[VDSUSP] = _POSIX_VDISABLE;

    /*  VEOF: (004, EOT, Ctrl-D) End-of-file character (EOF). More pre‐
        cisely: this character causes the pending tty buffer to be
        sent to the waiting user program without waiting for end-of-
        line. If it is the first character of the line, the read(2)
        in the user program returns 0, which signifies end-of-file.
        Recognized when ICANON is set, and then not passed as input.  */
    portSettings.c_cc[VEOF] = _POSIX_VDISABLE;

    /*  VEOL: (0, NUL) Additional end-of-line character (EOL). Recognized
        when ICANON is set.  */
    portSettings.c_cc[VEOL] = _POSIX_VDISABLE;

    /*  VEOL2 (not in POSIX; 0, NUL) Yet another end-of-line character
        (EOL2). Recognized when ICANON is set.  */
    portSettings.c_cc[VEOL2] = _POSIX_VDISABLE;

    /*  VERASE: (0177, DEL, rubout, or 010, BS, Ctrl-H, or also #) Erase char‐
        acter (ERASE). This erases the previous not-yet-erased char‐
        acter, but does not erase past EOF or beginning-of-line. Rec‐
        ognized when ICANON is set, and then not passed as input.  */
    portSettings.c_cc[VERASE] = _POSIX_VDISABLE;

    /*  VINTR: (003, ETX, Ctrl-C, or also 0177, DEL, rubout) Interrupt char‐
        acter (INTR).  Send a SIGINT signal.  Recognized when ISIG is
        set, and then not passed as input.  */
    portSettings.c_cc[VINTR] = _POSIX_VDISABLE;

    /*  VKILL: (025, NAK, Ctrl-U, or Ctrl-X, or also @) Kill character
        (KILL). This erases the input since the last EOF or begin‐
        ning-of-line. Recognized when ICANON is set, and then not
        passed as input.  */
    portSettings.c_cc[VKILL] = _POSIX_VDISABLE;

    /*  VLNEXT: (not in POSIX; 026, SYN, Ctrl-V) Literal next (LNEXT). Quotes
        the next input character, depriving it of a possible special
        meaning.  Recognized when IEXTEN is set, and then not passed
        as input.  */
    portSettings.c_cc[VLNEXT] = _POSIX_VDISABLE;

    /*  VMIN: Minimum number of characters for noncanonical read (MIN).
        This will block until n bytes are received.  */
    portSettings.c_cc[VMIN] = _POSIX_VDISABLE;

    /*  VQUIT: (034, FS, Ctrl-\) Quit character (QUIT). Send SIGQUIT signal.
        Recognized when ISIG is set, and then not passed as input.  */
    portSettings.c_cc[VQUIT] = _POSIX_VDISABLE;

    /*  VREPRINT: (not in POSIX; 022, DC2, Ctrl-R) Reprint unread characters
        (REPRINT). Recognized when ICANON and IEXTEN are set, and
        then not passed as input.  */
    portSettings.c_cc[VREPRINT] = _POSIX_VDISABLE;

    /*  VSTART (021, DC1, Ctrl-Q) Start character (START). Restarts output
        stopped by the Stop character. Recognized when IXON is set,
        and then not passed as input.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_SOFTWARE:
            portSettings.c_cc[VSTART] = XON; /*  CTRL Q  */
            break;

        case FlowControl::FLOW_CONTROL_HARDWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.c_cc[VSTART] = _POSIX_VDISABLE;
            break;
    }

    /*  VSTATUS: (not in POSIX; not supported under Linux; status request: 024,
        DC4, Ctrl-T). Status character (STATUS). Display status
        information at terminal, including state of foreground process
        and amount of CPU time it has consumed. Also sends a SIGINFO
        signal (not supported on Linux) to the foreground process
        group.  */
    //portSettings.c_cc[VSTATUS] = _POSIX_VDISABLE;

    /*  VSTOP: (023, DC3, Ctrl-S) Stop character (STOP). Stop output until
        Start character typed. Recognized when IXON is set, and then
        not passed as input.  */
    switch (flowControl)
    {
        case FlowControl::FLOW_CONTROL_SOFTWARE:
            portSettings.c_cc[VSTOP] = XOFF; /*  CTRL S  */
            break;

        case FlowControl::FLOW_CONTROL_HARDWARE:
        case FlowControl::FLOW_CONTROL_NONE:
        default:
            portSettings.c_cc[VSTOP] = _POSIX_VDISABLE;
            break;
    }

    /*  VSUSP: (032, SUB, Ctrl-Z) Suspend character (SUSP). Send SIGTSTP
        signal. Recognized when ISIG is set, and then not passed as
        input.  */
    portSettings.c_cc[VSUSP] = _POSIX_VDISABLE;

    /*  VSWTC(H): (not in POSIX; not supported under Linux; 0, NUL) Switch char‐
        acter (SWTCH). Used in System V to switch shells in shell
        layers, a predecessor to shell job control.  */
    portSettings.c_cc[VSWTC] = _POSIX_VDISABLE;

    /*  VTIME: Timeout in deciseconds for noncanonical read (TIME).
        This will block until a timer expires (n * 100 mSec.)  */
    portSettings.c_cc[VTIME] = _POSIX_VDISABLE;

    /*  VWERASE: (not in POSIX; 027, ETB, Ctrl-W) Word erase (WERASE). Recog‐
        nized when ICANON and IEXTEN are set, and then not passed as
        input.  */
    portSettings.c_cc[VWERASE] = _POSIX_VDISABLE;
}

bool SerialPortImpl::setPortSettings(const struct termios& portSettings) const
{
    return (systemCall(tcsetattr, fileDescriptor, TCSANOW, &portSettings) == 0);
}

int SerialPortImpl::getNativeControlLine(ControlLine controlLine) const
{
    int result{0};
    if ((controlLine & ControlLine::LINE_DCD) == ControlLine::LINE_DCD)
        result |= TIOCM_CD;
    if ((controlLine & ControlLine::LINE_DTR) == ControlLine::LINE_DTR)
        result |= TIOCM_DTR;
    if ((controlLine & ControlLine::LINE_DSR) == ControlLine::LINE_DSR)
        result |= TIOCM_DSR;
    if ((controlLine & ControlLine::LINE_RTS) == ControlLine::LINE_RTS)
        result |= TIOCM_RTS;
    if ((controlLine & ControlLine::LINE_CTS) == ControlLine::LINE_CTS)
        result |= TIOCM_CTS;
    if ((controlLine & ControlLine::LINE_RI) == ControlLine::LINE_RI)
        result |= TIOCM_RI;
    return result;
}

int SerialPortImpl::getControlLine(int controlLine, bool& state) const
{
    int status{0};
    const auto result = systemCall(ioctl, fileDescriptor, TIOCMGET, &status);
    state = ((status & controlLine) == controlLine);
    return result;
}

int SerialPortImpl::setControlLine(int controlLine, bool state) const
{
    return systemCall(ioctl, fileDescriptor, ((state == true) ? TIOCMBIS : TIOCMBIC), &controlLine);
}

END_NAMESPACE_LIBSERIAL
