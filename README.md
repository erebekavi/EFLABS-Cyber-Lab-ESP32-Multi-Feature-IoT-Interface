# \# EFLABS Cyber Lab — ESP32 Multi-Feature IoT Interface

# 

# <p align="center">

# &#x20; <b>ESP32-based IoT, wireless networking, and embedded systems project</b>

# </p>

# 

# <p align="center">

# &#x20; <img src="docs/images/hardware-front.jpg" alt="EFLABS Cyber Lab Hardware" width="420">

# </p>

# 

# \## 📌 Overview

# 

# \*\*EFLABS Cyber Lab\*\* is a custom ESP32 DevKit V1-based IoT project that combines embedded programming, OLED-based user interface design, physical controls, and wireless network visibility into a compact portable prototype.

# 

# The device provides a menu-driven interface that allows users to interact with WiFi, Bluetooth Low Energy (BLE), device diagnostics, and signal-monitoring features directly from the onboard OLED display.

# 

# > \*\*Purpose:\*\* This project was developed as a hands-on learning and experimentation platform for IoT, embedded systems, wireless networking, and hardware-software integration. Wireless scanning features should only be used on networks and devices you are authorized to inspect.

# 

# \---

# 

# \## ✨ Features

# 

# \### 📡 WiFi Scanner

# \- Scans nearby WiFi networks.

# \- Displays available SSIDs.

# \- Shows RSSI/signal strength.

# \- Displays WiFi channel information.

# 

# \### 📊 2.4 GHz Channel Analyzer

# \- Analyzes WiFi activity across the 2.4 GHz band.

# \- Provides a graphical view of channel usage.

# \- Helps identify comparatively less congested channels.

# 

# \### 🟦 BLE Scanner

# \- Discovers nearby Bluetooth Low Energy devices.

# \- Displays advertised device names when available.

# \- Displays RSSI values.

# \- Provides a simple device-list interface.

# 

# \### 🌐 WiFi Information

# Displays information about the current WiFi connection, including:

# \- Connection status

# \- IP address

# \- MAC address

# \- RSSI/signal strength

# 

# \### 📶 Signal Monitor

# \- Continuously monitors the RSSI of the connected WiFi network.

# \- Displays signal strength as a visual bar graph.

# \- Useful for observing changes in wireless signal quality.

# 

# \### 🖥️ Device Information

# Displays ESP32 system information such as:

# \- CPU information

# \- Free heap memory

# \- MAC address

# \- General device details

# 

# \### 💡 Settings

# \- Controls an optional NeoPixel LED.

# \- Allows the LED state to be toggled through the device interface.

# 

# \### ℹ️ About

# \- Displays project information directly on the OLED.

# \- Includes the EFLABS Cyber Lab branding.

# 

# \---

# 

# \## 🛠️ Hardware

# 

# | Component | Description |

# |---|---|

# | ESP32 DevKit V1 | Main microcontroller |

# | OLED Display | Small I2C OLED for the user interface |

# | Push Buttons | Menu navigation and feature selection |

# | Li-ion Battery | Portable power source |

# | Battery Charging Module | Battery charging/power management |

# | NeoPixel LED | Optional status/configuration LED |

# | Perfboard | Custom hardware prototype |

# 

# \---

# 

# \## 🔧 Software \& Libraries

# 

# The project is developed using the Arduino ecosystem.

# 

# \### Main libraries

# 

# \- `WiFi.h` — ESP32 WiFi functionality

# \- `BLEDevice.h` — Bluetooth Low Energy functionality

# \- `U8g2lib` — OLED display and graphics

# \- `Adafruit\_NeoPixel` — Optional NeoPixel control

# 

# \### Development Environment

# 

# \- Arduino IDE

# \- ESP32 Arduino Core

# \- C/C++

# \- ESP32 DevKit V1

# 

# \---

# 

# \## 🎮 User Interface

# 

# The device uses physical buttons to navigate the OLED menu.

# 

# The main interface provides access to:

# 

# ```text

# EFLABS CYBER LAB

# │

# ├── WiFi Scanner

# ├── Channel Analyzer

# ├── BLE Scanner

# ├── WiFi Info

# ├── Signal Monitor

# ├── Device Info

# ├── Settings

# └── About

# ```

# 

# Button handling includes software debouncing to reduce unintended multiple inputs during navigation.

# 

# \---

# 

# \## 🚀 Getting Started

# 

# \### 1. Clone the repository

# 

# ```bash

# git clone https://github.com/YOUR-USERNAME/eflabs-esp32-cyber-lab.git

# cd eflabs-esp32-cyber-lab

