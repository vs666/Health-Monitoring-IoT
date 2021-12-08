# 2. Hardware End

## 2.1 Hardware Used

### **2.1.1 ProtoCentral MAX30205 Body Temperature Sensor Breakout Board**

The MAX30205 temperature sensor accurately measures temperature and provide an overtemperature alarm/interrupt/shutdown output. This device converts the temperature measurements to digital form using a high-resolution, sigma-delta, analog-to-digital converter (ADC). Communication is through an I2C-compatible 2-wire serial interface.

Specifications of the sensor:

- 0.1°C Accuracy (37°C to 39°C)
- 16-Bit (0.00390625°C) Temperature Resolution
- 2.7V to 3.3V Supply Voltage Range
- 600µA (typ) Operating Supply Current
- 16 bit Resolution
- I2C interface
- Operating temperature: 0 to 50 °C
- “OS” open-drain pin can act as thermostat control
- I2C address 0x48

<img src="/home/sreeharsha/sreeharshafo/sreeharsha/Downloads/Sem5/ESW/documentation/temp.jpeg" alt="drawing" width="200"/>

|Symbol|Description     |
|:---------------: |:----------------:|
| SDA              |  Serial Data     |
| SCL              |  Serial Clock    |
| Vin              |  Power(5V)           |
| GND              |  Ground            |
| 3v3              |  Power(3.3V)            |

### **2.1.2 Pulse Express Pulse-Ox & Heart Rate Sensor with MAX32664**

