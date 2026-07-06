# UM2408 User Manual
## STM32H7 Nucleo-144 boards (MB1363)


<!-- Page 1 -->
1
User manual
STM32H7 Nucleo-144 boards (MB1363)
Introduction
The STM32H7 Nucleo-144 boards based on the MB1363 reference board (NUCLEO-
H745ZI-Q, NUCLEO-H755ZI-Q, and NUCLEO-H7A3ZI-Q) provide an affordable and
flexible way for users to try out new concepts and build prototypes, by choosing from the
various combinations of performance and power-consumption features provided by the
STM32H7 microcontroller. The ST Zio connector, which extends the ARDUINO
® Uno V3
connectivity, and the ST morpho headers provide an easy means of expanding the
functionality of the Nucleo open development platform with a wide choice of specialized
shields. The STM32H7 Nucleo-144 boards do not require any separate probe as they
integrate the STLINK-V3E debugger/programmer. The STM32H7 Nucleo-144 boards come
with comprehensive free software libraries and examples available with the STM32Cube
MCU Package.
Pictures are not contractual.

**Figure 1. Nucleo-144 board (Top view) Figure 2. Nucleo-144 board (Bottom view)**

www.st.com

<!-- Page 2 -->
Contents UM2408
Contents

## 1 Features . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 6


## 2 Ordering information  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 7


### 2.1 Codification . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 7


## 3 Development environment . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8


### 3.1 Development toolchains . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8


### 3.2 System requirements . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8


### 3.3 Demonstration software . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8


### 3.4 CAD resources . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 8


## 4 Conventions   . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 9


## 5 Safety recommendations  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 10


### 5.1 Targeted audience  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 10


### 5.2 Handling the board . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 10


## 6 Quick start  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 11


### 6.1 Getting started  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .11


## 7 Hardware layout and configurat ion . . . . . . . . . . . . . . . . . . . . . . . . . . . . 12


### 7.1 Nucleo-144 board layout  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 13


### 7.2 Mechanical drawing  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 14


### 7.3 Program and debug the onboard STM32H7 target  . . . . . . . . . . . . . . . . . 16

7.3.1 Use of STLINK-V3E to program and debug the onboard STM32H7 . . . 16
7.3.2 Using an external debug tool to program and debug the onboard
STM32H7 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 17

### 7.4 Power supply and power selection . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 18

7.4.1 Power supply input from STLINK-V3E  USB connector (default setting)  18
7.4.2 External power supply input from V IN (7-11 V, 800 mA max) . . . . . . . . . 19
7.4.3 External power supply input 5V_EXT (5 V, 500 mA max)  . . . . . . . . . . . 20
7.4.4 External power supply input from USB ch arger (5 V)  . . . . . . . . . . . . . . 21
7.4.5 External power supply input from 3V3_EXT  (3.3 V)  . . . . . . . . . . . . . . . 21
7.4.6 Debugging while using V IN or EXT as an external power supply  . . . . . 22

<!-- Page 3 -->
UM2408 Contents
3
7.4.7 External power supply output  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 22
7.4.8 Internal SMPS/LDO configuration  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 23
7.4.9 JP5: 1.8 V/3.3 V selection  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 26

### 7.5 External SMPS selection  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 26


### 7.6 LEDs  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 27


### 7.7 Push-buttons  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 28


### 7.8 JP4 (IDD)  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 28


### 7.9 OSC clock  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 28

7.9.1 OSC clock supply  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 28
7.9.2 OSC 32 KHz  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 29

### 7.10 USART communication  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 29


### 7.11 USB OTG or full-speed device . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 30


### 7.12 Ethernet (with NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q) . . . . . . . . . 31


### 7.13 Motor control  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 32


### 7.14 Solder bridges  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 32


### 7.15 Extension connectors . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 36


### 7.16 ST Zio connectors  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 37


### 7.17 ST morpho connector . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 46


## 8 STM32H7 Nucleo-144 pr oduct information . . . . . . . . . . . . . . . . . . . . . . 48


### 8.1 Product marking . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 48


### 8.2 NUCLEO-H745ZI-Q, NUCLEO-H755ZI-Q, and NUCLEO-H7A3ZI-Q

product history  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 49

### 8.3 Board revision history . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 51


## 9 Federal Communications Commission (FCC)

and ISED Canada Compliance Statements . . . . . . . . . . . . . . . . . . . . . . 52

### 9.1 FCC Compliance Statement . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 52


### 9.2 ISED Compliance Statement  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 53


## 10 CE conformity  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 54


### 10.1 Warning  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 54


## 11 Product disposal . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 55

Revision history  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 56

<!-- Page 4 -->
List of tables UM2408
List of tables

**Table 1. Ordering information . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 7**


**Table 2. Codification explanation . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 7**


**Table 3. ON/OFF conventions . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 9**


**Table 4. MIPI-10 debug connector (CN5). . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 17**


**Table 5. External power sources: V IN (7-11 V) . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 19**


**Table 6. External power sources: 5V_EXT. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 0**


**Table 7. External power sources: CHGR (5 V). . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21**


**Table 8. External power sources: 3V3_EXT (3.3 V) . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 21**


**Table 9. External power sources maximum currents . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 23**


**Table 10. SMPS/LDO configuration . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .  . . 23**


**Table 11. External SMPS selection  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 26**


**Table 12. USART3 pins  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 30**


**Table 13. USB pin configuration . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .  . . . . 30**


**Table 14. Ethernet pin configuration. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 31**


**Table 15. Motor-control shield configuration. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 32**


**Table 16. Solder bridges. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 32**


**Table 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q pin assignments. . . . . . . . . . . . . . . . . . . . . 38**


**Table 18. NUCLEO-H7A3ZI-Q pin assignments  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 42**


**Table 19. Pin assignment of MCU with ST morpho connector . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 46**


**Table 20. Product history . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 49**


**Table 21. Board revision history . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 51**


**Table 22. Document revision history  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 56**


<!-- Page 5 -->
UM2408 List of figures
5
List of figures

**Figure 1. Nucleo-144 board (Top view) . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .  . . 1**


**Figure 2. Nucleo-144 board (Bottom view)  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1**


**Figure 3. Hardware block diagram. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 12**


**Figure 4. Nucleo-144 board top layout. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .  . . 13**


**Figure 5. Nucleo-144 b oard mechanical drawing in millimeters. . . . . . . . . . . . . . . . . . . . . . . . . . . . . 14**


**Figure 6. Nucleo-144 board mechanical drawing in mils . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 15**


**Figure 7. USB composite device . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .  . . . 17**


**Figure 8. Power supply input from ST-LINK USB connec tor with PC (5 V, 500 mA max). . . . . . . . . 19**


**Figure 9. Power supply input from V IN (7-11 V, 800 mA max). . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20**


**Figure 10. Power supply input from 5V_EXT (5 V, 500 mA ma x) . . . . . . . . . . . . . . . . . . . . . . . . . . . . 20**


**Figure 11. Power supply input from ST-LINK USB connec tor with a USB charger (5 V). . . . . . . . . . . 21**


**Figure 12. Power supply input from 3V3V_ EXT (3.3 V). . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 22**


**Figure 13. Supply configuration 1: Internal LDO only  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 4**


**Figure 14. Supply configuration 2: Internal SMPS only (Defaul t). . . . . . . . . . . . . . . . . . . . . . . . . . . . . 24**


**Figure 15. Supply configuration 3: Internal SMPS and LDO cascaded . . . . . . . . . . . . . . . . . . . . . . . . 25**


**Figure 16. Supply configuration 4: Bypass  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 25**


**Figure 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q extens ion connectors. . . . . . . . . . . . . . . . . 36**


**Figure 18. NUCLEO-H7A3ZI-Q extension con nectors  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 37**


<!-- Page 6 -->
Features UM2408

## 1 Features

The STM32H7 Nucleo-144 boards offer the following features:
• STM32H7 Arm®(a) Cortex® core-based microcontroller in LQFP144 package
• Ethernet compliant with IEEE-802.3-2002 (Depending on STM32H7 support)
• USB OTG or full-speed device
• 3 user LEDs
• 2 push-buttons: user and reset
• LSE crystal:
– 32.768 kHz crystal oscillator
• Board connectors:
– USB with Micro-AB
– Ethernet RJ45 (only for NUCLEO -H745ZI-Q and NUCLEO-H755ZI-Q)
• Board expansion connectors:
– ST Zio including ARDUINO
® Uno V3
–S T  m o r p h o
• Flexible power-supply options: ST-LINK USB VBUS or external sources
• Onboard STLINK-V3E debugger/programmer with SWD connector:
– USB re-enumeration capabilit y: virtual COM port, mass storage, debug port
– STLINK-V3E standalone kit capability
• Comprehensive free software libraries and examples available with the STM32CubeH7
MCU Package
• Support of a wide choice of Integrated Development Environments (IDEs) including
IAR Embedded Workbench®, MDK-ARM, and STM32CubeIDE
a. Arm is a registered trademark of Arm Limited (o r its subsidiaries) in the US and/or elsewhere.

<!-- Page 7 -->
UM2408 Ordering information
56

## 2 Ordering information

To order the Nucleo-144 board corresponding to the target STM32, use the order code
given below in Table 1:

### 2.1 Codification

The meaning of the NUCLEO-XXYYZT-Q codification is explained in Table 2 with an
example:

**Table 1. Ordering information**

Order code Board reference Target STM32 Differentiating feature
NUCLEO-H745ZI-Q
MB1363(1)
1. Subsequently called main board in the rest of the documentation.
STM32H745ZIT6 – Ethernet
NUCLEO-H755ZI-Q STM32H755ZIT6 – Ethernet
– Cryptography
NUCLEO-H7A3ZI-Q STM32H7A3ZIT6Q – None

**Table 2. Codification explanation**

NUCLEO-XXYYZT-Q Description E xample: NUCLEO-H745ZI-Q
XX MCU series in STM32 32-bit Arm Cortex
MCUs STM32H7 series
YY MCU product line in the series STM32H745
Z STM32H7 package pin count:
– Z for 144 pins 144 pins
T STM32 flash memory size:
– I for 2 Mbytes 2 Mbytes
-Q STM32 has an internal SMPS function. Internal SMPS

<!-- Page 8 -->
Development environment UM2408

## 3 Development environment


### 3.1 Development toolchains

• IAR Systems® - IAR Embedded Workbench®(a)
• Keil® - MDK-ARM(a)
• STMicroelectronics - STM32CubeIDE

### 3.2 System requirements

• Windows® OS (7, 8 or 10), Linux®(b) 64-bit, or macOS®(c)
• USB Type-A or USB Type-C® to Micro-B cable

### 3.3 Demonstration software

The demonstration software, included in the STM32Cube package, is preloaded in the
STM32H7 flash memory for easy demonstration of the device peripherals in the Standalone
mode. The latest versions of the demonstration source code and associated documentation
can be downloaded from the www.st.com/stm32nucleo web page.

### 3.4 CAD resources

All board design resources, including schematics, CAD databases, manufacturing files, and
the bill of materials, are available from the NUCLEO-H745ZI-Q, NUCLEO-H755ZI-Q, and
NUCLEO-H7A3ZI-Q product pages at www.st.com.
a. On Windows only.
b. Linux ® is a registered trademark of Linus Torvalds.
c. macOS ® is a trademark of Apple Inc. registered in the U.S. and other countries.
All other trademarks are the property of their respective owners.

<!-- Page 9 -->
UM2408 Conventions
56

## 4 Conventions


**Table 3 provides the conventions used for the ON and OFF settings in the present**

document.
In this document, the references for all information that is common to all sale types, are
STM32H7 Nucleo-144 board and STM32H7 Nucleo-144 boards.

**Table 3. ON/OFF conventions**

Convention Definition
Jumper JPx ON Jumper fitted
Jumper JPx OFF Jumper not fitted
Solder bridge SBx ON SBx connections closed by solder or 0-ohm resistor
Solder bridge SBx OFF SBx connections left open

<!-- Page 10 -->
Safety recommendations UM2408

