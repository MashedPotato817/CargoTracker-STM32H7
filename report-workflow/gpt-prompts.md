# GPT 绘图 + 报告协作 Prompts

> 使用方法：每次复制一个 Prompt 给 ChatGPT/Claude，让它生成对应图表。
> 风格要求：暗色主题（#0f1117 底色, #4da6ff 强调色），适合嵌入 HTML 报告。

---

## Prompt 1: 硬件系统框图

```
请画一张嵌入式系统硬件框图。

要求：
- 中间是 STM32H7A3ZI 主控芯片（Cortex-M7, 280MHz, FreeRTOS）
- 周围 6 个外设模块环形排列，每个用方框，标注型号+接口+引脚：
  · SHT31 温湿度 — I2C1 (PB8/PB9), 地址 0x44
  · PN532 NFC — I2C1 (PB8/PB9), 地址 0x24（与 SHT31 共享总线）
  · W25Q128 Flash — SPI1 (PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI)
  · ATGM336H GPS — USART2 (PD5 TX, PD6 RX), 9600 baud
  · Air780E 4G — USART1 (PA9 TX, PA10 RX), 115200 baud
  · 报警模块 — GPIO (PE1 LED, PB14 LED, PC8 蜂鸣器, PC9 外接LED)
- 每个外设到 MCU 画连线，标注接口类型
- 左下角标注电源架构：
  USB 5V → TP4056 → 18650(3.7V) → MP1584×2 → 3.3V(MCU+传感器) + 4.0V(Air780E独立)
- 暗色主题，简洁专业风格，适合技术报告
```

---

## Prompt 2: 软件分层架构图

```
请画一张嵌入式软件分层架构图（3层）。

从顶到底：
第一层 APP层：
  - StateMachine（8状态状态机）
  - Alarm（LED心跳/报警 + 蜂鸣器）
  - Power（Stop Mode低功耗 + PWRKEY控制）

第二层 FreeRTOS层：
  - 6个Task横向排列: Task_StateMachine(H) | Task_4G_MQTT(H) | Task_I2C_Sensors(N) | Task_GPS(N) | Task_Flash(L) | Task_Alarm(L)
  - 3个Queue: queue_activation | queue_sensor_data | queue_cloud_cmd
  - 标注 "请求-响应模式"（状态机置位→传感器Task采样→入队）

第三层 Driver层：
  - 5个驱动模块: SHT31(I2C) | PN532(I2C) | W25Q128(SPI) | GPS(UART) | Air780E(UART) | GPIO
  - 标注 "条件编译双模式: *_USE_HAL_* = 0(stub) / 1(真HAL)"

底层 HAL层: STM32H7xx_HAL_Driver → CMSIS → STM32H7A3ZI (Cortex-M7)

用箭头表示层间调用方向。暗色主题。
```

---

## Prompt 3: 状态机流转图

```
请画一张嵌入式系统状态机流转图。8个状态，循环流转。

状态和转换：
SLEEP（低功耗 绿灯慢闪）→ NFC/按键事件 → 
NFC_ACTIVE（PWRKEY开机 Air780E初始化）→ 
GPS_LOCATE（请求GPS 等待7s）→ 
ENV_SAMPLE（请求SHT31 等待5s）→ [超阈值?] → ALARM（LED快闪+蜂鸣器）→ 
UPLOAD（MQTT发布 红灯亮 失败写Flash）→ 
WAIT_CLOUD（等待云指令 HOLD/RETURN/CONTINUE 30s）→ 
RETURN_SLEEP（Air780E断电 进入Stop Mode）→ SLEEP

要求：
- 每个状态画成圆角矩形，内含状态名 + 一句话动作
- 状态转换画箭头，箭头上标触发条件
- ALARM状态作为分支，可有路径到 UPLOAD
- 整体是环形结构，清晰标识循环
- 暗色主题
```

---

## Prompt 4: MQTT 端到端通信时序图

