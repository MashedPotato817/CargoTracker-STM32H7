# 硬件-代码映射表

> 依据 `CLAUDE.md`、`doc/STM32H7_CubeMX_Config.md`、`doc/STM32H7_Logistics_System.md` 汇总。反映当前实际配置状态。

## MCU 与开发环境

| 项目 | 实际状态 |
|------|----------|
| MCU | STM32H7A3ZI / STM32H7A3ZITxQ |
| 开发板 | NUCLEO-H7A3ZI-Q |
| 工程 | `test1/MDK-ARM/test1.uvprojx` |
| 配置工具 | STM32CubeMX + Keil MDK |

## 外设与引脚

| 模块 | 型号 | STM32 外设/引脚 | 当前状态 | 切换方式 |
|------|------|-----------------|----------|---------|
| 调试串口 | ST-LINK VCP | USART3: PD8 TX, PD9 RX, 115200 | ✅ 已配置 | — |
| 开发板按键 | USER Button | PC13 EXTI Falling, Pull-up | ⚠️ 已配置，硬件疑似断线 | — |
| 板载绿灯 | LD1 | PB0 GPIO_Output | ✅ 已释放为 Air780E PWRKEY | 不再作为心跳灯 |
| 板载黄灯 | LD2 | PE1 GPIO_Output | ✅ 已配置，心跳/报警指示 | 慢闪=心跳，快闪=报警 |
| 板载红灯 | LD3 | PB14 GPIO_Output | ✅ 已配置，上传指示 | — |
| 温湿度 | SHT31 | I2C1: PB8 SCL, PB9 SDA, 400kHz | ⚠️ 代码就绪，待接线验证 | `SHT31_USE_HAL_I2C=1` |
| NFC | PN532 + NTAG215 | I2C1: PB8 SCL, PB9 SDA | ⚠️ 代码就绪，待接线验证 | `PN532_USE_HAL_I2C=1` |
| GPS | ATGM336H | USART2: PD5 TX, PD6 RX, 9600（运行时重配） | ⚠️ 代码就绪，待接线验证 | `GPS_USE_HAL_UART=1` |
| 4G | Air780E | USART1: PA9 TX, PA10 RX, 115200 | ⛔ 代码就绪，阻塞于 PB0 | `AIR780E_USE_HAL_UART=1` |
| 4G PWRKEY | Air780E | PB0 GPIO_Output | ❌ 未配置，仍为 LD1_GREEN | 待 C 组解决 Task 2.6 |
| Flash | W25Q128 | SPI1: PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI | ⚠️ 代码就绪，待接线验证 | `W25Q128_USE_HAL_SPI=1` |
| 蜂鸣器 | Buzzer | PC8 GPIO_Output | ✅ 已配置 | CubeMX 已使能 |
| 外接 LED | Alarm LED | PC9 GPIO_Output | ✅ 已配置 | CubeMX 已使能 |

## CubeMX 外设使能状态

| 外设 | CubeMX 已使能 | 驱动代码 | 状态 |
|------|:---:|:---:|------|
| USART3 (调试) | ✅ | ✅ | 运行中 |
| GPIO (LED/按键) | ✅ | ✅ | 运行中 |
| FreeRTOS CMSIS_V2 | ✅ | ✅ | 运行中 |
| I2C1 (SHT31/PN532) | ✅ | ✅ | 待硬件验证 |
| SPI1 (W25Q128) | ✅ | ✅ | 待硬件验证 |
| USART2 (GPS) | ✅ | ✅ | 待硬件验证 |
| USART1 (Air780E) | ✅ | ✅ | 阻塞于 PB0 |
| PC8 (蜂鸣器) | ✅ | ✅ | 待硬件验证 |
| PC9 (外接LED) | ✅ | ✅ | 待硬件验证 |

## PB0 冲突说明

- **当前配置**：PB0 = LD1_GREEN（板载绿灯心跳）⚠️ **冲突未解决**
- **目标配置**：PB0 = AIR780E_PWRKEY（4G 模块电源控制）
- **心跳迁移目标**：PE1 = LD2_YELLOW，慢闪表示系统心跳，快闪表示报警
- **控制入口**：待 C 组 `Power_Air780E_SetPwrKey()` 统一控制 PB0
- **协作约束**：B 组 Air780E/MQTT 验证被阻塞，等待 Task 2.6
- **详见**：`doc-workflow/TROUBLESHOOTING.md` 第 10 条

## 当前开发阶段

**硬件集成阶段**。所有外部模块已到齐，CubeMX 外设全部使能，驱动标志位全部切到 `1`（真 HAL 驱动）。

下一优先级：**Task 2.6 PB0 冲突解决** → 逐个接线验证各外设。

详见 `doc-workflow/task2.md`。
