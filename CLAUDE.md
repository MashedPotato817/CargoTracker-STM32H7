# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

STM32H7 智能货物追踪物联网系统（CargoTracker-STM32H7）。

**核心功能**: GPS 实时定位 + NFC 一键激活 + 温湿度监测报警 + 云端指令交互（暂留/退货/继续运输）+ 深度睡眠低功耗。

**主控**: NUCLEO-H7A3ZI-Q（STM32H7A3ZI）
**开发环境**: Keil MDK + STM32CubeMX

## 目录结构

```
├── test1/                          # STM32CubeMX 生成的工程
│   ├── Core/Inc/                   # 头文件
│   │   ├── main.h
│   │   ├── stm32h7xx_hal_conf.h    # HAL 库配置
│   │   ├── stm32h7xx_it.h          # 中断声明
│   │   └── stm32h7xx_nucleo_conf.h # BSP 配置
│   ├── Core/Src/                   # 源文件
│   │   ├── main.c                  # 主程序入口
│   │   ├── stm32h7xx_hal_msp.c     # HAL MSP 初始化
│   │   ├── stm32h7xx_it.c          # 中断服务函数
│   │   └── system_stm32h7xx.c      # 系统时钟与初始化
│   ├── Drivers/                    # HAL 驱动 + BSP
│   │   ├── BSP/STM32H7xx_Nucleo/   # NUCLEO 板级支持包
│   │   ├── CMSIS/                  # CMSIS 核心/设备层
│   │   └── STM32H7xx_HAL_Driver/   # STM32H7 HAL 外设驱动
│   ├── MDK-ARM/                    # Keil MDK 项目文件
│   │   ├── test1.uvprojx           # Keil 项目文件（双击打开）
│   │   ├── startup_stm32h7a3xxq.s  # 启动文件
│   │   └── test1/                  # 编译输出（.o, .axf, .hex, .map ...）
│   └── test1.ioc                   # CubeMX 配置（修改外设后重新生成代码）
├── ST_NUCLEO-H743ZI2_BSP/          # 参考 BSP（已克隆）
├── STM32H743I-EVAL_BSP/            # 参考 BSP（已克隆）
├── demand.md                       # 需求文档
└── STM32H7A3ZI-Q_程序下载要点.md   # 烧录指导
```

## 开发与构建

### 硬件目标
- **NUCLEO-H7A3ZI-Q** — 当前工程主目标
- NUCLEO-H743ZI2 / STM32H743I-EVAL — 参考 BSP

### 常用操作

| 操作 | 方式 |
|------|------|
| **打开工程** | 双击 `test1/MDK-ARM/test1.uvprojx`（Keil MDK） |
| **编译** | Keil MDK 中按 **F7**（Build） |
| **下载烧录** | Keil MDK 中按 **Ctrl+F8**（Download） |
| **修改外设配置** | 用 STM32CubeMX 打开 `test1/test1.ioc` → 重新生成代码 |
| **添加新模块** | 在 `test1/Core/Src/` 下新建 `.c`，`test1/Core/Inc/` 下新建 `.h`，外部包含路径已在 Keil 项目中配置好 |

### Flash Download 配置（重要）

STM32H7A3ZI 的 Keil Flash Algorithm RAM 地址需设置为：
```
Start : 0x24000000
Size  : 0x00010000
```
否则会出现 `Cannot Load Flash Programming Algorithm` 错误。

## 固件架构规划（参考 demand.md）

| 模块 | 外设/协议 | 功能 |
|------|-----------|------|
| GPS | UART | 经纬度采集 |
| NFC | SPI（或 I2C） | 无源激活标签 |
| 温湿度传感器 | I2C（SHT30） | 温湿度监测 + 阈值报警 |
| 声光报警 | GPIO（LED/Buzzer） | 本地超限报警 |
| 低功耗管理 | RTC / Stop 模式 | 默认深度睡眠，NFC 唤醒 |
| 云平台通信 | UART → ESP8266（AT）/ 4G 模块 → MQTT | 数据上报 + 指令接收 |
| 云端指令处理 | MQTT 消息解析 | 暂留/退货/继续运输指令响应 |

## 重要提醒

- `test1/Drivers/CMSIS/` 和 `test1/Drivers/STM32H7xx_HAL_Driver/` 为 ST 官方库，**请勿手动修改**（通过 CubeMX 版本管理）
- CubeMX `USER CODE BEGIN/END` 段内的代码在重新生成时 **会保留**，段外的修改 **会被覆盖**
- 编译产物（`test1/MDK-ARM/test1/*.o`, `*.axf`, `*.hex`, `*.map` 等）**不应纳入版本控制**
