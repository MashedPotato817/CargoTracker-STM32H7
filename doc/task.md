# 毕业设计/竞赛项目分工建议

如果这是三人毕业设计/竞赛项目，最好的分工原则不是按"模块数量"分，而是按软件层次分。这样后期合并代码最容易，也方便 Git 管理。

你的项目包括：

- STM32H7（主控）
- Air780E（4G+MQTT）
- ATGM336H（GPS）
- PN532（NFC）
- SHT31（温湿度）
- W25Q128（Flash）
- 云平台交互
- 低功耗管理

推荐按下面的方式分工。

---

## 方案一（推荐，工作量最均衡）

### 成员A：底层驱动与硬件接口

负责所有硬件驱动（Drivers 层）

#### 工作内容（A）

- STM32CubeMX 工程建立
- GPIO 配置
- UART 驱动
- I2C 驱动
- SPI 驱动
- TIM 定时器
- RTC（如果使用）
- W25Q128 驱动
- SHT31 驱动
- PN532 驱动
- 蜂鸣器
- LED

#### 输出文件（A）

```text
Drivers/
BSP/
├── bsp_uart.c
├── bsp_spi.c
└── bsp_i2c.c
sensor/
└── sht31.c
nfc/
└── pn532.c
flash/
└── w25q128.c
```

---

### 成员B：通信与云平台

负责联网部分

#### 工作内容（B）

**Air780E：**

- AT 指令
- MQTT 连接
- TCP 通信
- 网络重连
- GPS 上传

**GPS：**

- NMEA 解析
- 经纬度解析
- 时间解析

**云平台（例如 OneNET / 阿里云 / 腾讯云）：**

- 数据上传
- 指令接收
- JSON 解析

#### 输出文件（B）

```text
air780e.c
mqtt.c
gps.c
json.c
cloud.c
```

---

### 成员C：系统逻辑与应用层

负责整个系统的大脑

#### 工作内容（C）

**系统状态机（例如）：**

```text
睡眠 → NFC激活 → GPS定位 → 温湿度检测 → 上传云端 → 等待云命令 → 报警 → 再次休眠
```

**还包括：**

- 数据融合
- 报警逻辑
- OLED 显示（如果有）
- 低功耗控制
- FreeRTOS 任务（如果使用）
- 主循环

#### 输出文件（C）

```text
main.c
system.c
app.c
alarm.c
power.c
state_machine.c
```

---

## 最终代码结构

```text
Project
├── Drivers
├── BSP
├── APP
│   ├── main.c
│   ├── app.c
│   ├── state_machine.c
│   ├── power.c
│   └── alarm.c
├── SENSOR
│   └── sht31.c
├── NFC
│   └── pn532.c
├── GPS
│   └── gps.c
├── AIR780E
│   ├── air780e.c
│   └── mqtt.c
├── FLASH
│   └── w25q128.c
└── BSP
```

---

## 三个人如何联调？

建议使用 Git，每个人负责自己的模块，最后统一集成。

调用流程：

```text
main.c → APP → Driver → HAL → STM32
```

这样：

- A 不用改 B 代码
- B 不用改 C 代码
- C 只负责调用接口

避免三个人同时修改 `main.c`。

---

## 推荐接口规范

例如由 A 提供接口：

```c
void SHT31_Init(void);
float SHT31_ReadTemperature(void);
float SHT31_ReadHumidity(void);
```

B 只需要调用：

```c
temp = SHT31_ReadTemperature();
```

不用关心 I2C 细节。

同理：

```c
GPS_GetLocation();
MQTT_Publish();
PN532_ReadCard();
Flash_Write();
```

全部统一接口。

---

## 工作量比例（比较均衡）

| 成员 | 主要内容 | 工作量 |
| ---- | ------- | ------ |
| A | STM32 底层驱动、SHT31、PN532、Flash、GPIO | ★★★★☆ |
| B | Air780E、MQTT、GPS、云平台通信 | ★★★★★ |
| C | 系统逻辑、状态机、低功耗、报警、主程序 | ★★★★☆ |

---

## 建议的开发顺序

1. **成员A**：完成硬件驱动，让所有模块都能正常读写。
2. **成员B**：实现 GPS 定位、4G 联网、MQTT 收发和云平台通信。
3. **成员C**：在前两者接口稳定后，编写系统状态机和业务逻辑，将所有模块整合成完整的物流追踪系统。

这种分工符合嵌入式项目的开发流程，后期合并代码冲突最少，也便于答辩时说明每个人负责的内容。
