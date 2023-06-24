# Task Tracker

## About

This repository contains the software and hardware sources for a task time recording device.

The purpose of this device is to provide a tool to track time which is used on tasks, in a handy way.

The user shall be able to start and stop the time recording for tasks using a simple press.
Implementation as a device - in contrast to pure software - shall provide the user a distraction free and permanently visible, dedicated interface.
The system shall offer to track a flexible number of tasks.
For quick access, a small number of tasks can be defined as favorites. 
Tasks shall be identifiable using at least a short label.
It is not a goal to display a full task description to the user.

In particular the objectives of the device are:

 - quick interaction with an easy human-machine interface
 - permanent feedback on which task is active at the moment
 - provide digital interface to recorded times (TBD: through WiFi, Bluetooth, wired serial interface, ...)

## Generate the documentation

Documentation may be generated with Doxygen. Simply call in the root directory of this repository:

    doxygen

The HTML documentation will be written to [`doc/html/index.xhtml`](doc/html/index.xhtml).

## Build

The build configuration for the device's software is generated with PlatformIO. Please refer to the [documentation](https://docs.platformio.org/) for build instructions.

## Simulate

[`wokwi.toml`](wokwi_files/wokwi.toml) is a project configuration for the Visual Studio Code extension [Wokwi Simulator](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode).
Please refer to its documentation for instructions on how to simulate the device.

## Contribute

### Document you code

Please document your code in Doxygen compatible syntax.

### C/C++ source code

Please format your changes using ClangFormat.
A style configuration file is given: [`.clang-format`](.clang-format)

### Doxygen configuration

If you change [`Doxyfile`](Doxyfile), please do a ['cleanup'](https://stackoverflow.com/questions/71157463/what-part-of-the-doxygen-configuration-doxyfile-is-recommended-for-version-con#comment125783393_71157463) by using the output of `doxygen -x`.
For example

    doxygen -x > Doxyfile.tmp && mv Doxyfile.tmp Doxyfile

### PlatformIO configuration

If you change [`platformio.ini`](platformio.ini), please note that [PlatformIO Home](https://docs.platformio.org/en/latest/home/index.html#platformio-home) [may delete comments unsolicitedly](https://github.com/platformio/platformio-core/issues/3862).
In general we want to preserve useful comments in the project configuration file.
Please revert unwanted deletions before committing them.

#### Windows Driver
See https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/establish-serial-connection.html#connect-esp32-s3-to-pc for driver installation guide.
