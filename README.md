# emblab-final-board
 board code 

# Esp8266

## Description

This project utilizes the [Firebase Firestore](https://firebase.google.com/docs/firestore) API to store data received from an STM32 sensor via the UART (Universal Asynchronous Receiver-Transmitter) protocol and transmitted to the Esp8266 module. The data is updated on the Esp8266 every 10 seconds and uploaded to Firestore every 10 minutes.

## Setup

To set up the project, follow these steps:

1. Download the [Arduino IDE](https://www.arduino.cc/en/software) software and install it.
2. Open the Arduino IDE and go to **File** -> **Preferences**.
3. Enter `http://arduino.esp8266.com/stable/package_esp8266com_index.json` into the "Additional Boards Manager URLs" field. Then, click the "OK" button.
4. Click **Tools** -> **Board** -> **Board Manager**.
5. Search for "nodemcu" in the "Board Manager" and install it.
6. Download the `Esp8266_Part` folder and open the `Esp8266_Part.ino` file with the Arduino IDE.
7. Install the following libraries from the library manager (located on the left side of the Arduino IDE panel):
   - [NTPClient](https://github.com/arduino-libraries/NTPClient)
   - [EspSoftwareSerial](https://github.com/plerup/espsoftwareserial/)
   - [Firebase Arduino Client Library for ESP8266, ESP32, and RP2040 Pico](https://github.com/mobizt/Firebase-ESP-Client)
8. Go to **Tool** -> **Board** -> **NodeMCU 0.9 (ESP-12 Module)**.
9. Open the `Network.cpp` file and update the define variables to match your configuration.
10. Connect your board to the computer and select the appropriate serial port in the Arduino IDE (**Tool** -> **Port** -> "select your serial port here").
11. Click the **Upload** button to compile and upload the code to your board (located on the upper-left side).

## Connecting to STM32

To connect the Esp8266 to the STM32:

- Connect the ESP8266 Ground to the STM32 Ground.
- Connect ESP8266 RX (`D7`) to STM32 TX (`PA9` for the default USART1 in STM32CubeIDE).
- Connect ESP8266 TX (`D8`) to STM32 RX (`PA10` for the default USART1 in STM32CubeIDE).
