# 硬件速查卡

## MCU

- STM32H7A3ZI (Cortex-M7, 280MHz)
- NUCLEO-H7A3ZI-Q 开发板
- **无外部 HSE 晶振！时钟源为 HSI 64MHz**

## 引脚速查

| 外设 | 引脚 |
|------|------|
| USART3 (调试) | PD8 TX, PD9 RX |
| USART1 (Air780E) | PA9 TX, PA10 RX |
| USART2 (GPS) | PD5 TX, PD6 RX |
| I2C1 (SHT31+PN532) | PB8 SCL, PB9 SDA |
| SPI1 (W25Q128) | PA4 CS, PA5 SCK, PA6 MISO, PA7 MOSI |
| 按键 | PC13 EXTI |
| LD1 绿灯 | PE1 心跳（PB0 已释放给 Air780E PWRKEY） |
| LD2 黄灯 | PE1 报警 |
| LD3 红灯 | PB14 上传 |
| PWRKEY (4G) | PB0 |
| 蜂鸣器 | PC8 |
| 外接LED | PC9 |

## 电源

```
USB 5V → TP4056 → 18650 → 3.3V (STM32+传感器) + 4.0V (Air780E)
```

## 时钟

- HSI 64MHz → PLL → 280MHz SYSCLK
- APB1: 140MHz, APB2: 140MHz
