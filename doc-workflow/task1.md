# Task 1：FreeRTOS 工程骨架 + 应用层框架 + 驱动 stub

> 目标：在仅有 NUCLEO-H7A3ZI-Q 开发板（无外设）的条件下，搭建可编译、可烧录、可验证的完整系统框架。

## 前置条件

- Keil MDK + STM32CubeMX + STM32H7 DFP 已安装
- NUCLEO-H7A3ZI-Q 开发板 x1
- 其他外设模块暂不需要（驱动使用 stub 替代）

## FreeRTOS 任务架构

采用**按模块拆 + 共享资源合并**方案，共 6 个 task，通过 3 条队列通信：

| Task | 成员 | 优先级 | 栈大小 | 职责 |
|------|------|--------|--------|------|
| `Task_I2C_Sensors` | A | 中 | 512B | SHT30 温湿度 + PN532 NFC（共享 I2C1） |
| `Task_Flash` | A | 低 | 512B | W25Q128 读写缓存 |
| `Task_GPS` | B | 中 | 512B | USART2 接收 + NMEA 解析 |
| `Task_4G_MQTT` | B | 高 | 1KB | USART1 AT指令 + MQTT 收发 |
| `Task_StateMachine` | C | 高 | 1KB | 状态机 + 低功耗管理 + 云指令分发 |
| `Task_Alarm` | C | 低 | 256B | 蜂鸣器 + LED 控制 |

### 任务间通信

```
Task_NFC ──(激活事件)──→ Task_StateMachine
Task_StateMachine ──(触发采集)──→ Task_I2C_Sensors
Task_StateMachine ──(触发定位)──→ Task_GPS
Task_I2C_Sensors ──(温湿度数据)──→ Task_StateMachine
Task_GPS ──(经纬度数据)──→ Task_StateMachine
Task_StateMachine ──(上传数据)──→ Task_4G_MQTT
Task_4G_MQTT ──(云端指令)──→ Task_StateMachine
Task_StateMachine ──(报警触发)──→ Task_Alarm
Task_StateMachine ──(缓存数据)──→ Task_Flash
```

3 条队列：
- `queue_activation` — NFC 激活事件
- `queue_sensor_data` — 温湿度 + GPS 数据 → 状态机
- `queue_cloud_cmd` — 云端指令 → 状态机

### 状态机

```
深度睡眠 → NFC激活 → GPS定位 → 温湿度采集 → 数据上传 → 等待云指令 → [暂留/退货/继续运输] → 休眠
     ↑                                                                                      │
     └──────────────────────────────────────────────────────────────────────────────────────┘
```

## 三步推进

### 第 1 步：CubeMX FreeRTOS 工程 + Task 骨架

#### 1-1 备份现有工程

```bash
# 在开始前先提交当前 git 状态，方便回退
cd Project
git add -A && git commit -m "before CubeMX FreeRTOS config"
```

#### 1-2 CubeMX 打开工程

双击 `test1/test1.ioc`，用 STM32CubeMX 打开。

> ⚠️ CubeMX 版本建议 6.12+ ，否则 FreeRTOS CMSIS_V2 选项位置可能不同。

#### 1-3 引脚配置（Pinout）

| 引脚 | 功能 | 模式 | 说明 |
|------|------|------|------|
| PB0 | GPIO_Output | Push-Pull | LD1 绿灯 — 系统心跳 |
| PE1 | GPIO_Output | Push-Pull | LD2 黄灯 — 报警 |
| PB14 | GPIO_Output | Push-Pull | LD3 红灯 — 联网/传输 |
| PC13 | GPIO_Input | Pull-up, EXTI (falling edge) | 用户按键 — 模拟 NFC 激活 |
| PD8 | USART3_TX | Asynchronous | ST-LINK VCP — 调试输出 |
| PD9 | USART3_RX | Asynchronous | ST-LINK VCP |

> 其余引脚（USART1/2, I2C1, SPI1）暂不配置，stub 阶段用不到。

**User Label 建议：**

| 引脚 | User Label |
|------|-----------|
| PB0 | LD1_GREEN |
| PE1 | LD2_YELLOW |
| PB14 | LD3_RED |
| PC13 | BTN_USER |

