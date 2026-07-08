# Task 3.1: EMQX Cloud 注册与配置

> 🔴 必做 | 无需硬件 | 预计 30 分钟

## 为什么选 EMQX Cloud

- 免费额度：**100 万条消息/月**，开发测试完全够
- 标准 MQTT 协议，Air780E AT 指令直接兼容
- 5 分钟注册完毕，有 Web 控制台可看数据

---

## Step 1: 注册

1. 打开 https://www.emqx.com/zh/cloud
2. 点「免费试用」→ 用邮箱注册
3. 选择 **Serverless** 部署（免费）
4. 区域选离你最近的（新加坡或杭州）

---

## Step 2: 创建 MQTT 连接参数

部署创建后会看到连接信息：

```
Broker:  xxx.emqx.cloud
Port:    1883 (TCP) 或 8883 (TLS)

客户端认证：用户名 + 密码  （或 Token）

用户名:  your_username
密码:    your_password
```

> ⚠️ 把这些记下来，Air780E 的 `mqtt.c` 需要填。

---

## Step 3: 测试连接（用网页工具）

在 EMQX Cloud 控制台 → **WebSocket 客户端**：

1. 点「连接」→ 输入上面用户名/密码
2. 订阅 Topic: `cargo/telemetry`
3. 手动发布一条测试消息到 `cargo/telemetry`：
   ```json
   {"test": "hello"}
   ```
4. 看到消息回显 → 平台正常工作

---

## Step 4: Topic 设计

不按阿里云复杂 Topic 规范，用最简单的：

| 方向 | Topic | 说明 |
|------|-------|------|
| 设备→云 | `cargo/telemetry` | 温湿度 + GPS + 电池 |
| 设备→云 | `cargo/alert` | 报警事件 |
| 云→设备 | `cargo/cmd` | 指令（HOLD/RETURN/CONTINUE） |

> 简单 Topic 更适配 Air780E AT 指令，减少拼接复杂度。

---

## Step 5: 开发阶段 — 使用公共 TCP Broker

> EMQX Cloud Serverless **仅支持 TLS(8883)**，不提供 TCP(1883)。
> Air780E TLS 支持待确认。开发阶段先用公共 TCP broker。

队友 `mqtt.c` 已配置：
```c
#define MQTT_BROKER_HOST  "broker.emqx.io"   // 公共测试 broker
#define MQTT_BROKER_PORT  1883               // TCP 端口
```

**无需修改，零改动直接用。**

## 云端查看与控制

1. **自建 Web 控制台**（`dashboard.html`）：
   - 直连 `broker.emqx.io`，实时温湿度/GPS/电池
   - 三个指令按钮（暂留/退货/继续运输）
   - GitHub Pages 托管，手机扫码即可

2. **MQTTX Web**（备用调试）：https://mqttx.app/web-client

## TLS 备用方案（答辩用）

| 参数 | 值 |
|------|-----|
| Broker | `h8111173.ala.cn-hangzhou.emqxsl.cn` |
| Port | `8883` (TLS) |
| 用户名 | `CargoTracker` |
| 密码 | `Cargo2026` |

---

## 验证清单

- [x] EMQX Cloud 部署运行中
- [x] MQTTX Web 收发验证通过
- [x] 公共 TCP broker 可用（队友 mqtt.c 已配置）
- [ ] MQTTX Web 订阅 `cargo/telemetry` 实时监控
- [ ] TLS 备用方案待 Air780E 确认