## 5 Safety recommendations


### 5.1 Targeted audience

This product targets users with at least basic electronics or embedded software
development knowledge such as engineers, technicians, or students. This board is not a toy
and is not suited for use by children.

### 5.2 Handling the board

This product contains a bare printed circuit board and like all products of this type, the user
must be careful about the following points:
• The connection pins on the board might be sharp. Be careful when handling the board
to avoid hurting yourself.
• This board contains static-sensitive devices. To avoid damaging it, handle the board in
an ESD.proof environment.
• While powered, do not touch the electric connections on the board with your fingers or
anything conductive. The board operates at a voltage level that is not dangerous, but
components might be damaged when shorted.
• Do not put any liquid on the board and avoid operating the board close to water or at a
high humidity level.
• Do not operate the board if dirty or dusty.

<!-- Page 11 -->
UM2408 Quick start
56

## 6 Quick start

The STM32H7 Nucleo-144 board is a low-cost and easy-to-use development kit, used to
evaluate and start development quickly with an STM32H7 microcontroller in an LQFP144
package.
Before installing and using the product, accept the evaluation product license agreement
from the www.st.com/epla web page. For more information on the STM32H7 Nucleo-144
and demonstration software, visit the www.st.com/stm32nucleo web page.

### 6.1 Getting started

Follow the sequence below to configure the Nucleo-144 board and launch the
demonstration application (For component locations, refer to Figure 4):
1. Check the JP2 (5 V) position (Refer to Section 7.4). Make sure JP4 (IDD) is selected
(Refer to Section 7.8) and make sure JP5 (1.8 V/3.3 V) is correctly configured (Refer to
Section 7.4.9)
2. To identify correctly the device interfaces from the host PC and before connecting the
board, install the Nucleo USB driver available on the www.st.com/stm32nucleo
website.
3. Power the board by connecting the STM32H7 Nucleo-144 board to a PC with a USB
Type-A to Micro-B cable through the USB connector CN1 on the ST-LINK. As a result,
the PWR green LED (LD5) and COM LED (LD4) light up and the red LED (LD3) blinks.
4. Press the B1 button (left button).
5. Observe the blinking frequency of the three LEDs (LD1 to LD3) changes by clicking on
the B1 button.
6. The software demonstration and the several software examples that allow the user to
use the Nucleo features are available at the www.st.com/stm32nucleo web page.
7. Develop an application, using the available examples.

<!-- Page 12 -->
Hardware layout and configuration UM2408

## 7 Hardware layout and configuration

The STM32H7 Nucleo-144 board is designed around the STM32H7 microcontrollers in a
144-pin LQFP package.

**Figure 3 shows the connections between the STM32H7 and its peripherals: STLINK-V3E,**

push-buttons, LEDs, USB, Ethernet (Only on NUCLEO-H745ZI-Q and
NUCLEO-H755ZI-Q), ST Zio connectors, and ST morpho headers.

**Figure 4 shows the location of these features on the STM32H7 Nucleo-144 board.**

The mechanical dimensions of the board are shown in Figure 5 and Figure 6.

**Figure 3. Hardware block diagram**

MSv51396V2
Embedded
STLINK-V3E
STM32
Microcontroller
ST morpho extension header
ST morpho extension header
IO
VCP
UART
B2
reset
button
B1
user
button
IO
Zio Connector
ST-LINK part
MCU part
LED1
LED2
LED3
Micro-AB
USB
connector
Zio Connector
RJ45
connector
Micro-B
USB
connector
SWD
SWD VCP
UART
USB RMII
No Ethernet on
NUCLEO-H7A3ZI-Q

<!-- Page 13 -->
UM2408 Hardware layout and configuration
56

### 7.1 Nucleo-144 board layout


**Figure 4. Nucleo-144 board top layout**

MSv51397V2
CN2: SWD connector CN1: ST-LINK USB connectorJP1: ST-LINK RST
LD6: ST-LINK OVCR LED (Red)
LD4: COM LED (Red/green)
LD5: PWR LED (Green)
JP2: Power selection
CN7: Zio connector
CN12: ST morpho
connector
CN10: Zio connector
CN14:
ETHERNET
RJ45 connector
(Only on
NUCLEO-H745ZI-Q
and
NUCLEO-H755ZI-Q)
B2: Reset button
CN5: MIPI10 connector
LD1-LD3: User LEDs
JP3: MCU RST
JP4: IDD measurement
CN13: User USB connector
JP5: 1.8 V / 3.3 V selection
CN8: Zio connector
CN9: Zio connector
U14: STM32
microcontroller
CN12: ST morpho
connector
B1: User button
LD7: USB OVCR LED
LD8: USB VBUS LED

<!-- Page 14 -->
Hardware layout and configuration UM2408

### 7.2 Mechanical drawing


**Figure 5. Nucleo-144 board mechanical drawing in millimeters**


<!-- Page 15 -->
UM2408 Hardware layout and configuration
56

**Figure 6. Nucleo-144 board mechanical drawing in mils**


<!-- Page 16 -->
Hardware layout and configuration UM2408

### 7.3 Program and debug th e onboard STM32H7 target

There are two different ways to program and debug the onboard STM32H7 MCU.
• Using the embedded STLINK-V3E
• Using an external debug tool
7.3.1 Use of STLINK-V3E to progr am and debug the onboard STM32H7
Features
The STLINK-V3E programming and debugging tool is integrated into the STM32H7 Nucleo-

## 144 board.

The embedded STLINK-V3E supports:
• 5 V power supplied by USB connector (CN1)
• USB 2.0 high-speed-compatible interface
• JTAG/serial wire debugging (SWD) specific features:
– 3 V to 3.6 V application voltage on th e JTAG/SWD interface and 5 V tolerant
inputs
–J T A G
– SWD and serial viewer (SWV) communication
• STDC14 (MIPI10) compatible connector (CN5)
• Status COM LED (LD4) which blinks during communication with the PC
• Fault red LED (LD6) alerting on USB overcurrent request
• 5 V/500 mA output power supply capability (U2) with current limitation and LED
• Green LED ON: 5 V enabled (LD5)
How to use
It is very easy to use the STLINK-V3E to program and debug the onboard STM32H7.
1. Verify that the jumper JP2 of power supply sources is correctly set (Refer to
Section 7.4).
2. Connect the board to a PC with a USB Type-A to Micro-B cable through the USB
connector CN1.
Note: For more details re garding the STLINK-V3E functionalities, refer to the STLINK-V3E user
manual on the www.st.com website
Drivers
Before connecting the product via USB to a PC running on Windows 7 or 8, a driver for the
STLINK-V3E must be installed (not required for Windows 10). It is available on the
www.st.com website.
In case the STM32H7 Nucleo-144 board is connected to the PC before installing the driver,
the PC device manager might report some Nucleo interfaces as Unknown.
To recover from this situation, after installing the dedicated driver, the association of
Unknown USB devices found on the STM32H7 Nucleo-144 board to this dedicated driver,
must be updated in the device manager manually.

<!-- Page 17 -->
UM2408 Hardware layout and configuration
56
Note: It is recommended to proceed using a USB composite device, as shown in Figure 7.

**Figure 7. USB composite device**

Note: 37xx = 374E for STLINK/V3 without bridge functions
374F for STLINK/V3 with bridge functions
STLINK-V3E firmware upgrade
The STLINK-V3E embeds a firmware upgrade mechanism for an in-place upgrade through
the USB port. As the firmware might evolve during the lifetime of the STLINK-V3E product
(for example new functionalities, bug fixes, and support for new microcontroller families), it
is recommended to keep the STLINK-V3E firmware up to date before starting to use the
STM32H7 Nucleo-144 board. The latest version of this firmware is available from the
www.st.com website.
7.3.2 Using an external debug tool to program and debug the onboard
STM32H7
There are two basic ways to support an external debug tool:
1. Keep the embedded STLINK-V3E running.
Power on the STLINK-V3E at first until the COM LED lights RED. Then the users
connect their external debug tool through the MIPI-10 debug connector (CN5).
2. Set the embedded STLINK-V3E in a high-impedance state:
When the users set the jumper JP1 (STLK_RST) ON, the embedded STLINK-V3E is in
RESET state, and all GPIOs are in high impedance. Then the users can connect their
external debug tool to the debug connector (CN5).
Note: Depending on the design requirement, the embedded STLINK-V3E hardware could be
completely disconnected if that is required early in the project.
37xx

**Table 4. MIPI-10 debug connector (CN5)**

MIPI-10
Pin
STDC14
Pin CN5 Designation
-1 N C Reserved
-2 N C Reserved

## 13 T _ V C C Target VCC


<!-- Page 18 -->
Hardware layout and configuration UM2408

### 7.4 Power supply and power selection

Five different sources can provide the power supply:
• A host PC is connected to CN1 through a USB cable (default setting),
• An external 7-11 V power supply connected to CN8 pin 15 or CN11 pin 24,
• An external 5 V power supply connected to CN11 pin 6,
• An external 5 V USB charger (5V_USB_CHGR) connected to CN1,
• Or an external 3.3 V power supply (3V3) connected to CN8 pin 7 or CN11 pin 16.
Note: In case V IN, EXT, CHGR, or 3.3 V is used to power the Nucleo-144 board, this power source
must comply with the EN 62368 1:2014/A11:2017 standard and must be safety extralow
voltage (SELV) with limited power capability.
In case the power supply is 3.3 V or a 5 V USB charger on CN1, the ST-LINK is not powered
and cannot be used.
7.4.1 Power supply input from STLINK -V3E USB connector (default setting)
The STM32H7 Nucleo-144 board and shield can be powered from the ST-LINK USB
connector CN1 (U5V), by placing a jumper on pins 1-2 of the JP2 STLK (Refer to Figure 8).
This is the default setting.

## 24 T _ S W D I O Target SWDIO using SWD protocol or Target JTMS (T_JTMS) using

JTAG protocol

## 35 G N D Ground


## 46 T _ S W C L K Target SWCLK using SWD protocol or Target JCLK (T_JCLK) using

JTAG protocol

## 57 G N D Ground


## 68 T _ S W O Target SWO using SWD protocol or Target JTDO (T_JTMS) using

JTAG protocol
7 9 T_JRCLK Not used by SWD protocol, Target JRCLK (T_JRCLK) using JTAG
protocol, only for specific use
8 10 T_JTDI Not used by SWD protocol, Target JTDI (T_JTDI) using JTAG protocol,
only for external tools
9 11 GNDDetect GND detection for plug indicator, used on SWD and JTAG neither
10 12 T_NRST Target NRST using SWD protocol or Target JTMS (T_JTMS) using
JTAG protocol
-1 3 T_VCP_RX Target RX used for VCP (Must be UART dedicated to the bootloader)
-1 4 T_VCP_TX Target TX used for VCP (Must be UART dedicated to the bootloader)

**Table 4. MIPI-10 debug connector (CN5) (continued)**

MIPI-10
Pin
STDC14
Pin CN5 Designation

<!-- Page 19 -->
UM2408 Hardware layout and configuration
56

**Figure 8. Power supply input from ST-LINK USB connector with PC (5 V, 500 mA max)**

If the USB enumeration succeeds, the 5V_ST_LINK power is enabled, by asserting the
PWR_ENn signal from STM32F723IEK6 ‘STLINK V3’ (U7). This pin is connected to a
power switch (U2), which powers the board. The power switch (U2) also features a current
limitation to protect the PC in case of an onboard short circuit. If an overcurrent of more than

## 500 mA happens on-board, the red LED (LD6) is ON.

