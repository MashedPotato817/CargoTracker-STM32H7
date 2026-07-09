# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

STM32H7A3ZI-Q 智能货物追踪物联网系统。GPS + NFC + SHT31 温湿度 + Air780E(4G/MQTT) + W25Q128 Flash + 低功耗。

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
| USART1 PWRKEY | PB0 GPIO_Output | AIR780E_PWRKEY，默认 HIGH 释放；低电平表示按下 |
| USART2 (GPS) | PD5 TX, PD6 RX | 115200 baud（GPS_Init 运行时重配 9600） |
| I2C1 (SHT31/PN532) | PB8 SCL, PB9 SDA | 400kHz, 共享总线 |
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
main.c → FreeRTOS(Task调度/队列通信) → APP层(状态机/报警/低功耗) → Driver层(UART/I2C/SPI/GPIO) → HAL库 → STM32
```

### FreeRTOS 任务架构（6 Task + 3 Queue）

| Task | 负责人 | 优先级 | 职责 |
|------|--------|--------|------|
| Task_I2C_Sensors | A | 中 | SHT31 + PN532（共享 I2C1，同 task 无竞态） |
| Task_Flash | A | 低 | W25Q128 读写缓存 |
| Task_GPS | B | 中 | USART2 接收 + NMEA 解析 |
| Task_4G_MQTT | B | 高 | USART1 AT指令 + MQTT |
| Task_StateMachine | C | 高 | 状态机 + 低功耗 + 云指令分发 |
| Task_Alarm | C | 低 | 蜂鸣器 + LED |

3 条队列：`queue_activation`（NFC→状态机）、`queue_sensor_data`（传感器数据→状态机）、`queue_cloud_cmd`（MQTT→状态机）

### 开发策略

**当前阶段：竞赛交付**（截止 2026-07-09 18:00）。硬件集成基本完成，进入联调+产出物阶段。

```c
#define SHT31_USE_HAL_I2C    1  // 1=真实I2C驱动, 0=stub回退
#define PN532_USE_HAL_I2C    1
#define W25Q128_USE_HAL_SPI  1
#define GPS_USE_HAL_UART     1
#define AIR780E_USE_HAL_UART 1
```

- `0`（stub 模式）：返回模拟数据，系统可独立编译运行，无需外设
- `1`（真实模式）：通过 HAL 访问硬件，需 CubeMX 已使能对应外设

切换流程：CubeMX 使能外设 → 生成 HAL 句柄 → 设 flag=1 → 编译 → 硬件测试。失败可切回 0，不影响其他模块。

### 模块接口规范

各模块提供统一接口，驱动层与应用层分离：

```c
// 驱动层提供（成员A）
void SHT31_Init(void);
float SHT31_ReadTemperature(void);
float SHT31_ReadHumidity(void);
GPS_GetLocation();
MQTT_Publish();
PN532_ReadCard();
Flash_Write();

// 应用层调用（成员C），不关心总线细节
temp = SHT31_ReadTemperature();
```

### 目标代码结构

```
test1/Core/
├── Inc/
│   ├── main.h, FreeRTOSConfig.h
│   └── app/        app.h, state_machine.h, alarm.h, power.h
└── Src/
    ├── main.c      (初始化 + FreeRTOS task 创建，全部在 USER CODE 段内)
    ├── i2c.c, spi.c, usart.c, gpio.c  (CubeMX 生成 HAL 外设初始化)
    ├── app/        app.c, state_machine.c, alarm.c, power.c    ← 成员 C
    ├── sensor/     sht31.c                                     ← 成员 A
    ├── nfc/        pn532.c                                     ← 成员 A
    ├── flash/      w25q128.c                                   ← 成员 A
    ├── gps/        gps.c                                       ← 成员 B
    └── air780e/    air780e.c, mqtt.c                           ← 成员 B
