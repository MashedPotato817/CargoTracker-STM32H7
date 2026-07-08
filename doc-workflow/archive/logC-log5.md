# C组应用集成开发日志 5

> 日期：2026-07-09
> 范围：Air780E PWRKEY 初始电平问题复盘、状态文档同步

## 本次问题

加 PWRKEY 代码后，Air780E 所有 AT 指令 timeout，模块完全无响应。

## 根因

CubeMX 生成/修复 GPIO 初值时，如果把 PB0（`AIR780E_PWRKEY`）和 `LD3_RED` 一起初始化为 LOW：

```c
HAL_GPIO_WritePin(GPIOB, AIR780E_PWRKEY_Pin|LD3_RED_Pin, GPIO_PIN_RESET);
```

则 PB0 会被持续拉低，相当于 PWRKEY 一直接 GND，Air780E 被保持在关机/按键态。

这里的关键误区是：LED 的 LOW 可以表示灭灯，但 PWRKEY 的 LOW 表示按下。复用 PB0 后，初始电平语义完全相反。

## 正确修复

`gpio.c` 中应把 LED 和 PWRKEY 初值分开：

```c
HAL_GPIO_WritePin(GPIOB, LD3_RED_Pin, GPIO_PIN_RESET);
HAL_GPIO_WritePin(GPIOB, AIR780E_PWRKEY_Pin, GPIO_PIN_SET);
```

当前源码已符合该写法：

- `test1/Core/Src/gpio.c`：PB0 默认 `GPIO_PIN_SET`，PWRKEY 释放
- `test1/Core/Inc/main.h`：PB0 已命名为 `AIR780E_PWRKEY_Pin`
- `test1/Core/Src/app/power.c`：`Power_Air780E_SetPwrKey(1)` 拉低按下，`0` 拉高释放
- `test1/Core/Src/app/app.c`：上传前保持 PWRKEY 释放，不再把 `1` 当作开机态

## 文档同步

已同步以下文档状态：

- `doc-workflow/TROUBLESHOOTING.md`：补充 AT timeout 现象、PWRKEY 初值坑、正确写法
- `doc-workflow/current_status.md`：PB0 从“冲突未解决”改为“PWRKEY 已释放，待硬件 AT 验证”
- `doc-workflow/hardware_map.md`：4G/PWRKEY 状态改为待硬件 AT 验证
- `doc-workflow/task2.md`：解除 Task 2.5 的 PB0 前置阻塞

## 后续验证

1. 重新烧录当前固件。
2. 上电后确认 PB0 默认 HIGH。
3. 对 Air780E 重新执行 AT 指令序列：`AT` → `ATE0` → `AT+CSQ` → `AT+CREG?` → `AT+CGATT?`。
4. 若仍 timeout，优先复查 PB0 初值、Air780E 独立 4.0V 供电、USART1 PA9/PA10 交叉接线。
