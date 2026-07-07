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

## Task 2.2: I2C1 驱动验证（SHT31 + PN532）

**负责人：A** | 代码：✅ 完成 | 硬件验证：❌ 待做

### 2.2.1 SHT31 温湿度

- [x] `SHT31_USE_HAL_I2C = 1`（已配置）
- [ ] 连接 SHT31 到 I2C1 总线（PB8 SCL, PB9 SDA）
- [ ] 编译 → 烧录 → 串口观察温度读数（应在 ~20-30°C）
- [ ] 握手验证：手指靠近传感器，温度上升

### 2.2.2 PN532 NFC

- [x] `PN532_USE_HAL_I2C = 1`（已配置）
- [ ] 确认 PN532 已挂在 I2C1 总线（地址 0x24，与 SHT31 0x44 不冲突）
- [ ] 编译 → 烧录 → 靠近 NTAG215 标签
- [ ] 验证：串口输出真实 UID（非 MOCK-UID-001）

### 2.2.3 I2C 总线共享验证

- [ ] 交替读取 SHT31 和 PN532，确认无 I2C 总线冲突
- [ ] 长时间运行（10分钟+），无 I2C 超时或 HAL_ERROR

---

## Task 2.3: USART2 驱动验证（GPS）

**负责人：B** | 代码：✅ 完成 | 硬件验证：❌ 待做

- [x] `GPS_USE_HAL_UART = 1`（已配置）
- [x] USART2 HAL 真 UART 逐字节 NMEA 读取（已实现）
- [x] `GPS_Init()` 运行时自动重配 USART2 至 9600 baud
- [ ] 连接 ATGM336H GPS 到 USART2（PD5 TX, PD6 RX）
- [ ] 编译 → 烧录 → 户外或窗边测试
- [ ] 验证：串口输出真实经纬度（非 31.2304/121.4737）
- [ ] 检查：RMC/GGA 解析正确处理有效/无效定位

---

## Task 2.4: SPI1 驱动验证（W25Q128 Flash）

**负责人：A** | 代码：✅ 完成 | 硬件验证：❌ 待做

- [x] `W25Q128_USE_HAL_SPI = 1`（已配置）
- [x] 扇区擦除 + 读数据 + 写后校验（已实现）
- [ ] 连接 W25Q128 到 SPI1 引脚（PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI）
- [ ] 编译 → 烧录
- [ ] 验证 JEDEC ID：应读取 0xEF17（W25Q128 制造商ID+设备ID）
- [ ] 验证读写：写入测试数据 → 读回 → 比对
- [ ] 验证擦除：Sector Erase → 读回全 0xFF

---

## Task 2.5: USART1 驱动验证（Air780E 4G + MQTT）

**负责人：B** | 代码：✅ 完成 | ⛔ 阻塞：Task 2.6（PB0 PWRKEY 未解决）

- [x] `AIR780E_USE_HAL_UART = 1`（已配置）
- [x] HAL UART 真 AT 命令发送/接收/超时（已实现）
- [x] `AT+CREG?` 同时接受 0,1（本地）和 0,5（漫游）注册
- [x] MQTT 真 UART 路径（已实现）
- [ ] **前置**：PB0 释放为 Air780E PWRKEY（Task 2.6）
- [ ] 连接 Air780E 到 USART1（PA9 TX, PA10 RX）、独立 4.0V 供电
- [ ] AT 指令序列验证：AT → ATE0 → AT+CSQ → AT+CREG? → AT+CGATT?
- [ ] MQTT 验证（需 SIM 卡 + 云平台配置）
- [ ] 验证：串口输出真实 CSQ 值、网络注册状态、MQTT 发布结果

---

## Task 2.6: PB0 冲突解决 ❌ 未开始

**负责人：C** | 状态：最高优先级阻塞项

- [ ] CubeMX：PB0 从 LD1_GREEN → Air780E PWRKEY（GPIO_Output）
- [ ] CubeMX：系统心跳迁移到 PE1（LD2_YELLOW）
- [ ] alarm.c：PE1 心跳慢闪（1s周期）= 系统正常，快闪（200ms周期）= 报警
- [ ] power.c：`AIR780E_PWRKEY_GPIO_Port/Pin` 宏已定义
- [ ] power.c：`Power_Air780E_SetPwrKey()` 控制函数
- [ ] 编译 → 烧录 → 验证 LED 闪烁 + PWRKEY 电平切换
- [ ] 通知 B 组 PB0 已可用 → 解除 2.5 阻塞

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

| 文件 | 操作 | 负责人 |
|------|------|--------|
| `test1/test1.ioc` | 修改（使能外设） | A |
| `test1/Core/Inc/FreeRTOSConfig.h` | 修改（FPU=1） | A |
| `test1/Core/Src/sensor/sht31.c` | 验证（flag=1） | A |
| `test1/Core/Src/nfc/pn532.c` | 验证（flag=1） | A |
| `test1/Core/Src/flash/w25q128.c` | 验证（flag=1） | A |
| `test1/Core/Src/gps/gps.c` | 修改（USART2 替代 stub） | B |
| `test1/Core/Src/air780e/air780e.c` | 修改（USART1 替代 stub） | B |
| `test1/Core/Src/air780e/mqtt.c` | 修改（真实 AT 交互） | B |
| `test1/Core/Src/app/alarm.c` | 修改（LED 模式调整） | C |
| `test1/Core/Src/app/app.c` | 修改（PWRKEY 控制） | C |
