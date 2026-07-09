# B组通信定位开发日志 3

> 执行时间：2026-07-07  
> 负责范围：GPS / Air780E / MQTT 通信定位模块  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

## 本次目标

1. 在 CubeMX 已生成 USART1 / USART2 后，继续推进 B 组真实 UART 接入。
2. 将 GPS 从 stub NMEA 切换到 USART2 读取 NMEA 数据。
3. 将 Air780E / MQTT 从 stub AT 响应切换到 USART1 AT 收发路径。
4. 按 `TROUBLESHOOTING.md` 要求重新确认 FPU 配置。
5. 用 Keil 命令行构建验证 0 Error / 0 Warning。

## 生成结果确认

已确认 CubeMX 当前生成：

- `huart1` / `MX_USART1_UART_Init()`：USART1 PA9 TX、PA10 RX，115200 baud。
- `huart2` / `MX_USART2_UART_Init()`：USART2 PD5 TX、PD6 RX，但生成值为 115200 baud。
- `huart3` / `MX_USART3_UART_Init()`：调试串口保持 115200 baud。
- `main.c` 已调用 `MX_USART1_UART_Init()` 和 `MX_USART2_UART_Init()`。

注意：ATGM336H GPS 目标波特率为 9600，本轮在 `GPS_Init()` 中做运行时重初始化，避免直接修改 CubeMX 生成区。后续建议回 CubeMX 将 USART2 配置修正为 9600。

## 代码改动

### FreeRTOS FPU

文件：
- `test1/Core/Inc/FreeRTOSConfig.h`

改动：
- CubeMX 重新生成后 `configENABLE_FPU` 又回到 `0`，本轮重新改为 `1`。
- `configENABLE_MPU` 保持 `0`。

### GPS

文件：
- `test1/Core/Inc/gps/gps.h`
- `test1/Core/Src/gps/gps.c`

改动：
- `GPS_USE_HAL_UART` 从 `0` 切换为 `1`。
- GPS 真实模式改为包含 `usart.h`，直接使用 CubeMX 生成的 `huart2`。
- `GPS_Init()` 中检测 USART2 baud rate，如不是 9600，则将 `huart2.Init.BaudRate` 改为 9600 并调用 `HAL_UART_Init(&huart2)`。
- `GPS_GetLocation()` 保持逐行读取 NMEA，再走已有 RMC/GGA 解析框架。

### Air780E

文件：
- `test1/Core/Inc/air780e/air780e.h`
- `test1/Core/Src/air780e/air780e.c`

改动：
- `AIR780E_USE_HAL_UART` 从 `0` 切换为 `1`。
- Air780E 真实模式改为包含 `usart.h`，直接使用 CubeMX 生成的 `huart1`。
- 保持 AT 初始化序列：`AT`、`ATE0`、`AT+CSQ`、`AT+CREG?`、`AT+CGATT?`。
- 仍不直接控制 PB0 PWRKEY；该冲突按 `TROUBLESHOOTING.md` 由 C 组统一处理。

### MQTT

文件：
- `test1/Core/Src/air780e/mqtt.c`

改动：
- MQTT 真实模式随 `AIR780E_USE_HAL_UART=1` 一起切换。
- AT 命令发送/响应等待改用 `huart1`。
- 保留现有 telemetry payload 构造和云端命令 payload 解析框架。

## 未跨范围修改

- 未修改 `.ioc`。
- 未修改 `Drivers/` 官方 HAL/CMSIS。
- 未修改 app 层状态机。
- 未处理 PB0 / Air780E PWRKEY。

## 踩坑记录同步

已更新：
- `doc-workflow/TROUBLESHOOTING.md`

新增第 12 条：
- USART2 默认 115200 与 ATGM336H GPS 9600 不匹配。

## Keil 构建验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logB3.txt'
```

首次普通权限构建仍遇到既有 `.o` 覆盖 `permission denied`，按权限要求提权重跑同一命令后通过。

结果：

```text
Program Size: Code=43576 RO-data=1800 RW-data=56 ZI-data=20800
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:05
```

完整日志：
- `doc-workflow/keil_build_logB3.txt`

## 注意事项

1. GPS 已切换为真实 USART2 读取，但需要实际连接 ATGM336H 并在户外或窗边验证真实定位。
2. Air780E / MQTT 已切换为真实 USART1 AT 交互，但 PB0 PWRKEY 未解决前，模块可能不会上电或不会响应 AT。
3. 后续建议由 CubeMX 将 USART2 baud rate 正式改为 9600，避免运行时重初始化长期存在。
