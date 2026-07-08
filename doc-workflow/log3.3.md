# Task 3.3 执行日志 — 云端指令 + 前端控制台

> 2026-07-09 创建 | 负责人：C（前端） + B（后端）

## B 后端实现（3ac0d8b）

队友 B 在 commit `3ac0d8b` 中完成了 `mqtt.c` 的真 MQTT 云指令接收：

- 新增 `TCP_SendATCapture()` — AT 指令带响应捕获
- 新增 `MQTT_ReadUartBytes()` — 从 USART1 读原始字节（50ms 静默超时）
- 新增 `MQTT_ParsePublishBuffer()` — 逐字节 MQTT PUBLISH 帧解析
- 新增 `MQTT_ParseCommandBytes()` — 二进制搜索 HOLD/RETURN/CONTINUE（兼容 ASCII + HEX）
- 删除 stub 轮转 fallback（`poll_count % 3/6/9` 伪造逻辑）
- `STATE_WAIT_CLOUD` 窗口：5s → 30s
- `Task_4G_MQTT` 轮询周期：4s → 1s
- 订阅 `cargo/cmd` 失败检测 + `mqtt_ready=0` 回退

三阶段接收策略：
```
MQTT_PollCommand()
  → MQTT_ReadUartBytes()      读 USART1 实时字节
  → MQTT_ParsePublishBuffer()  帧解析
  → AT+CIPRXGET=2,256          从模块 TCP 缓冲拉
  → MQTT_ParsePublishBuffer()  再解析
  → MQTT_ReadUartBytes()       最后读一次
```

## C 前端优化

### dashboard 自动重连

**问题**：`mqtt.js` 连接断开后不自动重连，控制台变成摆设。

**修复**（`dashboard.html` 第 168-196 行）：

| 改动 | 配置 |
|------|------|
| `reconnectPeriod` | 3000ms — 断线后 3 秒开始重连 |
| `connectTimeout` | 10000ms — 10 秒连不上触发重试 |
| `reconnect` 事件 | 显示 `重连中(N)…` 带重试计数 |
| `close` 事件 | 显示 `已断开，等待重连…` |
| `error` 事件 | `console.error` + 数据日志记录 |
| 计数器归零 | `connect` 成功后 `reconnectAttempts = 0` |

**行为流程**：
```
正常连接 → 断线 → close: "已断开，等待重连…"
  → 3s 后 reconnect: "重连中(1)…"
  → 连上: connect → "已连接"（计数器归零）
  → 连不上: reconnect: "重连中(2)…" → 循环
```

### 此前已完成的 dashboard 优化

- CSQ 信号强度显示 + 颜色分级（≥20 绿 / ≥10 黄 / <10 红）
- 最后上报计时器（"刚刚" / "Xs 前" / "XmXs 前"）
- `gps_valid` 字段名兼容（`d.gps_v !== undefined ? d.gps_v : d.gps_valid`）
- 电池条区域替换为信号+计时

## 端到端数据链路

```
硬件 (Air780E)                  云端                        Web 控制台
                                                          
SHT31/GPS 数据                  broker.emqx.io            dashboard.html
  → MQTT PUBLISH ──────────→ cargo/telemetry ──────────→ 仪表盘显示
                              cargo/cmd ←────────────── 按钮下发
  ← MQTT_ParsePublishBuffer ←──────────────────
  → APP_CLOUD_CMD_HOLD
  → 状态机执行 ← queue_cloud_cmd
```

## 待验证

- [ ] 硬件端到端：dashboard 按钮 → 串口打印 `[MQTT] cloud cmd received: {"cmd":"HOLD"}`
- [ ] 状态机：`[StateMachine] cloud action: HOLD` → 对应动作执行
- [ ] dashboard 自动重连：断网 → 恢复 → 状态栏恢复"已连接"
- [ ] 长连接稳定性：dashboard 打开 1h+，MQTT 连接不丢

## 参考

- B 实现日志：`doc-workflow/archive/logB-log4.md`
- B 提交：`3ac0d8b` — feat(mqtt): implement real MQTT command reception and subscription logic
- 任务文档：`doc-workflow/task3.3.md`
- 前端文件：`dashboard.html`