├── Drivers/        (STM32 HAL + CMSIS，不手动修改)
└── Middlewares/    FreeRTOS (CMSIS_V2)
```

## 首次设置（clone 后必做！）

> ⚠️ `Drivers/`、`Middlewares/`、`test1.ioc`、`system_stm32h7xx.c`、`test1.uvprojx` **不在 Git 中**（`.gitignore` 已排除），clone 后无法直接编译。

**操作步骤：**
1. 用 **STM32CubeMX 6.17.0** 打开 `test1/test1.ioc`
2. 菜单 → **Generate Code**（固件包：STM32Cube FW_H7 V1.13.0）
3. 生成后所有缺失文件和目录出现在本地
4. Keil 打开 `test1/MDK-ARM/test1.uvprojx` → F7 编译

> 版本不一致会导致生成的 HAL 代码不同。团队统一使用 CubeMX 6.17.0 + FW_H7 V1.13.0。

## 开发操作

| 操作 | 方式 |
|------|------|
| 初次编译 | 上面「首次设置」做完 → Keil F7 |
| 日常编译 | Keil F7 (Build) |
| 下载烧录 | Keil Ctrl+F8 (Download) |
| 修改外设 | CubeMX 打开 `test1/test1.ioc` → 重新生成代码 |
| 添加模块 | `test1/Core/Src/` 下新建 `.c`，`Inc/` 下新建 `.h` |

### 时钟配置

- 时钟源: **HSI** 64MHz（NUCLEO-H7A3ZI-Q 无外部 HSE 晶振！）
- SYSCLK: 280 MHz (PLL: HSI / 4 * 35 / 2)
- APB1: 140 MHz, APB2: 140 MHz
- 详见 `doc-workflow/TROUBLESHOOTING.md` 第 1 条（HSE 晶振陷阱）

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
3. **CubeMX 生成文件不纳入版本控制**：`Drivers/`、`Middlewares/`、`.ioc`、`system_stm32h7xx.c`、`.uvprojx` 均由 CubeMX Generate Code 恢复
4. **4G模块独立供电** (4.0V)，不可与传感器共用 3.3V
5. **I2C 共享总线**：SHT31 + PN532 + OLED 共用 PB8/PB9
6. **低功耗**：默认 Stop Mode，NFC 唤醒；4G 模块空闲时关断 PWRKEY
7. **断网缓存**：温湿度 / GPS 数据写入 W25Q128，联网后补传

## 项目文件索引

| 文件 | 内容 |
|------|------|
| `README.md` | 项目介绍和快速开始 |
| `index.html` | 硬件接线指南（GitHub Pages） |
| `dashboard.html` | 云端控制台（MQTT + Leaflet 地图 + 指令下发） |
| `report.html` | 设计报告（GitHub Pages 在线阅读） |
| `doc/code-review-20260709.md` | 代码全面审查报告（29 问题，P0-P3） |
| `doc/STM32H7A3ZI-Q_程序下载要点.md` | Keil 烧录详细步骤 |
| `doc/STM32H7_CubeMX_Config.md` | CubeMX 引脚/时钟/DMA 完整配置 |
| `doc/STM32H7_Logistics_System.md` | 完整设计方案（BOM/接线/电源/状态机） |
| `doc-workflow/README.md` | 工作流目录使用指南 |
| `doc-workflow/last-tasks.md` | 🎯 当前未完成任务清单（每日更新） |
| `doc-workflow/task2.md` | 硬件集成任务计划 |
| `doc-workflow/task3.md` | 云平台对接（含 3.1/3.3/3.5 子任务） |
| `doc-workflow/task4.md` | 低功耗优化 |
| `doc-workflow/task5.md` | 整机测试 + 产出物 |
| `doc-workflow/TROUBLESHOOTING.md` | 团队踩坑记录 |
| `doc-workflow/GIT_WORKFLOW.md` | Git 协作规范（分支策略 / PR / CubeMX 协调） |
| `doc-workflow/hardware_map.md` | 硬件-代码映射表 |
| `doc-workflow/archive/` | 历史日志（logs/plans/debug 三个子目录） |
| `report-workflow/design_report.md` | 设计文档（8 章 + GPT 绘图结构） |
| `report-workflow/competition_report.md` | 竞赛报告正文 |
| `report-workflow/gpt-prompts.md` | GPT 绘图 + 润色 prompts |
| `report-workflow/video-guide.md` | 演示视频拍摄剪辑指南 |