#### 1-4 时钟配置（Clock Configuration）

保持与现有工程一致：
- HSE: 8 MHz（外部晶振）
- SYSCLK: 280 MHz
- AHB: 140 MHz
- APB1: 70 MHz, APB2: 140 MHz

> 注意：NUCLEO-H7A3ZI-Q 板载 **无 HSE 晶振**。CubeMX 默认启用 HSE 会导致编译通过但运行时卡在时钟初始化。实际使用 HSI（内部 64MHz RC），在 Clock Configuration 中将 PLL Source Mux 选为 HSI 而非 HSE。SYSCLK 可以设为 280 MHz（通过 PLL 倍频 HSI）。

#### 1-5 USART3 配置

| 参数 | 值 |
|------|-----|
| Mode | Asynchronous |
| Baud Rate | 115200 |
| Word Length | 8 Bits |
| Parity | None |
| Stop Bits | 1 |

#### 1-6 使能 FreeRTOS

1. **Pinout & Configuration** → **Middleware** → **FREERTOS**
2. Interface 选 **CMSIS_V2**
3. 保持默认参数，后续再调

**TOTAL_HEAP_SIZE** 默认约 15KB，本项目够用，不改。

#### 1-7 创建 6 个 Task

**Tasks and Queues** 标签页 → **Tasks** → 逐个添加：

| Task Name | Priority | Stack Size (Words) | Entry Function | 说明 |
|-----------|----------|-------------------|----------------|------|
| Task_StateMachine | osPriorityHigh | 256 | StartTask_StateMachine | 状态机 + 指令分发 |
| Task_4G_MQTT | osPriorityHigh | 256 | StartTask_4G_MQTT | 4G AT + MQTT |
| Task_I2C_Sensors | osPriorityNormal | 128 | StartTask_I2C_Sensors | SHT30 + PN532 |
| Task_GPS | osPriorityNormal | 128 | StartTask_GPS | GPS 接收解析 |
| Task_Flash | osPriorityLow | 128 | StartTask_Flash | Flash 读写 |
| Task_Alarm | osPriorityLow | 64 | StartTask_Alarm | LED + 蜂鸣器 |

> Stack Size 单位是 **Words（4 bytes）**，所以 256 Words = 1KB，128 Words = 512B。
> **Code Generation Option** 勾选 "As external" — 这样 task 函数声明在单独的文件，不污染 main.c。

#### 1-8 创建 3 条 Queue

**Tasks and Queues** 标签页 → **Queues** → 逐个添加：

| Queue Name | Size | Item Size | Allocation | Buffer Name | Control Block |
|-----------|------|-----------|------------|-------------|---------------|
| queue_activation | 16 | uint16_t | Dynamic | NULL | NULL |
| queue_sensor_data | 16 | uint32_t | Dynamic | NULL | NULL |
| queue_cloud_cmd | 8 | uint16_t | Dynamic | NULL | NULL |

> Item Size: `uint16_t` 用于传枚举值/命令；`uint32_t` 用于传数据指针（温湿度/GPS 是 struct 指针）。
> Dynamic = FreeRTOS 自动从 heap 分配，缓冲区不用手动创建。

#### 1-9 生成代码

1. **Project Manager** → **Code Generator** → 勾选 **"Generate peripheral initialization as a pair of .c/.h files"**
2. 点击 **GENERATE CODE**（齿轮图标）
3. 提示覆盖时确认。

> ⚠️ 生成完成后 CubeMX 会弹出对话框问是否打开工程，选 **"Open Project"** 或直接关闭用 Keil 手动打开。如果用 CubeMX 内建的 STM32CubeIDE 打开会报错（因为工程是 Keil MDK 格式），忽略即可。

#### 1-10 Keil 打开 + 编译验证

1. 双击 `test1/MDK-ARM/test1.uvprojx` 在 Keil MDK 中打开
2. **F7** 编译
3. 预期结果：0 Error(s), 0 Warning(s)

> 如果报 `Cannot Load Flash Programming Algorithm`：参考 `doc/STM32H7A3ZI-Q_程序下载要点.md`，设置 RAM for Algorithm Start: 0x24000000 Size: 0x00010000。
>
> 如果报 HSE 相关错误：回到 CubeMX 把 PLL Source Mux 从 HSE 改为 HSI，重新生成。

