# C 组执行日志 1 - 状态机集成与按键消抖

> 执行时间：2026-07-07  
> 负责范围：应用集成 C 组（状态机、FreeRTOS 任务编排、报警、低功耗、系统流程）

## 本次目标

1. 梳理当前流程：激活 -> GPS 定位 -> SHT31 采集 -> MQTT 上传 -> 等待云命令 -> 报警/缓存 -> 休眠。
2. 检查 3 条队列：`queue_activation`、`queue_sensor_data`、`queue_cloud_cmd`。
3. 修复 PC13 中断回调内 `HAL_Delay` 阻塞消抖问题。
4. 给出 PB0 冲突的 C 组统一处理方案。
5. 使用 Keil 命令行构建验证 0 Error / 0 Warning。

## 代码改动

### `test1/Core/Src/app/app.c`

- 状态机仍保持完整主流程：
  `SLEEP -> NFC_ACTIVE -> GPS_LOCATE -> ENV_SAMPLE -> UPLOAD -> WAIT_CLOUD -> RETURN_SLEEP -> SLEEP`
- 将 `queue_sensor_data` 的使用从“GPS/SHT31 周期性无条件投递”调整为“状态机按阶段请求一次采样，任务返回本轮数据”：
  - `STATE_GPS_LOCATE` 阶段设置 GPS 采样请求。
  - `STATE_ENV_SAMPLE` 阶段设置 SHT31 采样请求。
  - GPS task / I2C task 只有在收到请求标志后才向 `queue_sensor_data` 投递数据。
- `queue_activation` 继续作为 NFC/按键激活事件入口。
- `queue_cloud_cmd` 继续作为 MQTT 云命令入口。
- 云命令等待时间从 3000 ms 调整为 5000 ms，用于匹配当前 MQTT task 的 4000 ms 轮询周期。

### `test1/Core/Src/main.c`

- PC13 EXTI 回调移除中断上下文内的：
  - `printf`
  - `HAL_Delay(50)`
  - 等待按键释放的阻塞 `while`
- 改为使用 `HAL_GetTick()` 做 200 ms 非阻塞消抖节流。
- ISR 中只负责投递 `APP_ACTIVATION_BUTTON` 到应用层激活队列。
- 修改仍位于 `USER CODE BEGIN 4` / `USER CODE END 4` 区域内。

## 队列检查结论

| 队列 | 当前用途 | 结论 |
|------|----------|------|
| `queue_activation` | NFC/PC13 按键 -> 状态机 | 合理，作为系统唤醒/激活入口 |
| `queue_sensor_data` | GPS/SHT31 -> 状态机 | 已优化为状态机请求后返回本轮数据，避免旧采样包干扰流程 |
| `queue_cloud_cmd` | MQTT -> 状态机 | 合理，等待窗口已与 MQTT 轮询周期对齐 |

## PB0 冲突处理方案

当前阶段 PB0 仍保留为 `LD1_GREEN`，由 C 组状态机 task 用作系统心跳指示。

最终接入 Air780E 真实硬件前，必须由 C 组统一发起一次 CubeMX 引脚调整：

1. PB0 改为 Air780E `PWRKEY` GPIO_Output，并在 `main.h` 中生成明确 label。
2. 系统心跳 LED 从 PB0 迁移到未冲突的板载 LED 或外接 PC9 LED。
3. A/B 组驱动不得各自直接复用 PB0；Air780E 上电控制应通过 C 组电源/系统流程统一调用，避免 4G 驱动和状态机抢占同一引脚。

本次未修改 PB0 CubeMX 配置，因为当前仍是开发板 stub 验证阶段，且该修改会影响硬件映射和生成文件。

## Keil 构建验证

执行命令：

```powershell
& 'E:\App\Keil_v5\UV4\UV4.exe' -b 'E:\Claude\CargoTracker-STM32H7\test1\MDK-ARM\test1.uvprojx' -j0 -o 'E:\Claude\CargoTracker-STM32H7\doc-workflow\keil_build_logC1.txt'
```

说明：
- 第一次在 Codex 沙箱内构建时，Keil 替换已有 `.o` 文件出现 `permission denied`。
- 按权限规则申请后在沙箱外重跑成功。

构建结果：

```text
"test1\test1.axf" - 0 Error(s), 0 Warning(s).
Build Time Elapsed:  00:00:05
```

完整日志：
- `doc-workflow/keil_build_logC1.txt`

## 后续建议

1. 真实外设接入前，继续保持 A/B 驱动接口不变，C 组只调接口不改实现。
2. PB0/PWRKEY 冲突应在下一轮硬件接线确认后通过 CubeMX 一次性调整。
3. 若需要更严格的云命令闭环，可后续新增状态机到 MQTT task 的上传/等待请求信号；本轮未扩展队列数量，保持 3 queue 架构不变。
