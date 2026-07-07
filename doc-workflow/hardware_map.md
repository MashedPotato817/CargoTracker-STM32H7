# 当前硬件映射（程序匹配版）

> 依据 `AGENTS.md`、`doc/STM32H7_CubeMX_Config.md`、`doc/STM32H7_Logistics_System.md` 汇总。温湿度传感器按本轮要求统一为 **SHT31**。

## MCU 与开发环境

| 项目 | 当前假设 |
|------|----------|
| MCU | STM32H7A3ZI / STM32H7A3ZITxQ |
| 开发板 | NUCLEO-H7A3ZI-Q |
| 工程 | `test1/MDK-ARM/test1.uvprojx` |
| 配置工具 | STM32CubeMX + Keil MDK |

## 外设与引脚

| 模块 | 型号 | STM32 外设/引脚 | 当前代码状态 |
|------|------|-----------------|--------------|
| 调试串口 | ST-LINK VCP | USART3: PD8 TX, PD9 RX, 115200 | 已由 CubeMX 配置，`printf` 输出 |
| 开发板按键 | USER Button | PC13 EXTI Falling, Pull-up | 已配置，用于模拟激活 |
| 板载绿灯 | LD1 | PB0 GPIO_Output | 已配置，系统心跳 |
| 板载黄灯 | LD2 | PE1 GPIO_Output | 已配置，报警指示 |
| 板载红灯 | LD3 | PB14 GPIO_Output | 已配置，上传/传输指示 |
| 温湿度 | SHT31 | I2C1: PB8 SCL, PB9 SDA, 400kHz | Stub: `SHT31_*` |
| NFC | PN532 + NTAG215 | I2C1: PB8 SCL, PB9 SDA | Stub: `PN532_*` |
| GPS | ATGM336H | USART2: PD5 TX, PD6 RX, 9600 | Stub: `GPS_*` |
| 4G | Air780E | USART1: PA9 TX, PA10 RX, 115200 | Stub: `Air780E_*`, `MQTT_*` |
| 4G PWRKEY | Air780E | PB0 GPIO_Output | 文档规划；当前 PB0 已作为 LD1 心跳 |
| Flash | W25Q128 | SPI1: PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI | Stub: `W25Q128_*` |
| 蜂鸣器 | Buzzer | PC8 GPIO_Output | 文档规划，当前 CubeMX 未配置 |
| 外接 LED | Alarm LED | PC9 GPIO_Output | 文档规划，当前 CubeMX 未配置 |

## 当前开发阶段说明

- 外部硬件外设尚未由 CubeMX 实配，本轮代码使用 stub 跑通 FreeRTOS 任务、队列、状态机和日志。
- SHT31、PN532、GPS、Air780E、MQTT、W25Q128 的接口已按真实模块命名，后续接硬件时优先替换对应 `.c` 内部实现。
- PB0 在最终硬件规划中是 Air780E PWRKEY，但当前 Task 1 开发板验证阶段被 CubeMX 配为 LD1_GREEN。后续接 4G 模块前需要重新分配心跳 LED 或 PWRKEY 引脚。