#### 1-11 重定向 printf 到 USART3

CubeMX 生成的 `usart.c` 只有 `MX_USART3_UART_Init()`，没有 printf 重定向。

在 `main.c` 的 `/* USER CODE BEGIN 0 */` 段添加：

```c
/* USER CODE BEGIN 0 */
#include <stdio.h>

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
/* USER CODE END 0 */
```

#### 1-12 编写 Task 骨架代码

在 `main.c` 的 `/* USER CODE BEGIN 4 */` 段添加 6 个 task 入口函数。

> CubeMX 生成了 "As external" 的 task 声明，但函数体仍然需要手动写。声明在 `freertos.c`，函数体可以在 `freertos.c` 的 USER CODE 区补充，也可以集中在 `main.c` 的 USER CODE 4 区。建议**全部写在 `freertos.c` 的 USER CODE 区**，保持 main.c 干净。

打开 `freertos.c`，在每个 task 对应的 `/* USER CODE BEGIN xxx */` 段写入：

```c
/* USER CODE BEGIN StartTask_StateMachine */
void StartTask_StateMachine(void *argument)
{
    for (;;) {
        printf("[StateMachine] heartbeat\n");
        osDelay(1000);
    }
}
/* USER CODE END StartTask_StateMachine */

/* USER CODE BEGIN StartTask_4G_MQTT */
void StartTask_4G_MQTT(void *argument)
{
    for (;;) {
        printf("[4G_MQTT] heartbeat\n");
        osDelay(2000);
    }
}
/* USER CODE END StartTask_4G_MQTT */

/* USER CODE BEGIN StartTask_I2C_Sensors */
void StartTask_I2C_Sensors(void *argument)
{
    for (;;) {
        printf("[I2C_Sensors] heartbeat\n");
        osDelay(3000);
    }
}
/* USER CODE END StartTask_I2C_Sensors */

/* USER CODE BEGIN StartTask_GPS */
void StartTask_GPS(void *argument)
{
    for (;;) {
        printf("[GPS] heartbeat\n");
        osDelay(3000);
    }
}
/* USER CODE END StartTask_GPS */

/* USER CODE BEGIN StartTask_Flash */
void StartTask_Flash(void *argument)
{
    for (;;) {
        printf("[Flash] heartbeat\n");
        osDelay(5000);
    }
}
/* USER CODE END StartTask_Flash */

/* USER CODE BEGIN StartTask_Alarm */
void StartTask_Alarm(void *argument)
{
    for (;;) {
        printf("[Alarm] heartbeat\n");
        osDelay(5000);
    }
}
/* USER CODE END StartTask_Alarm */
```

#### 1-13 编写按键中断回调

在 `main.c` 的 `/* USER CODE BEGIN 4 */` 段添加 GPIO EXTI 回调（放在 task 函数下方）：

```c
/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_13) {
        uint16_t event = 1;  // 1 = NFC 激活事件
        osMessageQueuePut(queue_activationHandle, &event, 0, 0);
        printf("[BTN] NFC activation event sent\n");
        
        // 消抖：简单处理（正式版用定时器消抖）
        HAL_Delay(50);
        while (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_13) == GPIO_PIN_RESET);
    }
}
/* USER CODE END 4 */
```

#### 1-14 编译 + 烧录验证

1. **F7** 编译 → 0 Error(s)
2. **Ctrl+F8** 烧录
3. 打开串口助手（115200-8-N-1），按 RESET 键
4. 预期输出：

```
[StateMachine] heartbeat
[4G_MQTT] heartbeat
[I2C_Sensors] heartbeat
[GPS] heartbeat
[Flash] heartbeat
[Alarm] heartbeat
[StateMachine] heartbeat
...
```

5. 按下用户按键（PC13），输出：

```
[BTN] NFC activation event sent
```

#### 1-15 提交

```bash
git add -A && git commit -m "CubeMX FreeRTOS: 6 task + 3 queue + printf heartbeat OK"
```

---

