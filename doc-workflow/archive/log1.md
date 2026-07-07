# Task 1 执行日志 — FreeRTOS 骨架 + printf + 按键

> 执行时间：2026-07-06
> 对应 task1.md 第 1 步（1-1 ~ 1-15）

---

## 完成情况

| 步骤 | 内容 | 状态 |
|------|------|------|
| 1-1 ~ 1-10 | CubeMX 配置 FreeRTOS + 6 Task + 3 Queue | ✅ |
| 1-11 | printf 重定向到 USART3 | ✅ |
| 1-12 | 6 个 Task 心跳代码 | ✅ 串口验证通过 |
| 1-13 | 按键中断回调 | ✅ 代码就绪，硬件问题待解决 |
| 1-14 | 编译 + 烧录验证 | ✅ 心跳正常，按键未验证 |
| 1-15 | git 提交 | ⏳ |

---

## 关键修复记录

### ARM Compiler 6 + FreeRTOS RVDS port 编译错误

**现象**：177 个 `__forceinline` / `__asm` 编译错误

**原因**：CubeMX 默认 FreeRTOS 使用 `RVDS/ARM_CM4F` port（ARM Compiler 5 语法），Keil 使用 AC6 (armclang) 不兼容

**解决**：改用 `GCC/ARM_CM7/r0p1` port：
1. 从 `STM32Cube_FW_H7/Middlewares/.../portable/GCC/ARM_CM7/r0p1/` 复制 `port.c` + `portmacro.h`
2. uvprojx 修改 Include Path 和 port.c 引用为 `GCC\ARM_CM7\r0p1`
3. Rebuild → 0 Error

详见 `doc-workflow/TROUBLESHOOTING.md` 第 7 条。

### printf / queue_activationHandle 编译错误

| 错误 | 原因 | 修复 |
|------|------|------|
| `freertos.c` 中 `printf` 未声明 | 缺少 `#include <stdio.h>` | 在 `freertos.c` USER CODE Includes 区添加 |
| `main.c` 中 `queue_activationHandle` 未定义 | 变量声明在 `freertos.c` 中 | 在 `main.c` 加 `extern osMessageQueueId_t queue_activationHandle;` |

---

## 当前代码状态

### freertos.c — 6 个 Task 心跳

| Task | 打印 | 周期 | USER CODE 段 |
|------|------|------|-------------|
| StartTask_StateMachine | `[StateMachine] heartbeat` | 1s | `Begin/End StartTask_StateMachine` |
| StartTask_4G_MQTT | `[4G_MQTT] heartbeat` | 2s | `Begin/End StartTask_4G_MQTT` |
| StartTask_I2C_Sensors | `[I2C_Sensors] heartbeat` | 3s | `Begin/End StartTask_I2C_Sensors` |
| StartTask_GPS | `[GPS] heartbeat` | 3s | `Begin/End StartTask_GPS` |
| StartTask_Flash | `[Flash] heartbeat` | 5s | `Begin/End StartTask_Flash` |
| StartTask_Alarm | `[Alarm] heartbeat` | 5s | `Begin/End StartTask_Alarm` |

### main.c — printf + EXTI 回调

- **USER CODE BEGIN 0**：`fputc` 重定向 printf → USART3
- **USER CODE BEGIN Includes**：`#include <stdio.h>`
- **USER CODE BEGIN PV**：`extern osMessageQueueId_t queue_activationHandle;`
- **USER CODE BEGIN 4**：`HAL_GPIO_EXTI_Callback` — 按下 PC13 时向队列发事件 + 打印

---

## PC13 按键问题

**现象**：按按键不触发中断，轮询也读不到低电平。通过 MODER/IDR 寄存器验证：

```
GPIOC->MODER = 0xF3FFFFFF  → PC13[27:26]=00 输入模式 ✅
GPIOC->IDR   = 0x00002000  → bit13 始终为 1，按按键不变 ❌
```

**排查过程**：
- CubeMX 配置：`GPIO_MODE_IT_FALLING` + `GPIO_PULLUP` ✅
- HAL 底层 EXTI 路由：`SYSCFG->EXTICR` / `EXTI->FTSR1` / `EXTI_D1->IMR1` ✅
- 备份域释放：`HAL_PWR_EnableBkUpAccess()` 无效
- 万用表测按键触点：正常通断

**结论**：PC13 到按键的板子硬件连接有问题（走线断路或引脚虚焊）。代码本身正确，按键回调已就位，等硬件修复后即可验证。

---

## 后续步骤

1. 排查/修复 PC13 硬件连接
2. 按键验证通过 → 进入 task1.md 第 2 步（状态机 + 应用层）
3. git commit
