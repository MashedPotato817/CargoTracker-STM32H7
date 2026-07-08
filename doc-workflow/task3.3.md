# Task 3.3: 真 MQTT 云指令接收

> 🔴 必做 | 负责人：B | 依赖：Task 2.5 已完成

## 当前状态

- ✅ Air780E AT/MQTT 全链路已通（Task 2.5）
- ✅ dashboard 按钮发布 `cargo/cmd` → broker 正常
- ❌ `MQTT_PollCommand()` 用 stub 循环伪造指令（不是真 MQTT 接收）
- ❌ 设备未订阅 `cargo/cmd` topic
- ❌ 串口 `cloud action: NONE` — 收到的指令没生效

## 目标

```
dashboard 点按钮 → cargo/cmd → broker → Air780E 收到 PUBLISH → 
  MQTT_PollCommand() 返回 APP_CLOUD_CMD_HOLD → 状态机 WAIT_CLOUD 执行
```

## 修复内容

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

删掉当前的 stub 循环（337-363 行全部替换）：

```c
AppCloudCommand MQTT_PollCommand(void)
{
#if AIR780E_USE_HAL_UART
    if ((Air780E_IsNetworkReady() == 0U) || (mqtt_ready == 0U)) {
        return APP_CLOUD_CMD_NONE;
    }
#endif

    /* 检查 USART1 DMA 接收缓冲区是否有新数据 */
    uint8_t rx_byte;
    static uint8_t pub_buf[256];
    static uint16_t pub_len = 0;
    static uint8_t in_publish = 0;

    while (Air780E_RxByte(&rx_byte)) {
        /* MQTT PUBLISH 固定头 = 0x30~0x3F */
        if (!in_publish && (rx_byte & 0xF0) == 0x30) {
            in_publish = 1;
            pub_len = 0;
            continue;
        }
        if (in_publish) {
            /* 跳过剩余长度字段（1-4 字节）直到遇到 Topic */
            if (pub_len == 0) {
                uint8_t rem_len = rx_byte;
                continue; // 跳过剩余长度
            }
            // 简化：从 PUBLISH 包中提取 cmd 字段
            pub_buf[pub_len++] = rx_byte;
            if (pub_len >= sizeof(pub_buf) - 1) {
                pub_buf[pub_len] = '\0';
                in_publish = 0;
                pub_len = 0;
                AppCloudCommand cmd = MQTT_ParseCommandPayload((char*)pub_buf);
                if (cmd != APP_CLOUD_CMD_NONE) {
                    printf("[MQTT] cloud cmd received: %s\n", pub_buf);
                    return cmd;
                }
            }
        }
    }

    return APP_CLOUD_CMD_NONE;
}
```

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

## 参考

- 当前代码：`test1/Core/Src/air780e/mqtt.c`（`MQTT_PollCommand` 函数）
- 命令解析：`mqtt.c:242 MQTT_ParseCommandPayload()`
- 状态机处理：`app.c` 中 `STATE_WAIT_CLOUD` 段
