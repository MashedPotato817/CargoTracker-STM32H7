# CargoTracker 智能货物追踪系统 — 设计文档

## 1. 系统概述

本系统设计了一款基于 STM32H7A3ZI 的智能货物追踪设备，针对物流运输中**货物定位难、环境监控盲、激活效率低、云端交互滞后**等问题，实现了 GPS 实时定位 + NFC 一键激活 + 温湿度智能监测报警 + 云端指令控制的全链路功能。

### 1.1 核心功能

| 功能 | 实现方式 |
|------|---------|
| GPS 定位 | ATGM336H + NMEA 解析 |
| NFC 激活 | PN532 + NTAG215 标签 |
| 温湿度监测 | SHT31 I2C（含 CRC8 校验） |
| 4G 云通信 | Air780E + MQTT |
| 离线缓存 | W25Q128 SPI Flash 环形队列 |
| 低功耗 | FreeRTOS + Stop Mode |
| 声光报警 | LED + 蜂鸣器 |
| 云端指令 | 暂留 / 退货 / 继续运输 |

### 1.2 技术参数

| 项目 | 规格 |
|------|------|
| MCU | STM32H7A3ZI (Cortex-M7, 280MHz) |
| 开发板 | NUCLEO-H7A3ZI-Q |
| RTOS | FreeRTOS 10.6.2 (CMSIS_V2) |
| IDE | Keil MDK V5.39 + STM32CubeMX 6.17.0 |
| 固件包 | STM32Cube FW_H7 V1.13.0 |
| 时钟 | HSI 64MHz → PLL → SYSCLK 280MHz |

---

## 2. 硬件设计

### 2.1 物料清单

| 模块 | 型号 | 接口 | 工作电压 |
|------|------|------|---------|
| 主控 | STM32H7A3ZI (NUCLEO-H7A3ZI-Q) | — | 3.3V |
| 温湿度 | SHT31 | I2C1 (0x44) | 2.4~5.5V |
| NFC | PN532 | I2C1 (0x24) | 3.3V |
| GPS | ATGM336H | USART2 | 3.3V |
| 4G | Air780E | USART1 | 4.0V 独立 |
| Flash | W25Q128 | SPI1 | 3.3V |
| 充电 | TP4056 | — | USB 5V |
| 电池 | 18650 锂电池 | — | 3.7V |
| 降压 | MP1584 DC-DC ×2 | — | 3.3V / 4.0V |

### 2.2 引脚分配

| 外设 | 引脚 | 参数 |
|------|------|------|
| USART1 (Air780E) | PA9 TX, PA10 RX | 115200 baud |
| USART2 (GPS) | PD5 TX, PD6 RX | 9600 baud |
| USART3 (调试) | PD8 TX, PD9 RX | 115200 baud |
| I2C1 (SHT31+PN532) | PB8 SCL, PB9 SDA | 400kHz |
| SPI1 (W25Q128) | PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI | ~10MHz |
| PWRKEY (4G) | PB0 | GPIO Output |
| LD2 黄灯 (心跳/报警) | PE1 | GPIO Output |
| LD3 红灯 (上传) | PB14 | GPIO Output |
| 蜂鸣器 | PC8 | GPIO Output |
| 外接报警 LED | PC9 | GPIO Output |
| 按键 (激活) | PC13 | EXTI Falling |

### 2.3 电源架构

```
USB 5V → TP4056(充电+保护) → 18650 锂电池
                              ├── MP1584 → 3.3V → STM32 + 传感器
                              └── MP1584 → 4.0V → Air780E(独立供电)
```

- 3.3V 轨：SHT31 + PN532 + GPS + W25Q128 并联供电（总功耗 <100mA）
- 4.0V 轨：Air780E 独立供电（峰值电流 2A）
- 所有 GND 共地

### 2.4 接线图

详见项目根目录 `index.html`（GitHub Pages 移动端适配）。

---

## 3. 软件设计

### 3.1 软件分层

```
main.c → FreeRTOS(Task调度/队列通信) → APP层(状态机/报警/低功耗) → Driver层(UART/I2C/SPI/GPIO) → HAL → STM32
```

### 3.2 FreeRTOS 任务架构

| 任务 | 负责人 | 优先级 | 栈 | 职责 |
|------|--------|--------|-----|------|
| Task_StateMachine | C | High | 1KB | 状态机 + 低功耗 + 云指令分发 |
| Task_4G_MQTT | B | High | 1KB | USART1 AT指令 + MQTT |
| Task_I2C_Sensors | A | Normal | 512B | SHT31 + PN532（共享 I2C1） |
| Task_GPS | B | Normal | 512B | USART2 + NMEA 解析 |
| Task_Flash | A | Low | 512B | W25Q128 读写缓存 |
| Task_Alarm | C | Low | 512B | LED 心跳/报警 + 蜂鸣器 |

