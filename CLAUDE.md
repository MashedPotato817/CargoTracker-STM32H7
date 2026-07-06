# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

STM32H7A3ZI-Q 智能货物追踪物联网系统。GPS + NFC + SHT30 温湿度 + Air780E(4G/MQTT) + W25Q128 Flash + 低功耗。

## 硬件平台

| 项目 | 规格 |
|------|------|
| MCU | STM32H7A3ZI (Cortex-M7) |
| 开发板 | NUCLEO-H7A3ZI-Q |
| IDE | Keil MDK + STM32CubeMX |

### 引脚分配

| 外设 | 引脚 | 备注 |
|------|------|------|
| USART1 (Air780E) | PA9 TX, PA10 RX | 115200 baud |
| USART1 PWRKEY | PB0 GPIO_Output | 控制4G模块上电 |
| USART2 (GPS) | PD5 TX, PD6 RX | 9600 baud |
| I2C1 (SHT30/PN532/OLED) | PB8 SCL, PB9 SDA | 400kHz, 共享总线 |
| SPI1 (W25Q128) | PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI | 10~20MHz |
| 蜂鸣器 | PC8 | GPIO |
| LED | PC9 | GPIO |

### 电源架构

```
USB 5V → TP4056(充电+保护) → 18650锂电池
                              ├── 3.3V (MP1584 DC-DC) → STM32 + 传感器
                              └── 4.0V (DC-DC) → Air780E(独立供电)
```

## 系统架构

### 状态机

```
深度睡眠 → NFC激活 → GPS定位 → 温湿度采集 → 数据上传 → 等待云指令 → [暂留/退货/继续运输] → 休眠
```

### 软件分层

```
main.c → APP层(状态机/报警/低功耗) → Driver层(UART/I2C/SPI/GPIO) → HAL库 → STM32
```

### 模块接口规范

各模块提供统一接口，驱动层与应用层分离：

```c
// 驱动层提供（成员A）
void SHT30_Init(void);
float SHT30_ReadTemperature(void);
float SHT30_ReadHumidity(void);
GPS_GetLocation();
MQTT_Publish();
PN532_ReadCard();
Flash_Write();

// 应用层调用（成员C），不关心总线细节
temp = SHT30_ReadTemperature();
```

### 目标代码结构

```
├── APP/          main.c, app.c, state_machine.c, power.c, alarm.c
├── SENSOR/       sht30.c
├── NFC/          pn532.c
├── GPS/          gps.c
├── AIR780E/      air780e.c, mqtt.c
├── FLASH/        w25q128.c
├── BSP/          bsp_uart.c, bsp_spi.c, bsp_i2c.c
├── Drivers/      (STM32 HAL + CMSIS，不手动修改)
└── Middlewares/  (FreeRTOS 可选)
```

## 开发操作

| 操作 | 方式 |
|------|------|
| 打开工程 | 双击 `test1/MDK-ARM/test1.uvprojx` |
| 编译 | Keil F7 (Build) |
| 下载烧录 | Keil Ctrl+F8 (Download) |
| 修改外设 | CubeMX 打开 `test1/test1.ioc` → 重新生成代码 |
| 添加模块 | `test1/Core/Src/` 下新建 `.c`，`Inc/` 下新建 `.h` |

### 时钟配置

- HSE: 8MHz (外部晶振)
- SYSCLK: 240~280 MHz
- APB1: 70 MHz, APB2: 140 MHz

### Flash Download 配置（必须）

```
RAM for Algorithm
Start : 0x24000000
Size  : 0x00010000
```

否则 `Cannot Load Flash Programming Algorithm`。

### DMA 建议

- USART1 (Air780E): DMA RX, Mode Circular
- USART2 (GPS): 可选 DMA RX

## 关键约束

1. **不手动修改** `Drivers/` 下 CMSIS 和 HAL 驱动 — 通过 CubeMX 版本管理
2. **CubeMX 重新生成**：`USER CODE BEGIN/END` 段内代码保留，段外**被覆盖**
3. **编译产物** (`.o`, `.axf`, `.hex`, `.map`) **不纳入版本控制**
4. **4G模块独立供电** (4.0V)，不可与传感器共用 3.3V
5. **I2C 共享总线**：SHT30 + PN532 + OLED 共用 PB8/PB9
6. **低功耗**：默认 Stop Mode，NFC 唤醒；4G 模块空闲时关断 PWRKEY
7. **断网缓存**：温湿度 / GPS 数据写入 W25Q128，联网后补传

## 项目文件索引

| 文件 | 内容 |
|------|------|
| `demand.md` | 需求文档（一句话描述项目） |
| `task.md` | 三人分工方案、接口规范、开发顺序 |
| `STM32H7A3ZI-Q_程序下载要点.md` | Keil 烧录详细步骤 |
| `STM32H7_CubeMX_Config.md` | CubeMX 引脚/时钟/DMA 完整配置 |
| `STM32H7_Logistics_System.md` | 完整设计方案（BOM/接线/电源/状态机） |
| `README.md` | 项目介绍和快速开始 |
