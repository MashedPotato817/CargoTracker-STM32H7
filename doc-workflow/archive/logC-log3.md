# C组应用集成开发日志 3

> 日期：2026-07-07  
> 范围：Task 2.6 PB0 冲突正式解决  
> 依据：`doc-workflow/task2.md` Task 2.6、`doc-workflow/TROUBLESHOOTING.md` 第 10 条

## 本次目标

1. 将 PB0 从板载 LD1 心跳灯正式切换为 Air780E PWRKEY。
2. 保持 PE1 作为系统心跳/报警复用状态灯。
3. 同步 CubeMX `.ioc`、生成头文件、GPIO 初始化代码和硬件映射文档。
4. Keil 命令行构建验证 0 Error / 0 Warning。

## 代码和工程改动

### CubeMX 映射

文件：
- `test1/test1.ioc`

改动：
- `PB0.GPIO_Label` 从 `LD1_GREEN` 改为 `AIR780E_PWRKEY`。
- PB0 继续保持 `GPIO_Output`。

说明：
- 本次没有启用 USART1，本轮只解决 Task 2.6 的 PB0 PWRKEY 前置依赖。

### 生成宏与 GPIO 初始化

文件：
- `test1/Core/Inc/main.h`
- `test1/Core/Src/gpio.c`

改动：
- `main.h` 中 PB0 宏从：
  - `LD1_GREEN_Pin`
  - `LD1_GREEN_GPIO_Port`
  改为：
  - `AIR780E_PWRKEY_Pin`
  - `AIR780E_PWRKEY_GPIO_Port`
- `gpio.c` 中 PB0 初始化同步改为 `AIR780E_PWRKEY_Pin`。
- PB14 `LD3_RED` 和 PE1 `LD2_YELLOW` 保持不变。

### 应用层现状

文件：
- `test1/Core/Src/app/app.c`
- `test1/Core/Src/app/alarm.c`
- `test1/Core/Src/app/power.c`

当前逻辑：
- `Power_Air780E_SetPwrKey()` 统一控制 PB0。
- 状态机在上传前打开 PWRKEY，返回休眠前关闭 PWRKEY。
- PE1 慢闪表示心跳，快闪表示报警。
- 应用层不再直接使用 `LD1_GREEN`。

### 文档同步

文件：
- `doc-workflow/hardware_map.md`

改动：
- PB0 状态改为已配置 `AIR780E_PWRKEY`。
- PE1 说明改为心跳/报警指示：慢闪=心跳，快闪=报警。
- USART1 说明更新为 PB0 PWRKEY 已释放，后续可使能 USART1。

## Keil 构建验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logC3.txt'
```

过程记录：
- 普通沙箱构建时仍遇到 Keil/armclang 覆盖已有 `.o` 文件 `permission denied`。
- 使用同一命令按权限流程提权重跑后通过。

结果：

```text
Program Size: Code=34984 RO-data=2188 RW-data=48 ZI-data=20296
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:04
```

完整构建日志：
- `doc-workflow/keil_build_logC3.txt`

## 完成结论

Task 2.6 的软件侧 PB0 冲突已解决：

- PB0 已释放给 Air780E PWRKEY。
- 心跳已迁移到 PE1。
- A/B 组后续可在 USART1/Air780E 驱动中依赖 C 组的 PWRKEY 封装，但不要直接操作 PB0。

## 后续

1. B 组可以继续 Task 2.5：USART1 + Air780E + MQTT 真实硬件验证。
2. C 组后续等 Task 2.2~2.6 都完成后，进入 Task 2.7 系统集成测试。
