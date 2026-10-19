# 💡 Li-Fi Text Transmission Prototype

![ESP32](https://img.shields.io/badge/ESP32-1e6fd9?style=for-the-badge&logo=espressif&logoColor=white)
![ESP8266](https://img.shields.io/badge/ESP8266-1e6fd9?style=for-the-badge&logo=espressif&logoColor=white)
![C++](https://img.shields.io/badge/C++-0b1d3a?style=for-the-badge&logo=cplusplus&logoColor=white)
![Li-Fi](https://img.shields.io/badge/Li--Fi-f5a623?style=for-the-badge&logoColor=black)

A low-cost Li-Fi link that sends text from one laptop to another using light. An ESP32 drives an LED as the transmitter, and a solar panel connected to an ESP8266 acts as the receiver.

![Prototype](images/prototype.jpg)

## ✨ How It Works

1. A C++ program on the sender laptop reads a text file and sends its contents to the ESP32.
2. The ESP32 drives an LED so the text is transmitted as light.
3. A solar panel, connected to an ESP8266, picks up the light.
4. The ESP8266 passes the text to the receiving laptop.

## 🧩 System Overview

```mermaid
flowchart LR
    A["Laptop 1 (C++ program)"] -->|USB serial| B["ESP32"]
    B --> C["LED"]
    C -. light .-> D["Solar Panel"]
    D --> E["ESP8266"]
    E -->|USB serial| F["Laptop 2"]
```

## 🔧 Hardware & Software

| Item | Purpose |
| :--- | :------ |
| ESP32 | Transmitter controller |
| LED | Light source |
| Solar panel | Light receiver |
| ESP8266 | Receiver controller |
| C++ (g++ in MSYS2) | Reads a text file and sends its contents |

## 🧪 Test Results

| Parameter | Result |
| :-------- | :----- |
| Maximum text tested | 500 characters |
| Distance | 2 cm, closed setup |
| Direction | One-way |

## 🔭 Planned Next Steps

- Two-way link
- Faster photodiode receiver
- File transfer with error checking

## 🚀 How to Use

1. Upload `transmitter/lifi_transmitter.ino` to the **ESP32** (LED on GPIO 25, with a resistor).
2. Upload `receiver/lifi_receiver.ino` to the **ESP8266** (solar panel on A0, keep the voltage below 1V or use a divider).
3. Set `CALIBRATE = true` in the receiver once, read the values with the LED on and off, and set `THRESHOLD` between them.
4. Face the LED towards the solar panel at about 2 cm in a closed (dark) setup.
5. On Laptop 1 (MSYS2), compile and run the sender:
   ```bash
   g++ sender.cpp -o sender.exe
   sender.exe COM3 message.txt
   ```
6. Open the Serial Monitor of the ESP8266 on Laptop 2 to see the text.

## 📡 Protocol

Serial-style framing over light: idle = LED on, start bit = LED off, 8 data bits (LSB first), stop bit = LED on, 10 ms per bit.

## 💻 About the Code

The files in this repository are reference implementations of the transmitter, receiver and sender program.

## 📸 Photos

| Setup | Demo |
| :---: | :--: |
| ![Setup](images/circuit.jpg) | ![Demo](images/demo.gif) |

## 📁 Repository Structure

```text
├── transmitter/lifi_transmitter.ino   # ESP32
├── receiver/lifi_receiver.ino         # ESP8266
├── sender.cpp     # C++ program that reads the text file
├── images/
└── README.md
```

## 📄 Research

Published as a preprint: [Design of a Low-Cost Li-Fi Text Transmission Prototype Using an LED and Solar-Panel Receiver](https://doi.org/10.5281/zenodo.23389126)

## 👤 Author

**Mareeshwaran S**, ECE student · [GitHub](https://github.com/Mareeshwaran606) · mareesh606@gmail.com
