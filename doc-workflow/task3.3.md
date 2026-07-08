# Task 3.3: 真 MQTT 云指令接收

> 🔴 必做 | 负责人：B | 依赖：Task 2.5 已完成

## 当前状态

- ✅ Air780E AT/MQTT 全链路已通（Task 2.5）
- ✅ dashboard 按钮发布 `cargo/cmd` → broker 正常
- ✅ `MQTT_Init()` 在 CONNECT 成功后订阅 `cargo/cmd`
- ✅ `MQTT_PollCommand()` 已移除 stub 轮转伪造，改为读取真实 UART/TCP/MQTT 数据
- ⚠️ 待硬件串口验证：dashboard 下发后应看到 `cloud cmd received` 和状态机 action 日志

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
- 再发送 `AT+CIPRXGET=2,256` 拉取 TCP 缓冲数据，兼容 Air780E 将数据以 raw / ASCII / 十六进制文本返回的情况。
- 使用 `MQTT_ParsePublishBuffer()` / `MQTT_ParseCommandBytes()` 将 `HOLD`、`RETURN`、`CONTINUE` 映射为 `AppCloudCommand`。
- 没有真实云端数据时返回 `APP_CLOUD_CMD_NONE`，不再伪造云端指令。

### 3. 备用简化方案

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

## 验证

```
1. 编译 → 烧录 → 打开串口
2. 打开 dashboard.html → 点「暂留」
3. 串口应打印: [MQTT] cloud cmd received: {"cmd":"HOLD"}
4. 状态机应打印: [StateMachine] cloud action: HOLD
```

### 当前代码侧验证

- ✅ `test1/Core/Src/air780e/mqtt.c` 已删除真实 UART 模式下的 stub 轮转 fallback
- ✅ 当前实现使用 `MQTT_ParsePublishBuffer()` / `MQTT_ParseCommandBytes()` 解析 PUBLISH payload 或 raw/CIPRXGET 返回内容
- ✅ MinGW `gcc -fsyntax-only` 对 `mqtt.c` 语法检查通过（仅 CMSIS 在 x86 下的指针宽度 warning）
- ⚠️ 当前工作区缺少 `test1/test1.ioc` 和 `test1/MDK-ARM/test1.uvprojx`，无法运行 UV4 Keil 构建；需先从本地/团队工程恢复 CubeMX/Keil 工程文件，再执行完整编译验证

## 参考

- 当前代码：`test1/Core/Src/air780e/mqtt.c`（`MQTT_PollCommand` 函数）
- 命令解析：`mqtt.c` 中 `MQTT_ParsePublishBuffer()` / `MQTT_ParseCommandBytes()`
- 状态机处理：`app.c` 中 `STATE_WAIT_CLOUD` 段
