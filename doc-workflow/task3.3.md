# Task 3.3: 真 MQTT 云指令接收 + 前端控制台

> 🔴 必做 | 负责人：B（后端）+ C（前端） | 依赖：Task 2.5 已完成

## 当前状态

- ✅ Air780E AT/MQTT 全链路已通（Task 2.5）
- ✅ dashboard 按钮发布 `cargo/cmd` → broker 正常
- ✅ `MQTT_Init()` 在 CONNECT 成功后订阅 `cargo/cmd`（B: 3ac0d8b）
- ✅ `MQTT_PollCommand()` 已移除 stub 轮转伪造，改为读取真实 UART/TCP/MQTT 数据（B: 3ac0d8b）
- ✅ dashboard 断线自动重连（C: 2026-07-09）
- ⚠️ 端到端验证：串口日志显示 CONNECT / SUBSCRIBE 已通，待再次硬件验证

## 目标

```
dashboard 点按钮 → cargo/cmd → broker → Air780E 收到 PUBLISH → 
  MQTT_PollCommand() 返回 APP_CLOUD_CMD_HOLD → 状态机 WAIT_CLOUD 执行
```

## 修复内容（代码已完成，待硬件验证）

### 1. MQTT_Init() 中订阅 cargo/cmd

在 CONNECT 成功后（`mqtt_ready = 1U` 那行之后），发送 SUBSCRIBE 帧：

```c
// 订阅 cargo/cmd（QoS 0）
static const uint8_t mqtt_sub_cmd[] = {
    0x82, 0x0E,              // SUBSCRIBE, 剩余长度 14
    0x00, 0x01,              // Packet ID = 1
    0x00, 0x09,              // Topic 长度 9
    'c', 'a', 'r', 'g', 'o', '/', 'c', 'm', 'd',
    0x00                     // QoS 0
};
TCP_SendBinary(mqtt_sub_cmd, sizeof(mqtt_sub_cmd));
printf("[MQTT] subscribed cargo/cmd\n");
```

> 发送后等服务器回 SUBACK（可忽略不处理，broker.emqx.io 必定返回成功）。

### 2. 重写 MQTT_PollCommand()

已删掉原 stub 轮转伪造逻辑。当前实现：

- 先从 USART1 读取已到达的原始字节，按 MQTT PUBLISH 固定头 / Remaining Length / Topic / Payload 解析云端 payload。
- TCP 连接后尝试 `AT+CIPRXGET=1` 开启手动接收；轮询时再用 `AT+CIPRXGET=2,256` 拉取 TCP 缓冲数据，兼容 Air780E 将数据以 raw / ASCII / 十六进制文本返回的情况。
- `CIPRXGET` fallback 限制为 5s 一次，避免不支持该命令时反复 2s timeout 抢占 USART1。
- 使用 `MQTT_ParsePublishBuffer()` / `MQTT_ParseCommandBytes()` 将 `HOLD`、`RETURN`、`CONTINUE` 映射为 `AppCloudCommand`。
- 没有真实云端数据时返回 `APP_CLOUD_CMD_NONE`，不再伪造云端指令。

### 3. 状态机等待窗口

- `STATE_WAIT_CLOUD` 等待云命令时间从 5s 调整为 30s。
- `Task_4G_MQTT` 轮询周期从 4s 调整为 1s。
- 目的：给 dashboard 按钮下发和 MQTT PUBLISH 接收留出足够窗口，避免状态机过早 `cloud action: NONE` 并关断 Air780E。

### 4. 备用简化方案

如果 USART1 逐字节解析 MQTT PUBLISH 太复杂，用 AT+CIPRXGET 读取 TCP 数据：

```c
AppCloudCommand MQTT_PollCommand(void)
{
#if AIR780E_USE_HAL_UART
    if ((Air780E_IsNetworkReady() == 0U) || (mqtt_ready == 0U)) {
        return APP_CLOUD_CMD_NONE;
    }
#endif

    /* AT+CIPRXGET 读取 TCP 缓冲数据 */
    if (TCP_SendAT("AT+CIPRXGET=2,256", "+CIPRXGET:", 1000U)) {
        char rx[512];
        Air780E_ReadResponse(rx, sizeof(rx));
        // 在 rx 中搜索 {"cmd":"...
        if (strstr(rx, "\"cmd\":\"HOLD\"") != NULL)
            return APP_CLOUD_CMD_HOLD;
        if (strstr(rx, "\"cmd\":\"RETURN\"") != NULL)
            return APP_CLOUD_CMD_RETURN;
        if (strstr(rx, "\"cmd\":\"CONTINUE\"") != NULL)
            return APP_CLOUD_CMD_CONTINUE;
    }

    return APP_CLOUD_CMD_NONE;
}
```