The Nucleo board with its shield can be powered from ST-LINK USB connector CN1.
However, only the ST-LINK circuit gets power before USB enumeration because the host
PC only provides 100 mA to the board at that time.
During the USB enumeration, the Nucleo board asks for 500 mA power to the host PC.
• If the host can provide the required power, the enumeration finishes with a
SetConfiguration command. Then, the power switch (U2) is switched ON and the green
LED (LD5) is turned ON. Thus, the Nucleo board with its shield can consume 500 mA
current, but not more.
• If the host is not able to provide the requested current, the enumeration fails.
Therefore, the power switch (U2) remains OFF, and the MCU part including the
extension board is not powered. As a consequence, the green LED (LD5) remains
OFF. In this case, it is mandatory to use an external power supply.
7.4.2 External power supply input from VIN (7-11 V, 800 mA max)
When the STM32H7 Nucleo-144 board is power supplied by VIN (Refer to Table 4 and

**Figure 9), the jumper configuration must be the following: Jumper JP2 on pin 3-4 VIN.**


**Table 5. External power sources: VIN (7-11 V)**

Input power
name Connector pins Voltage range Max current Limitation
VIN
CN8 pin 15
CN11 pin 24 7 V to 11 V 800 mA
From 7 V to 11 V only and input current
capability is linked to input voltage:
– 800 mA input current when VIN=7 V
– 450 mA input current when 7 V<VIN<9 V
– 250 mA input current when 9 V<VIN<11 V

<!-- Page 20 -->
Hardware layout and configuration UM2408

**Figure 9. Power supply input from VIN (7-11 V, 800 mA max)**

Note: Refer to Section 7.4.6 about debugging when using an external power supply.
7.4.3 External power supply input  5V_EXT (5 V, 500 mA max)
When the STM32H7 Nucleo-144 board is power supplied by EXT (Refer to Table 6 and

**Figure 10), the jumper configuration must be the following: Jumper JP2 on pin 5-6 EXT.**

Note: Refer to 7.4.6 about debugging when using an external power supply.

**Figure 10. Power supply input from 5V_EXT (5 V, 500 mA max)**


**Table 6. External power sources: 5V_EXT**

Input power name Connector pins Voltage range Max current
EXT CN11 pin 6 4.75 V to

### 5.25 V 500 mA


<!-- Page 21 -->
UM2408 Hardware layout and configuration
56
7.4.4 External power supply i nput from USB charger (5 V)
When the STM32H7 Nucleo-144 board is power supplied by a USB charger on CN1 (Refer
to Table 7 and Figure 11), the jumper configuration must be the following: Jumper JP2 on
pin 7-8 CHGR.

**Figure 11. Power supply input from ST-LINK USB connector with a USB charger (5 V)**

7.4.5 External power supply input from 3V3_EXT (3.3 V)
When the 3.3 V is provided by a shield board, it is interesting to use the 3.3 V (CN8 pin 7 or
CN11 pin 16) directly as a power input (Refer to Table 8 and Figure 12). In this case, the
programming and debugging features are unavailable since the ST-LINK is not powered.

**Table 7. External power sources: CHGR (5 V)**

Input power name Connector pins Voltage range Max current
CHGR CN1 5 V -

**Table 8. External power sources: 3V3_EXT (3.3 V)**

Input power name Connector pins Voltage range Max current
3V3 CN8 pin 7
CN11 pin 16 3 V to 3.6 V -

<!-- Page 22 -->
Hardware layout and configuration UM2408

**Figure 12. Power supply input from 3V3V_EXT (3.3 V)**

7.4.6 Debugging while using VIN or EXT as an external power supply
When powered by VIN or EXT, it is still possible to use the ST-LINK for programming or
debugging only, but it is mandatory to power the board first using VIN or EXT, then to
connect the USB cable to the PC. In this way, the enumeration succeeds, thanks to the
external power source.
The following power-sequence procedure must be respected:
1. Connect jumper JP2 between pin 5 and pin 6 for EXT or between pin 3 and pin 4 for
VIN
2. Connect the external power source to V IN or EXT
3. Power on the external power supply: 7 V < V IN < 11 V for VIN or 5 V for EXT
4. Check that the green LED (LD5) is turned ON
5. Connect the PC to the USB connector CN1
If this order is not respected, USB (U5V) might power the board first, then by V
IN or EXT as
the following risks might be encountered:
1. If the board needs a current higher than  300 mA, the PC might be damaged or the
supplying PC can limit the current. As a consequence, the board is not powered
correctly.
2. 300 mA is requested at enumeration so th ere is a risk that the request is rejected.
Thus, the enumeration does not succeed if the PC cannot provide such a current.
Consequently, the board is not power supplied (the LED (LD5) remains OFF).
7.4.7 External pow er supply output
When powered by USB, VIN, or EXT, the 5 V (CN8 pin 9 or CN11 pin 18) can be used as an
output power supply for an ST Zio shield or an extension board. In this case, the maximum
current of the power source is specified in Table 9.

<!-- Page 23 -->
UM2408 Hardware layout and configuration
56
When the STM32H7 Nucleo-144 board is power supplied by VIN, the jumper configuration
must be the following: Jumper JP2 on pin 3-4 VIN.
The 3.3 V (CN8 pin 7 or CN11 pin 16) can be used also as a power supply output. The
current is limited by the maximum current capability of the regulator U11 (1300 mA max).
7.4.8 Internal SMPS/LDO configuration
Different internal SMPS/LDO configurations are supported. Refer to Table 10 for the solder
bridge configuration corresponding to each of them.

**Table 9. External power sources maximum currents**

Input power
name Connector pins Voltage range Max current Limitation
VIN CN8 pin 15
CN11 pin 24 7 V to 11 V 800 mA
From 7 V to 11 V only and input current
capability is linked to input voltage:
– 800 mA input current when VIN=7 V
– 450 mA input current when 7 V<VIN<9 V
– 250 mA input current when9 V<VIN<11 V
EXT CN11 pin 6 4.75 V to 5.25 V 500 mA -
CHGR CN1 5 V - -
3.3V CN8 pin 7
CN11 pin 16 3 V to 3.6 V - -

**Table 10. SMPS/LDO configuration**

Supply config 1:
Internal LDO only
Supply config 2:
Internal SMPS only
(Default)
Supply config 3:
Internal SMPS and
LDO cascaded
Supply config 4:
Bypass
SMPS OFF ON ON OFF
LDO ON OFF ON OFF
SB25 X - --
R33 X - -X
SB74 X - -X
C58 2.2 uF 100 nF 2.2 uF 100 nF
C54 2.2 uF 100 nF 2.2 uF 100 nF
SB92 - X X-
SB79 - X X-
SB75 - X -X
SB76 - X XX
R35 - X --
R34 - - X-

<!-- Page 24 -->
Hardware layout and configuration UM2408

**Figure 13. Supply configuration 1: Internal LDO only**


**Figure 14. Supply configuration 2: Internal SMPS only (Default)**

SB97 - - -X
SB95 - - -X

**Table 10. SMPS/LDO configuration (continued)**

Supply config 1:
Internal LDO only
Supply config 2:
Internal SMPS only
(Default)
Supply config 3:
Internal SMPS and
LDO cascaded
Supply config 4:
Bypass

<!-- Page 25 -->
UM2408 Hardware layout and configuration
56

**Figure 15. Supply configuration 3: Internal SMPS and LDO cascaded**


**Figure 16. Supply configuration 4: Bypass**

Warning: Board SMPS/LDO firmware po wer configuration must match
with the hardware configuration. If not, the user faces a
deadlock: after the reset, STLINK cannot connect to the
target anymore.
The firmware power configuration corresponds with the
following in the main.c file, in the function
SystemClock_Config:
-in the case of the direct SMPS hardware configuration
(default): HAL_PWREx_ConfigSupply

<!-- Page 26 -->
Hardware layout and configuration UM2408
(PWR_DIRECT_SMPS_SUPPLY);
-In the case of “LDO” hardware configuration:
HAL_PWREx_ConfigSupply (PWR_LDO_SUPPLY);
If a deadlock is faced due to a mismatch between the
hardware board setting and the firmware setting
(LDO/SMPS), the user can recover the board by doing the
following:
-Power off the board
-Connect the CN11 BT0 pin (BOOT0) to VDD using a wire
-This changes the BOOT0 pin to HIGH instead of LOW and
thus the device boot address is changed to boot address 1.
This makes the bootloader start in the system memory,
instead of starting the firmware in the user flash (firmware
that is setting a wrong LDO/SMPS configuration)
-Power on the board and connect using
STM32CubeProgrammer
-Erase the user flash
-Power off the board and remove the wire between BOOT0
and VDD
-The recovery is now completed and the board can be used
normally.
7.4.9 JP5: 1.8 V/3.3 V selection
JP5 labeled VDD_MCU is used to select the VDD supply for the MCU:
• Jumper JP5 pin 1-2: VDD_MCU = 3.3 V (default configuration)
• Jumper JP5 pin 2-3: VDD_MCU = 1.8 V

### 7.5 External SMPS selection

The user can connect an external SMPS to the CN6 footprint (for example
ST1PS02D1QTR).
The CN6 pinout is described in Table 11.

**Table 11. External SMPS selection**

CN6 pin Pin name Function
Configuration when
using an external
module connected to
CN6
CN6 Pin 1 3V3_VDD 3.3V VDD supply for MCU -
CN6 Pin 2 SMPS_V1 (PG8) External SMPS output voltage
selection “D1”(1) SB34 ON/SB31 OFF
CN6 pin 3 VOUTVDD External 3.3V VDD supply for
MCU -
CN6 Pin 4 SMPS_EN  (PG9) External SMPS Enable “EN” (1) SB35 ON/SB41 OFF

<!-- Page 27 -->
UM2408 Hardware layout and configuration
56

### 7.6 LEDs

User LD1: a green user LED is connected to the STM32H7 I/O PB0 (SB65 OFF and SB54
ON) or PA5 (SB65 ON and SB54 OFF) corresponding to the ST Zio D13.
User LD2: a yellow user LED is connected to PE1.
User LD3: a red user LED is connected to PB14.
Caution: LD3 cannot be used if Zio D65 is used (In this case, remove R75).
These user LEDs are on when the I/O is HIGH value, and are off when the I/O is LOW.
LD4 COM: the tricolor LED (LD4), green, orange, and red, provides information about ST-
LINK communication status. LD4 default color is red. LD4 turns to green to indicate that
communication is in progress between the PC and the STLINK-V3E, with the following
setup:
• Slow blinking red or OFF: At power-on before USB initialization
• Fast blinking red or OFF: After the first correct communication between the PC and
STLINK-V3E (Enumeration)
• Red LED ON: When the initialization between the PC and STLINK-V3E is complete
• Green LED ON: After a successful target communication initialization
• Blinking red/green: During communication with the target
• Green ON: Communication finished and successful
• Orange ON: Communication failure
LD5 PWR: the green LED indicates that the STM32H7 part is powered and +5 V power is
available on CN8 pin 9 and CN11 pin 18.
LD6 USB power fault: LD5 indicates that the board power consumption on the USB
exceeds 500 mA, consequently, the user must power the board using an external power
supply.
LD7 and LD8 USB FS: refer to Section 7.11: USB OTG or full-speed device.
CN6 pin 5 1V2_VOUTCORE
External SMPS 1.2V or external
power supply providing 1.2V for
MCU VCORE (VCAP pins)
-
CN6 Pin 6 SMPS_SW  (PG10) External SMPS Enable for the
auxiliary output voltage “AUX”(1) SB33 ON/SB30 OFF
CN6 pin 7 GND GND -
CN6 Pin 8 SMPS_PG (PG12) External SMPS Power good
“PGOOD”
(1) SB42 ON/SB45 OFF
1. Refer to the ST1PS02D1QTR datasheet for more information about possible use cases.

**Table 11. External SMPS selection (continued)**

CN6 pin Pin name Function
Configuration when
using an external
module connected to
CN6

<!-- Page 28 -->
Hardware layout and configuration UM2408

### 7.7 Push-buttons

B1 user (Blue button): The user button is connected to the I/O PC13 by default (tamper
support, SB82 ON and SB81 OFF). If not, it is connected to PA0 (wake-up support, SB81
ON and SB82 OFF) of the STM32H7 microcontroller.
B2 reset (Black button): This push button is connected to NRST and is used to reset the
STM32H7 microcontroller.

### 7.8 JP4 (IDD)

