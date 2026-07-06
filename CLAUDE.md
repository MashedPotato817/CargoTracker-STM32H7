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
main.c → FreeRTOS(Task调度/队列通信) → APP层(状态机/报警/低功耗) → Driver层(UART/I2C/SPI/GPIO) → HAL库 → STM32
```

### FreeRTOS 任务架构（6 Task + 3 Queue）

| Task | 负责人 | 优先级 | 职责 |
|------|--------|--------|------|
| Task_I2C_Sensors | A | 中 | SHT30 + PN532（共享 I2C1，同 task 无竞态） |
| Task_Flash | A | 低 | W25Q128 读写缓存 |
| Task_GPS | B | 中 | USART2 接收 + NMEA 解析 |
| Task_4G_MQTT | B | 高 | USART1 AT指令 + MQTT |
| Task_StateMachine | C | 高 | 状态机 + 低功耗 + 云指令分发 |
| Task_Alarm | C | 低 | 蜂鸣器 + LED |

3 条队列：`queue_activation`（NFC→状态机）、`queue_sensor_data`（传感器数据→状态机）、`queue_cloud_cmd`（MQTT→状态机）

### 开发策略

**硬件只有开发板时**：所有驱动用 stub 实现（返回模拟数据），先跑通 FreeRTOS 调度 + 状态机 + 任务通信全流程。硬件到后替换驱动内部实现，接口不变，应用层零改动。

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
test1/Core/
├── Inc/
│   ├── main.h, FreeRTOSConfig.h
│   └── app/        app.h, state_machine.h, alarm.h, power.h
└── Src/
    ├── main.c      (初始化 + FreeRTOS task 创建，全部在 USER CODE 段内)
    ├── app/        app.c, state_machine.c, alarm.c, power.c    ← 成员 C
    ├── sensor/     sht30.c                                     ← 成员 A
    ├── nfc/        pn532.c                                     ← 成员 A
    ├── flash/      w25q128.c                                   ← 成员 A
    ├── gps/        gps.c                                       ← 成员 B
    └── air780e/    air780e.c, mqtt.c                           ← 成员 B
├── Drivers/        (STM32 HAL + CMSIS，不手动修改)
└── Middlewares/    FreeRTOS (CMSIS_V2)
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
| `README.md` | 项目介绍和快速开始 |
| `doc/task.md` | 三人分工方案、接口规范、开发顺序 |
| `doc/STM32H7A3ZI-Q_程序下载要点.md` | Keil 烧录详细步骤 |
| `doc/STM32H7_CubeMX_Config.md` | CubeMX 引脚/时钟/DMA 完整配置 |
| `doc/STM32H7_Logistics_System.md` | 完整设计方案（BOM/接线/电源/状态机） |
| `doc-workflow/task1.md` | 第一轮开发计划（FreeRTOS + 状态机 + stub，含 15 步详细操作） |
| `doc-workflow/TROUBLESHOOTING.md` | 团队踩坑记录（CubeMX/Keil/FreeRTOS/Git 常见问题） |
| `.claude/projects/` | Claude Code 项目记忆（MCU/开发板参考资料） |
