
# STM32H7A3ZI-Q CubeMX 工程配置结构（智能物流系统）

## 一、工程目标
基于 STM32H7A3ZI-Q 构建智能物流追踪系统，外设包括：
- Air780E（4G通信）
- ATGM336H（GPS）
- PN532（NFC）
- SHT31（温湿度）
- W25Q128（SPI Flash）
- OLED（可选）
- 低功耗电池系统

---

## 二、CubeMX 基础配置

### 1. MCU选择
- STM32H7A3ZITx
- Package: LQFP144（NUCLEO-H7A3ZI-Q）

---

### 2. 时钟系统（Clock Configuration）

推荐配置：

- HSE: 8 MHz（外部晶振）
- SYSCLK: 280 MHz（或 240 MHz 稳定模式）
- AHB: 140 MHz
- APB1: 70 MHz
- APB2: 140 MHz

说明：
- 保持稳定优先（避免极限超频）
- 4G通信不依赖高主频

---

### 3. 电源配置（Power）

- VCORE: Scale 0（高性能模式）
- Flash latency: 自动配置
- Enable SMPS（如开发板支持）

---

## 三、引脚分配（Pinout）

### 1. USART1（Air780E 4G模块）

- PA9  → USART1_TX
- PA10 → USART1_RX
- PB0  → GPIO_Output（PWRKEY）

配置：
- Mode: Asynchronous
- Baudrate: 115200 / 9600（AT默认）

---

### 2. USART2（GPS模块）

- PD5 → USART2_TX
- PD6 → USART2_RX

配置：
- Mode: Asynchronous
- Baudrate: 9600

---

### 3. I2C1（共享总线）

- PB8 → I2C1_SCL
- PB9 → I2C1_SDA

连接设备：
- SHT31
- PN532
- OLED（可选）

配置：
- Speed: 400kHz (Fast Mode)
- Pull-up: Enable

---

### 4. SPI1（Flash存储）

- PA4 → SPI1_NSS
- PA5 → SPI1_SCK
- PA6 → SPI1_MISO
- PA7 → SPI1_MOSI

配置：
- Mode: Full Duplex Master
- Baudrate prescaler: /8 或 /16

---

### 5. GPIO控制

| 功能 | 引脚 |
|------|------|
| 蜂鸣器 | PC8 |
| LED | PC9 |
| NFC IRQ | PC0 (可选) |

---

## 四、DMA配置（建议）

### USART1（Air780E）
- Enable DMA RX
- Mode: Circular

### USART2（GPS）
- 可选DMA RX（建议开启）

---

## 五、中断配置（NVIC）

必须开启：

- USART1 global interrupt
- USART2 global interrupt
- EXTI line（NFC IRQ 可选）

---

## 六、Middleware（可选）

### 1. FreeRTOS（推荐开启）

任务划分：

- Task_GPS
- Task_4G_MQTT
- Task_Sensor
- Task_SystemState

---

### 2. FATFS（可选）

用于：
- Flash文件系统
- 数据缓存

---

## 七、低功耗配置（重点）

### 1. Sleep模式策略
- NFC未触发 → Stop Mode
- 定时唤醒 → RTC

### 2. 4G控制
- PWRKEY GPIO控制上电
- 空闲关闭模块

---

## 八、关键工程参数

### UART配置
- Air780E: 115200
- GPS: 9600

### I2C
- 400kHz（稳定）

### SPI
- Flash: 10~20MHz

---

## 九、生成代码结构建议

CubeMX生成后结构：

Core/
 ├── Inc/
 ├── Src/

Drivers/
 ├── STM32H7xx_HAL_Driver/

Middlewares/
 ├── FreeRTOS/
 ├── FATFS/

User/
 ├── app.c
 ├── gps.c
 ├── air780e.c
 ├── sensor.c

---

## 十、总结

本CubeMX配置适用于：
- 智能物流追踪
- 4G IoT设备
- 低功耗GPS终端
- NFC激活系统

特点：
- UART/I2C/SPI资源清晰分离
- 支持低功耗扩展
- 可直接用于工程开发
