# A组底层外设驱动准备日志

> 执行时间：2026-07-07  
> 范围：SHT31 温湿度、PN532 NFC、W25Q128 Flash  
> Keil 路径：`E:\App\Keil_v5\UV4\UV4.exe`

## 本次目标

1. 阅读并完善 `sht31.c/h`、`pn532.c/h`、`w25q128.c/h`。
2. 根据 `doc-workflow/hardware_map.md` 确认连接：
   - SHT31 + PN532 共用 I2C1 PB8/PB9。
   - W25Q128 使用 SPI1 PA4 CS、PA5 SCK、PA6 MISO、PA7 MOSI。
3. 不改变对外函数名，补充真实驱动准备代码。
4. 当前 CubeMX 尚未生成 I2C1/SPI1 外设文件时，保持工程 0 Error / 0 Warning。

## 代码改动

### SHT31

文件：
- `test1/Core/Inc/sensor/sht31.h`
- `test1/Core/Src/sensor/sht31.c`

保留接口：
- `SHT31_Init`
- `SHT31_ReadTemperature`
- `SHT31_ReadHumidity`

实现内容：
- 增加 `SHT31_USE_HAL_I2C` 配置宏，默认 `0`，当前仍走 stub fallback。
- 准备真实 I2C1 访问路径：地址 `0x44`，命令 `0x2400`，读取 6 字节数据。
- 增加 SHT31 CRC8 校验和温湿度换算公式。
- 保存最近一次有效温湿度，读取失败时不破坏上次数据。

### PN532

文件：
- `test1/Core/Inc/nfc/pn532.h`
- `test1/Core/Src/nfc/pn532.c`

保留接口：
- `PN532_Init`
- `PN532_ReadCard`

实现内容：
- 增加 `PN532_USE_HAL_I2C` 配置宏，默认 `0`，当前仍走 stub fallback。
- 准备 PN532 I2C 帧发送/响应读取代码。
- 准备 `SAMConfiguration` 初始化和 `InListPassiveTarget` 寻卡流程。
- 真实模式下将 UID 格式化为十六进制字符串输出。

### W25Q128

文件：
- `test1/Core/Inc/flash/w25q128.h`
- `test1/Core/Src/flash/w25q128.c`

保留接口：
- `W25Q128_Init`
- `W25Q128_WriteTelemetry`
- `W25Q128_Task`

实现内容：
- 增加 `W25Q128_USE_HAL_SPI` 配置宏，默认 `0`，当前仍走 stub fallback。
- 准备 JEDEC ID 读取、Write Enable、Busy 状态等待、Page Program 写入流程。
- 将 `TelemetryData` 打包为固定记录写入环形 slot。
- 真实模式下使用 SPI1 和 PA4 作为片选。

## CubeMX 后续需要配置

当前工程没有 `Core/Inc/i2c.h`、`Core/Src/i2c.c`、`Core/Inc/spi.h`、`Core/Src/spi.c`，也没有 `hi2c1` / `hspi1` 句柄。因此本次没有硬改 CubeMX 生成区。

接真实硬件前需用 CubeMX 增加：

| 模块 | CubeMX 配置 |
|------|-------------|
| SHT31 / PN532 | I2C1，PB8=SCL，PB9=SDA，400 kHz |
| W25Q128 | SPI1，PA5=SCK，PA6=MISO，PA7=MOSI，建议 10~20 MHz |
| W25Q128 CS | PA4 GPIO_Output，默认高电平 |

CubeMX 生成对应外设文件后，再将模块宏打开：

```c
#define SHT31_USE_HAL_I2C 1
#define PN532_USE_HAL_I2C 1
#define W25Q128_USE_HAL_SPI 1
```

## Keil 构建验证

执行命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_log_A_tmp.txt'
```

结果：

```text
Program Size: Code=34584 RO-data=2196 RW-data=48 ZI-data=20128
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:03
```

## 注意事项

1. 本次仅修改 A 组负责目录，未修改 app 层、CubeMX 生成区和 Drivers 官方库。
2. 首次在沙箱内构建时，Keil 覆盖已有 `.o` 文件被拦截，随后经授权以同一命令完成构建验证。
3. W25Q128 真实写入路径已准备 Page Program；后续做断网缓存正式落地时，还应补充 Sector Erase 和缓存索引管理策略。
