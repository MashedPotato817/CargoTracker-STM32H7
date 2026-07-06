# 踩坑记录

> 遇到问题解决后马上记到这里，队友 pull 后直接搜索即可，不用重复踩坑。

---

## CubeMX

### 1. HSE 晶振陷阱

**现象**：编译通过，烧录后程序卡死——串口无输出、LED 不闪。

**原因**：NUCLEO-H7A3ZI-Q 开发板**没有 HSE 外部晶振**。CubeMX 默认启用 HSE，`SystemClock_Config()` 中等待 HSE 就绪超时死循环。

**解决**：CubeMX → Clock Configuration → PLL Source Mux → 选 **HSI**（内部 64MHz RC），重新生成代码。

---

### 2. CubeMX 提示 "Open Project" 时不要点

**现象**：CubeMX 生成代码后弹出"Open Project"，点击后 STM32CubeIDE 报错打不开。

**原因**：工程是 Keil MDK 格式，CubeMX 内建 IDE 打不开。忽略弹窗，直接用 Keil 手动打开 `test1.uvprojx`。

---

## Keil MDK

### 3. Cannot Load Flash Programming Algorithm

**现象**：烧录时报 `Cannot Load Flash Programming Algorithm`。

**解决**：Keil → Flash → Configure Flash Tools → Debug → Settings → Flash Download → 点 Add → 选 STM32H7x 512KB → RAM for Algorithm:

```
Start : 0x24000000
Size  : 0x00010000
```

---

## FreeRTOS

### 4. 栈溢出（Stack Overflow）

**现象**：某个 task 跑着跑着不打印了，或进 HardFault。

**原因**：task 栈太小。注意 CubeMX 中 Stack Size 单位是 **Words（4 字节）**，256 Words = 1KB。

**建议**：
- 用 `uxTaskGetStackHighWaterMark(NULL)` 查当前 task 的剩余栈空间
- printf 大量使用时栈要更大（格式化字符串占栈）
- `Task_4G_MQTT` 和 `Task_StateMachine` 栈建议 ≥ 1KB

---

### 5. TIM6 被 FreeRTOS 占用

**现象**：CubeMX 中 TIM6 显示黄色感叹号，提示冲突。

**原因**：FreeRTOS CMSIS_V2 用 TIM6 做 HAL 时钟基准（`HAL_InitTick`）。

**解决**：正常现象，忽略即可。自己的应用代码用其他定时器（TIM2~TIM5 等）。

---

## Git

### 6. CubeMX 重新生成后文件冲突

**现象**：CubeMX 重新生成后 `git diff` 一片红，分不清哪些是配置变更、哪些是覆盖。

**原因**：CubeMX 会覆盖 `/* USER CODE BEGIN/END */` 段以外的所有内容。

**最佳实践**：
1. 改 CubeMX 前先 `git commit`（干净状态）
2. CubeMX 生成后再 `git diff` 一目了然
3. 所有自定义代码必须写在 `USER CODE` 段内