**Step 1 完成标准：**
- [x] CubeMX 生成 FreeRTOS 工程，编译 0 Error
- [x] 6 个 task 心跳串口可见
- [x] 按键中断 → 队列消息 → 串口打印

---

### 第 2 步：状态机 + 应用层

**从成员 C 开始：**
- `state_machine.c/h` — 状态枚举 + 转移表 + 核心调度函数
- `alarm.c/h` — 阈值比较 + LED/蜂鸣器控制
- `power.c/h` — 低功耗模式控制（Stop Mode 入口/唤醒）
- `app.c/h` — 系统初始化 + 主流程串联

**LED 状态指示：**
| LED | 颜色 | 含义 |
|-----|------|------|
| LD1 (PB0) | 绿 | 系统运行心跳 |
| LD2 (PE1) | 黄 | 报警状态 |
| LD3 (PB14) | 红 | 联网/数据传输中 |

**按键（PC13）作用：** 模拟 NFC 激活（短按 = 标签靠近，长按 = 另一指令）

**验证：** 按键触发 → LED 状态变化 → 串口打印状态转移日志，完整跑通一次流程。

### 第 3 步：外设驱动 stub

**从成员 A/B 开始：**

每个驱动模块提供两个文件：
- `<module>.c/h` — 真实初始化 + Stub 数据函数
- Stub 返回模拟数据，真实硬件到后替换内部实现即可

```c
// sht30.c — stub 版本
void SHT30_Init(void) {
    printf("[SHT30] Init OK (stub)\n");
}
float SHT30_ReadTemperature(void) {
    return 25.5f;  // 模拟室温
}
float SHT30_ReadHumidity(void) {
    return 60.0f;  // 模拟湿度
}
```

**所有需要 stub 的模块：**
| 模块 | 文件 | 初始化 | stub 返回值 |
|------|------|--------|-----------|
| SHT30 | sensor/sht30.c | I2C1 Init | temp=25.5°C, hum=60% |
| PN532 | nfc/pn532.c | I2C1 Init | UID="MOCK-UID-001" |
| GPS | gps/gps.c | USART2 Init | lat=31.23, lon=121.47 |
| Air780E | air780e/air780e.c | USART1 Init | AT OK, CSQ=25 |
| MQTT | air780e/mqtt.c | — | publish OK, cmd queue |
| W25Q128 | flash/w25q128.c | SPI1 Init | read/write OK |

**验证：** 烧录后，全流程自动运行 — 按键"激活" → 采集模拟数据 → "上传"到云 → 打印状态转移 → LED 闪烁 → 进入休眠 → 等待下次按键。

## 硬件到后切换

每个 stub 模块替换为真实驱动时，只需修改对应 `<module>.c` 的内部实现，**接口函数签名不变**，应用层代码零改动。

例如 SHT30：
```c
// stub → 真实：只改函数体，不改头文件
float SHT30_ReadTemperature(void) {
    uint8_t data[6];
    HAL_I2C_Master_Receive(&hi2c1, SHT30_ADDR, data, 6, 100);
    // ... 真实 CRC 校验 + 温度计算公式
    return temp;
}
```

## 输出物

完成后的代码结构：

```
test1/Core/
├── Inc/
│   ├── FreeRTOSConfig.h          # CubeMX 生成
│   ├── main.h
│   ├── stm32h7xx_it.h
│   └── app/
│       ├── app.h
│       ├── state_machine.h
│       ├── alarm.h
│       └── power.h
└── Src/
    ├── main.c                    # 初始化 + task 创建
    ├── stm32h7xx_it.c            # 中断服务（含按键 EXTI）
    ├── app/
    │   ├── app.c                 # 成员 C — 系统框架
    │   ├── state_machine.c       # 成员 C — 状态机
    │   ├── alarm.c               # 成员 C — 报警逻辑
    │   └── power.c               # 成员 C — 低功耗
    ├── sensor/sht30.c            # 成员 A — stub
    ├── nfc/pn532.c               # 成员 A — stub
    ├── flash/w25q128.c           # 成员 A — stub
    ├── gps/gps.c                 # 成员 B — stub
    └── air780e/
        ├── air780e.c             # 成员 B — stub
        └── mqtt.c                # 成员 B — stub
```
