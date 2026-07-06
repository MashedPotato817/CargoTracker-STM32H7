# UM2408 User Manual
## STM32H7 Nucleo-144 boards (MB1363)

**UM2408 Rev 6**  
**February 2025**

---

## Introduction

The STM32H7 Nucleo-144 boards based on the MB1363 reference board (NUCLEO-H745ZI-Q, NUCLEO-H755ZI-Q, and NUCLEO-H7A3ZI-Q) provide an affordable and flexible way for users to try out new concepts and build prototypes, by choosing from the various combinations of performance and power-consumption features provided by the STM32H7 microcontroller. 

The ST Zio connector, which extends the ARDUINO® Uno V3 connectivity, and the ST morpho headers provide an easy means of expanding the functionality of the Nucleo open development platform with a wide choice of specialized shields. The STM32H7 Nucleo-144 boards do not require any separate probe as they integrate the STLINK-V3E debugger/programmer.

---

## Contents

1. Features
2. Ordering information
3. Development environment
4. Conventions
5. Safety recommendations
6. Quick start
7. Hardware layout and configuration
8. STM32H7 Nucleo-144 product information
9. FCC and ISED Canada Compliance Statements
10. CE conformity
11. Product disposal

---

## 1 Features

The STM32H7 Nucleo-144 boards offer the following features:
- STM32H7 Arm® Cortex® core-based microcontroller in LQFP144 package
- Ethernet compliant with IEEE-802.3-2002 (Depending on STM32H7 support)
- USB OTG or full-speed device
- 3 user LEDs
- 2 push-buttons: user and reset
- LSE crystal: 32.768 kHz crystal oscillator
- Board connectors: USB Micro-AB, Ethernet RJ45 (only for H745/H755)
- Board expansion connectors: ST Zio including ARDUINO® Uno V3, ST morpho
- Flexible power-supply options: ST-LINK USB VBUS or external sources
- Onboard STLINK-V3E debugger/programmer with SWD connector
- Comprehensive free software libraries and examples available with STM32CubeH7 MCU Package
- Support of IDEs including IAR Embedded Workbench®, MDK-ARM, and STM32CubeIDE

---

## 2 Ordering Information

| Order code | Board reference | Target STM32 | Differentiating feature |
| :--- | :--- | :--- | :--- |
| NUCLEO-H745ZI-Q | MB1363 | STM32H745ZIT6 | Ethernet |
| NUCLEO-H755ZI-Q | MB1363 | STM32H755ZIT6 | Ethernet, Cryptography |
| NUCLEO-H7A3ZI-Q | MB1363 | STM32H7A3ZIT6Q | None |

---

## 3 Development Environment

### 3.1 Development toolchains
- IAR Systems® - IAR Embedded Workbench®
- Keil® - MDK-ARM
- STMicroelectronics - STM32CubeIDE

### 3.2 System requirements
- Windows® OS (7, 8 or 10), Linux® 64-bit, or macOS®
- USB Type-A or USB Type-C® to Micro-B cable

---

## 5 Safety Recommendations

### 5.1 Targeted audience
This product targets users with at least basic electronics or embedded software development knowledge. This board is not a toy and is not suited for use by children.

### 5.2 Handling the board
- Connection pins might be sharp. Handle with care.
- Contains static-sensitive devices. Handle in an ESD-proof environment.
- Do not touch electric connections while powered.
- Do not put liquid on the board or operate near water/high humidity.
- Do not operate if dirty or dusty.

---

## 6 Quick Start

1. Check JP2 (5 V) position. Ensure JP4 (IDD) is selected and JP5 (1.8 V/3.3 V) is correctly configured.
2. Install the Nucleo USB driver from www.st.com/stm32nucleo before connecting.
3. Power the board via USB connector CN1 on the ST-LINK using a USB Type-A to Micro-B cable. PWR LED (LD5) and COM LED (LD4) should light up.
4. Press B1 button (left button).
5. Observe LED blinking frequency changes.
6. Download software examples from www.st.com/stm32nucleo.

---

## 7 Hardware Layout and Configuration

### 7.3 Program and debug
- **Using embedded STLINK-V3E**: Supports USB 2.0 high-speed, JTAG/SWD, Virtual COM port, Mass storage. Drivers required for Win7/8.
- **Using external debug tool**: Connect via MIPI-10 debug connector (CN5). Set JP1 ON to put embedded ST-LINK in high-impedance state if needed.

### 7.4 Power supply and selection
Five sources available:
1. Host PC via CN1 (Default, JP2 pins 1-2)
2. External 7-11V via VIN (JP2 pins 3-4)
3. External 5V via EXT (JP2 pins 5-6)
4. USB Charger 5V via CN1 (JP2 pins 7-8)
5. External 3.3V via 3V3_EXT (JP2 removed/ext source)

> **Warning:** When using VIN or EXT, power the board *before* connecting USB to PC to ensure correct enumeration.

### 7.4.8 Internal SMPS/LDO configuration
Supported configurations:
1. Internal LDO only
2. Internal SMPS only (Default)
3. Internal SMPS and LDO cascaded
4. Bypass

> **Critical Warning:** Firmware power configuration (`HAL_PWREx_ConfigSupply`) MUST match hardware solder bridge settings. Mismatch causes deadlock. Recovery requires booting from system memory via BOOT0 pin.

### 7.6 LEDs
- **LD1 (Green)**: User LED (PB0 or PA5)
- **LD2 (Yellow)**: User LED (PE1)
- **LD3 (Red)**: User LED (PB14)
- **LD4 (COM)**: ST-LINK communication status
- **LD5 (PWR)**: 5V Power indicator
- **LD6 (OVCR)**: USB overcurrent fault

### 7.14 Solder Bridges
Key configurations include:
- **SB16/SB17**: Virtual COM port enable
- **SB62/SB39 vs SB69/SB40**: I2C vs ADC on A4/A5
- **SB80/SB84/etc**: Ethernet RMII signals (Conflict with Zio/Morpho)
- **SB26/SB27**: USB D+/D-

---

## 8 Product Information

### 8.1 Product Marking
Stickers provide order code, product ID, serial number, and board revision (e.g., MB1363-H745ZIQ-D01). "ES" or "E" marked products are engineering samples and not qualified for production.

### 8.3 Board Revision History
- **Rev C01**: IDD measurement limitation in Standby mode due to shared VDD_MCU rail.
- **Rev D01**: Modified power wiring to allow IDD jumper; added SB13 bypass option. No limitations.

---

## Revision History

| Date | Rev | Changes |
| :--- | :--- | :--- |
| 13-Mar-2019 | 1 | Initial version |
| 19-Dec-2019 | 2 | Updated PCB Rev.D, Added H7A3ZI-Q support |
| 10-Mar-2020 | 3 | Updated SMPS/LDO config and Motor control |
| 25-Mar-2020 | 4 | Added ST morpho connector details |
| 23-Mar-2021 | 5 | Updated Product marking info |
| 06-Feb-2025 | 6 | Updated SB49/SB50, Safety/CE/Disposal sections added |

---

## IMPORTANT NOTICE – READ CAREFULLY

STMicroelectronics NV and its subsidiaries (“ST”) reserve the right to make changes, corrections, enhancements, modifications, and improvements to ST products and/or to this document at any time without notice. Purchasers should obtain the latest relevant information on ST products before placing orders.

© 2025 STMicroelectronics – All rights reserved