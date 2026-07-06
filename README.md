# CargoTracker-STM32H7

> 基于 STM32H7 的智能货物追踪物联网系统

## 项目概述

本作品设计了一款基于 **STM32H7A3ZI** 的智能货物追踪设备，针对物流运输中货物定位难、环境监控盲、激活效率低、云端交互滞后等问题，实现了 **GPS 实时定位 + NFC 一键激活 + 温湿度智能监测报警 + 云端指令控制**的全链路功能。

设备上电后默认进入深度睡眠模式，通过外部 NFC 标签实现无源激活，成功激活后自动采集 GPS 模块的经纬度信息，并周期性读取高精度温湿度传感器，当温湿度超出阈值时立即本地声光报警并上报云平台。同时，设备支持远程指令接收，可实时响应云端下发的"暂留""退货""继续运输"等指令，实现物流全程智能化闭环管理。

## 硬件平台

| 项目 | 规格 |
|------|------|
| **主控 MCU** | STM32H7A3ZI (Cortex-M7, 480MHz) |
| **开发板** | NUCLEO-H7A3ZI-Q |
| **开发环境** | Keil MDK + STM32CubeMX |

## 功能模块

| 模块 | 外设/协议 | 功能描述 |
|------|-----------|---------|
| GPS 定位 | UART | 经纬度实时采集 |
| NFC 激活 | SPI（或 I2C） | 无源标签一键唤醒设备 |
| 温湿度监测 | I2C（SHT30） | 实时温湿度采集 + 阈值报警 |
| 声光报警 | GPIO（LED/Buzzer） | 本地超限声光提示 |
| 低功耗管理 | RTC / Stop 模式 | 默认深度睡眠，NFC 唤醒 |
| 云平台通信 | UART → ESP8266(AT) / 4G → MQTT | 数据上报 + 指令接收 |
| 云端指令 | MQTT 消息解析 | 暂留 / 退货 / 继续运输 |

## 项目结构

```
CargoTracker-STM32H7/
├── .claude/                       # Claude Code 配置
├── .git/                          # Git 仓库
├── .gitignore                     # Git 忽略规则
├── CLAUDE.md                      # 项目指引（供 Claude Code 使用）
├── README.md                      # 本文件
├── doc/                           # 文档目录
│   ├── task.md                    # 三人分工方案、接口规范、开发顺序
│   ├── STM32H7A3ZI-Q_程序下载要点.md # Keil 烧录详细步骤
│   ├── STM32H7_CubeMX_Config.md   # CubeMX 引脚/时钟/DMA 完整配置
│   └── STM32H7_Logistics_System.md # 完整设计方案（BOM/接线/电源/状态机）
└── test1/                         # STM32CubeMX 生成的工程（Keil MDK）
    ├── Core/                      # 核心代码
    │   ├── Inc/                   #   头文件
    │   └── Src/                   #   源文件
    ├── Drivers/                   # HAL 驱动 + CMSIS（不手动修改）
    └── MDK-ARM/                   # Keil MDK 项目文件
```

## 快速开始

### 环境准备

1. **Keil MDK**（建议 V5.39 或以上）
2. **STM32CubeMX**（修改外设配置时使用）
3. **STM32H7 Device Family Pack**（DFP）
4. **ST-Link 驱动**

### 打开工程

双击 `test1/MDK-ARM/test1.uvprojx` 即可在 Keil MDK 中打开。

### 编译 & 烧录

| 操作 | 快捷键 |
|------|--------|
| 编译 | **F7**（Build） |
| 下载 | **Ctrl+F8**（Download） |
| 复位运行 | 按下开发板 **RESET** 键 |

### Flash Download 配置（重要）

STM32H7A3ZI 的 Keil Flash Algorithm RAM 地址需要手动设置：

```
RAM for Algorithm
Start : 0x24000000
Size  : 0x00010000
```

否则会出现 `Cannot Load Flash Programming Algorithm` 错误。

### 验证

烧录后运行点灯测试，确认开发环境正常：

```c
BSP_LED_Init(LED_GREEN);

while (1)
{
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(500);
}
```

若绿色 LED 以 0.5 秒间隔闪烁，说明 ST-Link、Flash 下载、BSP 配置均正常。

## 学习路线

按顺序掌握以下外设驱动，逐步完成系统：

1. LED 点灯 — GPIO 输出
2. 按键中断 — EXTI
3. USART 串口 — 调试输出
4. 定时器 — TIM
5. I2C — SHT30 温湿度传感器
6. SPI — NFC 模块
7. UART — GPS 模块
8. MQTT — 云平台通信

## 开发注意事项

- `Drivers/` 目录下的 CMSIS 和 HAL 驱动为 ST 官方库，**请勿手动修改**，应通过 CubeMX 版本管理。
- CubeMX 重新生成代码时，`USER CODE BEGIN/END` 段内的代码会保留，段外的修改会被覆盖。
- 编译产物（`.o`、`.axf`、`.hex`、`.map` 等）**不应纳入版本控制**。

## License

本项目仅供学习研究使用。
