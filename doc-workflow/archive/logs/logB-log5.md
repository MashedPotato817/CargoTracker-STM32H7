# B组通信定位开发日志 5

> 执行时间：2026-07-09  
> 负责范围：GPS / USART2 NMEA 接收诊断  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

## 本次目标

用户现场反馈：GPS 模块硬件侧能收到卫星信号，但固件串口检测结果仍显示没有 GPS 信号。

本轮目标：

1. 复查 GPS USART2 初始化、NMEA 读取和定位有效性判断。
2. 区分“USART2 根本没有收到 NMEA”和“收到 NMEA 但暂时没有有效 fix”。
3. 兼容 GPS 模块可能输出 9600 或 115200 baud 的现场差异。

## 代码改动

文件：

- `test1/Core/Src/gps/gps.c`

改动：

- 新增 `GPS_SetBaudRate()`，在 `GPS_Init()` 和采样过程中统一切换 USART2 波特率。
- `GPS_Init()` 默认先切到 9600，并打印当前支持 9600/115200 自动探测。
- `GPS_GetLocation()` 从原来的只读 3 条 NMEA，改为每个波特率最多扫描 16 条 NMEA。
- 采样时优先沿用上次有效波特率，避免每次都从 9600 重新试。
- 新增现场诊断日志：
  - `[GPS] USART2 baud=...`
  - `[GPS] NMEA no fix: ...`
  - `[GPS] NMEA received but no valid fix, baud=...`
  - `[GPS] no NMEA on USART2, check PD6 RX wiring and GPS baud`
  - `[GPS] fix OK baud=...`

## 结论

原逻辑容易把以下两种情况混为“无 GPS”：

1. USART2 已收到 NMEA，但当前 RMC/GGA 仍未给出有效定位。
2. GPS 模块实际波特率不是 9600，导致 USART2 读不到完整 NMEA。

本轮改动后，串口日志可以直接判断是哪一种：

- 若出现 `NMEA no fix` 或 `NMEA received but no valid fix`：说明 MCU 串口已经收到 GPS 数据，问题在卫星 fix / 天线 / 室内环境 / NMEA 有效性。
- 若出现 `no NMEA on USART2`：说明 MCU 侧 USART2 未收到可解析 NMEA，优先检查 GPS TX 到 STM32 PD6 RX、共地、模块波特率和 USART2 引脚复用。
- 若出现 `fix OK baud=...`：说明串口和解析都正常，日志中的 baud 就是当前有效波特率。

## 验证结果

已执行：

```powershell
gcc -fsyntax-only -std=c99 -DSTM32H7A3xxQ -DUSE_HAL_DRIVER `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Core\Inc' `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Core\Inc\gps' `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Drivers\STM32H7xx_HAL_Driver\Inc' `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Drivers\CMSIS\Device\ST\STM32H7xx\Include' `
  -I'E:\Claude\CargoTracker-STM32H7\test1\Drivers\CMSIS\Core\Include' `
  'E:\Claude\CargoTracker-STM32H7\test1\Core\Src\gps\gps.c'
```

结果：

- `gps.c` 语法检查通过。
- 仅有 CMSIS 在本机 x86 `gcc -fsyntax-only` 环境下的指针宽度 warning，不是 GPS 代码新增错误。
- `git diff --check` 通过，仅提示工作区中文件未来会被 Git 转换 LF/CRLF。

## Keil 构建状态

本轮未执行 UV4 全量构建，因为当前工作区仍缺少：

```text
test1/test1.ioc
test1/MDK-ARM/test1.uvprojx
```

恢复 CubeMX/Keil 工程文件后，应执行：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logB5.txt'
```

## 现场验证步骤

1. 烧录新固件。
2. 打开串口日志，观察 `GPS` 前缀。
3. 如果看到 `USART2 baud=115200` 后出现 NMEA 或 fix，说明模块当前输出是 115200。
4. 如果只看到 `no NMEA on USART2`，优先查线：
   - GPS TX -> STM32 PD6 / USART2_RX
   - GPS RX -> STM32 PD5 / USART2_TX（若模块需要配置）
   - GPS GND 与 STM32 GND 共地
   - 模块供电电压稳定
5. 如果能看到 `NMEA no fix`，说明串口已通，继续把天线放到室外或窗边等待有效定位。