### 3.3 队列通信

| 队列 | 大小 | 元素 | 方向 |
|------|------|------|------|
| `queue_activation` | 16 | uint16_t | NFC/Button → StateMachine |
| `queue_sensor_data` | 16 | 指针 | GPS/I2C → StateMachine |
| `queue_cloud_cmd` | 8 | uint16_t | MQTT → StateMachine |

传感器数据采用**请求-响应模式**：状态机置位请求标志 → 传感器 Task 采样 → 入队。避免周期性无效推送。

### 3.4 状态机

```
SLEEP → NFC_ACTIVE → GPS_LOCATE → ENV_SAMPLE → UPLOAD → WAIT_CLOUD → RETURN_SLEEP
  ^                                                                         │
  └─────────────────────────────────────────────────────────────────────────┘
```

| 状态 | 动作 | 超时 |
|------|------|------|
| SLEEP | 等待激活事件（NFC/按键），绿灯慢闪 | — |
| NFC_ACTIVE | 过渡状态 | — |
| GPS_LOCATE | 请求 GPS 采样，等待数据入队 | 7s |
| ENV_SAMPLE | 请求 SHT31 采样，超阈值则报警 | 5s |
| UPLOAD | 红灯亮，MQTT 发布；失败则写 Flash | — |
| WAIT_CLOUD | 等待云指令（Hold/Return/Continue） | 5s |
| RETURN_SLEEP | 进入低功耗 Stop Mode | — |
| ALARM | 黄灯快闪，蜂鸣器响 | — |

### 3.5 驱动层设计

所有驱动采用**条件编译双模式**：

```c
#define SHT31_USE_HAL_I2C     1   // 1=真实I2C, 0=stub回退
#define PN532_USE_HAL_I2C     1
#define W25Q128_USE_HAL_SPI   1
#define GPS_USE_HAL_UART      1
#define AIR780E_USE_HAL_UART  1
```

- `0`：stub 模式，返回模拟数据，系统可独立运行
- `1`：真实 HAL 驱动，需 CubeMX 使能对应外设

应用层调用统一接口，不关心总线细节：

```c
temp = SHT31_ReadTemperature();
GPS_GetLocation(&lat, &lon);
MQTT_Publish(payload);
```

### 3.6 关键驱动技术细节

**SHT31**：I2C 地址 0x44，发送 0x2400 测量命令，读取 6 字节（Temp MSB/LSB/CRC + Hum MSB/LSB/CRC），CRC8 校验，转换公式 `T = -45 + 175*raw/65535`。

**PN532**：I2C 帧协议（前导码 + 长度 + 校验），SAMConfiguration 初始化，InListPassiveTarget 卡检测。

**W25Q128**：JEDEC ID 0xEF17，Page Program（256B/页），Sector Erase（4KB），环形队列缓存储（128槽×32B），magic number `0xA5C35A3C`。

**GPS**：USART2 DMA 接收 NMEA 语句，解析 $GNRMC/$GNGGA，DDMM.MMMM → 十进制坐标转换。

**Air780E**：USART1 AT 指令，ATE0 关回显，AT+CSQ 信号强度，AT+CREG? 注册状态，AT+CGATT? 附着状态。PWRKEY >1s 低脉冲开关机。

**按键消抖**：PC13 EXTI 回调中用 `HAL_GetTick()` 实现 200ms 非阻塞消抖，ISR 仅入队 `APP_ACTIVATION_BUTTON`，无 `printf`/`HAL_Delay`。

---

## 4. 通信协议设计

### 4.1 MQTT Topic

| 方向 | Topic | QoS | 说明 |
|------|------|:---:|------|
| 上行（设备→云） | `cargo/telemetry` | 0 | 温湿度+GPS+信号强度 |
| 下行（云→设备） | `cargo/cmd` | 0 | 云端指令 |

> 开发阶段使用公共 broker：`broker.emqx.io:1883`（TCP）。生产可切换 EMQX Cloud TLS。

### 4.2 数据上报 JSON（实际格式，已验证 ✅）

```json
{"temp":25.0,"hum":49.5,"lat":31.230000,"lon":121.470000,"gps_valid":1,"csq":28}
```

| 字段 | 类型 | 说明 |
|------|------|------|
| `temp` | float(1位小数) | 温度 ℃ |
| `hum` | float(1位小数) | 湿度 % |
| `lat` | float(6位小数) | 纬度（WGS-84） |
| `lon` | float(6位小数) | 经度（WGS-84） |
| `gps_valid` | 0/1 | 定位有效标志 |
| `csq` | 0-31 | 4G 信号强度（≥20 优秀） |