The JP4 labeled jumper is used to measure the STM32H7 microcontroller consumption by
removing the jumper and connecting an ammeter:
• JP4 ON: STM32H7 is powered (default position),
• JP4 OFF: An ammeter must be connected to measure the STM32H7 current. If there is
no ammeter, the STM32H7 is not powered.
As VDD is supplying level shifters (U1 and U10), for very-low power measurement, it is
recommended to set a pull-up or pull-down resistor, via software for PA13, PB3, and PD8
pins.
Warning: On MB1363 Rev.C, VDD_MCU is also supplying Ethernet PHY
(U15) and debug voltage translation (U1 and U10).
On MB1363 Rev.C: If needed, for low-power measurement (for example the Standby
mode), to measure only MCU (U7) power consumption, the user must remove the following
components: R4, R43, R44, R45, R46, R47, R48, R49, R50, R51, R52, R53, R59, R64, U1,
U10, U15, and SB72.
After removing these components, it becomes impossible to use Ethernet, and 1.8 V debug
with ST-LINK.

### 7.9 OSC clock

7.9.1 OSC clock supply
There are four ways to configure the pins corresponding to the external high-speed clock
(HSE):
• MCO from ST-LINK (default): The MCO output of ST-LINK is used as an input clock.
By default, it is fixed at 8 MHz and connected to the PF0/PH0-OSC_IN of the
STM32H7 series microcontroller. The frequency might be changed during an ST-LINK
firmware upgrade (refer to RN0093 for details). The configuration must be:
–S B 7 1  O F F
–S B 7 2  O N
– SB3 and SB4 OFF
• The onboard HSE oscillator from the X2 crystal: For typical frequencies, capacitors,
and resistors, refer to the STM32H7 microcontroller datasheet and the application note
Oscillator design guide for STM8AF/AL/S and STM32 microcontrollers (AN2867)

<!-- Page 29 -->
UM2408 Hardware layout and configuration
56
available at www.st.com. The X2 crystal has the following characteristics: 25 MHz,

## 6 pF, 20 ppm. It is recommended to use NX2016SA-25MHz-EXS00A-CS11321

manufactured by NDK. The configuration must be:
– SB71 and SB90 OFF
– SB3 and SB4 ON
– C74 and C76 ON with 5.1 pF capacitors
–S B 7 2  O F F
Oscillator from external PH0: from an external oscillator through pin 29 of the CN11
connector. The configuration must be:
–S B 7 1  O N
–S B 7 2  O F F
– SB3 and SB4 removed
• HSE not used: PH0 and PH1 are used as GPIOs instead of clocks. The configuration
must be:
– SB71 and SB90 ON
– SB72 (MCO) OFF
– SB3 and SB4 removed
7.9.2 OSC 32 KHz
There are three ways to configure the pins corresponding to the low-speed clock (LSE):
• Onboard oscillator (Default): X3 crystal. Refer to the application note Oscillator
design guide for STM8AF/AL/S and STM32 microcontrollers (AN2867) available at
www.st.com. It is recommended to use NX3215SA-32.768KHZ EXS00A-MU00525
(32.768 kHz, 6 pf load capacitance, 20 ppm) from NDK.
• Oscillator from external PC14: From the external oscillator, through the pin 25 of the
CN11 connector. The configuration must be:
– SB66 and SB67 ON
– R41 and R42 removed
• LSE not used: PC14 and PC15 are used as GPIOs instead of low-speed clocks. The
configuration must be:
– SB66 and SB67 ON
– R41 and R42 removed

### 7.10 USART communication

The USART3 interface, available on PD8 and PD9 of the STM32H7, can be connected
either to ST-LINK or to the ST morpho connector. The choice is changed by setting the
related solder bridges. By default, the USART3 communication between the target
STM32H7 and the STLINK is enabled, to support the Virtual COM port (SB16 and SB17
ON).

<!-- Page 30 -->
Hardware layout and configuration UM2408

### 7.11 USB OTG or full-speed device

The STM32H7 Nucleo-144 board supports USB OTG or full-speed device communication
via a USB Micro-AB connector (CN13) and USB power switch (U18) connected to VBUS.
Warning: The USB Micro–AB connector (CN13) cannot power the
Nucleo-144 board. To avoid damaging the STM32, it is
mandatory to power the Nucleo-144 before connecting a USB
cable to the CN13 connector. Otherwise, there is a risk of
current injection on STM32H7 I/Os.
A green LED (LD8) lights in one of these cases:
• The power switch (U18) is ON and the STM32H7 Nucleo-144 board works as a USB
host.
• VBUS is powered by another USB host when the STM32H7 Nucleo-144 board works as
a USB Device.
The red LED (LD7) lights if overcurrent occurs when 5 V is enabled on VBUS in the USB
Host mode.
Note: 1.It is recommended to power the Nucleo-144 board with an external power supply when
using the USB OTG or host function.

**Table 12. USART3 pins**

Pin
name Function Virtual COM port (Default
configuration)
ARDUINO® D0 and
D1
ST morpho
connection
PD8 USART3 TX SB103 OFF, SB16 ON and
SB15 OFF
SB103 OFF, SB16
OFF and SB15 ON
SB103 ON, SB16
OFF, SB15 OFF
PD9 USART3 RX SB104 OFF, SB17 ON and
SB94 OFF
SB104 OFF, SB17
OFF and SB94 ON
SB104 ON, SB17
OFF and SB94 OFF

**Table 13. USB pin configuration**

Pin
name Function Configuration when using USB
connector(1)
Configuration when using ST
morpho connector Remark
PA8 USB SOF - - Test point
TP4
PA9 USB VBUS SB21 ON SB21 OFF -
PA10 USB ID SB29 ON SB29 OFF -
PA11 USB DM SB26 ON SB26 OFF -
PA12 USB DP SB27 ON SB27 OFF -
PD10 USB FS PWR
EN
NUCLEO-H745ZI-Q/
NUCLEO-H755ZI-Q: JP8 ON
NUCLEO-H7A3ZI-Q: JP8 OFF
NUCLEO-H745ZI-Q/
NUCLEO-H755ZI-Q: JP8 OFF
NUCLEO-H7A3ZI-Q: JP8 OFF
-

<!-- Page 31 -->
UM2408 Hardware layout and configuration
56
ESD protection part USBLC6-2SC6 is implemented on the USB port because all USB pins
on STM32H7 are dedicated to the USB port only on the STM32H7 Nucleo-144 board. USB
pin ID is not used.

### 7.12 Ethernet (with NUCLEO-H74 5ZI-Q and NUCLEO-H755ZI-Q)

The STM32H7 Nucleo-144 board supports 10M/100M Ethernet communication by a PHY
LAN8742A-CZ-TR (U15) and RJ45 connector (CN14). Ethernet PHY is connected to the
STM32H7 microcontroller via the RMII interface. The PHY RMII_REF_CLK generates the

## 50 MHz clock for the STM32H7 microcontroller.

Note: 1.NUCLEO-H7A3ZI-Q does not support the Ethernet function.
2.JP6 and JP7 must be ON when using Ethernet.
3.On MB1363 Rev.C: Ethernet PHY LAN8742A must be set in the Power-down mode (In
this mode, the Ethernet PHY reference clock turns off) to achieve the expected Low-power
mode current. This is done by configuring the Ethernet PHY LAN8742A basic control
register, at the 0x00 address, bit 11 (power down) set HIGH. SB13 can also be removed to
get the same effect.
PF10 USB FS PWR
EN
NUCLEO-H745ZI-Q/
NUCLEO-H755ZI-Q: SB91 OFF
NUCLEO-H7A3ZI-Q: SB91 ON
NUCLEO-H745ZI-Q/
H755ZI-Q: SB91 OFF
NUCLEO-H7A3ZI-Q: SB91 OFF
-
PG7 USB FS OVCR SB100 ON SB100 OFF -
1. The default SBx/JPx state is shown in bold.

**Table 13. USB pin configuration (continued)**

Pin
name Function Configuration when using USB
connector(1)
Configuration when using ST
morpho connector Remark

**Table 14. Ethernet pin configuration**

Pin
name Function
Conflict
with ST
Zio
connector
signal
Configuration when using Ethernet(1)
Configuration
when using ST
Zio or ST morpho
connector
PA1 RMII Ref
Clock - NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: SB80 ON SB80 OFF
PA2 RMII MDIO A8 NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: JP7 ON JP7 OFF
PC1 RMII MDC - NUCLEO-H745ZI -Q/NUCLEO-H755ZI-Q: SB84 ON SB84 OFF
PA7 RMII RX
Data Valid - NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: SB52 ON SB52 OFF
PC4 RMII RXD0 - NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: SB63 ON SB63 OFF
PC5 RMII RXD1 - NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: SB48 ON SB48 OFF
PG11 RMII TX
Enable - NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: SB36 ON SB36 OFF

<!-- Page 32 -->
Hardware layout and configuration UM2408

### 7.13 Motor control

To be compatible with X-NUCLEO-IHM09M1, X-NUCLEO-IHM07M1 (L6230), and X-
NUCLEO-IHM08M1 (STL220N6F7) motor-control shields, the user must fit CN11 and CN12
(Samtec TSW-135-07-x-D), and set solder bridges according to Table 15.

### 7.14 Solder bridges

PG13 RXII TXD0 - NUCLEO-H745 ZI-Q/NUCLEO-H755ZI-Q: SB46 ON SB46 OFF
PB13 RMII TXD1 D18 NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: JP6 ON, SB8
OFF JP6 OFF, SB8 ON
1. The default SBx state is shown in bold.

**Table 14. Ethernet pin configuration (continued)**

Pin
name Function
Conflict
with ST
Zio
connector
signal
Configuration when using Ethernet(1)
Configuration
when using ST
Zio or ST morpho
connector

**Table 15. Motor-control shield configuration**

Pin name Function Configuration when using ST morpho
connector for motor control(1)
1. The default SBx state is shown in bold.
PE6 MORPHO RIGHT 14 (BKIN2) SB10 ON/SB22 OFF
PE15 MORPHO RIGHT 28 (BKIN1) SB38 ON/SB37 OFF
PA4 MORPHO RIGHT 29 (DAC) SB77 ON/SB59 OFF
PE8 MORPHO RIGHT 30 (TIM1_CH1N) SB47 ON/SB43 OFF
PA1 RMII reference clock SB80 OFF
PC1 RMII MDC SB84 OFF

**Table 16. Solder bridges**

Bridge State (1) Description
SB1 (3V3_PER)
(Only on MB1363
Rev.C)
ON Peripheral power 3V3_PER is connected to 3.3V.
OFF Peripheral power 3V3_PER is not connected.
SB2 (3.3V)
ON Output of voltage regulator ST1L05CPU33R is connected to 3.3V.
OFF Output of voltage regulator ST1L05CPU33R is not connected.
SB103, SB104
(GPIO)
ON
PD8 and PD9 on STM32H7 are connected to ST morpho connectors CN11
and CN12. If these pins are used on ST morpho connectors, SB16, SB17,
SB15, and SB94 must be OFF.
OFF PD8 and PD9 on STM32H7 are disconnected from ST morpho connectors
CN11 and CN12.

