# Task 4: 低功耗优化

> 硬件集成完成后进行。🔴必做 🟡加分 🟢可选

---

## 🔴 Task 4.1: Stop Mode 实现

**负责人：C** | 预计：1h

替换 `Power_EnterStopStub()` 为真实 HAL STOP：

```c
void Power_EnterStop(void) {
    HAL_SuspendTick();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
    SystemClockConfig_AfterWakeup();
    HAL_ResumeTick();
}
```

唤醒：NFC 中断（PC13 EXTI）。

> 这是低功耗核心功能，必须实现。论文需要这章。

---

## 🔴 Task 4.2: 功耗测量

**负责人：C** | 预计：0.5h

万用表串联在 3.3V 供电回路，分别测：

| 模式 | 记录 |
|------|------|
| 全速运行 | ___ mA |
| Stop Mode | ___ μA |

> 两个数字是论文和 PPT 的硬数据，必须测。

---

## 🟡 Task 4.3: 4G 空闲断电

**负责人：C** | 预计：1h

上传前 PWRKEY 开机 → 上传后 PWRKEY 关机。

> 显著延长续航，但不做也能答辩（"4G 在线功耗符合预期"一笔带过）。

---

## 🟢 Task 4.4: 电池续航估算

根据实测电流计算理论续航，论文中放一张估算表。

> 有数字加分，没时间可用计算值替代。
