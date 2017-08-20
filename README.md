FastArduino
===========

FastArduino is a C++ object-oriented library for Arduino boards based on AVR MCU and also bare AVR MCU.

FastArduino API is documented [here](jfpoilpret.github.io/fast-arduino-lib/) and documentation also includes a [simple tutorial](http://jfpoilpret.github.io/fast-arduino-lib/tutorial.html).

FastArduino objectives are:

- provide smaller and faster code than other libraries for the same functionality
- ensure you pay (size and speed) only for what you use
- use C++ Object-Oriented Design everywhere
- reduce risk of bad code, e.g. by preventing usage, at compile time, of unexisting features (pins, timers...) for the target MCU
- support event-driven programs
- support both ATmega and ATtiny chips

It was originally inspired by [Cosa library](https://github.com/mikaelpatel/Cosa) from Mikael Patel.

After usage of Cosa libraries for several projects on ATmega328 and particularly ATtiny84, I found out that the current way Cosa is built has a few drawbacks related to:

- code size (for small AVR MCU)
- speed (for specific situations such as software UART)

From my viewpoint, the main source of those drawbacks is essentially heavy usage of `virtual` methods, which quickly increases code size when you start to define deep classes hierarchies; this also can have a slight impact on speed due to additional indirection when calling methods.

FastArduino tries to favour C++ templates rather than virtual methods whenever possible; when virtual methods are used, their number is reduced to the minimum needed (abstract virtual methods only, typically used for event handlers, generally limited to hierarchy of 2 levels only, one abstract parent and direct children). 

Also, no ISR gets automatically declared by FastArduino: every program declares the ISR it needs by using pre-defined FastArduino ISR-registration macros (note that ISR registration is the only feature for which FastArduino uses macros). FastArduino does not use `virtual` methods for ISR callbacks, thus permitting optimization of ISR code size, which would not have been possible with `virtual` methods as callbacks.

All this comes at a cost: 

1. Template usage is often more complex in applications. The provided examples are here to help.
2. Build times may be increased a bit as most code is inside C++ headers (recompiled every time included); for this point however, please note that compile time difference is hardly noticeable.

Also, if you consider using FastArduino for your projects, be aware that FastArduino does not support Arduino API and does not intend to do so some day. That means you will have to first learn FastArduino API (you can use numerous examples provided for that) in order to reap its benefits. FastArduino is definitely not for newcomers to C++ programming as it makes heavy use of C++ specificities. Note that FastArduino currently uses C++11 standard.

In the initial drafts, I decided not to be compliant with Arduino IDE as I find it is a real pain. All my projects (including FastArduino) are built through [netbeans](https://netbeans.org/), which was hard to initially setup, but much more friendly to use once setup is done. Also netbeans automatically provides makefiles that make it possible to build projects in command line.

Making FastArduino buildable on Arduino IDE is not on my roadmap currently (and probably won't until long, as I'm more than happy with my current setup until now).

My special setup (I work on Windows but compile everything on an Ubuntu virtual machine) is described in [ArduinoDevSetup.docx](ArduinoDevSetup.docx). This document also describes how I setup netbeans for my projects.

Status
======

The project has started a bit more than one year ago, it covers a lot already, but is not fully complete yet; also its API may still change, more or less drastically, during Summer 2017 (see roadmap).

What the project already has:

- General utilities (queues, linked lists, busy loop delays)
- Fast IO support: template-based, optimized for speed and size.
- General Events handling
- Watchdog timer
- Timed (periodic or not) jobs scheduling
- Real Time Timer with microsecond precision
- Power sleep
- Pin Change Interrupt (PCI) handling
- External Pin Interrupt handling
- Hardware UART support (for MCU that support it, ie not for ATtiny)
- Software UART support (for all MCU)
- "C++ like" Input/Output streams (used by UART implementations)
- SPI master support
- WinBond flash memory support (SPI-based)
- NRF24L01 device support (SPI-based)
- Analog Digital Conversion support (in Single Conversion mode only)
- Power supply voltage measurement support
- Flash memory data support (PROGMEM data), particularly strings (useful to limit used SRAM)
- EEPROM support, with synchronous or asynchronous write
- PWM output support
- "Pulsed" Timer support (useful for Servos)
- Servo control API
- I2C master support (synchronous mode)
- DS1307 RTC device support (I2C-based)
- HMC5883L magnetometer device support (I2C-based)
- MPU-6050 accelerometer/gyroscope device support (I2C-based)

As of now, the following platforms are supported (and tested):

- Arduino UNO
- Arduino NANO
- Arduino MEGA
- Arduino Leonardo (without USB support)
- Breadboard ATmega328 at 8MHz and 16MHz
- Breadboard ATtiny84 at 8MHz

I intend to later add support for:

- Breadboard ATtiny85 at 8MHz
- Arduino Leonardo with USB support

Roadmap
=======

The roadmap of next activities and new supported features is the following:

August-September 2017
---------------------
1. Improve board specific enums to include Arduino pin aliases
2. Improve streams support ala C++
3. Improve Timer support (support input capture where available)
4. Finalize sonar ranger API
5. Add support for ATtinyX5
6. Improve I2C master support (asynchronous mode)

December 2017
-------------
7. Support other I2C devices: MCP23017 (IO multiplexer)
8. Improve Analog Input support and code
9. Improve SPI to support slave mode
10. Improve I2C to support slave mode
11. Add USB support for Arduino Leonardo
12. Add high-speed timer support of Arduino Leonardo

In addition to these activities, I intend to perform various continuous improvements in the following months, regarding:

- coding guidelines applied across the library code (not necessarily example code)
- complete documentation (ISR handling, Board support adding, API...)

The following features are not in FastArduino roadmap currently (but may be in the future):

- OWI support