# ```

# 

# \### 2. Install the required libraries

# 

# Open Arduino IDE and install:

# 

# \- U8g2

# \- Adafruit NeoPixel

# 

# `WiFi.h` and the ESP32 BLE functionality are provided through the ESP32 Arduino framework.

# 

# \### 3. Install ESP32 Board Support

# 

# In Arduino IDE, install the ESP32 board package and select an appropriate ESP32 DevKit board.

# 

# \### 4. Open the project

# 

# Open:

# 

# ```text

# ESP32-Multi-Feature-IoT-Interface.ino

# ```

# 

# \### 5. Configure the hardware

# 

# Verify the GPIO assignments in the sketch and connect:

# 

# \- OLED display

# \- Navigation buttons

# \- Optional NeoPixel

# \- Power/charging circuit

# 

# \### 6. Upload

# 

# Connect the ESP32 through USB, select the correct board and COM port, then upload the sketch.

# 

# \---

# 

# \## 📸 Project Gallery

# 

# \### Hardware Prototype

# 

# <p align="center">

# &#x20; <img src="docs/images/hardware-front.jpg" alt="ESP32 Cyber Lab front" width="400">

# &#x20; <img src="docs/images/hardware-back.jpg" alt="ESP32 Cyber Lab back" width="400">

# </p>

# 

# \### Startup \& Main Menu

# 

# <p align="center">

# &#x20; <img src="docs/images/startup-screen.jpg" alt="Startup screen" width="300">

# &#x20; <img src="docs/images/main-menu.jpg" alt="Main menu" width="300">

# </p>

# 

# \### WiFi Scanner

# 

# <p align="center">

# &#x20; <img src="docs/images/wifi-scanner.jpg" alt="WiFi Scanner" width="350">

# </p>

# 

# \### BLE Scanner

# 

# <p align="center">

# &#x20; <img src="docs/images/ble-scanner.jpg" alt="BLE Scanner" width="350">

# </p>

# 

# > Replace the image filenames above with the actual filenames you upload to `docs/images/`.

# 

# \---

# 

# \## 🧠 What This Project Demonstrates

# 

# This project demonstrates practical experience with:

# 

# \- ESP32 development

# \- Embedded C/C++ programming

# \- OLED graphical interfaces

# \- GPIO and button handling

# \- Software debouncing

# \- WiFi APIs

# \- BLE APIs

# \- RSSI measurement

# \- 2.4 GHz wireless channel analysis

# \- Portable power integration

# \- Hardware prototyping on perfboard

# \- Modular feature-based software design

# \- Hardware and software integration

# 

# \---

# 

# \## 🔐 Responsible Use

# 

# The wireless features in this project are intended for \*\*learning, network visibility, diagnostics, and authorized testing\*\*.

# 

# Do not use the device to access, interfere with, or monitor networks or Bluetooth devices without appropriate authorization.

# 

# \---

# 

# \## 🔮 Future Improvements

# 

# Potential future improvements include:

# 

# \- \[ ] Improved OLED graphics and animations

# \- \[ ] Battery voltage and battery percentage monitoring

# \- \[ ] Configurable GPIO settings

# \- \[ ] SD card logging

# \- \[ ] WiFi signal history

# \- \[ ] BLE device history

# \- \[ ] Improved channel visualization

# \- \[ ] Web-based configuration interface

# \- \[ ] Modular firmware architecture

# \- \[ ] 3D-printed enclosure

# \- \[ ] Low-power battery optimization

# 

# \---

# 

# \## 📁 Suggested Repository Structure

# 

# ```text

# eflabs-esp32-cyber-lab/

# │

# ├── ESP32-Multi-Feature-IoT-Interface.ino

# ├── README.md

# ├── LICENSE

# ├── .gitignore

# │

# └── docs/

# &#x20;   └── images/

# &#x20;       ├── hardware-front.jpg

# &#x20;       ├── hardware-back.jpg

# &#x20;       ├── startup-screen.jpg

# &#x20;       ├── main-menu.jpg

# &#x20;       ├── wifi-scanner.jpg

# &#x20;       └── ble-scanner.jpg

# ```

# 

# \---

# 

# \## 👨‍💻 Project

# 

# \*\*EFLABS Cyber Lab\*\*

# 

# Developed as an IoT and embedded systems project focused on practical wireless networking, ESP32 development, and interactive hardware interfaces.

# 

# 🌐 \*\*Portfolio:\*\* https://eflabs.in

# 

# \---

# 

# \## 📄 License

# 

# This project is intended for educational and research purposes. Add an appropriate open-source license to the repository before redistributing the source code.



