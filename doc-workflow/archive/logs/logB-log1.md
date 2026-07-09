# B组通信定位开发日志 1

> 日期：2026-07-07
> 范围：GPS / Air780E / MQTT 通信定位模块

## 本次目标

1. 阅读并完善 `gps.c/h`、`air780e.c/h`、`mqtt.c/h` 现有 stub。
2. 保持对外接口不变：
   - `GPS_Init`
   - `GPS_GetLocation`
   - `Air780E_Init`
   - `Air780E_IsNetworkReady`
   - `MQTT_Init`
   - `MQTT_PublishTelemetry`
   - `MQTT_PollCommand`
3. 按 `hardware_map.md` 确认 GPS 使用 USART2 PD5/PD6，Air780E 使用 USART1 PA9/PA10。
4. 先落地清晰的 NMEA / AT / MQTT 命令解析框架，不硬改 CubeMX 生成区。

## 硬件配置确认

- GPS：ATGM336H，计划连接 USART2 PD5/PD6，9600 baud。
- 4G：Air780E，计划连接 USART1 PA9/PA10，115200 baud。
- Air780E PWRKEY：文档规划为 PB0，但当前 CubeMX 中 PB0 已作为 `LD1_GREEN` 心跳 LED。
- 当前 `usart.c/h` 只有 USART3 调试串口，没有生成 `huart1` / `huart2`，因此本轮未直接接 HAL UART。

后续接真实硬件前需要在 CubeMX 中补充：

- USART1 async：PA9 TX、PA10 RX、115200、8N1，建议 RX DMA circular。
- USART2 async：PD5 TX、PD6 RX、9600、8N1，可选 RX DMA。
- 重新确认 PB0 是继续做心跳 LED，还是切换为 Air780E PWRKEY。

## 代码改动

### GPS

文件：
- `test1/Core/Src/gps/gps.c`

改动：
- 增加 RMC/GGA NMEA 行解析框架。
- 增加经纬度 `ddmm.mmmm` 到十进制度转换。
- 当前通过内部 stub NMEA 句子模拟 ATGM336H 输出。
- 保留最近一次有效定位，解析失败时返回上次有效值。

### Air780E

文件：
- `test1/Core/Src/air780e/air780e.c`

改动：
- 增加内部 `Air780E_SendAT` 框架。
- 增加 `AT`、`ATE0`、`AT+CSQ`、`AT+CREG?`、`AT+CGATT?` 初始化流程。
- 增加 CSQ 信号质量解析。
- 网络 ready 由 AT、回显关闭、注册状态、附着状态共同决定。
- 日志中明确提示 USART1 尚未配置，以及 PB0 PWRKEY 与 LD1_GREEN 的当前冲突。

### MQTT

文件：
- `test1/Core/Src/air780e/mqtt.c`

改动：
- 增加内部 MQTT AT 命令发送框架。
- `MQTT_Init` 使用 `AT+CMQTTSTART` 初始化模拟 MQTT 会话。
- `MQTT_PublishTelemetry` 构造遥测 JSON payload，再走 `AT+CMQTTPUB` 框架。
- `MQTT_PollCommand` 增加云端 payload 解析，继续映射到现有枚举：
  - `HOLD` -> `APP_CLOUD_CMD_HOLD`
  - `RETURN` -> `APP_CLOUD_CMD_RETURN`
  - `CONTINUE` -> `APP_CLOUD_CMD_CONTINUE`

## 未跨范围修改

- 未修改 app 层。
- 未修改 CubeMX 生成区。
- 未修改 `Drivers/` 官方 HAL/CMSIS。
- 未修改 Keil 工程文件。

## Keil 构建验证

命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logB1.txt'
```

首次在普通沙箱权限下构建时，Keil/armclang 无法覆盖既有 `.o` 文件，日志报 `permission denied`。按权限要求提权重跑同一命令后通过。

结果：

```text
Program Size: Code=34584 RO-data=2196 RW-data=48 ZI-data=20128
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:03
```

完整日志：
- `doc-workflow/keil_build_logB1.txt`
