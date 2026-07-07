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
| 温湿度 | SHT31 | I2C1: PB8 SCL, PB9 SDA, 400kHz | 🔧 待使能 CubeMX I2C1 | `SHT31_USE_HAL_I2C=1` |
| NFC | PN532 + NTAG215 | I2C1: PB8 SCL, PB9 SDA | 🔧 待使能 CubeMX I2C1 | `PN532_USE_HAL_I2C=1` |
| GPS | ATGM336H | USART2: PD5 TX, PD6 RX, 9600 | 🔧 待使能 CubeMX USART2 | 替换 stub NMEA 源 |
| 4G | Air780E | USART1: PA9 TX, PA10 RX, 115200 | 🔧 待使能 CubeMX USART1 | 替换 stub AT 响应 |
| 4G PWRKEY | Air780E | PB0 GPIO_Output | ✅ 已配置为 `AIR780E_PWRKEY` | C 组 `Power_Air780E_SetPwrKey()` |
| Flash | W25Q128 | SPI1: PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI | 🔧 待使能 CubeMX SPI1 | `W25Q128_USE_HAL_SPI=1` |
| 蜂鸣器 | Buzzer | PC8 GPIO_Output | 🔧 待使能 CubeMX | 添加 GPIO_Output |
| 外接 LED | Alarm LED | PC9 GPIO_Output | 🔧 待使能 CubeMX | 添加 GPIO_Output |

## CubeMX 外设使能状态

| 外设 | CubeMX 已使能 | 驱动代码 | 状态 |
|------|:---:|:---:|------|
| USART3 (调试) | ✅ | ✅ | 运行中 |
| GPIO (LED/按键) | ✅ | ✅ | 运行中 |
| FreeRTOS CMSIS_V2 | ✅ | ✅ | 运行中 |
| I2C1 (SHT31/PN532) | ❌ | ✅ (条件编译) | **下一步** |
| SPI1 (W25Q128) | ❌ | ✅ (条件编译) | **下一步** |
| USART2 (GPS) | ❌ | ✅ | **下一步** |
| USART1 (Air780E) | ❌ | ✅ | PB0 PWRKEY 已释放，待使能 USART1 |
| PC8 (蜂鸣器) | ❌ | ❌ | 非关键 |
| PC9 (外接LED) | ❌ | ❌ | 非关键 |

## PB0 冲突说明

- **当前配置**：PB0 = AIR780E_PWRKEY（4G 模块电源控制）
- **心跳配置**：PE1 = LD2_YELLOW，慢闪表示系统心跳，快闪表示报警
- **控制入口**：C 组 `Power_Air780E_SetPwrKey()` 统一控制 PB0
- **协作约束**：A/B 禁止单独直接操作 PB0
- **详见**：`doc-workflow/TROUBLESHOOTING.md` 第 10 条

## 当前开发阶段

**硬件集成阶段**。所有外部模块已到齐，驱动采用条件编译双模式：
- `_USE_HAL_*=0`：stub 回退，不依赖外设
- `_USE_HAL_*=1`：真实 HAL 驱动，需 CubeMX 已使能对应外设

CubeMX 外设使能是当前第一优先级任务，详见 `doc-workflow/task2.md`。
