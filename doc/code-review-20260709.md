# 单片机代码全面审查报告

> 审查日期：2026-07-09 | 审查范围：`test1/Core/Src/` 全部团队代码（11 个 C 文件） | 3 组并行审计

## 审查范围

| 组 | 文件 | 负责人 |
|------|------|:---:|
| 驱动层 | `sensor/sht31.c` `nfc/pn532.c` `flash/w25q128.c` | A |
| 通信层 | `air780e/air780e.c` `air780e/mqtt.c` `gps/gps.c` | B |
| 应用层 | `main.c` `app/app.c` `app/state_machine.c` `app/alarm.c` `app/power.c` | C |

跳过 9 个 CubeMX/HAL 自动生成文件（`*_hal_*.c`、`freertos.c`、`system_stm32h7xx.c` 等）。

---

## 🔴 致命缺陷（7 个）

### 1. USART1 多任务竞态条件

**文件**：`air780e/air780e.c`、`air780e/mqtt.c`、`freertos.c`
**严重度**：🔴 致命
**负责人**：B

Task_StateMachine 和 Task_4G_MQTT 均为 `osPriorityHigh`，FreeRTOS 默认启用时间片轮转（`configUSE_TIME_SLICING=1`），两个任务交替执行时同时访问 `huart1`：

| 任务 | UART 操作 | 触发时机 |
|------|----------|---------|
| Task_StateMachine | `TCP_SendBinary` → `TCP_SendAT` → `TCP_SendATCapture`（读 huart1，等待 `>`） | 每次遥测上传 |
| Task_4G_MQTT | `MQTT_PollCommand` → `MQTT_ReadUartBytes`（读 huart1，最多 600ms） | 每 1000ms |

竞态场景：StateMachine 在 `TCP_SendATCapture` 中逐字节等待 `>` 提示符，时间片到后 Task_4G_MQTT 被调度，调用 `MQTT_ReadUartBytes(rx, 512, 600)` 一口气读取最多 512 字节。如果 Air780E 恰好返回了 `>`，它被 MQTT 任务吃掉，StateMachine 的 AT 命令超时返回 0。此外 `HAL_UART_Receive` 不可重入。

**修复**：创建互斥锁保护 huart1 所有访问，或改为 DMA 中断接收 + 环形缓冲区。

---

### 2. USART1 FIFO 溢出 — 逐字节轮询不适用于 115200bps + FreeRTOS

**文件**：`air780e/air780e.c`、`air780e/mqtt.c`
**严重度**：🔴 致命
**负责人**：B

两个文件均使用 `HAL_UART_Receive(&huart1, &ch, 1U, 20U)` 逐字节读取。115200bps 下每字节 ~87μs。STM32H7 USART FIFO 仅 8 字节深度，约 0.7ms 即溢出。一次 FreeRTOS 上下文切换远超此值，导致字节丢失、AT 响应校验失败、MQTT 包损坏。

**修复**：启用 USART1 DMA 循环接收 + 环形缓冲区，或 UART RX 中断 → `xStreamBufferSendFromISR`。

---

### 3. TCP_SendBinary 始终返回成功，未验证 SEND OK

**文件**：`air780e/mqtt.c` 第 229-240 行
**严重度**：🔴 致命
**负责人**：B

```c
// drain 200ms 后无条件返回 1
return 1U;
```

二进制数据发送后等待 200ms 就返回成功，从未解析模块返回的 `SEND OK`。如果 `AT+CIPSEND` 因模块忙、TCP 断开或网络故障返回 `ERROR`，数据实际未发送但代码报告成功。遥测数据**静默丢失**且不会触发 Flash 缓存回退。

**修复**：在 drain 循环中检测 `"SEND OK"` 子串，超时或收到 ERROR 返回 0。

---

### 4. CONNACK 返回码未检查

**文件**：`air780e/mqtt.c` 第 526-546 行
**严重度**：🔴 致命
**负责人**：B

MQTT CONNACK 第 4 字节是返回码：0=接受，1-5=拒绝。代码收到 4 字节后无条件设 `mqtt_ready=1`，无论 broker 是否拒绝连接。

**修复**：检查 `drain[3] == 0x00`；非零则打印错误原因并 `return` 不设 ready。

---

### 5. W25Q128 重启擦除扇区 — 缓存数据不可逆销毁

**文件**：`flash/w25q128.c` 第 25 行、第 215-219 行
**严重度**：🔴 致命
**负责人**：A

`write_slot` 为零初始化的静态变量，每次上电后为 0。首次写调用时 `addr=0`，`0 % 4096 == 0` 触发 `sector_erase(0)`，将整个 4096 字节扇区（含上次运行期间所有缓存数据）不可逆销毁。

