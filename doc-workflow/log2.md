# Task 2 执行日志 — 硬件集成

> 执行时间：2026-07-08 ~ 2026-07-09

---

## 硬件验证结果

| 模块 | 接口 | 状态 | 验证数据 |
|------|------|------|---------|
| SHT31 | I2C1 (PB8/PB9) | ✅ | T=24~25°C, H=48~60% |
| PN532 | I2C1 (PB8/PB9) | ✅ | UID 读取成功 (04F9B222B02190) |
| GPS ATGM336H | USART2 (PD5/PD6) | 🟡 | NMEA 数据正常，室内无卫星信号 |
| W25Q128 | SPI1 (PA4~PA7) | ✅ | JEDEC ID: EF 40 18 |
| Air780E 4G | USART1 (PA9/PA10) | ✅ | CSQ=25~31, CREG=0,1, 网络已注册 |
| MQTT | Air780E TCP Socket | ✅ | broker.emqx.io:1883, PUBLISH + SUBSCRIBE + CIPRXGET |
| 蜂鸣器 | PC8 | ✅ | 跟 LD2 同步，NFC 控制开关 |
| LD2 黄灯 | PE1 | ✅ | NFC 激活控制，慢闪/快闪 |
| PC13 按键 | EXTI | ❌ | 板子硬件问题 |

---

## 关键修复

### 1. I2C 地址问题

SHT31/PN532 的 `(0x44 << 1)` 是正确写法——STM32 HAL 要求地址已左移。

调试过程中误改为 `0x44`（不左移），导致 HAL 发错地址。已恢复。

根本诊断手段：I2C 寄存器直操作扫描，绕过 HAL 找到真实设备地址。

### 2. PWRKEY 时序

Air780E 接 VBAT 自动开机，PWRKEY 拉低 1.5s 可关机。

gpio.c 初始将 PB0 与 LD3_RED 一起设为 LOW → PWRKEY 持续低 → 模块不启动。

修复：PB0 单独设 HIGH，PowerOn 发 1.5s 低脉冲开机，等 5s 再发 AT 指令。

### 3. Air780E 启动延迟

模块上电后需 ~6 秒才响应 AT 指令。第一条 AT 可能超时，之后全部正常。不再依赖第一条 AT 的结果。

### 4. MQTT TCP 方案

Air780E 固件不支持内置 MQTT AT 指令。改用手写 MQTT 协议 + TCP Socket (AT+CIPSTART/CIPSEND)，兼容所有固件版本。

CONNECT 后需读掉 broker 返回的 4 字节 CONNACK，否则残留数据干扰后续 CIPSEND。

### 5. 蜂鸣器驱动

`3.3V → 蜂鸣器 → PC8` 直接驱动：HIGH=关，LOW=响。gpio.c 初始需设为 HIGH。

---

## 当前系统状态

```
启动 → PWRKEY开机 → Air780E联网 → MQTT连broker
                                            ↓
        循环 ← 休眠 ← 云端指令 ← MQTT发布 ← 状态机(GPS→SHT31→报警→上传)
```

---

## 新增功能（2026-07-09）

### NFC 拨动开关
- NFC 刷卡 → Air780E 开机 + 系统激活 + LD2 心跳开始
- 再次刷卡 → Air780E 关机 + LED/蜂鸣器全部关闭 + 休眠
- NFC 检测在 GPS/SHT31 采样期间实时响应

### 定时上报
- Air780E 在线期间每 10 秒自动采集 GPS+SHT31+MQTT 发布
- NFC 刷卡可在上报循环中随时关机

### 报警防抖
- 连续 2 次超阈值才触发报警（防误报）
- 需连续正常读数才能解除

### 蜂鸣器 + LED（跟随 NFC 状态）
| 状态 | LD2 | 蜂鸣器 |
|------|-----|--------|
| 系统休眠 | 灭 | 关 |
| 在线正常 | 慢闪(1s) | 关 |
| 报警触发 | 快闪(200ms) | 同步响 |

### 云指令接收 (Task 3.3)
- MQTT SUBSCRIBE cargo/cmd ✅
- AT+CIPRXGET 轮询接收 ✅
- dashboard.html → 设备 → StateMachine 全链路 ✅

## 待完成

- [ ] NFC 关机验证（硬件响应偶有延迟）
- [ ] GPS 户外真实坐标验证（室内无卫星信号，模块正常输出 NMEA）
- [ ] 低功耗 Stop Mode
- [ ] PC13 按键硬件修复
- [ ] 长时间运行稳定性测试
- [ ] CubeMX 重新生成后必做三步修复（TROUBLESHOOTING.md 第 13 条）
