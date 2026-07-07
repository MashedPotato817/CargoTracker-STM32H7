# A组执行日志3 - CubeMX 外设启用与 A组真实驱动路径编译验证

> 执行时间：2026-07-07  
> 负责范围：A组底层外设（SHT31、PN532、W25Q128）  
> 对应任务：`doc-workflow/task2.md` Task 2.1，Task 2.2/2.4 编译准备  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

## 本次目标

1. 在用户完成 CubeMX Generate Code 后，检查 I2C1、SPI1、USART1、USART2、PC8、PC9 是否已生成。
2. 修复 CubeMX 重新生成后带回的 FreeRTOS RVDS port 残留。
3. 处理 PB0 未释放时 `AIR780E_PWRKEY_*` label 不存在导致的编译阻塞。
4. 先验证 Task 2.1 外设启用后工程 0 Error / 0 Warning。
5. 再将 A 组 SHT31、PN532、W25Q128 切换到真实 HAL 路径并验证编译通过。

## CubeMX 生成结果确认

已生成文件：

- `test1/Core/Inc/i2c.h`
- `test1/Core/Src/i2c.c`
- `test1/Core/Inc/spi.h`
- `test1/Core/Src/spi.c`

`main.c` 已调用：

- `MX_I2C1_Init()`
- `MX_SPI1_Init()`
- `MX_USART1_UART_Init()`
- `MX_USART2_UART_Init()`

`main.h` 已生成 label：

- `W25Q128_CS_Pin` / `W25Q128_CS_GPIO_Port`
- `BUZZER_Pin` / `BUZZER_GPIO_Port`
- `ALARM_LED_Pin` / `ALARM_LED_GPIO_Port`

`stm32h7xx_hal_conf.h` 已启用：

- `HAL_I2C_MODULE_ENABLED`
- `HAL_SPI_MODULE_ENABLED`
- `HAL_UART_MODULE_ENABLED`

SPI1 配置确认：

- `SPI_BAUDRATEPRESCALER_16`
- `SPI_DATASIZE_8BIT`
- `SPI_POLARITY_LOW`
- `SPI_PHASE_1EDGE`
- `SPI_NSS_SOFT`

## 代码改动

### FreeRTOS port 修复

CubeMX 重新生成后，`test1/MDK-ARM/test1.uvprojx` 再次带回：

```text
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F
../Middlewares/Third_Party/FreeRTOS/Source/portable/RVDS/ARM_CM4F/port.c
```

已按 `TROUBLESHOOTING.md` 第 7 条移除，并保留：

```text
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
```

### PWRKEY 兼容修复

文件：

- `test1/Core/Src/app/power.c`

原因：

- 当前按项目规则没有改 PB0。
- CubeMX 因此没有生成 `AIR780E_PWRKEY_GPIO_Port` / `AIR780E_PWRKEY_Pin`。
- 但 `power.c` 已引用该 label，导致构建失败。

处理：

- `Power_Air780E_SetPwrKey()` 增加条件编译。
- 当 `AIR780E_PWRKEY_*` label 未生成时，该函数降级为 no-op stub。
- 日志改为提示 `Air780E PWRKEY not configured`。

说明：

- 这是为了保证 Task 2.1 构建通过的最小跨范围修复。
- 没有占用 PB0。
- PB0/PWRKEY 冲突仍按 `TROUBLESHOOTING.md` 第 10 条，由 C 组统一处理。

### A组真实驱动宏切换

文件：

- `test1/Core/Inc/sensor/sht31.h`
- `test1/Core/Inc/nfc/pn532.h`
- `test1/Core/Inc/flash/w25q128.h`

改动：

```c
#define SHT31_USE_HAL_I2C 1
#define PN532_USE_HAL_I2C 1
#define W25Q128_USE_HAL_SPI 1
```

### W25Q128 CS label 修正

文件：

- `test1/Core/Src/flash/w25q128.c`

改动：

- 片选控制从硬编码 `GPIOA` / `GPIO_PIN_4` 改为 CubeMX label：

```c
W25Q128_CS_GPIO_Port
W25Q128_CS_Pin
```

## Keil 构建验证

### 1. Task 2.1 外设启用后验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logA3.txt'
```

结果：

```text
Program Size: Code=36744 RO-data=2188 RW-data=48 ZI-data=20808
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:04
```

### 2. A组真实 HAL 驱动路径验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logA4.txt'
```

结果：

```text
Program Size: Code=42808 RO-data=2128 RW-data=56 ZI-data=20808
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:04
```

完整日志：

- `doc-workflow/keil_build_logA3.txt`
- `doc-workflow/keil_build_logA4.txt`

## 未完成/待硬件验证

1. SHT31 真实温湿度读数尚未串口观察验证。
2. PN532 真实 NTAG215 UID 尚未贴卡验证。
3. W25Q128 JEDEC ID、写入、读回、擦除尚未上板验证。
4. USART1/USART2 由 CubeMX 生成并参与构建，但 B 组真实 GPS/Air780E 串口通信验证不属于本次 A 组范围。

## 后续建议

1. 上板后先观察串口日志，确认 SHT31 不再返回 stub 值。
2. PN532 如果持续无卡或 I2C timeout，优先确认模块 I2C 地址、供电和上拉。
3. W25Q128 首先验证 JEDEC ID，确认 SPI 模式和 CS 电平正确后再做写擦测试。
4. CubeMX 后续若再次生成代码，需复查 `uvprojx` 是否又恢复 RVDS port。