**修复**：
1. 启动时扫描 128 个 slot，通过 magic `0xA5C35A3C` 找到最后写入位置
2. 仅在所有 slot 用完且数据已上传后才擦除

---

### 6. W25Q128 读取/上传路径完全缺失

**文件**：`flash/w25q128.c` 第 215-219 行、第 241-244 行
**严重度**：🔴 致命
**负责人**：A

`W25Q128_Task()` 是空壳——只打印一行日志即返回。**不存在任何读取缓存数据并通过 MQTT 上传的逻辑**。当 `write_slot` 从 127 回绕到 0 时，整个扇区被擦除，128 条记录无任何保护地全部销毁。

**修复**：
1. 提供 `W25Q128_ReadTelemetry(slot_index, ...)` 公开接口
2. 联网后遍历有效 slot、上传、标记、再擦除

---

### 7. printf 多任务无互斥 — UART3 输出乱码

**文件**：全项目 printf 调用点
**严重度**：🔴 致命
**负责人**：全员

`fputc` 重定向到 `HAL_UART_Transmit(&huart3, ...)`（轮询方式）。6 个任务 + 2 个错误 hook 均可能同时调用 `printf()`。两个高优先级任务（StateMachine、4G_MQTT）同优先级，FreeRTOS 时间片轮转可在 `HAL_UART_Transmit` 执行中途将其抢占，导致另一个任务的字节插入，**输出乱码、数据损坏**。

**修复**：创建全局互斥锁，在 `fputc` 内部 `osMutexAcquire`/`osMutexRelease`。

---

## 🟡 警告（12 个）

| # | 文件 | 行号 | 负责人 | 问题 |
|:---:|------|:---:|:---:|------|
| 8 | `sht31.c` `pn532.c` `power.c` `mqtt.c` | 54/32/28/223 | A/B/C | **HAL_Delay 忙等待** — TIM6 时基下不释放 CPU，共 5 处 |
| 9 | `sht31.c` + `pn532.c` | 整体 | A | **I2C1 无互斥** — SHT31 和 PN532 共享总线，无锁。当前在同一 task 顺序调用暂安全，但结构脆弱 |
| 10 | `pn532.c` | 104,119 | A | **缓冲区越界** — `frame[32]`，`LEN` 可取值 0-255，未检查 `>24` |
| 11 | `pn532.c` | 88-124 | A | **未验证 DCS 校验和** — 数据损坏静默漏检 |
| 12 | `w25q128.c` | 62-73 | A | **Write Enable 未验证 WEL** — 发送 0x06 后不读状态寄存器确认，写操作可能静默失败 |
| 13 | `gps.c` | 97,120 | B | **空坐标字段未校验** — `atof("") = 0.0`，货物错误显示在 (0,0) 大西洋 |
| 14 | `gps.c` | 89,112 | B | **strtok 非线程安全** — 应替换为 `strtok_r()` |
| 15 | `mqtt.c` | 341-375 | B | **命令误触发** — `MQTT_BufferContains` 搜索 "HOLD" 等 ASCII 可能在二进制数据中偶然匹配 |
| 16 | `mqtt.c` | 89,597 | B | **高优先级任务栈大型数组** — `response[512]` + `rx[512]` 栈接近极限 |
| 17 | `air780e.c` | 18-79 | B | **AT 命令无重试** — 模块冷启动时前几条 AT 可能无响应 |
| 18 | `gps.c` | 71-83 | B | **double→float 精度损失** — NMEA 9 位有效数字被截断为 float 7 位 |
| 19 | `air780e.c` `mqtt.c` | 整体 | B | **模块掉线无恢复** — `network_ready`/`mqtt_ready` 一旦为 0 永久离线 |

### 详细说明（关键项）

#### 8. HAL_Delay 忙等待（5 处）

| 文件 | 行号 | 阻塞时长 | 影响 |
|------|:---:|:---:|------|
| `sht31.c` | 54 | 20ms | Task_I2C_Sensors 阻塞 |
| `pn532.c` | 32 | 5ms×20次=100ms | Task_I2C_Sensors 阻塞 |
| `power.c` | 28,31,39 | 1500+5000=6500ms | Task_StateMachine 阻塞 6.5s！ |
| `mqtt.c` | 223 | 50ms | osPriorityHigh 阻塞 |

TIM6 时基下 `HAL_Delay` 是纯忙等待，不调用 `portYIELD()`。**特别是 power.c 中 PowerOn 的 6.5 秒阻塞，期间状态机无法处理 NFC 事件。**

修复：全部替换为 `osDelay()` 或 `vTaskDelay()`。

#### 15. MQTT 命令误触发

`MQTT_BufferContains` 在原始 PUBLISH 载荷中暴力搜索 ASCII 字符串 "HOLD"、"RETURN"、"CONTINUE"。这些序列可能偶然出现在二进制数据中（温度 ASCII 表示、base64 片段等），导致误触发云指令。