```
请画一张 MQTT 端到端通信时序图。参与者从左到右：STM32/Air780E, EMQX Broker, dashboard.html

时序步骤：
1. STM32→Broker: AT+CIPSTART TCP broker.emqx.io:1883
2. Broker→STM32: CONNECT OK
3. STM32→Broker: MQTT CONNECT (29 bytes)
4. Broker→STM32: CONNACK (accepted)
5. STM32→Broker: SUBSCRIBE cargo/cmd
6. Broker→STM32: SUBACK

循环框（标注"每10s上报"）:
  7. STM32→Broker: PUBLISH cargo/telemetry {"temp":25.0,"hum":49.5,"lat":31.23,"lon":121.47,"gps_valid":1,"csq":28}
  8. Broker→dashboard: WebSocket 推送 telemetry
  9. dashboard: 更新仪表盘 + 地图 marker

可选框（标注"用户操作"）:
  10. dashboard→Broker: PUBLISH cargo/cmd {"cmd":"HOLD"}
  11. Broker→STM32: 推送 cmd
  12. STM32→STM32: MQTT_ParsePublishBuffer → queue_cloud_cmd → 状态机执行 LED/蜂鸣器动作

要求：标准 UML 时序图风格，暗色主题。
```

---

## Prompt 5: 前后端交互架构图

```
请画一张 IoT 系统前后端交互架构图。分 3 层：

底层（设备层）:
  STM32H7 + FreeRTOS (6 Tasks)
  5传感器: SHT31(I2C) PN532(I2C) ATGM336H(UART) Air780E(UART) W25Q128(SPI)
  箭头向上标注 "MQTT (4G)"

中层（云端层）:
  EMQX Cloud / broker.emqx.io
  Topics: cargo/telemetry(上行) cargo/cmd(下行)

顶层（前端层）:
  GitHub Pages 托管
  dashboard.html (MQTT.js + Leaflet地图)
  index.html (接线指南)
  report.html (设计报告)
  → 实时地图 + 仪表盘 + 指令下发

层间用双向箭头连接，标注数据流向。暗色主题。
```

---

## Prompt 6: 电源树拓扑图

```
请画一张嵌入式系统电源树拓扑图。

从左到右单向流动：
USB 5V → [TP4056 充电管理+保护] → 18650 锂电池(3.7V)
                                      ├── [MP1584 DC-DC] → 3.3V → STM32 MCU + SHT31 + PN532 + ATGM336H + W25Q128
                                      └── [MP1584 DC-DC] → 4.0V → Air780E 4G模块(独立供电)

标注每个节点的电压值和电流范围：
- 3.3V轨: 总功耗 <100mA
- 4.0V轨: Air780E 峰值 2A
- 所有 GND 共地

暗色主题，简洁专业。
```

---

## 报告润色 Prompts

### 摘要润色

```
请润色以下嵌入式竞赛报告的摘要，使其更精炼、更有冲击力，控制在 300 字以内：

本作品设计了一款基于 STM32H7A3ZI 的智能货物追踪设备，针对物流运输中货物定位难、环境监控盲、激活效率低、云端交互滞后等问题，实现了 GPS 实时定位 + NFC 一键激活 + 温湿度智能监测报警 + 云端指令控制的全链路功能。系统采用 FreeRTOS 实时操作系统，以 6 任务+3 队列架构实现模块间解耦通信，通过条件编译双模式支持开发板独立验证与外设硬件集成。云端基于 EMQX Cloud 搭建 MQTT Broker，配套自建 Web 控制台实现设备数据实时监控与指令下发。

要求：
- 必须包含量化指标（温度精度±0.3°C、功耗、定位精度等）
- 匿名——不能出现学校名称
- 突出创新点（条件编译双模式、二进制MQTT栈、自建控制台）
```

### 创新点总结

```
请根据以下信息，写出 5 个嵌入式竞赛作品的创新点（每条 50 字以内），用简洁有力的语言：

1. 条件编译双模式驱动框架：5个宏切换 stub/真实HAL，硬件未到时开发板独立验证，到齐后零代码切换
2. 二进制 MQTT 协议栈：直接操作 AT 指令构建 CONNECT/PUBLISH/SUBSCRIBE 帧，无三方库依赖
3. Flash 环形队列离线缓存：128槽 magiC 验证，断网本地存储，恢复自动补传
4. 自建 Web 控制台：单文件 HTML+MQTT.js，替代商业 IoT 平台，实时地图+仪表盘+指令下发
5. 请求-响应采样模式：状态机置位→传感器采样→入队，避免周期无效推送降低功耗
```

---

## 使用流程

1. 实物照片拍好后 → 嵌入 `report.html` 第三部分
2. 以上 6 个 Prompt → 逐个发给 GPT 出图 → 保存 PNG → 嵌入报告
3. 摘要润色 → 替换 `report.html` 和 `competition_report.md` 中的摘要
4. `report.html` 加 `@media print` → 浏览器打印 → 导出 PDF
5. `git archive` → 代码打包 RAR
