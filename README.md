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

## Deployment

### Generate the documentation

Documentation may be generated with [Doxygen](https://www.doxygen.org/).
Simply call in the root directory of this repository:

    doxygen

The HTML documentation will be written to [`doc/html/index.xhtml`](doc/html/index.xhtml).

### Build and Upload the Software

The project consists of a software and a hardware device.
The target system to run the software on is the device developed in this project.
Alternatively the software may be executed in a simulation environment.

This project uses [PlatformIO](https://platformio.org/) as build system and package manager.
PlatformIO may be used via a GUI (PlatformIO IDE) or command line interface (PlatformIO Core). The project configuration ([`platformio.ini`](platformio.ini)) is part of this repository.

In order to use the software (some call it "firmware"), the following steps are required:

1. Build (the default configuration of) the project.
   For example via:
   ```
   platformio run
   ```
2. [Connect](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/establish-serial-connection.html) the device with the host system (where you build the software).
3. Upload the built binary to the device.
   For example via:
   ```
   platformio run --target upload
   ```

Please refer to the [PIO documentation](https://docs.platformio.org/) for detailed instructions.

### Simulate

The Visual Studio Code extension [Wokwi Simulator](https://marketplace.visualstudio.com/items?itemName=wokwi.wokwi-vscode) may be used to simulate the software on the device.
The project configuration for the simulator is provided as [`wokwi.toml`](wokwi_files/wokwi.toml).

Note, that you must build the software (using the default build configuration) before it can be simulated.

Please refer to the documentation of the simulator for instructions on how to simulate the device.

## Contribute

Please refer to [`CONTRIBUTING.md`](CONTRIBUTING.md).