修复：使用专用单字节命令码（0x01=HOLD, 0x02=RETURN, 0x03=CONTINUE）或 JSON 解析。

---

## 🟢 建议（10 个）

| # | 文件 | 行号 | 负责人 | 问题 |
|:---:|------|:---:|:---:|------|
| 20 | `app.c` | 36-55 | C | `wait_for_sensor_packet()` 死代码，从未被调用 |
| 21 | `state_machine.h` | 10 | C | `STATE_WAIT_CLOUD` 枚举定义但从未被使用 |
| 22 | `app.c` | 101-106 | C | ISR 队列满时静默丢弃 NFC 事件（返回值被 `(void)` 忽略） |
| 23 | `alarm.h` | 7-8 | C | 只有高温/高湿报警，无低温/低湿阈值（冷链场景需要） |
| 24 | `power.c` | 24-31 | C | PowerOn 无 `#if` 保护，stub 模式仍执行 HAL_Delay 6.5s |
| 25 | `power.c` | 34-42 | C | PWRKEY 硬关机应先尝试 `AT+CPOWD=1` 软关机 |
| 26 | `freertos.c` | 88-93 | C | Task_Alarm 栈 128 words 偏小，调 printf 可能溢出 |
| 27 | `mqtt.c` | 550-556 | B | SUBSCRIBE 包硬编码 topic 字符串和长度，与 `MQTT_TOPIC_CMD` 宏不同步 |
| 28 | `w25q128.c` | 143-169 | A | `page_program()` 未检查页边界跨越（当前调用安全，未来可能出错） |
| 29 | `sht31.c` | 107 | A | 温度读取无条件触发 I2C 重读，与湿度缓存策略不一致 |

---

## 已验证安全（免检项）

| 疑点 | 结论 |
|------|------|
| `osMessageQueuePut` 在 ISR 中是否安全？ | ✅ CMSIS-RTOS2 允许 timeout=0 时从 ISR 调用 |
| `osKernelGetTickCount()` 溢出？ | ✅ 使用无符号减法的差值模式，正确绕回 |
| `TelemetryData telemetry = {0}` 浮点零初始化？ | ✅ IEEE 754 全零字节 = 0.0f |
| `env_packet`/`gps_packet` static 变量并发写入？ | ✅ 请求-响应模式，同一时段只有一个 task 写入 |
| `W25Q128_WriteTelemetry` 在 MQTT 失败时写 Flash 逻辑反了？ | ✅ 正确 — `MQTT_PublishTelemetry` 返回 0 表示失败，此时写入 Flash |
| `MQTT_PollCommand` 在 Air780E 断电时调用？ | ✅ 内部 `Air780E_IsNetworkReady()` 检查，关电时立即返回 NONE |
| `goto` 跨块跳转？ | ✅ 只跳出内层循环，不跳入块内 |

---

## 优先修复建议

| 优先级 | 编号 | 问题 | 负责人 | 预计工时 |
|:---:|:---:|------|:---:|:---:|
| **P0** | #1, #2 | USART1 互斥锁 + DMA/中断接收 | B | 3-4h |
| **P0** | #3 | TCP_SendBinary 验证 SEND OK | B | 30min |
| **P0** | #4 | CONNACK 返回码检查 | B | 15min |
| **P1** | #5, #6 | W25Q128 重启不擦除 + 补上传路径 | A | 3-4h |
| **P1** | #7 | printf 加互斥锁 | C | 15min |
| **P1** | #8 | HAL_Delay → osDelay 全局替换 | A/B/C | 30min |
| **P2** | #10, #11, #12 | PN532 DCS + 越界 + WEL 校验 | A | 1h |
| **P2** | #13, #14, #18 | GPS 空字段/strtok_r/精度 | B | 1h |
| **P2** | #15 | MQTT 命令误触发 | B | 1h |
| **P3** | #17, #19 | AT 重试 + 掉线恢复 | B | 2h |
| **P3** | #20-#29 | 死代码/一致性/栈/THRE 等 | A/B/C | 2h |

---

## 审查方法

3 个子 agent 并行审计，每个阅读完整源码并逐文件检查：
- HAL 外设用法正确性
- 缓冲区/内存安全
- FreeRTOS 任务安全（栈、优先级、竞态）
- 协议正确性（MQTT 帧、NMEA 解析、AT 指令）
- 条件编译路径完整性
- 中断安全性

**审查结论**：系统整体架构合理，FreeRTOS 配置正确。最严重的问题集中在 USART1 并发访问和 W25Q128 缓存路径缺失。printf 互斥和 HAL_Delay 是低成本的快速修复。建议在答辩前至少完成 P0 和 P1 修复。
