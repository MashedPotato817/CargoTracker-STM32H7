# Task 2 执行日志 — 硬件集成

> 执行时间：2026-07-08 ~ 2026-07-09

---

## 硬件验证结果

| 模块 | 接口 | 状态 | 验证数据 |
|------|------|------|---------|
| SHT31 | I2C1 (PB8/PB9) | ✅ | T=24°C, H=50% |
| PN532 | I2C1 (PB8/PB9) | ✅ | init OK |
| GPS ATGM336H | USART2 (PD5/PD6) | ✅ | stub 数据 (待户外测真坐标) |
| W25Q128 | SPI1 (PA4~PA7) | ✅ | JEDEC ID: EF 40 18 |
| Air780E 4G | USART1 (PA9/PA10) | ✅ | CSQ=28~31, 网络注册 |
| MQTT | Air780E TCP Socket | ✅ | broker.emqx.io:1883, PUBLISH 72 bytes |
| 蜂鸣器 | PC8 | ✅ | 报警快闪响/不报警不响 |
| PC13 按键 | EXTI | ❌ | 硬件问题，暂用自动触发绕过 |

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

## 待完成

- [ ] NFC 标签刷卡测试（NTAG215）
- [ ] GPS 户外真实坐标验证
- [ ] MQTT 订阅真实云端指令（替换 stub）
- [ ] 低功耗 Stop Mode
- [ ] PC13 按键硬件修复
- [ ] CubeMX 重新生成后必做三步修复（TROUBLESHOOTING.md 第 13 条）
