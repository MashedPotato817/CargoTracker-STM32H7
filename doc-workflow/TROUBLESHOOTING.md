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

### 3. CubeMX 重新生成后必查清单

每次 CubeMX **GENERATE CODE** 之后，Keil 编译前检查以下 4 项（CubeMX 可能还原或改错）：

**① system_stm32h7xx.c 路径**

CubeMX 可能把该文件放在 `Drivers/CMSIS/.../Templates/` 下，但 uvprojx 引用的是 `Core/Src/`。编译报 `no such file or directory` 时，在 uvprojx 中把 FilePath 改为实际路径：
```
../Drivers/CMSIS/Device/ST/STM32H7xx/Source/Templates/system_stm32h7xx.c
```

**② FreeRTOS Include Path**

CubeMX 会还原为默认的 `RVDS/ARM_CM4F`。确认 Include Path 是：
```
../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
```

**③ Flash 烧录算法**

CubeMX 可能改 `<Flash2>` 为 `UL2CM3.DLL`（Cortex-M3 用）。确认是：
```xml
<Flash2>BIN\UL2V8M.DLL</Flash2>
```

**④ MicroLIB**

确认 Options for Target → Target → **Use MicroLIB** 已勾选（printf 依赖）。

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

### 6. Task Stack Size 最小 128 Words

**现象**：CubeMX 任务栈大小字段无法输入小于 128 的值。

**原因**：FreeRTOS CMSIS_V2 强制最小 task 栈为 128 Words（512 字节）。

**解决**：所有 task 栈均为 ≥ 128 Words。即使简单 task（如 LED 闪烁）也填 128，512B 对 STM32H7 的 1.4MB RAM 来说微不足道。

---

### 7. ARM Compiler 6 + FreeRTOS RVDS port = 177 个编译错误

**现象**：编译报大量 `__forceinline` / `__asm` unknown type / `PRESERVE8` undeclared 错误（~177 个），所有 FreeRTOS 源文件都报。

**原因**：CubeMX 默认添加 FreeRTOS 的 `RVDS` port（ARM_CM4F 或 ARM_CM7），RVDS = ARM Compiler 5 (armcc) 语法。Keil 如果选了 **ARM Compiler 6 (V6.x / armclang)**，不认 `__forceinline`、`__asm` 等 AC5 关键字。

**解决**：改用 `GCC/ARM_CM7` port（armclang 兼容 GCC 语法）。

三步操作（以 STM32H7 为例）：

1. **复制 GCC port 文件** — 从 `STM32Cube_FW_H7_Vx.x.x/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/` 复制 `port.c` + `portmacro.h` 到工程的同路径 `portable/GCC/ARM_CM7/r0p1/` 下

2. **修改 uvprojx**（或 Keil 界面操作）：
   - Include Path：`RVDS\ARM_CM7\r0p1` → `GCC\ARM_CM7\r0p1`
   - port.c 文件引用：`RVDS\ARM_CM7\r0p1\port.c` → `GCC\ARM_CM7\r0p1\port.c`

3. **Keil 重开工程 → Rebuild all**

**验证**：编译 0 错误。

> 注意 1：H7 是 Cortex-M7，ARM_CM7 里还有 `r0p1` 子文件夹（区分内核版本），Include Path 要写到底层 `r0p1` 目录。  
> 注意 2：如果 CubeMX 重新生成代码，可能会恢复 Include Path，需要重新改一回。可在 CLAUDE.md 中注明此操作。

---

## Git

### 8. CubeMX 重新生成后文件冲突

**现象**：CubeMX 重新生成后 `git diff` 一片红，分不清哪些是配置变更、哪些是覆盖。

**原因**：CubeMX 会覆盖 `/* USER CODE BEGIN/END */` 段以外的所有内容。

**最佳实践**：
1. 改 CubeMX 前先 `git commit`（干净状态）
2. CubeMX 生成后再 `git diff` 一目了然
3. 所有自定义代码必须写在 `USER CODE` 段内

---

### 9. FPU 未启用导致浮点运算性能差

**现象**：SHT31 温度转换、GPS 坐标解析等浮点运算极慢。

**原因**：`FreeRTOSConfig.h` 中 `configENABLE_FPU=0`，Cortex-M7 硬件 FPU 被禁用，所有 float 运算走软件模拟。

**解决**：

```c
// FreeRTOSConfig.h
#define configENABLE_FPU    1   // 启用硬件 FPU
#define configENABLE_MPU    0   // 保持禁用（FreeRTOS MPU wrapper 未使用）
```

> 注意：CubeMX 可能不会自动打开此项，需手动修改 FreeRTOSConfig.h。

---

### 10. PB0 引脚冲突：LD1_GREEN vs Air780E PWRKEY

**现象**：硬件到齐后发现 PB0 同时用于心跳灯（LD1_GREEN）和 Air780E 4G 模块电源控制（PWRKEY）。

**原因**：stub 阶段 PB0 被配置为 LD1_GREEN 指示灯，但设计文档中 PB0 是 Air780E PWRKEY。

**解决**：
1. 心跳灯迁移到 PE1（LD2_YELLOW），通过不同闪烁模式区分心跳（慢闪）和报警（快闪）
2. PB0 释放为 Air780E PWRKEY（GPIO_Output）
3. C 组负责统一修改 CubeMX 并协调 A/B 组

> 注意：此变更必须由 C 组统一操作，A/B 组禁止单独使用 PB0。
