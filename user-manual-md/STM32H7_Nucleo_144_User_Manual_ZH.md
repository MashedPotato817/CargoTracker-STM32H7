# UM2408 用户手册
## STM32H7 Nucleo-144 开发板 (MB1363)

### 简介
STM32H7 Nucleo-144 开发板基于 MB1363 参考板（包括 NUCLEO-H745ZI-Q、NUCLEO-H755ZI-Q 和 NUCLEO-H7A3ZI-Q），为用户提供了一种经济且灵活的方式来试用新概念并构建原型。用户可以从 STM32H7 微控制器提供的各种性能和功耗特性组合中进行选择。

ST Zio 连接器扩展了 ARDUINO® Uno V3 的连接性，而 ST morpho 排针则提供了简单的功能扩展手段，可通过丰富的专用扩展板（Shields）轻松扩展 Nucleo 开放式开发平台的生态与功能。STM32H7 Nucleo-144 开发板集成了 STLINK-V3E 调试器/编程器，因此不需要任何单独的外部仿真器。开发板附带了全面的免费软件库，以及包含在 STM32Cube MCU 固件包中的丰富示例。

---

## 1 产品特性 (Features)

STM32H7 Nucleo-144 开发板具备以下核心特性：
* **微控制器**：采用 LQFP144 封装的基于 Arm® Cortex® 核心的 STM32H7 微控制器。
* **以太网**：符合 IEEE-802.3-2002 标准（取决于具体 STM32H7 型号的支持情况）。
* **USB**：支持 USB OTG 或全速（Full-speed）设备接口。
* **LED 指示灯**：板载 3 个用户可编程 LED。
* **按键**：2 个轻触按键（1 个用户自定义按键和 1 个复位按键）。
* **低速外部时钟 (LSE)**：
  - 32.768 kHz 晶振。
* **板载连接接口**：
  - Micro-AB 型 USB 接口。
  - RJ45 以太网接口（仅适用于 NUCLEO-H745ZI-Q 和 NUCLEO-H755ZI-Q）。
* **板载扩展连接器**：
  - 支持 ARDUINO® Uno V3 的 **ST Zio 接口**。
  - **ST morpho** 全引脚排针。
* **灵活的供电方式**：支持 ST-LINK USB VBUS 供电或多种外部电源供电。
* **板载 STLINK-V3E 调试器/编程器**（带 SWD 连接器）：
  - 支持 USB 重新枚举功能：虚拟串口（Virtual COM port）、大容量存储器（Mass storage）和调试端口。
  - 可作为独立的 STLINK-V3E 工具使用。
* **全面的软件支持**：STM32CubeH7 MCU 包中提供了丰富的免费软件库和全套示例。
* **广泛的开发工具链支持**：支持多种集成开发环境（IDE），包括 IAR Embedded Workbench®、MDK-ARM 和 STM32CubeIDE。

---

## 2 订购信息 (Ordering information)

要订购对应目标 STM32 微控制器的 Nucleo-144 开发板，请参考下表中的订购代码：

### **表 1. 订购信息表**

| 订购代码 (Order code) | 开发板参考号 (Board reference) | 目标微控制器 (Target STM32) | 差异化特性 (Differentiating feature) |
| :--- | :--- | :--- | :--- |
| **NUCLEO-H745ZI-Q** | MB1363 | STM32H745ZIT6 | 带以太网接口 (Ethernet) |
| **NUCLEO-H755ZI-Q** | MB1363 | STM32H755ZIT6 | 带以太网接口与硬件加密加速 (Cryptography) |
| **NUCLEO-H7A3ZI-Q** | MB1363 | STM32H7A3ZIT6Q | 无以太网接口/加密 |

### 2.1 命名编码规则 (Codification)
下表以 **NUCLEO-H745ZI-Q** 为例，详细阐述了具体编码代表的含义：

### **表 2. 命名规则说明**

| 编码位置 | 说明 | 示例：NUCLEO-H745ZI-Q |
| :--- | :--- | :--- |
| **XX** | STM32 32位 Arm Cortex MCU 的产品系列 | **H7** 系列 |
| **YY** | 该系列中的具体产品线 | **H745** 产品线 |
| **Z** | STM32 封装的引脚数 | **Z** 代表 144 个引脚 (LQFP144) |
| **T** | STM32 的 Flash 内存容量大小 | **I** 代表 2 Mbytes 大小 |
| **-Q** | 电源配置标识 | **-Q** 表示该 STM32 芯片内置 SMPS（开关电源变换器）功能 |

---

## 3 开发环境 (Development environment)

### 3.1 开发工具链
* **IAR Systems®** - IAR Embedded Workbench® (仅限 Windows)
* **Keil®** - MDK-ARM (仅限 Windows)
* **STMicroelectronics** - STM32CubeIDE (支持多平台)

### 3.2 系统要求
* Windows® 操作系统 (7, 8, 10 或 11)、Linux® 64位 或 macOS®
* USB Type-A 或 USB Type-C® 转 Micro-B 的数据连接线

### 3.3 演示固件 (Demonstration software)
包含在 STM32Cube 包中的演示软件已经预加载到 STM32H7 的 Flash 闪存中，以便于在独立模式下直观展示外设功能。最新版本的演示源码及相关配套文档可以从 www.st.com/stm32nucleo 官方页面下载。

### 3.4 CAD 设计资源
包括原理图、CAD 数据库、生产制造文件和物料清单（BOM）在内的所有硬件设计资源，均可在 ST 官方对应的产品页面（NUCLEO-H745ZI-Q、NUCLEO-H755ZI-Q 和 NUCLEO-H7A3ZI-Q）直接获取。
