# README

`libserial` is a C++ implementation of a cross-platform library targeting Linux and Windows operating systems,
for a simple serial port access.

## Table of Contents

- [README](#readme)
  - [Table of Contents](#table-of-contents)
  - [Features](#features)
  - [Limitations](#limitations)
  - [Getting `libserial`](#getting-libserial)
  - [Compile and build](#compile-and-build)
  - [Installation](#installation)
  - [Documentation](#documentation)
  - [Localization](#localization)
  - [Usage](#usage)
  - [Contributing](#contributing)
  - [Bug Reports](#bug-reports)
  - [Contact the Authors](#contact-the-authors)
  - [Copyright notice](#copyright-notice)

## Features

`libserial` main features are:

  * C++ 17 implementation
  * Cross-platform support for Linux (`gcc`) and Windows (`mingw-w64`)
  * Unified clean interface with a platform specific code wrapped in the library
  * Provides `SerialPort` class for serial port access
  * Provides `Enumerator` class for serial port list enumeration
  * Uses CMake build generator for build and install
  * Extensive tests via gtest framework
  * High line and branch code coverage (> 90% on Linux)

## Limitations

The limitations of `libserial` are:

  * Additional platform support is missing (MSVC on Windows, macOS, etc.)
  * Asynchronous support for serial port read/write is not supported

## Getting `libserial`

Currently no release channels are provisioned.

## Compile and build

A user who wishes to compile and build `libserial` should clone the git repository at:
  * `git@github.com:blazzakrajsek/libserial.git`

The project should be configured by the CMake build generator where build configuration for
various compilers can be generated. With the generated build configuration it is possible
to compile, build and install libserial. 

On a Linux system, it can be build and installed using:
```sh
$ make
$ sudo make install
```

On a Windows system, it can be build and installed using:
```sh
$ mingw32-make.exe
$ mingw32-make.exe install
```

## Installation

See the [`INSTALL`](INSTALL.md) file for more information about how to configure
and install `libserial`.

## Documentation

`libserial` currently provides no official documentation besides the documentation
contained in this document and any other documents contained in the repository.

## Localization

`libserial` currenly provides no localization facilities.

## Usage

Compiled and built library can be linked with the application or binary using the library.

## Contributing

Everybody is welcome to contribute to `libserial`. Please, see
[`CONTRIBUTING`](CONTRIBUTING.md) for further information.

## Bug Reports

Bug reports can be sent directly to the authors via email.

## Contact the Authors

The author can be contacted via email at zakrajsek dot blaz at gmail com.

## Copyright notice

Copyright (c) 2020-2021  Blaž Zakrajšek

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this library.  If not, see <http://www.gnu.org/licenses/>.
