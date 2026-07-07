# CargoTracker-STM32H7

> 基于 STM32H7 的智能货物追踪物联网系统

## 项目概述

本作品设计了一款基于 **STM32H7A3ZI** 的智能货物追踪设备，针对物流运输中货物定位难、环境监控盲、激活效率低、云端交互滞后等问题，实现了 **GPS 实时定位 + NFC 一键激活 + 温湿度智能监测报警 + 云端指令控制**的全链路功能。

设备上电后默认进入深度睡眠模式，通过外部 NFC 标签实现无源激活，成功激活后自动采集 GPS 模块的经纬度信息，并周期性读取高精度温湿度传感器，当温湿度超出阈值时立即本地声光报警并上报云平台。同时，设备支持远程指令接收，可实时响应云端下发的"暂留""退货""继续运输"等指令，实现物流全程智能化闭环管理。

## 硬件平台

| 项目 | 规格 |
|------|------|
| **主控 MCU** | STM32H7A3ZI (Cortex-M7, 280MHz) |
| **开发板** | NUCLEO-H7A3ZI-Q |
| **开发环境** | Keil MDK + STM32CubeMX |

## 功能模块

| 模块 | 外设/协议 | 功能描述 |
|------|-----------|---------|
| GPS 定位 | UART | 经纬度实时采集 |
| NFC 激活 | I2C（PN532） | 无源标签一键唤醒设备 |
| 温湿度监测 | I2C（SHT31） | 实时温湿度采集 + 阈值报警 |
| 声光报警 | GPIO（LED/Buzzer） | 本地超限声光提示 |
| 低功耗管理 | RTC / Stop 模式 | 默认深度睡眠，NFC 唤醒 |
| 云平台通信 | UART → Air780E 4G → MQTT | 数据上报 + 指令接收 |
| 云端指令 | MQTT 消息解析 | 暂留 / 退货 / 继续运输 |

## 项目结构

```
CargoTracker-STM32H7/
├── .claude/                       # Claude Code 配置（团队共享 settings.json）
├── AGENTS.md                      # Codex 指引（指向 CLAUDE.md）
├── CLAUDE.md                      # 项目权威指引（供 Claude Code/Codex 使用）
├── README.md                      # 本文件
├── doc/                           # 设计文档
│   ├── task.md                    # 三人分工方案、接口规范
│   ├── STM32H7A3ZI-Q_程序下载要点.md # Keil 烧录详细步骤
│   ├── STM32H7_CubeMX_Config.md   # CubeMX 引脚/时钟/DMA 完整配置
│   └── STM32H7_Logistics_System.md # 完整设计方案（BOM/接线/电源/状态机）
├── doc-workflow/                  # 开发工作流
│   ├── task2.md                   #   当前任务：硬件集成
│   ├── GIT_WORKFLOW.md            #   Git 协作规范
│   ├── TROUBLESHOOTING.md         #   踩坑记录
│   ├── hardware_map.md            #   硬件-代码映射表
│   └── archive/                   #   历史日志归档
├── user-manual-md/                # 参考手册（Markdown）
└── test1/                         # Keil MDK 工程
    ├── Core/                      # 核心源码
    ├── Drivers/                   # HAL + CMSIS（CubeMX 生成，不入库）
    └── MDK-ARM/                   # Keil 项目文件
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

## 当前开发阶段

**硬件集成阶段**。所有外设模块（SHT31、PN532、ATGM336H、Air780E、W25Q128）已到齐。驱动采用条件编译双模式（`*_USE_HAL_*` 宏），可独立切换 stub/真实模式。

详见 [`doc-workflow/task2.md`](doc-workflow/task2.md) — 硬件集成任务计划。

> ⚠️ 遇到问题先查 [`doc-workflow/TROUBLESHOOTING.md`](doc-workflow/TROUBLESHOOTING.md)（团队踩坑记录），解决后往里面补充。

### FreeRTOS 任务架构（6 Task + 3 Queue）

| Task | 负责人 | 优先级 | 职责 |
|------|--------|--------|------|
| Task_StateMachine | C | 高 | 状态机调度 + 低功耗 + 云指令分发 |
| Task_4G_MQTT | B | 高 | USART1 AT指令 + MQTT 收发 |
| Task_I2C_Sensors | A | 中 | SHT31 + PN532（共享 I2C1） |
| Task_GPS | B | 中 | USART2 + NMEA 解析 |
| Task_Flash | A | 低 | W25Q128 读写缓存 |
| Task_Alarm | C | 低 | LED + 蜂鸣器 |

**3 条队列：** `queue_activation`（NFC/按钮→状态机）、`queue_sensor_data`（GPS/温湿度→状态机）、`queue_cloud_cmd`（MQTT→状态机）

## 开发注意事项

- `Drivers/` 目录下的 CMSIS 和 HAL 驱动为 ST 官方库，**请勿手动修改**，应通过 CubeMX 版本管理。
- CubeMX 重新生成代码时，`USER CODE BEGIN/END` 段内的代码会保留，段外的修改会被覆盖。
- 编译产物（`.o`、`.axf`、`.hex`、`.map` 等）**不应纳入版本控制**。

## License

本项目仅供学习研究使用。
