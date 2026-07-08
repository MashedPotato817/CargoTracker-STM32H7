# 项目当前状态 — 2026-07-07

> 基于 commit `a44ffc2`（merge W 分支后）。

---

## 开发阶段：Task 2 代码完成 — 待硬件验证

CubeMX 外设已全部使能，所有驱动标志位切到 `1`（真 HAL 驱动）。Keil 编译 0 Error / 0 Warning。下一步是接线 + 硬件验证。

---

## 硬件对照表

| 硬件 | 文件名 | 当前状态 | 备注 |
|------|--------|----------|------|
| **NUCLEO-H7A3ZI-Q** | `main.c` / `freertos.c` | ✅ 运行 | 时钟 280MHz (HSI)，6 Task + 3 Queue，USART3 printf |
| **Air780E (4G)** | `air780e/air780e.c` | ⚠️ 代码就绪 | HAL UART 真 AT 实现，PB0 PWRKEY 已释放，待硬件 AT 验证 |
| **Air780E MQTT** | `air780e/mqtt.c` | ⚠️ 代码就绪 | 真 UART MQTT AT 路径，待 Air780E 硬件验证 |
| **ATGM336H (GPS)** | `gps/gps.c` | ⚠️ 代码就绪 | HAL UART NMEA 真实现，GPS_Init 自动重配 9600 baud |
| **PN532 (NFC)** | `nfc/pn532.c` | ⚠️ 代码就绪 | `PN532_USE_HAL_I2C=1`，等硬件接线验证 |
| **SHT31** | `sensor/sht31.c` | ⚠️ 代码就绪 | `SHT31_USE_HAL_I2C=1`，等硬件接线验证 |
| **W25Q128** | `flash/w25q128.c` | ⚠️ 代码就绪 | `W25Q128_USE_HAL_SPI=1`，含扇区擦除+写后校验 |
| **MP1584 + TP4056** | — | 文档规划 | 电源架构已设计，无代码 |
| **3.7V 锂电池** | — | 文档规划 | 电源架构已设计，无代码 |
| **蜂鸣器 (PC8)** | `app/alarm.c` | ✅ 已配置 | GPIO_Output，CubeMX 已使能 |
| **外接 LED (PC9)** | `app/alarm.c` | ✅ 已配置 | GPIO_Output，CubeMX 已使能 |

---

## FreeRTOS 任务

| Task | 优先级 | 栈大小 | Entry Function | 当前行为 |
|------|--------|--------|----------------|----------|
| Task_StateMachine | High | 1KB (256 Words) | `StartTask_StateMachine` → `App_TaskStateMachine` | 状态机主循环：激活→GPS→SHT31→报警→上传→云指令→休眠 |
| Task_4G_MQTT | High | 1KB (256 Words) | `StartTask_4G_MQTT` → `App_Task4GMQTT` | 每 4s 轮询云端指令，通过 queue_cloud_cmd 发给状态机 |
| Task_I2C_Sensors | Normal | 512B (128 Words) | `StartTask_I2C_Sensors` → `App_TaskI2CSensors` | 轮询 PN532 检测卡片，响应 env_sample_requested 采样 SHT31 |
| Task_GPS | Normal | 512B (128 Words) | `StartTask_GPS` → `App_TaskGPS` | 响应 gps_sample_requested，USART2 读取并解析 NMEA |
| Task_Flash | Low | 512B (128 Words) | `StartTask_Flash` → `App_TaskFlash` | 每 10s 空闲轮询（`W25Q128_Task`） |
| Task_Alarm | Low | 512B (128 Words) | `StartTask_Alarm` → `App_TaskAlarm` | 每 500ms 更新 LED 报警状态，含 Buzzer(PC8)/LED(PC9) |

## 队列通信

| 队列 | 长度 | Item Size | 数据流 |
|------|------|-----------|--------|
| queue_activation | 16 | uint16_t | PN532/按键 → StateMachine |
| queue_sensor_data | 16 | uint32_t | I2C/GPS → StateMachine（传 struct 指针） |
| queue_cloud_cmd | 8 | uint16_t | MQTT → StateMachine |

---

## CubeMX 引脚配置现状

| 引脚 | 功能 | 状态 | 说明 |
|------|------|------|------|
| PB0 | AIR780E_PWRKEY GPIO_Output | ✅ 已配 | PWRKEY 默认 HIGH 释放，避免持续拉低关机 |
| PE1 | LD2_YELLOW GPIO_Output | ✅ 已配 | 报警指示灯 |
| PB14 | LD3_RED GPIO_Output | ✅ 已配 | 数据传输指示灯 |
| PC13 | BTN_USER EXTI | ✅ 已配 | 用户按键（模拟 NFC 激活） |
| PD8/PD9 | USART3 TX/RX | ✅ 已配 | 调试串口 115200 |
| PA9/PA10 | USART1 TX/RX | ✅ 已配 | Air780E 115200，hal_uart |
| PD5/PD6 | USART2 TX/RX | ✅ 已配 | GPS 115200（GPS_Init 重配 9600） |
| PB8/PB9 | I2C1 SCL/SDA | ✅ 已配 | SHT31 + PN532 共享，400kHz |
| PA4-PA7 | SPI1 CS/SCK/MISO/MOSI | ✅ 已配 | W25Q128 |
| PC8 | Buzzer GPIO_Output | ✅ 已配 | CubeMX 已使能 |
| PC9 | Alarm LED GPIO_Output | ✅ 已配 | CubeMX 已使能 |

---

## 驱动标志位

| 标志 | 文件 | 值 |
|------|------|-----|
| `SHT31_USE_HAL_I2C` | `sht31.h` | `1` |
| `PN532_USE_HAL_I2C` | `pn532.h` | `1` |
| `W25Q128_USE_HAL_SPI` | `w25q128.h` | `1` |
| `GPS_USE_HAL_UART` | `gps.h` | `1` |
| `AIR780E_USE_HAL_UART` | `air780e.h` | `1` |
| `configENABLE_FPU` | `FreeRTOSConfig.h` | `1` |

---

## 待解决问题

1. **Air780E 硬件验证待做** — PB0 PWRKEY 已释放为 HIGH，需重新烧录后验证 AT 指令不再 timeout
2. **USART2 默认 115200** — CubeMX 生成值，GPS_Init 已做运行时 9600 重配。建议 CubeMX 改为 9600 避免漂移（TROUBLESHOOTING #12）
3. **低功耗未实现** — `Power_EnterStopStub()` 是空函数，未实现真正的 Stop Mode 和 NFC 唤醒
4. **硬件验证全部待做** — SHT31 / PN532 / GPS / W25Q128 / Air780E 均未接线验证
5. **系统集成测试未开始** — Task 2.7 的 9 个测试用例待执行

---

## 下一步推进方向

- **Air780E/MQTT 硬件验证**（最高优先级）→ 先确认 PB0 默认 HIGH 后 AT 指令响应
- **按顺序接线验证**：SHT31 → PN532 → GPS → W25Q128 → Air780E
- **实现低功耗** — 接入真正的 Stop Mode + NFC 唤醒
- **24h 长时间稳定性测试**
