# A组执行日志2 - 根据 TROUBLESHOOTING 修复 FPU/FreeRTOS port 并完善 W25Q128

> 执行时间：2026-07-07  
> 负责范围：A组底层外设（SHT31、PN532、W25Q128）  
> 参考文件：`doc-workflow/TROUBLESHOOTING.md`  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

## 本次目标

1. 根据 `TROUBLESHOOTING.md` 第 9 条，启用 Cortex-M7 硬件 FPU，避免 SHT31/GPS 等浮点换算走软件模拟。
2. 根据 `TROUBLESHOOTING.md` 第 7 条，修复 ARM Compiler 6 下 FreeRTOS RVDS port 残留导致的编译错误。
3. 继续完善 A 组 W25Q128 真实驱动准备代码，补充擦除、读回和写后校验流程。
4. 使用 Keil 命令行构建验证 0 Error / 0 Warning。

## 代码改动

### `test1/Core/Inc/FreeRTOSConfig.h`

- 将：

```c
#define configENABLE_FPU                         0
```

改为：

```c
#define configENABLE_FPU                         1
```

说明：
- 这是跨 A 组目录的最小必要修改。
- 原因来自 `TROUBLESHOOTING.md` 第 9 条：当前项目已经有 SHT31 温湿度换算等浮点计算，Cortex-M7 应启用硬件 FPU。

### `test1/MDK-ARM/test1.uvprojx`

根据 `TROUBLESHOOTING.md` 第 7 条修复 FreeRTOS port 配置：

- 移除 Include Path 末尾残留的：

```text
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F
```

- 移除工程文件列表中残留的：

```text
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c
```

- 保留并使用：

```text
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
```

### FreeRTOS GCC port 文件

本地工程缺少 `GCC/ARM_CM7/r0p1` 实体文件，但 `.uvprojx` 已引用该路径。已从本机统一版本固件包复制：

来源：

```text
C:\Users\MyDife\STM32Cube\Repository\STM32Cube_FW_H7_V1.13.0\Middlewares\Third_Party\FreeRTOS\Source\portable\GCC\ARM_CM7\r0p1\
```

目标：

```text
test1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
test1/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/portmacro.h
```

说明：
- `Drivers/` 未修改。
- `Middlewares/` 属于 CubeMX/固件生成依赖，通常不纳入 Git；本次复制是为了让本地 Keil 构建与 `.uvprojx` 引用一致。

### `test1/Core/Src/flash/w25q128.c`

继续完善 W25Q128 真实 SPI 驱动准备代码：

- 新增命令：
  - `W25Q128_CMD_SECTOR_ERASE`
  - `W25Q128_CMD_READ_DATA`
- 新增超时和扇区参数：
  - `W25Q128_ERASE_TIMEOUT_MS`
  - `W25Q128_SECTOR_SIZE`
- 新增内部函数：
  - `w25q128_sector_erase`
  - `w25q128_read_data`
- 修改 `w25q128_wait_ready`，支持普通写入和扇区擦除使用不同 timeout。
- `W25Q128_WriteTelemetry` 在真实 SPI 模式下：
  1. slot 地址位于 sector 边界时先执行 Sector Erase。
  2. Page Program 写入 telemetry record。
  3. Read Data 读回同长度数据。
  4. `memcmp` 校验写入内容。

## 未修改内容

- 未修改 SHT31/PN532 对外接口。
- 未修改 app 层逻辑。
- 未修改 PB0 / Air780E PWRKEY 配置；`TROUBLESHOOTING.md` 第 10 条明确该冲突由 C 组统一处理，A 组不单独占用 PB0。
- 未手动修改 `Drivers/` 官方 HAL/CMSIS 文件。
- 未清理 Keil 编译产物。

## Keil 构建验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logA2.txt'
```

第一次普通沙箱构建暴露出 `permission denied`，原因是 Keil/armclang 需要覆盖已有 `.o` 文件。按权限规则提权重跑同一命令后通过。

最终结果：

```text
Program Size: Code=34984 RO-data=2188 RW-data=48 ZI-data=20296
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:04
```

完整日志：

- `doc-workflow/keil_build_logA2.txt`

## 后续建议

1. CubeMX 启用 SPI1 后，将 `W25Q128_USE_HAL_SPI` 切换为 `1`，先验证 JEDEC ID，再验证写入/读回。
2. W25Q128 后续若要正式承载断网缓存，还需要增加缓存索引、掉电恢复和满区回绕策略。
3. CubeMX 重新生成后需复查 FreeRTOS port 是否又恢复 RVDS 引用；如恢复，按 `TROUBLESHOOTING.md` 第 7 条重新修正。