> JSON 体积 ~72 bytes，每 10s 上报一次。

### 4.3 云指令

dashboard 按钮 → `cargo/cmd` → Air780E SUBSCRIBE 接收 → MQTT_ParsePublishBuffer 解析

| 指令 | Payload | 设备动作 |
|------|------|---------|
| 暂留 | `{"cmd":"HOLD"}` | LED2 常亮，保持唤醒 |
| 退货 | `{"cmd":"RETURN"}` | 触发报警，LED2 快闪+蜂鸣器 |
| 继续运输 | `{"cmd":"CONTINUE"}` | 清除报警，恢复正常 |

### 4.4 AT 指令序列（串口实测 ✅）

```
AT          → OK              AT+CGREG?   → +CGREG: 0,1
ATE0        → OK              AT+CEREG?   → +CEREG: 0,1
AT+CSQ      → +CSQ: 28,0      AT+CGATT?   → +CGATT: 1
AT+CREG?    → +CREG: 0,1
```

### 4.5 MQTT 连接流程

```
Air780E Init → AT+PDP 激活 → AT+CIPSTART TCP broker.emqx.io:1883
  → MQTT CONNECT (29 bytes) → CONNACK (4 bytes)
  → SUBSCRIBE cargo/cmd → mqtt_ready=1
  → PUBLISH cargo/telemetry (循环)
  → PollCommand (1s 周期，CIPRXGET 拉取)
```

### 4.6 断网缓存策略

```
PUBLISH 失败/超时 → W25Q128 写入 telemetry 记录（128槽环形队列）
网络恢复 → 遍历有效 slot → 按序补传 → 标记已上传 → 释放槽位
```

---

## 5. 云端交互

### 5.1 云平台

| 项目 | 开发环境 | 生产（答辩） |
|------|---------|------------|
| Broker | `broker.emqx.io:1883` TCP | EMQX Cloud `h8111173.ala.cn-hangzhou.emqxsl.cn:8883` TLS |
| 认证 | 无（公共） | 用户名 CargoTracker / 密码 Cargo2026 |
| Web 控制台 | `dashboard.html`（自建，GitHub Pages） | 同左 |

### 5.2 数据流（已实现 ✅）

```
STM32 采集 → JSON 封装 → MQTT PUBLISH cargo/telemetry → broker
  → dashboard.html 订阅 cargo/telemetry → 实时仪表盘（温湿度+GPS+信号+地图）
  
dashboard 按钮 → MQTT PUBLISH cargo/cmd → broker
  → Air780E SUBSCRIBE 接收 → MQTT_ParsePublishBuffer → queue_cloud_cmd
  → 状态机 WAIT_CLOUD → AppCloudCommand → LED/蜂鸣器动作
```

### 5.3 前后端架构图（GPT 可画）

> 复制以下结构给 GPT，让它画系统前后端交互架构图：

```
一张图包含 3 层：
┌─ 设备层 ───────────────────────────────────────┐
│ STM32H7 + FreeRTOS                             │
│ 6 Tasks: StateMachine/4G_MQTT/I2C_Sensors/     │
│          GPS/Flash/Alarm                       │
│ 5 Sensors: SHT31(GPIO), PN532(I2C),            │
│           ATGM336H(UART), Air780E(UART),        │
│           W25Q128(SPI)                          │
└────────────────────┬───────────────────────────┘
                     │ MQTT (4G)
┌─ 云端 ─────────────────────────────────────────┐
│ EMQX Cloud / broker.emqx.io                    │
│ Topics: cargo/telemetry (上行)                  │
│         cargo/cmd (下行)                        │
└────────────────────┬───────────────────────────┘
                     │ WebSocket
┌─ 前端 ─────────────────────────────────────────┐
│ GitHub Pages hosting                           │
│ dashboard.html (MQTT.js)                       │
│ index.html (接线指南) | report.html (设计报告)   │
│ 实时地图(Leaflet+OSM) + 仪表盘 + 指令下发        │
└────────────────────────────────────────────────┘
```

---

## 6. 低功耗设计

| 模式 | 电流 | 唤醒方式 | 状态 |
|------|------|---------|:---:|
| 运行模式 | ~50mA | — | ✅ |
| Stop Mode | <100μA | NFC EXTI (PC13) | ⚠️ stub→真实待实现 |
| 4G 空闲断电 | 0（PWRKEY 关断） | 上传前 PWRKEY 开机 | ⚠️ 待实现 |

> PWRKEY 时序：>1s 低脉冲开关机，默认保持 HIGH 释放（PB0）。支持软关机 AT+CPOWD=1 + 硬脉冲兜底。

---

## 7. 开发工具链

