# STM32H7A3ZI-Q 程序下载要点（Keil MDK）

## 1. 开发环境

-   Keil MDK（建议 V5.39 或以上）
-   STM32CubeMX
-   STM32H7 Device Family Pack（DFP）
-   ST-Link 驱动

## 2. CubeMX 建立工程

### 2.1 新建工程

1. 打开 **STM32CubeMX**。
2. 在首页选择 **New Project from a Board**。
3. 在搜索框中输入：

   `NUCLEO-H7A3ZI-Q`

4. 在搜索结果中选择 **NUCLEO-H7A3ZI-Q** 开发板。
5. 点击 **Start Project**，进入工程配置界面。

> 建议初学者采用 **Board** 方式创建工程，CubeMX 会自动完成板载 LED、按键等基础资源配置。

### 2.2 配置工程

1. 打开 **Project Manager**。
2. 在 **Project Name** 中输入工程名称，例如 `LED_Test`。
3. 将 **Toolchain / IDE** 设置为 **MDK-ARM**。
4. 选择工程保存路径。
5. 点击 **Generate Code** 生成工程。

### 2.3 打开工程

生成完成后，在工程目录中找到：

`LED_Test.uvprojx`

双击该文件即可使用 **Keil MDK** 打开工程。

## 3. Keil 配置

### Flash Download

选择： - STM32H7A-B3 Flash 2MB

对于 STM32H7A3，建议修改：

    RAM for Algorithm
    Start : 0x24000000
    Size  : 0x00010000

该配置可解决：

    Cannot Load Flash Programming Algorithm

错误。

## 4. 下载程序

1.  使用开发板 **ST-LINK USB** 接口连接电脑。
2.  点击 **Build（F7）** 编译。
3.  点击 **Download（Ctrl+F8）** 下载。

若出现：

    Flash Download completed

说明程序已成功烧录。

若下载完成后程序没有立即运行，可按一次 **RESET** 键启动程序。

## 5. 常见问题

### （1）Cannot Load Flash Programming Algorithm

原因： - Flash Algorithm 使用 RAM 地址不正确。

解决：

    Start : 0x24000000
    Size  : 0x00010000

### （2）ST-Link 无法识别

检查： - 是否连接 ST-LINK USB 接口 - ST-Link 驱动是否安装 - ST-Link
固件是否需要升级

## 6. 点灯程序验证

``` c
BSP_LED_Init(LED_GREEN);

while (1)
{
    BSP_LED_Toggle(LED_GREEN);
    HAL_Delay(500);
}
```

若按 RESET 后 LED 闪烁，则说明： - ST-Link 正常 - Flash 下载正常 - BSP
配置正常 - GPIO 工作正常

## 7. 学习建议

1.  LED 点灯
2.  按键中断（EXTI）
3.  USART 串口
4.  定时器（TIM）
5.  I2C（SHT31）
6.  SPI（NFC）
7.  UART（GPS）
8.  MQTT 云平台