<!-- Page 33 -->
UM2408 Hardware layout and configuration
56
SB16, SB17, SB12,
SB64 (ST-LINK-
USART)
ON, ON,
OFF, OFF
PA2 and PA3 on ST-LINK STM32F723IEK6 are connected to PD8 and PD9
to enable Virtual COM port for Mbed support. Thus PD8 and PD9 on ST
morpho connectors cannot be used.
OFF, OFF,
ON, ON
PA2 and PA3 on ST-LINK STM32F723IEK6 are connected to PB6 and PB7
to enable Virtual COM port for Mbed support. Thus PB6 and PB7 on ST
morpho connectors cannot be used.
OFF, OFF,
OFF, OFF
PB6, PB7, PA2, and PA3 on ST-LINK STM32F723IEK6 are disconnected
from PD8 and PD9 on STM32H7.
SB11, SB96, SB15,
SB94 (ARDUINO®
USART)
ON, ON,
OFF, OFF
ARD D1 (USART TX) and ARD D0 (USART RX) are connected to PB6 and
PB7. Thus PB6 and PB7 on ST morpho connectors cannot be used.
OFF, OFF,
ON, ON
ARD D1 (USART TX) and ARD D0 (USART RX) are connected to PD8 and
PD9. Thus PD8 and PD9 on ST morpho connectors cannot be used.
OFF, OFF,
OFF, OFF
ARD D1 (USART TX) and ARD D0 (USART RX) are disconnected from
PB6, PB7, PD8, and PD9 on STM32H7.
R40 (VDDA)
ON VDDA and VREF+ on STM32H7 are connected to VDD.
OFF
VDDA and VREF+ on STM32H7 are not connected to VDD and can be
provided from pin 6 of CN7 (Used for external VREF+ provided by
ARDUINO® shield).
JP1 (STLINK NRST)
OFF No incidence on ST-LINK STM32F723IEK6 NRST signal.
ON ST-LINK STM32F723IEK6 NRST signal is connected to GND (ST-LINK
reset to reduce power consumption).
SB49, SB50 (SWO)
ON, ON
SWO signal of the STM32H7 (PB3) is connected to ST-LINK SWO input
(connected to pin 15 of CN7 Zio connector and to pin 8 of CN5 MIPI10
debug connector).
OFF, ON SWO signal of the STM32H7 (PB3) is connected to ST-LINK SWO input
(connected to pin 8 of CN5 MIPI10 debug connector only).
ON, OFF Alternate functions of PB3, I2S_B_CK/SPI_B_SCK are connected to Zio
pin 15 of CN7 and can be used.
OFF, OFF PB3 is not connected.
JP3 (T_NRST)
ON Board RESET signal (NRST) is connected to ST-LINK reset control I/O
(T_NRST).
OFF Board RESET signal (NRST) is not connected to ST-LINK reset control I/O
(T_NRST).
SB24, SB13, SB14
(IOREF)
OFF, OFF, ON IOREF is connected to VDD_MCU
OFF, ON,
OFF IOREF can be connected to 3V3_PER only on Rev.C
ON, OFF,
OFF IOREF is connected to 3V3
SB20 (SDMMC_D0),
SB9 (SDMMC_D1)
ON These pins are connected to ST morpho connector CN12.
OFF These pins are disconnected from ST morpho connector CN12 to avoid
stub of SDMMC data signals on PCB.

**Table 16. Solder bridges (continued)**

Bridge State (1) Description

