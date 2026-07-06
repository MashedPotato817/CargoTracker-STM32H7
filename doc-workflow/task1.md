# Task 1：FreeRTOS 工程骨架 + 应用层框架 + 驱动 stub

> 目标：在仅有 NUCLEO-H7A3ZI-Q 开发板（无外设）的条件下，搭建可编译、可烧录、可验证的完整系统框架。

## 前置条件

- Keil MDK + STM32CubeMX + STM32H7 DFP 已安装
- NUCLEO-H7A3ZI-Q 开发板 x1
- 其他外设模块暂不需要（驱动使用 stub 替代）

## FreeRTOS 任务架构

采用**按模块拆 + 共享资源合并**方案，共 6 个 task，通过 3 条队列通信：

| Task | 成员 | 优先级 | 栈大小 | 职责 |
|------|------|--------|--------|------|
| `Task_I2C_Sensors` | A | 中 | 512B | SHT30 温湿度 + PN532 NFC（共享 I2C1） |
| `Task_Flash` | A | 低 | 512B | W25Q128 读写缓存 |
| `Task_GPS` | B | 中 | 512B | USART2 接收 + NMEA 解析 |
| `Task_4G_MQTT` | B | 高 | 1KB | USART1 AT指令 + MQTT 收发 |
| `Task_StateMachine` | C | 高 | 1KB | 状态机 + 低功耗管理 + 云指令分发 |
| `Task_Alarm` | C | 低 | 256B | 蜂鸣器 + LED 控制 |

### 任务间通信

```
Task_NFC ──(激活事件)──→ Task_StateMachine
Task_StateMachine ──(触发采集)──→ Task_I2C_Sensors
Task_StateMachine ──(触发定位)──→ Task_GPS
Task_I2C_Sensors ──(温湿度数据)──→ Task_StateMachine
Task_GPS ──(经纬度数据)──→ Task_StateMachine
Task_StateMachine ──(上传数据)──→ Task_4G_MQTT
Task_4G_MQTT ──(云端指令)──→ Task_StateMachine
Task_StateMachine ──(报警触发)──→ Task_Alarm
Task_StateMachine ──(缓存数据)──→ Task_Flash
```

3 条队列：
- `queue_activation` — NFC 激活事件
- `queue_sensor_data` — 温湿度 + GPS 数据 → 状态机
- `queue_cloud_cmd` — 云端指令 → 状态机

### 状态机

```
深度睡眠 → NFC激活 → GPS定位 → 温湿度采集 → 数据上传 → 等待云指令 → [暂留/退货/继续运输] → 休眠
     ↑                                                                                      │
     └──────────────────────────────────────────────────────────────────────────────────────┘
```

## 三步推进

### 第 1 步：CubeMX FreeRTOS 工程 + Task 骨架

**CubeMX 配置：**
- FreeRTOS CMSIS_V2 使能
- 6 个 task 定义（名称/优先级/栈大小如上表）
- 3 条队列定义
- USART3（ST-LINK VCP PD8/PD9）用于 printf 调试输出
- 板载 LED（PB0/PE1/PB14）和按键（PC13 EXTI）配置

**框架代码：**
- `main.c` — 只做初始化，不写逻辑
- 每个 task 骨架 — 死循环 + `printf("Task xxx heartbeat\n")` + `osDelay()`
- 按键中断 — EXTI 回调中发送队列消息

**验证：** 串口终端打印 6 个 task 心跳，确认 FreeRTOS 调度正常。按键触发后打印消息。

### 第 2 步：状态机 + 应用层

**从成员 C 开始：**
- `state_machine.c/h` — 状态枚举 + 转移表 + 核心调度函数
- `alarm.c/h` — 阈值比较 + LED/蜂鸣器控制
- `power.c/h` — 低功耗模式控制（Stop Mode 入口/唤醒）
- `app.c/h` — 系统初始化 + 主流程串联

**LED 状态指示：**
| LED | 颜色 | 含义 |
|-----|------|------|
| LD1 (PB0) | 绿 | 系统运行心跳 |
| LD2 (PE1) | 黄 | 报警状态 |
| LD3 (PB14) | 红 | 联网/数据传输中 |

**按键（PC13）作用：** 模拟 NFC 激活（短按 = 标签靠近，长按 = 另一指令）

**验证：** 按键触发 → LED 状态变化 → 串口打印状态转移日志，完整跑通一次流程。

### 第 3 步：外设驱动 stub

**从成员 A/B 开始：**

每个驱动模块提供两个文件：
- `<module>.c/h` — 真实初始化 + Stub 数据函数
- Stub 返回模拟数据，真实硬件到后替换内部实现即可

```c
// sht30.c — stub 版本
void SHT30_Init(void) {
    printf("[SHT30] Init OK (stub)\n");
}
float SHT30_ReadTemperature(void) {
    return 25.5f;  // 模拟室温
}
float SHT30_ReadHumidity(void) {
    return 60.0f;  // 模拟湿度
}
```

**所有需要 stub 的模块：**
| 模块 | 文件 | 初始化 | stub 返回值 |
|------|------|--------|-----------|
| SHT30 | sensor/sht30.c | I2C1 Init | temp=25.5°C, hum=60% |
| PN532 | nfc/pn532.c | I2C1 Init | UID="MOCK-UID-001" |
| GPS | gps/gps.c | USART2 Init | lat=31.23, lon=121.47 |
| Air780E | air780e/air780e.c | USART1 Init | AT OK, CSQ=25 |
| MQTT | air780e/mqtt.c | — | publish OK, cmd queue |
| W25Q128 | flash/w25q128.c | SPI1 Init | read/write OK |

**验证：** 烧录后，全流程自动运行 — 按键"激活" → 采集模拟数据 → "上传"到云 → 打印状态转移 → LED 闪烁 → 进入休眠 → 等待下次按键。

## 硬件到后切换

每个 stub 模块替换为真实驱动时，只需修改对应 `<module>.c` 的内部实现，**接口函数签名不变**，应用层代码零改动。

例如 SHT30：
```c
// stub → 真实：只改函数体，不改头文件
float SHT30_ReadTemperature(void) {
    uint8_t data[6];
    HAL_I2C_Master_Receive(&hi2c1, SHT30_ADDR, data, 6, 100);
    // ... 真实 CRC 校验 + 温度计算公式
    return temp;
}
```

## 输出物

完成后的代码结构：

```
test1/Core/
├── Inc/
│   ├── FreeRTOSConfig.h          # CubeMX 生成
│   ├── main.h
│   ├── stm32h7xx_it.h
│   └── app/
│       ├── app.h
│       ├── state_machine.h
│       ├── alarm.h
│       └── power.h
└── Src/
    ├── main.c                    # 初始化 + task 创建
    ├── stm32h7xx_it.c            # 中断服务（含按键 EXTI）
    ├── app/
    │   ├── app.c                 # 成员 C — 系统框架
    │   ├── state_machine.c       # 成员 C — 状态机
    │   ├── alarm.c               # 成员 C — 报警逻辑
    │   └── power.c               # 成员 C — 低功耗
    ├── sensor/sht30.c            # 成员 A — stub
    ├── nfc/pn532.c               # 成员 A — stub
    ├── flash/w25q128.c           # 成员 A — stub
    ├── gps/gps.c                 # 成员 B — stub
    └── air780e/
        ├── air780e.c             # 成员 B — stub
        └── mqtt.c                # 成员 B — stub
```
