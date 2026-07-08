# EMQX Cloud 部署配置日志

> 2026-07-08 创建

## 部署信息

| 配置项 | 值 |
|--------|-----|
| 项目 | CargoTracker-STM32H7 |
| 版本 | Serverless（免费） |
| 区域 | 杭州 |
| 规格 | 1,000 连接 / 1,000 TPS |
| EMQX 版本 | v5 |
| MQTT 端口 | 8883 (TLS/SSL) |
| WebSocket 端口 | 8084 (TLS/SSL) |

### 连接参数

```
Broker:   h8111173.ala.cn-hangzhou.emqxsl.cn
MQTT:     mqtts://h8111173.ala.cn-hangzhou.emqxsl.cn:8883
WS:       wss://h8111173.ala.cn-hangzhou.emqxsl.cn:8084/mqtt
API:      https://h8111173.ala.cn-hangzhou.emqxsl.cn:8443/api/v5
CA 证书:  2031.11.10 到期（emqx 自带 CA）
```

## ⚠️ 重要：Serverless 仅支持 TLS

Serverless 版本**不支持** 1883 (TCP) 和 8083 (非TLS WebSocket)。

Air780E 必须通过 TLS 连接：`tls://h8111173.ala.cn-hangzhou.emqxsl.cn:8883`

mqtt.c 中 AT 指令需用 `tls://` 前缀而非 `tcp://`。

## 下一步

- [x] Broker 地址已获取
- [x] 客户端认证已创建
- [x] WebSocket 客户端验证（2026-07-09 00:04, MQTTX Web, testtopic/1. 收发成功）
- [ ] 确认 Air780E AT 固件是否支持 MQTT over TLS

## 开发期间：使用公共 TCP Broker

EMQX Cloud Serverless 仅 TLS，Air780E TLS 支持待确认。开发阶段用公共 TCP broker：

| 参数 | 值 |
|------|-----|
| Broker | `broker.emqx.io` |
| Port | `1883` (TCP) |
| 用户名/密码 | 无需（公共 broker） |

> 队友 `mqtt.c` 已配置此地址，无需修改。TLS 方案保留备用，答辩时切换。

## 云端查看与控制

公共 broker 无控制台，替代方案：

1. **MQTTX Web**（https://mqttx.app/web-client）：
   - 连接到 `broker.emqx.io:8084/mqtt`（WebSocket）
   - 订阅 `cargo/telemetry` → 实时看设备上报
   - 发布到 `cargo/cmd` → 模拟云端指令下发

2. **答辩时切换 EMQX Cloud**：`h8111173.ala.cn-hangzhou.emqxsl.cn:8883`（TLS）→ 有完整 Web 控制台

## TLS 备用方案（答辩用）

| 参数 | 值 |
|------|-----|
| Broker | `h8111173.ala.cn-hangzhou.emqxsl.cn` |
| Port | `8883` (TLS) |
| 用户名 | `CargoTracker` |
| 密码 | `Cargo2026` |

## 认证凭据

| 参数 | 值 |
|------|-----|
| 用户名 | CargoTracker |
| 密码 | Cargo2026 |
| Client ID | cargo_001 |

> mqtt.c 中填入以上值即可连接。

## Air780E TLS 支持确认

需先验证 Air780E 的 AT 固件版本是否支持 TLS：

```
AT+CSSLCFG?        // 查询 SSL 配置能力
AT+CMQTTCONNECT=?  // 查询是否支持 tls:// 前缀
```

如不支持 TLS，备选方案：
- 换 EMQX 专业版（支持 TCP 1883）
- 或在本地搭 EMQX Docker（支持 TCP 1883）