> 注意：`AT+CIPRXGET` 需要 Air780E 固件支持。如果不支持，用方案 2（USART 逐字节解析）。

---

## 前端控制台（C）

### 已完成

- ✅ dashboard 自动重连：`reconnectPeriod: 3s`、`connectTimeout: 10s`
- ✅ 断线 → 重连中状态显示（`重连中(1)…`）
- ✅ `error` 事件捕获 + 日志记录
- ✅ 兼容硬件 `gps_valid` 字段名（`d.gps_v !== undefined ? d.gps_v : d.gps_valid`）
- ✅ CSQ 信号强度 + 最后上报计时器（替代原电池条）

### 验证前端

```
1. 打开 dashboard.html → 状态栏显示「已连接」绿灯
2. 断开网络（飞行模式） → 状态栏显示「已断开，等待重连…」红灯
3. 恢复网络 → 状态栏显示「重连中(1)…」→ 重新「已连接」绿灯
4. 数据日志持续显示上报记录，无断连丢失
```

## 端到端验证（硬件）

```
1. 编译 → 烧录 → 打开串口
2. 打开 dashboard.html → 点「暂留」
3. 串口应打印: [MQTT] cloud cmd received: {"cmd":"HOLD"}
4. 状态机应打印: [StateMachine] cloud action: HOLD
```

### 代码侧验证

- ✅ `test1/Core/Src/air780e/mqtt.c` 已删除真实 UART 模式下的 stub 轮转 fallback
- ✅ 当前实现使用 `MQTT_ParsePublishBuffer()` / `MQTT_ParseCommandBytes()` 解析 PUBLISH payload 或 raw/CIPRXGET 返回内容
- ✅ `STATE_WAIT_CLOUD` 已改为 30s，`Task_4G_MQTT` 已改为 1s 轮询
- ✅ MinGW `gcc -fsyntax-only` 对 `mqtt.c` 语法检查通过（仅 CMSIS 在 x86 下的指针宽度 warning）
- ✅ `dashboard.html` 自动重连机制已验证（`mqtt.js` reconnect 事件）
- ⚠️ 当前工作区缺少 `test1/test1.ioc` 和 `test1/MDK-ARM/test1.uvprojx`，无法运行 UV4 Keil 构建；需先从本地/团队工程恢复 CubeMX/Keil 工程文件，再执行完整编译验证

## 变更文件

| 文件 | 改动 | 提交 |
|------|------|------|
| `test1/Core/Src/air780e/mqtt.c` | 真 MQTT 指令接收（删 stub、三阶段解析） | 3ac0d8b (B) |
| `dashboard.html` | 断线自动重连 + 状态反馈 | 待提交 (C) |

## 参考

- 当前代码：`test1/Core/Src/air780e/mqtt.c`（`MQTT_PollCommand` 函数）
- 命令解析：`mqtt.c` 中 `MQTT_ParsePublishBuffer()` / `MQTT_ParseCommandBytes()`
- 状态机处理：`app.c` 中 `STATE_WAIT_CLOUD` 段
- 前端日志：`doc-workflow/log3.3.md`
---

## 2026-07-09 补充：云指令设备动作 + GPS 精度修复

- `dashboard.html` 按钮仍发送英文命令码：`HOLD` / `RETURN` / `CONTINUE`，按钮文案分别显示为「暂留 / 退货 / 继续运输」。
- `app.c` 现在在 `STATE_WAIT_CLOUD` 真正等待 `APP_CLOUD_WAIT_MS = 30000U`，不再只等待 2s 队列消息；等待期间仍可响应 NFC 关机。
- 云指令动作已落到 `Alarm` 模块：
  - `HOLD`：LED2 常亮，蜂鸣器关闭。
  - `RETURN`：触发报警，LED2 快闪，蜂鸣器随快闪鸣叫。
  - `CONTINUE`：清除云端报警动作，LED2 恢复正常心跳闪烁。
- `gps.c` 坐标解析从 `atof()` 浮点解析改为 NMEA 定点微度解析，保留到 6 位小数后再写入 `GpsLocation`，并继续由 MQTT payload 输出 `lat/lon` 6 位小数。
- 代码侧已用 MinGW `gcc -fsyntax-only` 检查 `app.c` / `alarm.c` / `gps.c`；仅剩 CMSIS 在 x86 检查环境下的指针宽度 warning。
- 当前仍缺少 `test1/test1.ioc` 和 `test1/MDK-ARM/test1.uvprojx`，所以不能声明 UV4/Keil 全量构建已验证。
