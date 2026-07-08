# B组通信定位开发日志 4

> 执行时间：2026-07-09  
> 负责范围：Air780E / MQTT 云端指令接收  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

## 本次目标

1. 完成 `doc-workflow/task3.3.md`：真 MQTT 云指令接收。
2. 在 MQTT CONNECT 成功后订阅 `cargo/cmd`。
3. 将 `MQTT_PollCommand()` 从 stub 轮转伪造改为读取真实 UART/TCP/MQTT 数据。
4. 做代码侧语法检查和 CubeMX/Keil 易漂移项审计。

## 代码改动

文件：
- `test1/Core/Src/air780e/mqtt.c`

改动：
- 新增 `TCP_SendATCapture()`，保留 AT 命令返回内容，供 `AT+CIPRXGET` 路径解析。
- `MQTT_Init()` 在 CONNACK drain 后设置 `mqtt_ready=1`，随后发送 MQTT SUBSCRIBE 帧订阅 `cargo/cmd`；订阅发送失败时回退 `mqtt_ready=0`。
- 新增 MQTT PUBLISH 解析逻辑：按固定头、Remaining Length、Topic、Payload 提取云端 payload。
- `MQTT_PollCommand()` 先读取 UART 已到达数据，再用 `AT+CIPRXGET=2,256` 拉取 TCP 缓冲数据。
- 删除真实 UART 模式下的 stub 轮转 fallback，避免设备在没有真实云端消息时伪造 HOLD / RETURN / CONTINUE。
- 保留对 raw JSON / 十六进制文本的简化搜索，兼容 Air780E 不同返回格式。

## 未跨范围修改

- 未修改 `.ioc`。
- 未修改 `Drivers/` / `Middlewares/`。
- 未修改 app 状态机和队列结构。
- 未改 Air780E AT 初始化序列。

## 验证结果

已完成：

```powershell
gcc -fsyntax-only -std=c99 -Wall -Wextra -DSTM32H7A3xxQ -DUSE_HAL_DRIVER `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Core\Inc' `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Drivers\STM32H7xx_HAL_Driver\Inc' `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Drivers\CMSIS\Device\ST\STM32H7xx\Include' `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Drivers\CMSIS\Include' `
  'E:\Claude\CargoTracker-STM32H7\test1\Core\Src\air780e\mqtt.c'
```

结果：
- `mqtt.c` 语法检查通过。
- 仅有 CMSIS 在 MinGW/x86 语法检查环境下的指针宽度 warning，未出现 `mqtt.c` 本文件 warning。

同时确认：
- `FreeRTOSConfig.h`：`configENABLE_FPU=1`，`configENABLE_MPU=0`。
- `main.c`：已调用 `MX_I2C1_Init()`、`MX_SPI1_Init()`、`MX_USART1_UART_Init()`、`MX_USART2_UART_Init()`。
- `gpio.c`：PB0 `AIR780E_PWRKEY` 默认 `GPIO_PIN_SET` 释放，未与 LED 低电平混写。

## Keil 构建状态

本轮未能执行 UV4 完整构建，因为当前工作区缺少：

```text
test1/test1.ioc
test1/MDK-ARM/test1.uvprojx
```

已确认：
- `E:\App\Keil_v5\UV4\UV4.exe` 存在。
- `test1/` 下没有 `test1.ioc`。
- `test1/MDK-ARM/` 下存在 `test1.uvoptx` 和启动文件，但没有 `test1.uvprojx`。

后续需要先从本地或团队工程恢复 `test1.ioc` / `test1.uvprojx`，或用正确 CubeMX 工程重新 Generate Code，再执行：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logB4.txt'
```

## 硬件验证步骤

1. CubeMX Generate Code 恢复 `test1.uvprojx` 后先 Keil Rebuild，目标 0 Error / 0 Warning。
2. 烧录当前固件。
3. 打开串口，确认出现：
   - `[MQTT] subscribed cargo/cmd`
4. 打开 `dashboard.html`，点击「暂留」。
5. 串口期望：
   - `[MQTT] cloud cmd received: {"cmd":"HOLD"}` 或 `[MQTT] cloud cmd received (raw)`
   - `[StateMachine] cloud action: HOLD`

## 注意事项

1. 如果串口没有收到云指令，先确认 Air780E 仍保持 TCP MQTT 连接，并检查 broker/topic 是否仍为 `broker.emqx.io:1883` / `cargo/cmd`。
2. 如果 `AT+CIPRXGET=2,256` 不返回数据，观察模块是否以 `+IPD` 形式主动吐出 TCP 数据；当前代码已先读 UART 原始数据再走 `CIPRXGET`。
3. 如果 CubeMX 重新生成，按 `TROUBLESHOOTING.md` 重新检查 FreeRTOS port、FPU 和 PB0 PWRKEY 初值。
