# Task 2: 硬件集成

> 所有外设模块已到齐。本阶段将 CubeMX 使能外设，把条件编译 stub 逐步切换为真实 HAL 驱动。

## 前置条件

- [x] Task 1 完成（FreeRTOS 骨架 + 状态机 + stub 驱动）
- [x] 外设硬件到齐（SHT31、PN532、ATGM336H GPS、Air780E 4G、W25Q128 Flash）
- [ ] `.gitignore` 已优化（Drivers/Middlewares 不入库）
- [ ] 团队已同步最新 main 分支

## 关键规则

1. **同一时间只有一个人操作 `.ioc` 文件**（修改前在群里声明）
2. **每次 CubeMX 操作前后都要 git commit**（干净 diff 基线）
3. **每个模块独立验证**，失败可切回 stub 不阻塞他人
4. **所有自定义代码必须在 `USER CODE BEGIN/END` 段内**

---

## Task 2.1: CubeMX 外设使能

**负责人：A** | 预计：~2 小时

按顺序在 `test1/test1.ioc` 中使能外设：

| 步骤 | 外设 | 配置 | 验证 |
|------|------|------|------|
| 2.1.1 | I2C1 | PB8 SCL, PB9 SDA, 400kHz | 编译 0 Error |
| 2.1.2 | SPI1 | PA5 SCK, PA6 MISO, PA7 MOSI; PA4 GPIO_Output (CS) | 编译 0 Error |
| 2.1.3 | USART2 | PD5 TX, PD6 RX, 9600 baud, 8N1 | 编译 0 Error |
| 2.1.4 | USART1 | PA9 TX, PA10 RX, 115200 baud, DMA RX Circular | 编译 0 Error |
| 2.1.5 | PC8, PC9 | GPIO_Output (Buzzer, Alarm LED) | 编译 0 Error |
| 2.1.6 | FPU | FreeRTOSConfig.h: `configENABLE_FPU=1` | 编译 0 Error |

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

**负责人：A** | 依赖：Task 2.1 | 预计：~4 小时

### 2.2.1 SHT31 温湿度

```
1. 连接 SHT31 到 I2C1 总线（PB8 SCL, PB9 SDA）
2. sht31.h: #define SHT31_USE_HAL_I2C  1
3. 编译 → 烧录 → 串口观察输出
4. 验证：温度读数应在室温范围（~20-30°C），湿度合理（~40-70%）
5. 握手验证：手指靠近传感器，温度应上升
```

### 2.2.2 PN532 NFC

```
1. 确认 PN532 已挂在 I2C1 总线（地址 0x24，与 SHT31 0x44 不冲突）
2. pn532.h: #define PN532_USE_HAL_I2C  1
3. 编译 → 烧录 → 靠近 NTAG215 标签
4. 验证：串口输出真实 UID（非 MOCK-UID-001）
```

### 2.2.3 I2C 总线共享验证

```
1. 交替读取 SHT31 和 PN532，确认无 I2C 总线冲突
2. 长时间运行（10分钟+），无 I2C 超时或 HAL_ERROR
```

---

## Task 2.3: USART2 驱动验证（GPS）

**负责人：B** | 依赖：Task 2.1 | 预计：~3 小时

```
1. 连接 ATGM336H GPS 到 USART2（PD5 TX, PD6 RX）
2. 修改 gps.c：从 USART2 接收缓冲区读取 NMEA 数据
   （替换当前硬编码的 stub NMEA 句子）
3. 编译 → 烧录 → 户外或窗边测试
4. 验证：串口输出真实经纬度（非 31.2304/121.4737）
5. 检查：RMC/GGA 解析正确处理有效/无效定位
```

---

## Task 2.4: SPI1 驱动验证（W25Q128 Flash）

**负责人：A** | 依赖：Task 2.1 | 预计：~3 小时

```
1. 连接 W25Q128 到 SPI1 引脚（PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI）
2. w25q128.h: #define W25Q128_USE_HAL_SPI  1
3. 编译 → 烧录
4. 验证 JEDEC ID：应读取 0xEF17（W25Q128 制造商ID+设备ID）
5. 验证读写：写入测试数据 → 读回 → 比对
6. 验证擦除：Sector Erase → 读回全 0xFF
```

---

## Task 2.5: USART1 驱动验证（Air780E 4G + MQTT）

**负责人：B** | **⚠️ 依赖 Task 2.6（PB0 解决）** | 预计：~5 小时

```
前置：PB0 已释放为 Air780E PWRKEY

1. 连接 Air780E 到 USART1（PA9 TX, PA10 RX）、独立 4.0V 供电
2. 修改 air780e.c：
   - Air780E_SendAT() 改为通过 USART1 发送、等待响应
   - 替换 stub AT 响应为真实响应解析
3. AT 指令序列验证：AT → ATE0 → AT+CSQ → AT+CREG? → AT+CGATT?
4. MQTT 验证（需 SIM 卡 + 云平台配置）：
   - AT+CMQTTSTART → 连接 broker
   - AT+CMQTTPUB → 发布 telemetry JSON
5. 验证：串口输出真实 CSQ 值、网络注册状态、MQTT 发布结果
```

---

## Task 2.6: PB0 冲突解决

**负责人：C** | 依赖：Task 2.1 | 预计：~2 小时

```
1. CubeMX 修改：
   - PB0 从 LD1_GREEN → Air780E PWRKEY（GPIO_Output）
   - 系统心跳迁移到 PE1（LD2_YELLOW）
2. 代码修改（alarm.c / app.c）：
   - PE1 心跳慢闪（1s周期）= 系统正常
   - PE1 快闪（200ms周期）= 报警状态
   - 新增 Air780E_PWRKEY_ON/OFF 控制函数
3. 编译 → 烧录 → 验证 LED 闪烁 + PWRKEY 电平切换
4. 通知 B 组 PB0 已可用
```

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