<!-- Page 34 -->
Hardware layout and configuration UM2408
SB54, SB65 (LD1-
LED)
ON, OFF Green user LED (LD1) is connected to PB0.
OFF, ON Green user LED (LD1) is connected to D13 of ARDUINO® signal (PA5).
OFF, OFF Green user LED (LD1) is not connected.
ON, ON Forbidden
R74 (LD2-LED)
ON Yellow user LED (LD2) is connected to PE1.
OFF Yellow user LED (LD2) is not connected.
R75 (LD3-LED)
ON Red user LED (LD3) is connected to PB14.
OFF Red user LED (LD3) is not connected.
SB56, SB57 (D11)
NUCLEO-
H745ZI-
Q/NUCLEO-
H755ZI-Q:
OFF, ON
NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: D11 (Pin 14 of CN7) is
connected to STM32H7 PB5 (SPI_A_MOSI/TIM_D_PWM2).
NUCLEO-
H7A3ZI-Q:
ON, OFF
NUCLEO-H7A3ZI-Q: D11 (Pin 14 of CN7) is connected to STM32 PA7
(SPI_A_MOSI/TIM_E_PWM1).
SB66, SB67 (X2
crystal)
OFF PC14 and PC15 are not connected to ST morpho connector CN11 (X3 is
used to generate a 32 KHz clock).
ON PC14 and PC15 are connected to ST morpho connector CN11. (R41 and
R42 must be removed).
SB71 (PH0), SB90
(PH1) (Main clock)
OFF, ON
PH0 is not connected to ST morpho
PH1 is connected to the ST morpho connector CN11
(MCO is used as the main clock for STM32H7 on PH0).
OFF, OFF PH0 and PH1 are not connected to the ST morpho connector CN11.
ON, ON PH0 and PH1 are connected to the ST morpho connector CN11. SB3, SB4,
and SB72 must be removed.
SB72 (MCO)
ON MCO of ST-LINK (STM32F723IEK6) is connected to PF0/PH0 of
STM32H7.
OFF MCO of ST-LINK (STM32F723IEK6) is not connected to PF0/PH0 of
STM32H7.
SB3, SB4 (External

## 25 MHz crystal)

OFF PH0 and PH1 are not connected to external 25 MHz crystal X2.
ON PH0 and PH1 are connected to external 25 MHz crystal X2.
SB83 (VBAT)
ON VBAT pin of STM32H7 is connected to VDD_MCU.
OFF VBAT pin of STM32H7 is not connected to VDD_MCU.
SB82, SB81 (B1-
USER)
ON, OFF B1 push-button is connected to PC13.
OFF, ON B1 push-button is connected to PA0 (Set SB82 OFF if the ST Zio connector
is used).
OFF, OFF B1 push-button is not connected.
SB89 (PA0)
ON PA0 is connected to the ST Zio connector (Pin 29 of CN10)
OFF PA0 is not connected to the ST Zio connector (Pin 29 of CN10)

**Table 16. Solder bridges (continued)**

Bridge State (1) Description

<!-- Page 35 -->
UM2408 Hardware layout and configuration
56
All the other solder bridges present on the STM32H7 Nucleo-144 board are used to
configure several I/Os and power supply pins for compatibility of features and pinout with
the supported STM32H7 target.
The STM32H7 Nucleo-144 board is delivered with the solder bridges configured according
to the supported target STM32H7.
SB69, SB40 (ADC on
A4 and A5)
SB62, SB39 (I2C on
A4 and A5)
ON, ON,
OFF, OFF
PC2, PF11 (ADC) are connected to A4 and A5 (Pins 9 and 11) on the ST
Zio connector CN9.
OFF, OFF,
ON, ON
PB9 and PB8 (I2C) are connected to A4 and A5 (Pins 9 and 11) on the ST
Zio connector CN9.
SB73, SB70 (A6)
ON, OFF PF6 connected to Zio A6  (NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q)
OFF, ON PC1 connected to Zio A6 (NUCLEO-H7A3ZI-Q)
SB93, SB58 (A7)
ON, OFF PF10 connected to Zio A7  (NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q)
OFF, ON PC5 connected to Zio A7 (NUCLEO-H7A3ZI-Q)
SB78 (A8)
ON PA2 connected to Zio A8 (NUCLEO-H7A3ZI-Q)
OFF PA2 disconnected from Zio A8 (H745)
RMII signals
SB80 (PA1), SB84
(PC1), JP7 (PA2),
SB63 (PC4), SB48
(PC5), SB46 (PG13),
SB36 (PG11)
ON These pins are used as RMII signals and connected to the Ethernet PHY.
These ports must not be used on ST morpho or ST Zio connectors.
OFF(2)
1.NUCLEO-H745ZI-Q/NUCLEO-H755ZI-Q: These pins are used as GPIOs
on ST morpho connectors and not connected to the Ethernet PHY .
2.NUCLEO-H7A3ZI-Q: No Ethernet
RMII signals
SB98 (Ethernet
nRST)
ON NRST of STM32H7 is connected to Ethernet PHY (U15).
OFF(2) NRST of STM32H7 is not connected to Ethernet PHY (U15).
SB26 (PA12), SB27
(PA11)
ON These pins are used as D+ and D- on the USB connector (CN13).
OFF These pins are used as GPIOs on ST morpho connectors.
1. The default SBx state is shown in bold.
2. No default configuration for RMII signals.

**Table 16. Solder bridges (continued)**

Bridge State (1) Description

<!-- Page 36 -->
Hardware layout and configuration UM2408

### 7.15 Extensio n connectors

For each STM32H7 Nucleo-144 board, the following figures show the signals connected by
default to the ST Zio connectors (CN7, CN8, CN9, and CN10), including the support for
ARDUINO® Uno V3.

**Figure 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q extension connectors**

MSv61211V1
NUCLEO-H745ZI-Q
NUCLEO-H755ZI-Q
CN7
CN10
CN8
CN9
1
3
5
7
9
11
13
15
17
19
2
4
6
8
10
12
14
16
18
20
1
3
5
7
9
11
13
15
2
4
6
8
10
12
14
16
1
3
5
7
9
11
13
15
17
19
21
23
25
27
29
2
4
6
8
10
12
14
16
18
20
22
24
26
28
30
1
3
5
7
9
11
13
15
17
19
21
23
25
27
29
31
33
2
4
6
8
10
12
14
16
18
20
22
24
26
28
30
32
34
D65
D66
D67
GND
D68
D69
D70
D71
D72
VIN
GND
GND
+5V
+3V3
RESET
IOREF
NC
A5
A4
A3
A2
A1
A0
D50
D49
D48
D47
D46
D45
D44
D43
GND
D55
D54
D53
D52
D51
D64
D63
D62
D61
D60
D59
D58
D57
D56
D34
D33
D32
GND
D31
D30
D29
D28
GND
D25
D24
D23
D22
D21
D20
D19
D18
D17
D16
D27
D26
A8
A7
A6
GND
AGND
AVDD
D8
D9
D10
D11
D12
D13
GND
AVDD
D14
D15
D0
D1
D2
D3
D4
D5
D6
D7
D35
D36
D37
D38
D39
D40
GND
D41
D42
PB14
PD1
PD0
GND
PF15
PF14
PB5
PE9
PB2
VIN
GND
GND
5V
3V3
NRST
IOREF
NC
PF11
PC2
PB1
PC3
PC0
PA3
PC8
PG8
PG10
PD2
PC12
PC11
PC10
PC9
GND
PD3
PD4
PD5
PD6
PD7
PD10
PF9
PF7
PF8
PE3
PE6
PE4
PE2
PE5
PE0
PB0
PA0
GND
PE2
PD11
PD12
PD13
GND
PB4
PA4
PB3
PB5
PC7
PA15
PB12
PB13
PB15
PC6
PB2
PG6
PA2
PF10
PF6
GND
AGND
VDDA
PG9
PD15
PD14
PB5
PA6
PA5
GND
VREFP
PB9
PB8
PB7
PB6
PG14
PE13
PE14
PE11
PA8
PG12
PB11
PB10
PE15
PE6
PE12
PE10
PE7
PE8
GND
Arduino subset of Zio =      A0 to A5 and D0 to D15
Zio extension =                  A6 to A8 and D16 to D72
USB
OTG
USB
ST-LINK
PXX: Differences compared with NUCLEO-H743ZI
ETHERNET

<!-- Page 37 -->
UM2408 Hardware layout and configuration
56

**Figure 18. NUCLEO-H7A3ZI-Q extension connectors**


### 7.16 ST Zio connectors

CN7, CN8, CN9, and CN10 are female connectors on the top side and male connectors on
the bottom side. They include support for ARDUINO® Uno V3. Most shields designed for
ARDUINO® Uno V3 can fit the STM32H7 Nucleo-144 board.
To cope with ARDUINO® Uno V3, apply the following modifications:
• SB39 and SB62 must be ON
• SB40 and SB69 must be OFF to connect I2C on A4 (pin 5) and A5 (pin 6 of CN9).
Caution:1 The I/Os of the STM32H7 microcontroller are 3.3 V compatible instead of 5 V for
ARDUINO® Uno V3.
Caution:2 R40 must be removed before implementing the ARDUINO® shield with VREF+ power being
provided on CN7 pin 6. Refer to Table 16: Solder bridges for details on R40.

**Table 17 shows the pin assignment for each STM32H7 microcontroller on the ST Zio**

connectors.
MSv53277V1
NUCLEO-H7A3ZI-Q
CN7
CN10
CN8
CN9
1
3
5
7
9
11
13
15
17
19
2
4
6
8
10
12
14
16
18
20
1
3
5
7
9
11
13
15
2
4
6
8
10
12
14
16
1
3
5
7
9
11
13
15
17
19
21
23
25
27
29
2
4
6
8
10
12
14
16
18
20
22
24
26
28
30
1
3
5
7
9
11
13
15
17
19
21
23
25
27
29
31
33
2
4
6
8
10
12
14
16
18
20
22
24
26
28
30
32
34
D65
D66
D67
GND
D68
D69
D70
D71
D72
VIN
GND
GND
+5V
+3V3
RESET
IOREF
NC
A5
A4
A3
A2
A1
A0
D50
D49
D48
D47
D46
D45
D44
D43
GND
D55
D54
D53
D52
D51
D64
D63
D62
D61
D60
D59
D58
D57
D56
D34
D33
D32
GND
D31
D30
D29
D28
GND
D25
D24
D23
D22
D21
D20
D19
D18
D17
D16
D27
D26
A8
A7
A6
GND
AGND
AVDD
D8
D9
D10
D11
D12
D13
GND
AVDD
D14
D15
D0
D1
D2
D3
D4
D5
D6
D7
D35
D36
D37
D38
D39
D40
GND
D41
D42
PB14
PD1
PD0
GND
PF15
PF14
PB5
PE9
PB2
VIN
GND
GND
5V
3V3
NRST
IOREF
NC
PF11
PC2
PB1
PC3
PC0
PA3
PC8
PG8
PG10
PD2
PC12
PC11
PC10
PC9
GND
PD3
PD4
PD5
PD6
PD7
PD10
PF9
PF7
PF8
PE3
PE6
PE4
PE2
PE5
PE0
PB0
PA0
GND
PE2
PD11
PD12
PD13
GND
PB4
PA4
PB3
PB5
PC7
PA15
PB12
PB13
PB15
PC6
PB2
PG6
PA2
PC5
PC1
GND
AGND
VDDA
PG9
PD15
PD14
PA7
PA6
PA5
GND
VREFP
PB9
PB8
PB7
PB6
PG14
PE13
PE14
PE11
PA8
PG12
PB11
PB10
PE15
PE6
PE12
PE10
PE7
PE8
GND
Arduino subset of Zio =      A0 to A5 and D0 to D15
Zio extension =                  A6 to A8 and D16 to D72
USB
OTG
USB
ST-LINK
PXX: Differences compared with NUCLEO-H743ZI

<!-- Page 38 -->
Hardware layout and configuration UM2408

**Table 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q pin assignments**

CN Pin Pin
name Signal name STM32H7
pin Function Remark
CN8
1N C N C - - ARDUINO® support

## 3 IOREF IOREF - 3.3 V Ref ARDUINO® support


## 5 NRST NRST NRST RESET ARDUINO® support

7 3V3 3V3 - 3.3 V input/output ARDUINO® support
9 5V 5V - 5 V output ARDUINO® support

## 11 GND GND - ground ARDUINO® support


## 13 GND GND - ground ARDUINO® support


## 15 VIN VIN - Power input ARDUINO® support


## 2 D43 SDMMC_D0 PC8 SDMMC -


## 4 D44 SDMMC_D1/SAI2_EXTCLK PC9 (1) SDMMC/I2S_A -


## 6 D45 SDMMC_D2 PC10 SDMMC -


## 8 D46 SDMMC_D3 PC11 SDMMC -


## 10 D47 SDMMC_CK PC12 SDMMC -


## 12 D48 SDMMC_CMD PD2 SDMMC -


## 14 D49 I/O PG10 (2) I/O -


## 16 D50 I/O PG8 (3) I/O -


<!-- Page 39 -->
UM2408 Hardware layout and configuration
56
CN9

## 1 A0 ADC PA3 ADC_12_INP15 ARDUINO® support


## 3 A1 ADC PC0 ADC_123_INP10 ARDUINO® support


## 5 A2 ADC PC3_C ADC_3_INP1 ARDUINO® support


## 7 A3 ADC PB1 ADC_12_INP5 ARDUINO® support

9A 4 A D C PC2_C or
PB9
ADC3_INP0
(PC2_C) or
I2C1_SDA (PB9)
ARDUINO® support

## 11 A5 ADC PF11 or PB8

ADC1_INP2
(PF11) or
I2C1_SCL (PB8)
ARDUINO® support

## 13 D72 COMP1_INP PB2 (4) COMP NC by default


## 15 D71 COMP2_INP PE9 COMP -


## 17 D70 I2C_B_SMBA PB5 (5) I2C_2 NC by default


## 19 D69 I2C_B_SCL PF14 I2C_2 -


## 21 D68 I2C_B_SDA PF15 I2C_2 -


## 23 GND GND - ground -


## 25 D67 CAN_RX PD0 CAN_1 -


## 27 D66 CAN_TX PD1 CAN_1 -


## 29 D65 I/O PB14

(6) I/O -

## 2 D51 USART_B_SCLK PD7 USART_2 -


## 4 D52 USART_B_RX PD6 USART_2 -


## 6 D53 USART_B_TX PD5 USART_2 -


## 8 D54 USART_B_RTS PD4 USART_2 -


## 10 D55 USART_B_CTS PD3 USART_2 -


## 12 GND GND - ground -


## 14 D56 SAI_A_MCLK PE2

(7) SAI_1_A -

## 16 D57 SAI_A_FS PE4 SAI_1_A -


## 18 D58 SAI_A_SCK PE5 SAI_1_A -


## 20 D59 SAI_A_SD PE6

(8) SAI_1_A -

## 22 D60 SAI_B_SD PE3 SAI_1_B -


## 24 D61 SAI_B_SCK PF8 SAI_1_B -


## 26 D62 SAI_B_MCLK PF7 SAI_1_B -


## 28 D63 SAI_B_FS PF9 SAI_1_B -


## 30 D64 I/O PD10

(9) I/O NC by default

**Table 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q pin assignments (continued)**

CN Pin Pin
name Signal name STM32H7
pin Function Remark

<!-- Page 40 -->
Hardware layout and configuration UM2408
CN7

## 1 D16 I2S_A_MCK PC6 I2S_2 -


## 3 D17 I2S_A_SD PB15 I2S_2 -


## 5 D18 I2S_A_CK PB13

(10) I2S_2 NC by default

## 7 D19 I2S_A_WS PB12 I2S_2 -


## 9 D20 I2S_B_WS PA15 I2S_3 -


## 11 D21 I2S_B_MCK PC7 I2S_3 -


## 13 D22 I2S_B_SD/SPI_B_MOSI PB5 (5) I2S_3/SPI3 -


## 15 D23 I2S_B_CK/SPI_B_SCK PB3 (11) I2S_3/SPI3 -


## 17 D24 SPI_B_NSS PA4 SPI3 -


## 19 D25 SPI_B_MISO PB4 SPI3 -


## 2 D15 I2C_A_SCL PB8 I2C1_SCL ARDUINO® support


## 4 D14 I2C_A_SDA PB9 I2C1_SDA ARDUINO® support


## 6 AREF AREF - AVDD/VREF+ ARDUINO® support


## 8 GND GND  ground ARDUINO® support


## 10 D13 SPI_A_SCK PA5 SPI1_SCK ARDUINO® support


## 12 D12 SPI_A_MISO PA6 SPI1_MISO ARDUINO® support


## 14 D11 SPI_A_MOSI/TIM_E_PWM


## 1 PB5(5) SPI1_MOSI/TIM3

_CH2 ARDUINO® support

## 16 D10 SPI_A_CS/TIM_B_PWM3 PD14 SPI1_CS/TIM4_C

H3 ARDUINO® support

## 18 D9 TIMER_B_PWM2 PD15 TIM4_CH4 ARDUINO® support


## 20 D8 I/O PG9 (12) - ARDUINO® support


**Table 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q pin assignments (continued)**

CN Pin Pin
name Signal name STM32H7
pin Function Remark

<!-- Page 41 -->
UM2408 Hardware layout and configuration
56
CN10

## 1 VDDA VDDA - Analog VDD -


## 3 AGND AGND - Analog ground -


## 5 GND GND - ground -


## 7 A6 ADC_A_IN PF6 ADC3_INP8 -


## 9 A7 ADC_B_IN PF10 ADC3_INP6 -


## 11 A8 ADC_C_IN PA2

(13) ADC12_INP14 NC by default

## 13 D26 QSPI_CS PG6 QSPI_BK1 -


## 15 D27 QSPI_CLK PB2 (4) QSPI_CLK -


## 17 GND GND - ground -


## 19 D28 QSPI_BK1_IO3 PD13 QSPI_BK1 -


## 21 D29 QSPI_BK1_IO1 PD12 QSPI_BK1 -


## 23 D30 QSPI_BK1_IO0 PD11 QSPI_BK1 -


## 25 D31 QSPI_BK1_IO2 PE2 (7) QSPI_BK1 -


## 27 GND GND - ground -


## 29 D32 TIMER_C_PWM1 PA0 TIM2_CH1 -


## 31 D33 TIMER_D_PWM1 PB0 TIM3_CH3 -


## 33 D34 TIMER_B_ETR PE0 TIM4_ETR -

2D 7 I / O P G 1 2
(14) - ARDUINO® support

## 4 D6 TIMER_A_PWM1 PA8 (15) TIM1_CH1 ARDUINO® support


## 6 D5 TIMER_A_PWM2 PE11 TIM1_CH2 ARDUINO® support

8D 4 I / O  P E 1 4 - ARDUINO® support

## 10 D3 TIMER_A_PWM3 PE13 TIM1_CH3 ARDUINO® support


## 12 D2 I/O PG14 - ARDUINO® support


## 14 D1 USART_A_TX PB6 LPUART1 ARDUINO® support


## 16 D0 USART_A_RX PB7 LPUART1 ARDUINO® support


## 18 D42 TIMER_A_PWM1N PE8 TIM1_CH1N -


## 20 D41 TIMER_A_ETR PE7 TIM1_ETR -


## 22 GND GND - ground -


## 24 D40 TIMER_A_PWM2N PE10 TIM1_CH2N -


## 26 D39 TIMER_A_PWM3N PE12 TIM1_CH3N -


## 28 D38 I/O PE6

(8) I/O -

## 30 D37 TIMER_A_BKIN1 PE15 TIM1_BKIN1 -


## 32 D36 TIMER_C_PWM2 PB10 TIM2_CH3 -


## 34 D35 TIMER_C_PWM3 PB11 TIM2_CH4


**Table 17. NUCLEO-H745ZI-Q and NUCLEO-H755ZI-Q pin assignments (continued)**

CN Pin Pin
name Signal name STM32H7
pin Function Remark

<!-- Page 42 -->
Hardware layout and configuration UM2408
For more details, refer to Table 16: Solder bridges.
1. PC9 = SDMMC_D1/SAI2_EXTCLK
2. PG10 = D49/SMPS_SW
3. PG8 = D50/SMPS_V1
4. PB2 = D27/D72
5. PB5 = D11 (H745ZI/H755ZI)/D22/D70
6. PB14 = D65/LED_RED
7. PE2 = D31/D56
8. PE6 = D59/D38
9. PD10 = D64/USB_FS_PWR_EN (H745ZI/H755ZI)
10. PB13 = D18/RMII_TXD1 (H745ZI/H755ZI)
11. PB3 = D23/SWO
12. PG9 = D8/SMPS EN
13. PA2 = A8/RMII_MDIO (H745ZI/H755ZI)
14. PG12 = D7/SMPS_PG
15. PA8 = D6/USB_SOF/MCO1

**Table 18. NUCLEO-H7A3ZI-Q pin assignments**

CN Pin Pin
name Signal name STM32H7
pin Function Remark
CN8
1N C N C - - ARDUINO® support

## 3 IOREF IOREF - 3.3 V Ref ARDUINO® support


## 5 NRST NRST NRST RESET ARDUINO® support

7 3V3 3V3 - 3.3 V input/output ARDUINO® support
9 5V 5V - 5 V output ARDUINO® support

## 11 GND GND - ground ARDUINO® support


## 13 GND GND - ground ARDUINO® support


## 15 VIN VIN - Power input ARDUINO® support


## 2 D43 SDMMC_D0 PC8 SDMMC -


## 4 D44 SDMMC_D1/SAI2_EXTCLK PC9 (1) SDMMC/I2S_A -


## 6 D45 SDMMC_D2 PC10 SDMMC -


## 8 D46 SDMMC_D3 PC11 SDMMC -


## 10 D47 SDMMC_CK PC12 SDMMC -


## 12 D48 SDMMC_CMD PD2 SDMMC -


## 14 D49 I/O PG10

(2) I/O -

## 16 D50 I/O PG8 (3) I/O -


<!-- Page 43 -->
UM2408 Hardware layout and configuration
56
CN9

## 1 A0 ADC PA3 ADC_12_INP15 ARDUINO® support


## 3 A1 ADC PC0 ADC_123_INP10 ARDUINO® support


## 5 A2 ADC PC3_C ADC_3_INP1 ARDUINO® support


## 7 A3 ADC PB1 ADC_12_INP5 ARDUINO® support

9A 4 A D C PC2_C or
PB9
ADC3_INP0
(PC2_C) or
I2C1_SDA (PB9)
ARDUINO® support

## 11 A5 ADC PF11 or PB8 ADC1_INP2 (PF11)

or I2C1_SCL (PB8) ARDUINO® support

## 13 D72 COMP1_INP PB2 (4) COMP NC by default


## 15 D71 COMP2_INP PE9 COMP -


## 17 D70 I2C_B_SMBA PB5 (5) I2C_2 NC by default


## 19 D69 I2C_B_SCL PF14 I2C_2 -


## 21 D68 I2C_B_SDA PF15 I2C_2 -


## 23 GND GND - ground -


## 25 D67 CAN_RX PD0 CAN_1 -


## 27 D66 CAN_TX PD1 CAN_1 -


## 29 D65 I/O PB14

(6) I/O -

## 2 D51 USART_B_SCLK PD7 USART_2 -


## 4 D52 USART_B_RX PD6 USART_2 -


## 6 D53 USART_B_TX PD5 USART_2 -


## 8 D54 USART_B_RTS PD4 USART_2 -


## 10 D55 USART_B_CTS PD3 USART_2 -


## 12 GND GND - ground -


## 14 D56 SAI_A_MCLK PE2

(7) SAI_1_A -

## 16 D57 SAI_A_FS PE4 SAI_1_A -


## 18 D58 SAI_A_SCK PE5 SAI_1_A -


## 20 D59 SAI_A_SD PE6 (8) SAI_1_A -


## 22 D60 SAI_B_SD PE3 SAI_1_B -


## 24 D61 SAI_B_SCK PF8 SAI_1_B -


## 26 D62 SAI_B_MCLK PF7 SAI_1_B -


## 28 D63 SAI_B_FS PF9 SAI_1_B -


## 30 D64 I/O PD10

(9) I/O -

**Table 18. NUCLEO-H7A3ZI-Q pin assignments (continued)**

CN Pin Pin
name Signal name STM32H7
pin Function Remark

<!-- Page 44 -->
Hardware layout and configuration UM2408
CN7

## 1 D16 I2S_A_MCK PC6 I2S_2 -


## 3 D17 I2S_A_SD PB15 I2S_2 -


## 5 D18 I2S_A_CK PB13

(10) I2S_2 -

## 7 D19 I2S_A_WS PB12 I2S_2 -


## 9 D20 I2S_B_WS PA15 I2S_3 -


## 11 D21 I2S_B_MCK PC7 I2S_3 -


## 13 D22 I2S_B_SD/SPI_B_MOSI PB5 (5) I2S_3/SPI3 -


## 15 D23 I2S_B_CK/SPI_B_SCK PB3 (11) I2S_3/SPI3 -


## 17 D24 SPI_B_NSS PA4 SPI3 -


## 19 D25 SPI_B_MISO PB4 SPI3 -


## 2 D15 I2C_A_SCL PB8 I2C1_SCL ARDUINO® support


## 4 D14 I2C_A_SDA PB9 I2C1_SDA ARDUINO® support


## 6 AREF AREF - AVDD/VREF+ ARDUINO® support


## 8 GND GND  ground ARDUINO® support


## 10 D13 SPI_A_SCK PA5 SPI1_SCK ARDUINO® support


## 12 D12 SPI_A_MISO PA6 SPI1_MISO ARDUINO® support


## 14 D11 SPI_A_MOSI/TIM_E_PWM


## 1 PA7(5) SPI1_MOSI/TIM3_

CH2 ARDUINO® support

## 16 D10 SPI_A_CS/ TIM_B_PWM3 PD14 SPI1_CS/TIM4_CH


## 3 ARDUINO® support


## 18 D9 TIMER_B_PWM2 PD15 TIM4_CH4 ARDUINO® support


## 20 D8 I/O PG9 (12) - ARDUINO® support


**Table 18. NUCLEO-H7A3ZI-Q pin assignments (continued)**

CN Pin Pin
name Signal name STM32H7
pin Function Remark

<!-- Page 45 -->
UM2408 Hardware layout and configuration
56
CN10

## 1 VDDA VDDA - Analog VDD -


## 3 AGND AGND - Analog ground -


## 5 GND GND - ground -


## 7 A6 ADC_A_IN PC1 ADC12_INP1 -


## 9 A7 ADC_B_IN PC5 ADC12_INP8 -


## 11 A8 ADC_C_IN PA2

(13) ADC1_INP14 -

## 13 D26 QSPI_CS PG6 QSPI_BK1 -


## 15 D27 QSPI_CLK PB2 (4) QSPI_CLK -


## 17 GND GND - ground -


## 19 D28 QSPI_BK1_IO3 PD13 QSPI_BK1 -


## 21 D29 QSPI_BK1_IO1 PD12 QSPI_BK1 -


## 23 D30 QSPI_BK1_IO0 PD11 QSPI_BK1 -


## 25 D31 QSPI_BK1_IO2 PE2 (7) QSPI_BK1 -


## 27 GND GND - ground -


## 29 D32 TIMER_C_PWM1 PA0 TIM2_CH1 -


## 31 D33 TIMER_D_PWM1 PB0 TIM3_CH3 -


## 33 D34 TIMER_B_ETR PE0 TIM4_ETR -


## 2 D7 I/O PG12

(14) - ARDUINO® support

## 4 D6 TIMER_A_PWM1 PA8 (15) TIM1_CH1 ARDUINO® support


## 6 D5 TIMER_A_PWM2 PE11 TIM1_CH2 ARDUINO® support


## 8 D4 I/O PE14 - ARDUINO® support


## 10 D3 TIMER_A_PWM3 PE13 TIM1_CH3 ARDUINO® support


## 12 D2 I/O PG14 - ARDUINO® support


## 14 D1 USART_A_TX PB6 LPUART1 ARDUINO® support


## 16 D0 USART_A_RX PB7 LPUART1 ARDUINO® support


## 18 D42 TIMER_A_PWM1N PE8 TIM1_CH1N -


## 20 D41 TIMER_A_ETR PE7 TIM1_ETR -


## 22 GND GND - ground -


## 24 D40 TIMER_A_PWM2N PE10 TIM1_CH2N -


## 26 D39 TIMER_A_PWM3N PE12 TIM1_CH3N -


## 28 D38 I/O PE6

(8) I/O -

## 30 D37 TIMER_A_BKIN1 PE15 TIM1_BKIN1 -


## 32 D36 TIMER_C_PWM2 PB10 TIM2_CH3 -


## 34 D35 TIMER_C_PWM3 PB11 TIM2_CH4 -


**Table 18. NUCLEO-H7A3ZI-Q pin assignments (continued)**

CN Pin Pin
name Signal name STM32H7
pin Function Remark

<!-- Page 46 -->
Hardware layout and configuration UM2408

### 7.17 ST morpho connector

The ST morpho connector consists of two male pin header footprints (CN11 and CN12, OFF
by default).
They are used to connect the STM32H7 Nucleo-144 board to an extension board or a
prototype/wrapping board placed on top of the STM32H7 Nucleo-144 board. All signals and
power pins of the STM32H7 are available on the ST morpho connector. An oscilloscope,
logical analyzer, or voltmeter can also probe this connector.

**Table 19 shows the assignment of each STM32H7 signal on the ST morpho connector.**

1. PC9 = SDMMC_D1/SAI2_EXTCLK
2. PG10 = D49/SMPS_SW
3. PG8 = D50/SMPS_V1
4. PB2 = D27/D72
5. PB5 = D11 (H745ZI/H755ZI)/D22/D70
6. PB14 = D65/LED_RED
7. PE2 = D31/D56
8. PE6 = D59/D38
9. PD10 = D64/USB_FS_PWR_EN (H745ZI/H755ZI)
10. PB13 = D18/RMII_TXD1 (H745ZI/H755ZI)
11. PB3 = D23/SWO
12. PG9 = D8/SMPS EN
13. PA2 = A8/RMII_MDIO (H745ZI/H755ZI)
14. PG12 = D7/SMPS_PG
15. PA8 = D6/USB_SOF/MCO1

**Table 19. Pin assignment of MCU with ST morpho connector**

CN11 odd pins CN11 even pins CN12 odd pins CN12 even pins
Pin
nbr Pin name Pin
nbr Pin name Pin
nbr Pin name Pin
nbr Pin name

## 1 PC10 2 PC11 1 PC9 2 PC8


## 3 PC12 4 PD2 3 PB8 4 PC6

5 3V3_VDD 6 5V_EXT 5 PB9 6 PC5
7B O O T 0
(1) 8 GND 7 VREFP 8 5V_USB_STLK (2)

## 9 PF6 10 NC 9 GND 10 PD8


## 11 PF7 12 IOREF 11 PA5 12 PA12 or NC (3)


## 13 PA13 (4) 14 NRST 13 PA6 14 PA11 or PE6 (3)


## 15 PA14 (4) 16 3V3 15 PA7 16 PB12


## 17 PA15 18 5V 17 PB6 18 PB11


## 19 GND 20 GND 19 PC7 20 GND


## 21 PB7 22 GND 21 PA9 22 PB2


## 23 PC13 24 VIN 23 PA8 24 PB1


<!-- Page 47 -->
UM2408 Hardware layout and configuration
56

## 25 PC14 26 NC 25 PB10 26 PB15


## 27 PC15 28 PA0 27 PB4 28 PB14 or PE15 (3)


## 29 PH0 30 PA1 29 PB5 or PA4 (3) 30 PB13 or PE8 (3)


## 31 PH1 32 PA4 31 PB3 32 AGND


## 33 VBAT 34 PB0 33 PA10 34 PC4


## 35 PC2 36 PC1 35 PA2 36 NC


## 37 PC3 38 PC0 37 PA3 38 NC


## 39 PD4 40 PD3 39 GND 40 PE8


## 41 PD5 42 NC 41 PD13 42 PF10


## 43 PD6 44 NC 43 PD12 44 PE7


## 45 PD7 46 PE2 45 PD11 46 PD14


## 47 PE3 48 PE4 47 PE10 48 PD15


## 49 GND 50 PE5 49 PE12 50 PF14


## 51 NC 52 NC 51 PE14 52 PE9


## 53 NC 54 PF8 53 PE15 54 GND


## 55 PD1 56 PF9 55 PE13 56 PE11


## 57 PD0 58 NC 57 NC 58 NC


## 59 NC 60 GND 59 NC 60 PF15


## 61 PE1 62 PE6 61 PG14 62 PF11


## 63 PG9 64 NC 63 GND 64 PE0


## 65 PG12 66 PG10 65 PD10 66 PG8


## 67 NC 68 PG13 67 PG7 68 NC


## 69 PD9 70 PG11 69 NC 70 PG6

1. The default state of BOOT0 is LOW. It can be set to  HIGH when a jumper is ON between pins 5 and 7 of
CN11. Refer to the warning at the end of Section 7.4.8: Internal SMPS/LDO configuration.
2. 5V_USB_STLK is the 5 V power coming from the ST-LINK/V3 USB connector that rises before and rises
before the +5 V rising on the board.
3. Solder bridges allow connecting to different pins. Refer to Section 7.13: Motor control.
4. PA13 and PA14 are shared with SWD signals connect ed to ST-LINK/V3. It is not recommended to use
them as I/O pins.

**Table 19. Pin assignment of MCU with ST morpho connector (continued)**

CN11 odd pins CN11 even pins CN12 odd pins CN12 even pins
Pin
nbr Pin name Pin
nbr Pin name Pin
nbr Pin name Pin
nbr Pin name

<!-- Page 48 -->
STM32H7 Nucleo-144 product information UM2408

## 8 STM32H7 Nucleo-144 product information


### 8.1 Product marking

The product and each board composing the product are identified with one or several
stickers. The stickers, located on the top or bottom side of each PCB, provide product
information:
• Main board featuring the target device: product order code, product identification, serial
number, and board reference with revision.
Single-sticker example:
Dual-sticker example:
• Other boards if any: board reference with revision and serial number.
Examples:
On the main board sticker, the first line provides the product order code, and the second line
the product identification.
On all board stickers, the line formatted as “MBxxxx-Variant-yzz” shows the board reference
“MBxxxx”, the mounting variant “Variant” when several exist (optional), the PCB revision “y”,
and the assembly revision “zz”, for example B01. The other line shows the board serial
number used for traceability.
Products and parts labeled as “ES” or “E” are not yet qualified or feature devices that are not
yet qualified. STMicroelectronics disclaims any responsibility for consequences arising from
their use. Under no circumstances will STMicroelectronics be liable for the customer's use
of these engineering samples. Before deciding to use these engineering samples for
qualification activities, contact STMicroelectronics' quality department.
“ES” or “E” marking examples of location:
• On the targeted STM32 that is soldered on the board (for an illustration of STM32
marking, refer to the STM32 datasheet Package information paragraph at the
www.st.com website).
• Next to the evaluation tool ordering part number that is stuck or silk-screen printed on
the board.
Some boards feature a specific STM32 device version, which allows the operation of any
bundled commercial stack/library available. This STM32 device shows a “U” marking option
at the end of the standard part number and is not available for sales.
To use the same commercial stack in their applications, the developers might need to
purchase a part number specific to this stack/library. The price of those part numbers
includes the stack/library royalties.
DT73589V1
Product order code
Product identification
syywwxxxxx
MBxxxx-Variant-yzz
Product order code
Product identification
Product order code
Product identification
MBxxxx-Variant-yzz
syywwxxxxx
and
or MBxxxx-Variant-yzz
syywwxxxxx
syywwxxxxx
MBxxxx-Variant-yzz
or or

<!-- Page 49 -->
UM2408 STM32H7 Nucleo-144 product information
56

### 8.2 NUCLEO-H745ZI-Q, NUCLE O-H755ZI-Q, and NUCLEO-

H7A3ZI-Q
product history

**Table 20. Product history**

Order
code
Product
identification Product details Product change
description Product limitations
NUCLEO-H745ZI-Q
NUH745ZIQ$AT1
MCU:
– STM32H745ZIT6
silicon revision ‘Z’
Initial revision
The IDD measurement of
the STM32H7x5 MCU
cannot be done in Standby
mode.
MCU errata sheet:
– STM32H745xI/G,
STM32H755xI,
STM32H747xI/G,
STM32H757xI device
errata (ES0445)
Boards:
– MB1363-H745ZIQ-C01
(main board)
NUH745ZIQ$AT2
MCU:
– STM32H745ZIT6
silicon revision ‘Z’
Main board revision
changed No limitation
MCU errata sheet:
– STM32H745xI/G,
STM32H755xI,
STM32H747xI/G,
STM32H757xI device
errata (ES0445)
Boards:
– MB1363-H745ZIQ-D01
(main board)

<!-- Page 50 -->
STM32H7 Nucleo-144 product information UM2408
NUCLEO-H755ZI-Q
NUH755ZIQ$AT1
MCU:
– STM32H755ZIT6
silicon revision ‘Z’
Initial revision
The IDD measurement of
the STM32H7x5 MCU
cannot be done in Standby
mode.
MCU errata sheet:
– STM32H745xI/G,
STM32H755xI,
STM32H747xI/G,
STM32H757xI device
errata (ES0445)
Boards:
– MB1363-H755ZIQ-C01
(main board)
NUH755ZIQ$AT2
MCU:
– STM32H755ZIT6
silicon revision ‘Z’
Main board revision
changed No limitation
MCU errata sheet:
– STM32H745xI/G,
STM32H755xI,
STM32H747xI/G,
STM32H757xI device
errata (ES0445)
Boards:
– MB1363-H755ZIQ-D01
(main board)
NUCLEO-H7A3ZI-Q
NUH7A3ZIQ$AT1
MCU:
– STM32H7A3ZIT6Q
silicon revision ‘Z’
Initial revision No limitation
MCU errata sheet:
– STM32H7A3xI/G,
STM32H7B0xB and
STM32H7B3xI device
errata (ES0478)
Boards:
– MB1363-H7A3ZIQ-D01
(main board)

**Table 20. Product history (continued)**

Order
code
Product
identification Product details Product change
description Product limitations

<!-- Page 51 -->
UM2408 STM32H7 Nucleo-144 product information
56

### 8.3 Board revision history


**Table 21. Board revision history**

Board
reference
Board variant and
revision Board change description Board limitations
MB1363
(main board)
H745ZIQ-C01 Initial revision
The IDD measurement of
the STM32H7x5 MCU
cannot be done in Standby
mode because the
VDD_MCU is also
supplying the Ethernet
PHY (U15), the pull-up
resistors (R43, R46, R49,
R50, and R51), and the
debug voltage translation
(U1 and U10).
H745ZIQ-D01
– Modification of power supply wiring to
allow the addition of an IDD jumper to
measure IDD consumption of VDD
MCU
– Addition of SB13 (not fitted by default)
between T_VCP_TX and
STLK_VCP_RX to bypass U1 level
shifter
No limitation
H755ZIQ-C01 Initial revision
The IDD measurement of
the STM32H7x5 MCU
cannot be done in Standby
mode because the
VDD_MCU is also
supplying the Ethernet
PHY (U15), the pull-up
resistors (R43, R46, R49,
R50, and R51), and the
debug voltage translation
(U1 and U10).
H755ZIQ-D01
– Modification of power supply wiring to
allow the addition of an IDD jumper to
measure IDD consumption of VDD
MCU
– Addition of SB13 (not fitted by default)
between T_VCP_TX and
STLK_VCP_RX to bypass U1 level
shifter
No limitation
H7A3ZIQ-D01 Initial revision No limitation

<!-- Page 52 -->
Federal Communications Commission (FCC)  and ISED Canada Compliance Statements UM2408

## 9 Federal Communications Commission (FCC)

and ISED Canada Compliance Statements

### 9.1 FCC Compliance Statement

Part 15.19
This device complies with Part 15 of the FCC Rules. Operation is subject to the following
two conditions: (1) this device may not cause harmful interference, and (2) this device must
accept any interference received, including interference that may cause undesired
operation.
Part 15.21
Any changes or modifications to this equipment not expressly approved by
STMicroelectronics may cause harmful interference and void the user's authority to operate
this equipment.
Part 15.105
This equipment has been tested and found to comply with the limits for a Class A digital
device, pursuant to part 15 of the FCC Rules. These limits are designed to provide
reasonable protection against harmful interference when the equipment is operated in a
commercial environment. This equipment generates, uses, and can radiate radio frequency
energy and, if not installed and used in accordance with the instruction manual, may cause
harmful interference to radio communications. Operation of this equipment in a residential
area is likely to cause harmful interference in which case the user will be required to correct
the interference at his own expense.
Responsible party (in the USA)
Francesco Doddo
STMicroelectronics, Inc.

## 200 Summit Drive | Suite 405 | Burlington, MA 01803

USA
Telephone: +1 781-472-9634

<!-- Page 53 -->
UM2408 Federal Communications Commission (FCC)  and ISED Canada Compliance Statements
56

### 9.2 ISED Compliance Statement

ISED Canada ICES-003 Compliance Label: CAN ICES-3 (A) / NMB-3 (A).
Étiquette de conformité à la NMB-003 d'ISDE Canada: CAN ICES-3 (A) / NMB-3 (A).

<!-- Page 54 -->
CE conformity UM2408

## 10 CE conformity


### 10.1 Warning

EN 55032 / CISPR32 (2012) Class A product
Warning: this device is compliant with Class A of EN55032 / CISPR32. In a residential
environment, this equipment may cause radio interference.
Avertissement : cet équipement est conforme à la Classe A de la EN55032 / CISPR 32.
Dans un environnement résidentiel, cet équipement peut créer des interférences radio.

<!-- Page 55 -->
UM2408 Product disposal
56

## 11 Product disposal

Disposal of this product: WEEE (Waste Electrical and Electronic Equipment)
(Applicable in Europe)
This symbol on the product, accessories, or accompanying documents indicates that
the product and its electronic accessories should not be disposed of with household
waste at the end of their working life.
To prevent possible harm to the environment and human health from uncontrolled
waste disposal, please separate these items from other type of waste and recycle
them responsibly to the designated collection point to promote the sustainable reuse
of material resources.
Household users:
You should contact either the retailer where you buy the product or your local authority
for further details of your nearest designated collection point.
Business users:
You should contact your dealer or supplier for further information.

<!-- Page 56 -->
Revision history UM2408
Revision history

**Table 22. Document revision history**

Date Revision Changes
13-Mar-2019 1 Initial version
19-Dec-2019 2
Updated:
– New PCB (MB1363 Rev.D) with corrections of IDD current
measurement issues
– Important Warning concerning firmware versus hardware
SMPS power configuration mismatch
Added:
– Support of NUCLEO-H7A3ZI-Q
10-Mar-2020 3
Updated:
– Section 6.4.8: Internal SMPS/LDO Configuration with

**Table 10 and Figure 16 modifies the supply configuration**

from external SMPS to bypass
– Section 6.13: Motor control with Table 15
25-Mar-2020 4
Added:
–N e w  Section 6.17: ST morpho connector detailing
connections with MCU in Table 19
23-Mar-2021 5
Added:
–N e w  Section 7: STM32H7 Nucleo-144 product information
with the updated Product marking
Removed:
– Direct firmware update from Section 6.3.1: Using the
STLINK-V3E to program and debug the onboard STM32H7
06-Feb-2025 6
Updated:
– Table 16 regarding SB49 and SB50 configuration and R74
ON state description
– STM32H7 Nucleo-144 product information with Product
marking, Product history, and Board revision history
– Federal Communications Commission (FCC)  and ISED
Canada Compliance Statements
Added:
– Safety recommendations, CE conformity, and Product
disposal
Removed:
– Assembly drawings

<!-- Page 57 -->
57
IMPORTANT NOTICE – READ CAREFULLY
STMicroelectronics NV and its subsidiaries (“ST”) reserve the right to make changes, corrections, enhancements, modifications, and
improvements to ST products and/or to this document at any time without notice. Purchasers should obtain the latest relevant information on
ST products before placing orders. ST products are sold pursuant to ST’s terms and conditions of sale in place at the time of order
acknowledgment.
Purchasers are solely responsible for the choice, selection, and use of ST products and ST assumes no liability for application assistance or
the design of purchasers’ products.
No license, express or implied, to any intellectual property right is granted by ST herein.
Resale of ST products with provisions different from the information set forth herein shall void any warranty granted by ST for such product.
ST and the ST logo are trademarks of ST. For additional information about ST trademarks, refer to www.st.com/trademarks. All other product
or service names are the property of their respective owners.
Information in this document supersedes and replaces information previously supplied in any prior versions of this document.
© 2025 STMicroelectronics – All rights reserved