Pulse Express is an efficient and versatile breakout board with integrated high-sensitivity optical sensors MAX30102 and also a chip that does the calculations (biometric sensor hub MAX32664D. Integrating Maxim’s MAX32664 Version D makes Pulse Express unique, with an internal algorithm that works to measure different data as you start. With its built-in low power capability, the board is suitable for any wearable health for finger-based applications. And it measures Pulse Heart Rate,Pulse Blood Oxygen Saturation (SpO2), Estimated Blood Pressure.

Specifications of the sensor:

- Integrates a high-sensitivity pulse oximeter and heart rate sensor ([MAX30102](https://datasheets.maximintegrated.com/en/ds/MAX30102.pdf)).
- Integrated biometric sensor hub ([MAX32664](https://www.maximintegrated.com/en/products/interface/sensor-interface/MAX32664.html))
- In-built accelerometer for robust detection and compensation of motion artifacts(Optional).
- Easy-to-use I2C interface to connect to any host microcontroller.
- Ultra-Low Power
- Dimensions: 35 mm x 17 mm
- I2C address 0x55

<img src="/home/sreeharsha/sreeharshafo/sreeharsha/Downloads/Sem5/ESW/documentation/bp.jpg" alt="drawing" width="400"/>

|Symbol|Description     |
|:---------------: |:----------------:|
| SDA              |  Serial Data     |
| SCL              |  Serial Clock    |
| Vin              |  Power           |
| GND              |  Ground             |
| MFIO Pin         |  MFIO            |
| RESET Pin        |  Reset           |

### **2.1.3 SeeedStudio Grove GSR sensor**

Grove – GSR Sensor stands for galvanic skin response and it is a method of measuring the electrical conductance of the skin. It can be used to reflect the human emotional activity. When we are emotionally stressed or have strong expressions on the face, sympathetic activity increases and promotes the secretion of sweat glands, which increases the skin’s electrical conductivity.

This sensor allows us to spot such strong emotions by simply attaching two electrodes to two fingers on one hand.

Specifications of the sensor:

- Input Voltage: 5V/3.3V

- Sensitivity adjustable via a potentiometer

- External measuring finger cots

<img src="/home/sreeharsha/sreeharshafo/sreeharsha/Downloads/Sem5/ESW/documentation/gsr.jpg" alt="drawing" width="100"/>  <img src="/home/sreeharsha/sreeharshafo/sreeharsha/Downloads/Sem5/ESW/documentation/gsrback.jpeg" alt="drawing" width="200"/>

  | Symbol |       Description       |
  | :----: | :---------------------: |
  |  VCC   |        Power(5V)        |
  |  GND   |         Ground          |
  |  SIG   | Genral Input output pin |

### **2.1.4 Waveshare 2 Inch LCD Display Module 240×320**

This is a Waveshare 2 Inch LCD Display Module. A general LCD display Module with IPS screen, 2inch diagonal, 240×320 resolution, embedded controller, communicating via SPI interface. Supports Raspberry Pi, STM32, Arduino, etc.

Specifications of the display:

- Driver: ST7789

- Interface: SPI

- Display color: RGB, 262K color

- Resolution: 240×320

- Backlight: LED

- Operating voltage: 3.3V

- SPI interface requires minimum GPIO for controlling.

<img src="/home/sreeharsha/sreeharshafo/sreeharsha/Downloads/Sem5/ESW/documentation/led.jpeg" alt="drawing" width="300"/>  <img src="/home/sreeharsha/sreeharshafo/sreeharsha/Downloads/Sem5/ESW/documentation/ledback.jpg" alt="drawing" width="300"/>

| SYMBOL |                       DESCRIPTION                       |
  | :----: | :-----------------------------------------------------: |
  |  VCC   |                   Power (3.3V input)                    |
  |  GND   |                         Ground                          |
  |  DIN   |                     SPI data input                      |
  |  CLK   |                     SPI clock input                     |
  |   CS   |               Chip selection, low active                |
  |   DC   | Data/Command selection (high for data, low for command) |
  |  RST   |                    Reset, low active                    |
  |   BL   |                        Backlight                        |

### **2.2 Board: ESP32 Wroom Board**

ESP32 Development board is based on the ESP WROOM32 WIFI + BLE Module. This is the latest generation of the ESP32 IoT development modules. This development board breaks out all ESP32 modules pins into a 0.1" header and also provides a 3.3 Volt power regulator, Reset and programming button, and an onboard CP2102 USB to TTL converter for programming directly via USB port.
For more detailed specifications, refer to the manufacturer [Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf).

We used the following features of ESP32 Wroom Board:

- We use WiFi feature of the board to connect to Internet in order to push data to oneM2M server and ThingSpeak channels

- We use the GPIO pins on the board are used for communication with GSR-Groove Sensor. And also with other sensors for reset type of pins etc.

- I2C (Inter-Integrated Circuit) is used for interfacing the board and the Pulse Express Pulse-Ox & Heart Rate Sensor and ProtoCentral MAX30205 Body Temperature Sensors. And the pins for I2C Communication:

  | Pin label |    Description    |
  | :-------: | :---------------: |
  | SDA (D21) |     I2C data      |
  | SCL (D23) |     I2C clock     |
  |    VCC    | Power(3.3V or 5V) |
  |    GND    |      Ground       |

<img src="/home/sreeharsha/sreeharshafo/sreeharsha/Downloads/Sem5/ESW/documentation/esp.jpg" alt="drawing" width="300"/>

The following pins of the esp32 board are used for this project.

- Pin VCC : Used to connect VCC of the sensors

- Pin 3.3V : Used to connect VCC of the low power sensors

- Pin GND : Used to connect GND of the seonsors

- Pin D21 : Used for data transfer of I2C (SDA pin)

- Pin D23 : Used for clock of I2C (SCL pin)

- Pin D18 : Used for clock of SPI.

- Pin D22 : Used for data transfer of SPI.

- Pins D4,D34,D15,D5,D2: Used for GPIO of sensors

### **2.3 Connection**

The following diagram is the circuit diagram of the project on IoT side.

![img](https://lh5.googleusercontent.com/zsiFDhha2T829DXC30Hp3P4s5JN3LSBNwzXnky834L7IBXooPwvhU-UMJOdYstjm3fRbYuMlnoniVrDefJjKbWpgDWm-Oe1fgEBd7NvVRDPiSHQDKEyV5q8azJNSa12WGNSrsi5ZuQFj)

# 3.Software End

## 3.1 Libraries Used

1. **WiFi.h** : This library is used for establishing WiFiClient and connecting to WiFi
2. **WiFiClientSecure.h** : This library is used for establishing WiFiSecure Client connect with https requests as they are secure.
3. **Protocentral_MAX30205.h** : This library is used for collecting data from MAX30205 Temperature sensor.
4. **WebServer.h** : This library is used to create the localhost webpage using the board.
5. **mbedtls/aes.h and mbedtls/md.h** : Encrpt and hash functions of these libraries are used to encrpting the data and also hashing the data.
6. **Adafruit_GFX.h and Adafruit_ST7789.h** : These libraries are used for ST7789 driver of the LCD display
7. **TFT_eSPI.h and SPI.h** : These libraries are used to display text and rxplore other features (color, etc) of LCD display
8. **UniversalTelegramBot.h** : This library is used for comunication between board and telegram bot which gives updates on data.
9. **ArduinoJson.h** : This library is used to convert string to JSON string which is uploaded to oneM2M server
10. **ThingSpeak.h** : This library is used to send the data to ThingSpeak channels using the write API key of the channel.
