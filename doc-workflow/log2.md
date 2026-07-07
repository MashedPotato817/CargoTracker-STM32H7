# Task 2 执行日志 — 硬件映射 + SHT31 + 应用层 stub

> 执行时间：2026-07-07  
> 对应阶段：Task 1 第 2/3 步，状态机 + 应用层框架 + 外设 stub  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

---

## 本轮目标

1. 先浏览项目框架与硬件文档，整理当前程序应匹配的硬件信息。
2. 将温湿度传感器从 SHT30 统一改为 SHT31。
3. 在现有 FreeRTOS 6 task + 3 queue 骨架上接入应用层和外设 stub。
4. 用 Keil 命令行验证工程可以编译通过。

---

## 硬件信息整理

新增硬件映射文档：

- `doc-workflow/hardware_map.md`

当前程序匹配的主要硬件映射：

| 模块 | 型号/用途 | 程序接口 |
|------|-----------|----------|
| 温湿度 | SHT31，I2C1 PB8/PB9 | `SHT31_Init`, `SHT31_ReadTemperature`, `SHT31_ReadHumidity` |
| NFC | PN532 + NTAG215，I2C1 PB8/PB9 | `PN532_Init`, `PN532_ReadCard` |
| GPS | ATGM336H，USART2 PD5/PD6 | `GPS_Init`, `GPS_GetLocation` |
| 4G | Air780E，USART1 PA9/PA10 | `Air780E_Init`, `MQTT_PublishTelemetry` |
| Flash | W25Q128，SPI1 PA4-PA7 | `W25Q128_Init`, `W25Q128_WriteTelemetry` |
| 调试串口 | USART3 PD8/PD9 | `printf` |
| 开发板按键 | PC13 EXTI | 模拟 NFC/按键激活 |
| 板载 LED | PB0/PE1/PB14 | 心跳/报警/上传指示 |

注意：最终硬件规划中 PB0 是 Air780E PWRKEY，但当前 CubeMX 阶段 PB0 已作为 LD1_GREEN 心跳 LED。接入真实 4G 模块前需要重新确认该冲突。

---

## 代码改动

### 新增应用层

新增头文件：

- `test1/Core/Inc/app/app_types.h`
- `test1/Core/Inc/app/app.h`
- `test1/Core/Inc/app/state_machine.h`
- `test1/Core/Inc/app/alarm.h`
- `test1/Core/Inc/app/power.h`

新增源文件：

- `test1/Core/Src/app/app.c`
- `test1/Core/Src/app/state_machine.c`
- `test1/Core/Src/app/alarm.c`
- `test1/Core/Src/app/power.c`

实现内容：

- 状态机：`SLEEP -> NFC_ACTIVE -> GPS_LOCATE -> ENV_SAMPLE -> UPLOAD -> WAIT_CLOUD -> RETURN_SLEEP`
- 报警逻辑：SHT31 温湿度阈值判断，LD2 黄灯指示
- 低功耗：先保留 Stop Mode stub，不直接进入真实 Stop Mode
- 应用调度：统一封装 6 个 FreeRTOS task 的业务入口

### 新增驱动 stub

新增头文件：

- `test1/Core/Inc/sensor/sht31.h`
- `test1/Core/Inc/nfc/pn532.h`
- `test1/Core/Inc/gps/gps.h`
- `test1/Core/Inc/air780e/air780e.h`
- `test1/Core/Inc/air780e/mqtt.h`
- `test1/Core/Inc/flash/w25q128.h`

新增源文件：

- `test1/Core/Src/sensor/sht31.c`
- `test1/Core/Src/nfc/pn532.c`
- `test1/Core/Src/gps/gps.c`
- `test1/Core/Src/air780e/air780e.c`
- `test1/Core/Src/air780e/mqtt.c`
- `test1/Core/Src/flash/w25q128.c`

Stub 返回值：

- SHT31：25.5 C，60%
- PN532：周期性模拟 `MOCK-UID-001`
- GPS：31.2304，121.4737
- Air780E：网络默认 ready
- MQTT：发布成功，周期性模拟 `CONTINUE` 云命令
- W25Q128：缓存写入成功

### 接入 FreeRTOS

修改 `test1/Core/Src/freertos.c`：

- `StartTask_StateMachine` -> `App_TaskStateMachine`
- `StartTask_4G_MQTT` -> `App_Task4GMQTT`
- `StartTask_I2C_Sensors` -> `App_TaskI2CSensors`
- `StartTask_GPS` -> `App_TaskGPS`
- `StartTask_Flash` -> `App_TaskFlash`
- `StartTask_Alarm` -> `App_TaskAlarm`

修改 `test1/Core/Src/main.c`：

- PC13 EXTI 回调改为调用 `App_SendActivationFromISR(APP_ACTIVATION_BUTTON)`
- 保留原有简单消抖代码，未在本轮重构

修改 `test1/MDK-ARM/test1.uvprojx`：

- 新增 Keil 分组 `Application/User/App`
- 加入 10 个新增 `.c` 文件

---

## 文档同步

以下文件已将 SHT30 / `sht30.c` / `SHT30_*` 同步为 SHT31 / `sht31.c` / `SHT31_*`：

- `AGENTS.md`
- `CLAUDE.md`
- `README.md`
- `doc/task.md`
- `doc-workflow/task1.md`
- `doc/STM32H7A3ZI-Q_程序下载要点.md`

验证命令（排除本日志说明文字）：

```powershell
rg -n "SHT30|sht30" E:\Claude\CargoTracker-STM32H7 -g '!log2.md'
```

结果：无残留。

---

## Keil 编译验证

执行命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_log2.txt'
```

编译结果：

```text
*** Using Compiler 'V6.21', folder: 'E:\App\Keil_v5\ARM\ARMCLANG\Bin'
Build target 'test1'
Program Size: Code=28668 RO-data=1460 RW-data=28 ZI-data=20108
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:05
```

完整构建日志：

- `doc-workflow/keil_build_log2.txt`

---

## 当前注意事项

1. 本轮 Keil 构建刷新了 `test1/MDK-ARM/test1/` 下的编译产物，并新增了 app/sht31/pn532/gps/air780e/mqtt/w25q128 等模块的 `.o/.d` 文件。
2. 当前仓库历史里已经跟踪了一批 Keil 编译产物，因此本轮没有擅自删除或取消跟踪这些文件。
3. `main.c` 的 EXTI 回调里仍有 `HAL_Delay(50)`，这是之前的简单消抖实现；严格来说不适合放在中断回调中，后续可单独改为非阻塞消抖。
4. 真实硬件接入前，需要用 CubeMX 重新确认 I2C1、USART1、USART2、SPI1、PC8、PC9、Air780E PWRKEY 等外设引脚。

---

## 完成标准

- [x] 硬件映射已整理并写入 `hardware_map.md`
- [x] SHT30 全项目命名同步为 SHT31
- [x] 新增应用层和外设 stub
- [x] FreeRTOS 6 个 task 已接入应用任务入口
- [x] Keil 工程已加入新增源文件
- [x] Keil 命令行构建通过：0 Error, 0 Warning
