# Task 2: 硬件集成

> 所有外设模块已到齐。本阶段将 CubeMX 使能外设，把条件编译 stub 逐步切换为真实 HAL 驱动。

## 前置条件

- [x] Task 1 完成（FreeRTOS 骨架 + 状态机 + stub 驱动）
- [x] 外设硬件到齐（SHT31、PN532、ATGM336H GPS、Air780E 4G、W25Q128 Flash）
- [x] `.gitignore` 已优化（Drivers/Middlewares 不入库）
- [x] 团队已同步最新 main 分支（W 分支已合并）

## 关键规则

1. **同一时间只有一个人操作 `.ioc` 文件**（修改前在群里声明）
2. **每次 CubeMX 操作前后都要 git commit**（干净 diff 基线）
3. **每个模块独立验证**，失败可切回 stub 不阻塞他人
4. **所有自定义代码必须在 `USER CODE BEGIN/END` 段内**

---

## Task 2.1: CubeMX 外设使能 ✅ 代码完成，⚠️ 待硬件验证

**负责人：A** | 状态：代码已完成（W 分支合并）

| 步骤 | 外设 | 配置 | 状态 |
|------|------|------|------|
| 2.1.1 | I2C1 | PB8 SCL, PB9 SDA, 400kHz | ✅ `MX_I2C1_Init()` 已生成 |
| 2.1.2 | SPI1 | PA5 SCK, PA6 MISO, PA7 MOSI; PA4 GPIO_Output (CS) | ✅ `MX_SPI1_Init()` 已生成 |
| 2.1.3 | USART2 | PD5 TX, PD6 RX, 115200 baud, GPS_Init 重配 9600 | ⚠️ 建议 CubeMX 改 9600 |
| 2.1.4 | USART1 | PA9 TX, PA10 RX, 115200 baud | ✅ `MX_USART1_UART_Init()` 已生成 |
| 2.1.5 | PC8, PC9 | GPIO_Output (Buzzer, Alarm LED) | ✅ `MX_GPIO_Init()` 已配置 |
| 2.1.6 | FPU | FreeRTOSConfig.h: `configENABLE_FPU=1` | ✅ 已生效 |

**操作流程**：
```
git commit -m "pre-cube: 外设使能前快照"
→ CubeMX 使能外设 → Generate Code
→ Keil Rebuild → 0 Error 0 Warning
→ git add + commit -m "cube: 使能 I2C1/SPI1/USART1/USART2/PC8/PC9 + FPU"
→ push
→ 通知团队 rebase
```

---

## Task 2.2: I2C1 驱动验证（SHT31 + PN532）⚠️ 部分验证

**负责人：A** | 代码：✅ | 硬件：SHT31 ✅ / PN532 ⚠️ 待标签测试

### 2.2.1 SHT31 温湿度 ✅

- [x] `SHT31_USE_HAL_I2C = 1`（已配置）
- [x] 连接 SHT31 到 I2C1 总线（PB8 SCL, PB9 SDA）
- [x] 编译 → 烧录 → 串口：`T=24~25C H=47~51%`（真实数据，非 stub）
- [x] 连续多次采样数据正常波动

### 2.2.2 PN532 NFC ⚠️

- [x] `PN532_USE_HAL_I2C = 1`（已配置）
- [x] 串口：`PN532 init OK`，I2C 地址 0x24 无冲突
- [ ] 靠近 NTAG215 标签验证 UID 输出

### 2.2.3 I2C 总线共享验证

- [x] SHT31 + PN532 共享 I2C1，init 均正常
- [ ] 长时间运行（10分钟+），无 I2C 超时

---

## Task 2.3: USART2 驱动验证（GPS）⚠️ stub 数据

**负责人：B** | 代码：✅ | 硬件：⚠️ 串口输出 lat=31 lon=121（stub 固定值）

- [x] `GPS_USE_HAL_UART = 1`（已配置）
- [x] USART2 HAL 真 UART 逐字节 NMEA 读取（已实现）
- [x] `GPS_Init()` 运行时自动重配 USART2 至 9600 baud
- [ ] 户外或窗边测试：需真实 NMEA 数据替代 stub
- [ ] 验证：串口输出真实经纬度（非 31/121）
- [ ] 检查：RMC/GGA 解析正确处理有效/无效定位

---

## Task 2.4: SPI1 驱动验证（W25Q128 Flash）⚠️ 部分验证

**负责人：A** | 代码：✅ | 硬件：✅ JEDEC ID OK / ⚠️ 读写待验

- [x] `W25Q128_USE_HAL_SPI = 1`（已配置）
- [x] 扇区擦除 + 读数据 + 写后校验（已实现）
- [x] 连接 W25Q128 到 SPI1
- [x] 编译 → 烧录 → 串口：`JEDEC ID: EF 40 18`（真实 Flash）
- [ ] 验证读写：写入测试数据 → 读回 → 比对
- [ ] 验证擦除：Sector Erase → 读回全 0xFF

---

## Task 2.5: USART1 驱动验证（Air780E 4G + MQTT）✅

**负责人：B** | 代码：✅ | 硬件：✅ AT/MQTT 全部打通

