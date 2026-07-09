# C组应用集成开发日志 2

> 日期：2026-07-07  
> 范围：状态机 / 报警指示 / 低功耗与 Air780E PWRKEY 协调  
> 依据：`doc-workflow/TROUBLESHOOTING.md` 第 7、9、10 条

## 本次目标

1. 按排障记录启用 FreeRTOS 硬件 FPU。
2. 按 PB0 冲突处理方案，将系统心跳从 PB0 迁移到 PE1。
3. 将 PB0 作为 Air780E PWRKEY 由 C 组电源流程统一封装。
4. 保持 A/B 组驱动对外接口不变，不修改 A/B 驱动实现。
5. Keil 命令行构建验证 0 Error / 0 Warning。

## 代码改动

### FreeRTOS FPU

文件：
- `test1/Core/Inc/FreeRTOSConfig.h`

改动：
- `configENABLE_FPU` 从 `0` 改为 `1`。
- `configENABLE_MPU` 保持 `0`。

目的：
- 避免 SHT31 温湿度换算、GPS 坐标转换等浮点运算全部走软件模拟。

### PB0 / Air780E PWRKEY

文件：
- `test1/Core/Inc/main.h`
- `test1/Core/Inc/app/power.h`
- `test1/Core/Src/app/power.c`
- `test1/Core/Src/app/app.c`

改动：
- 在 `main.h` 的 `USER CODE BEGIN Private defines` 区域增加兼容别名：
  - `AIR780E_PWRKEY_Pin`
  - `AIR780E_PWRKEY_GPIO_Port`
- 当前 CubeMX label 仍是 `LD1_GREEN`，别名临时映射到 PB0；后续 CubeMX 正式改 label 后不会覆盖真实 `AIR780E_PWRKEY_*`。
- 新增 `Power_Air780E_SetPwrKey(uint8_t active)`，由 C 组统一控制 PB0。
- 状态机不再直接 toggle `LD1_GREEN`。
- 上传前调用 `Power_Air780E_SetPwrKey(1)`，返回休眠前调用 `Power_Air780E_SetPwrKey(0)`。

说明：
- 本次没有手工改 `.ioc` 的 PB0 label，避免绕过 CubeMX 协作流程。
- 从应用代码角度，PB0 已不再作为心跳 LED 使用。

### PE1 心跳 / 报警指示

文件：
- `test1/Core/Src/app/alarm.c`
- `test1/Core/Src/app/app.c`

改动：
- PE1 `LD2_YELLOW` 改为 C 组统一状态灯：
  - 无报警：1000 ms 慢闪，表示系统心跳。
  - 有报警：200 ms 快闪，表示温湿度越限报警。
- `App_TaskAlarm` 调度周期从 500 ms 改为 100 ms，支持快闪模式。

## Keil 构建验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logC2.txt'
```

过程记录：
- 首次普通沙箱构建时，Keil/armclang 无法覆盖已有 `.o` 文件，出现 `permission denied`。
- 按权限要求使用同一命令提权重跑后通过。

结果：

```text
Program Size: Code=34984 RO-data=2188 RW-data=48 ZI-data=20296
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:03
```

完整构建日志：
- `doc-workflow/keil_build_logC2.txt`

## 注意事项

1. `test1/test1.ioc` 里 PB0 当前仍显示为 `LD1_GREEN`，后续 Task 2.6 真正做 CubeMX 引脚调整时，应将 PB0 label 改为 Air780E PWRKEY。
2. A/B 组不要直接操作 PB0；Air780E 上电控制应继续通过 C 组 `Power_Air780E_SetPwrKey` 封装。
3. 若后续确认 Air780E PWRKEY 为低有效或需要固定脉冲时序，应只修改 `power.c` 内部实现，不改变对外接口。
4. 本次未修改 `Drivers/` 官方 HAL/CMSIS 文件。