| 工具 | 版本 | 用途 |
|------|------|------|
| Keil MDK | V5.39 | 编译 + 烧录 |
| STM32CubeMX | 6.17.0 | 外设配置 + 代码生成 |
| STM32Cube FW_H7 | V1.13.0 | HAL 库 |
| ARM Compiler | V6.21 | armclang |
| Git | — | 版本控制 |

### 7.1 CubeMX 首次设置

`Drivers/`、`Middlewares/`、`.ioc`、`.uvprojx` 不在 Git 中。clone 后需 CubeMX 打开 `test1.ioc` → Generate Code。

### 7.2 Generate Code 后必做

每次 CubeMX Generate Code 后执行三步修复（详见 TROUBLESHOOTING.md #13）：

1. uvprojx：`RVDS/ARM_CM4F` → `GCC/ARM_CM7/r0p1`
2. FreeRTOSConfig.h：`configENABLE_FPU = 1`
3. main.h/gpio.c：`LD1_GREEN` → `AIR780E_PWRKEY`

---

## 8. 测试方案

| # | 测试项 | 方法 | 预期 | 实测 |
|:---:|------|------|------|:---:|
| 1 | SHT31 | 串口输出温度 | 室温波动 | ✅ 24-25°C / 47-51% |
| 2 | PN532 | 贴 NFC 标签 | 串口打印 UID | ✅ init OK / ⚠️ 待测标签 |
| 3 | W25Q128 | 上电自检 JEDEC ID | 0xEF17 | ✅ EF 40 18 |
| 4 | GPS | 户外窗边测试 | NMEA status='A' | ⚠️ 待户外（当前 stub 31/121） |
| 5 | Air780E AT | 串口 AT 交互 | CSQ≥20, 注册正常 | ✅ CSQ=27~28, 全部 OK |
| 6 | MQTT 发布 | dashboard 订阅 | JSON 数据到达 | ✅ 72 bytes, 到达 dashboard |
| 7 | 云指令 | 按钮下发 → 设备响应 | 状态变更 | ⚠️ 代码就绪，待联调 |
| 8 | 断网缓存 | 拔天线 → 恢复 | Flash 补传成功 | ❌ 未测 |
| 9 | 低功耗 | 万用表测电流 | Stop <100μA | ❌ 待实现 |
| 10 | 24h 稳定 | 长时间运行 | 无死机 | ❌ 未测 |

### 8.1 系统框图（GPT 可画）

> 复制以下结构给 GPT，让它画硬件系统框图：

```
STM32H7A3ZI 居中，四周环绕 6 个外设：

┌─────────────┐                    ┌─────────────┐
│   SHT31     │── I2C1(0x44) ─────│             │
│   温湿度    │                    │   W25Q128   │
└─────────────┘                    │ SPI1(PA4-7) │
                                   │   Flash缓存 │
┌─────────────┐                    │             │
│   PN532     │── I2C1(0x24) ─────│             │
│   NFC读写   │                    │ STM32H7A3ZI │
└─────────────┘                    │  FreeRTOS   │──── LED(PE1/PB14)+蜂鸣器(PC8)
                                   │  280MHz     │
┌─────────────┐                    │             │
│  ATGM336H   │── USART2(PD5/6) ──│             │
│   GPS定位   │                    │             │
└─────────────┘                    │             │
                                   │             │
┌─────────────┐                    │             │
│  Air780E    │── USART1(PA9/10) ─│             │
│  4G+MQTT    │                    │             │
└─────────────┘                    └─────────────┘

左下标注：电源架构
USB 5V → TP4056 → 18650(3.7V) → MP1584×2 → 3.3V(MCU+传感器) + 4.0V(Air780E独立)
```

### 8.2 软件架构图（GPT 可画）

> 复制以下结构给 GPT，让它画 3 层软件架构图：

```
┌──────── App 层 ──────────────────────────────────────┐
│ StateMachine (8状态) │ Alarm (LED/蜂鸣器) │ Power(Stop Mode/PWRKEY) │
├──────── FreeRTOS 层 ─────────────────────────────────┤
│ 6 Tasks: StateMachine(H) 4G_MQTT(H) I2C_Sensors(N) GPS(N) Flash(L) Alarm(L) │
│ 3 Queues: queue_activation │ queue_sensor_data │ queue_cloud_cmd │
├──────── Driver 层 ───────────────────────────────────┤
│ SHT31(I2C) PN532(I2C) W25Q128(SPI) GPS(UART) Air780E(UART) GPIO │
│ 条件编译双模式: *_USE_HAL_*=0(stub) / 1(真HAL)       │
├──────── HAL 层 ──────────────────────────────────────┤
│ STM32H7xx_HAL_Driver │ CMSIS │ STM32H7A3ZI (Cortex-M7) │
└──────────────────────────────────────────────────────┘
```