- [x] `AIR780E_USE_HAL_UART = 1`（已配置）
- [x] 连接 Air780E 到 USART1 + 独立 4.0V 供电
- [x] AT 指令序列：AT ✅ ATE0 ✅ CSQ=27~28 ✅ CREG=0,1 ✅ CGREG=0,1 ✅ CEREG=0,1 ✅ CGATT=1 ✅
- [x] TCP 连接 broker.emqx.io:1883 ✅
- [x] MQTT CONNECT → CONNACK ✅
- [x] MQTT PUBLISH 成功 → dashboard 收到 `{"temp":24,"hum":49,...}`
- [x] 云端指令订阅 → 串口打印 `cloud payload: {"cmd":"HOLD"}` 等

> Air780E 全链路已打通。注意：多次启动后串口日志出现混叠（reset + 日志乱序），需排查复位原因。

---

## Task 2.6: PB0 冲突解决 ✅ 已完成

**负责人：C** | 状态：✅ 全部完成

- [x] CubeMX：PB0 从 LD1_GREEN → Air780E PWRKEY（GPIO_Output）
- [x] CubeMX：心跳保持 PE1（LD2_YELLOW）不变
- [x] `main.h`：自动生成 `AIR780E_PWRKEY_Pin/Port` 宏
- [x] `power.c`：PWRKEY 1.5s 低脉冲开关机，默认 HIGH 释放
- [x] `gpio.c`：PB0 默认 GPIO_PIN_SET（释放），不与 LED 混写
- [x] 串口验证：`Air780E power-on (PWRKEY low 1.5s)` ✅

---

## Task 2.7: 系统集成测试

**负责人：C** | 依赖：Task 2.2~2.6 全部完成 | 预计：~4 小时

### 测试用例

| # | 测试场景 | 预期结果 |
|---|---------|---------|
| 1 | NFC 标签靠近 | 激活设备，进入 GPS 定位 |
| 2 | GPS 获取定位 | 真实经纬度，status='A' |
| 3 | SHT31 温湿度 | 环境温度+湿度，正常范围 |
| 4 | 温湿度超阈值 | LD2 黄灯快闪，报警状态 |
| 5 | 4G 数据上传 | MQTT 发布成功，JSON 格式正确 |
| 6 | 云端指令返回 | "暂留"/"退货"/"继续运输" 正确解析 |
| 7 | 断网缓存 | MQTT 失败 → W25Q128 写入 → 恢复后补传 |
| 8 | 低功耗休眠 | Stop Mode 进入 + NFC 唤醒 |
| 9 | 长时间运行 | 24h 无死机、无内存泄漏、无队列溢出 |

### 完成标准

- [ ] 全部 9 个测试用例通过
- [ ] 编译 0 Error, 0 Warning
- [ ] 新发现的问题已记录到 TROUBLESHOOTING.md

---

## 文件变更清单

| 文件 | 操作 | 负责人 | 状态 |
|------|------|--------|------|
| `test1/test1.ioc` | 修改（使能外设 + PB0→PWRKEY） | A → C | ⚠️ 当前工作区缺失，需从本地/团队工程恢复 |
| `test1/Core/Inc/FreeRTOSConfig.h` | 修改（FPU=1） | A | ✅ 已设置 |
| `test1/Core/Src/sensor/sht31.c` | 验证（flag=1） | A | ⚠️ 待硬件 |
| `test1/Core/Src/nfc/pn532.c` | 验证（flag=1） | A | ⚠️ 待硬件 |
| `test1/Core/Src/flash/w25q128.c` | 验证（flag=1） | A | ⚠️ 待硬件 |
| `test1/Core/Src/gps/gps.c` | 修改（USART2 替代 stub） | B | ✅ 代码完成 |
| `test1/Core/Src/air780e/air780e.c` | 修改（USART1 替代 stub） | B | ✅ 代码完成 |
| `test1/Core/Src/air780e/mqtt.c` | 修改（真实 AT 交互） | B | ✅ 代码完成 |
| `test1/Core/Src/app/alarm.c` | 修改（PC8蜂鸣器+PE1心跳/报警灯） | C | ✅ 已实现，待硬件验证 |
| `test1/Core/Src/app/power.c` | 修改（PWRKEY 时序） | C | ✅ 已完成 |
| `test1/Core/Inc/main.h` | 补充引脚宏 | C | ✅ 已完成 |
| `test1/Core/Inc/usart.h` | 补充 huart1/2 声明 | C | ✅ 已完成 |
| `test1/Core/Src/usart.c` | 补充 USART1/2 初始化 | C | ✅ 已完成 |
| `test1/Core/Src/gpio.c` | 补充 W25Q128_CS/Buzzer/LED | C | ✅ 已完成 |
| `test1/Core/Inc/stm32h7xx_hal_conf.h` | 启用 SPI 模块 | C | ✅ 已完成 |
| `test1/MDK-ARM/test1.uvprojx` | 修复 IncludePath/Flash/port.c | C | ⚠️ 当前工作区缺失，需 CubeMX Generate Code 恢复后复查 |
