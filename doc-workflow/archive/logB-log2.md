# B组通信定位开发日志 2

> 执行时间：2026-07-07  
> 负责范围：GPS / Air780E / MQTT 通信定位模块  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

## 本次目标

1. 根据 `doc-workflow/TROUBLESHOOTING.md` 继续修正工程和通信定位代码。
2. 启用 FreeRTOS FPU 配置，避免 GPS 坐标解析等 float 运算走软件模拟。
3. 在不硬改 CubeMX 生成区的前提下，为 GPS / Air780E / MQTT 预留真实 UART 读写路径。
4. 遵守 PB0 冲突规则：B 组不单独占用 PB0，Air780E PWRKEY 等待 C 组统一处理。
5. 用 Keil 命令行构建验证 0 Error / 0 Warning。

## 代码改动

### FreeRTOS FPU

文件：
- `test1/Core/Inc/FreeRTOSConfig.h`

改动：
- 按 `TROUBLESHOOTING.md` 第 9 条，将 `configENABLE_FPU` 设为 `1`。
- 保持 `configENABLE_MPU` 为 `0`。

### GPS

文件：
- `test1/Core/Inc/gps/gps.h`
- `test1/Core/Src/gps/gps.c`

改动：
- 新增 `GPS_USE_HAL_UART` 条件编译宏，默认 `0`，当前仍走 stub NMEA。
- 当后续 CubeMX 生成 `huart2` 后，可将 `GPS_USE_HAL_UART` 改为 `1`，通过 USART2 逐字节读取 NMEA 行。
- 保留现有 RMC/GGA 解析框架和最近一次有效定位 fallback。

### Air780E

文件：
- `test1/Core/Inc/air780e/air780e.h`
- `test1/Core/Src/air780e/air780e.c`

改动：
- 新增 `AIR780E_USE_HAL_UART` 条件编译宏，默认 `0`，当前仍走 stub AT 响应。
- 当后续 CubeMX 生成 `huart1` 后，可将 `AIR780E_USE_HAL_UART` 改为 `1`，通过 USART1 发送 AT 命令并等待响应。
- `AT+CREG?` 解析同时接受本地注册 `+CREG: 0,1` 和漫游注册 `+CREG: 0,5`。
- 日志中明确 PB0 PWRKEY 冲突按 `TROUBLESHOOTING.md` 由 C 组统一处理。

### MQTT

文件：
- `test1/Core/Src/air780e/mqtt.c`

改动：
- 复用 `AIR780E_USE_HAL_UART` 宏，为 MQTT AT 命令预留 USART1 发送/接收路径。
- 默认 stub 模式不依赖 `huart1`，保证当前 CubeMX 尚未使能 USART1 时仍可构建。

### Keil 工程选项

文件：
- `test1/MDK-ARM/test1.uvoptx`

改动：
- 删除残留的 `portable/RVDS/ARM_CM4F/port.c` 文件条目。
- 保留 `.uvprojx` 中已有的 `portable/GCC/ARM_CM7/r0p1/port.c`，匹配 ARM Compiler 6。

## 未跨范围修改

- 未修改 app 层。
- 未修改 `.ioc`。
- 未修改 `Drivers/` 官方 HAL/CMSIS。
- 未单独处理 PB0 / Air780E PWRKEY。

## 踩坑记录同步

已更新：
- `doc-workflow/TROUBLESHOOTING.md`

新增第 11 条：
- Keil `uvoptx` 残留 RVDS `port.c` 时，即使 `.uvprojx` 已改为 GCC port，命令行构建仍可能额外编译 RVDS port。

## Keil 构建验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logB2.txt'
```

首次普通权限构建仍遇到既有 `.o` 覆盖 `permission denied`，按权限要求提权重跑同一命令后通过。

结果：

```text
Program Size: Code=34984 RO-data=2188 RW-data=48 ZI-data=20296
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:03
```

完整日志：
- `doc-workflow/keil_build_logB2.txt`

## 注意事项

1. `GPS_USE_HAL_UART` 和 `AIR780E_USE_HAL_UART` 当前默认均为 `0`，因为 CubeMX 尚未生成 `huart2` / `huart1`。
2. 后续 Task 2.1 完成 USART2 / USART1 使能后，再分别切到 `1` 做真实硬件验证。
3. Air780E PWRKEY 仍不能由 B 组直接使用 PB0，必须等待 C 组完成 PB0 冲突处理。
