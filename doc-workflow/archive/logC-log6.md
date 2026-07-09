# C组应用集成开发日志 6

> 日期：2026-07-09
> 范围：云端下发指令动作、LED2/报警模式、GPS 坐标精度

## 本次目标

1. 云端下发「暂留」后，LED2 从闪烁变为常亮。
2. 云端下发「退货」后，触发本地报警，并让 LED2 闪烁速度加快。
3. 云端下发「继续运输」后，LED2 恢复正常心跳闪烁。
4. 修复 GPS 坐标精度问题，避免经纬度小数被损失。

## 代码改动

文件：
- `test1/Core/Inc/app/alarm.h`
- `test1/Core/Src/app/alarm.c`
- `test1/Core/Src/app/app.c`
- `test1/Core/Src/gps/gps.c`

改动：
- 新增 `AlarmLedMode`：
  - `ALARM_LED_MODE_NORMAL`：正常心跳闪烁。
  - `ALARM_LED_MODE_HOLD_ON`：LED2 常亮。
  - `ALARM_LED_MODE_FAST_BLINK`：LED2 快闪。
- 新增 `Alarm_SetLedMode()`，由状态机根据云指令切换 LED2 模式。
- `HOLD`：调用 `Alarm_SetActive(0)` + `Alarm_SetLedMode(ALARM_LED_MODE_HOLD_ON)`。
- `RETURN`：调用 `Alarm_SetActive(1)` + `Alarm_SetLedMode(ALARM_LED_MODE_FAST_BLINK)`，蜂鸣器随快闪鸣叫。
- `CONTINUE`：调用 `Alarm_SetActive(0)` + `Alarm_SetLedMode(ALARM_LED_MODE_NORMAL)`。
- `STATE_WAIT_CLOUD` 现在实际等待 `APP_CLOUD_WAIT_MS = 30000U`，不再只等待 2s；等待期间仍检查 NFC 关机。
- `queue_cloud_cmd` 改为上传前清空旧命令，上传期间和后续 30s 窗口收到的云命令都会保留给本轮处理。
- `gps.c` 删除 `atof()` 坐标解析，改为按 NMEA `ddmm.mmmm` / `dddmm.mmmm` 定点换算到微度，再输出到 `GpsLocation`。

## 验证结果

已完成：

```powershell
gcc -fsyntax-only -std=c99 -Wall -Wextra -DSTM32H7A3xxQ -DUSE_HAL_DRIVER ...
```

结果：
- `test1/Core/Src/app/alarm.c`：语法检查通过。
- `test1/Core/Src/app/app.c`：语法检查通过。
- `test1/Core/Src/gps/gps.c`：语法检查通过。
- MinGW/x86 环境仍会对 CMSIS 头文件和队列指针转换给出指针宽度 warning，属于本地检查环境噪声，不是本轮逻辑错误。

未完成：
- 当前工作区缺少 `test1/test1.ioc`。
- 当前工作区缺少 `test1/MDK-ARM/test1.uvprojx`。
- 因此本轮无法运行 `E:\App\Keil_v5\UV4\UV4.exe` 做完整 Keil 构建。

## 硬件验证建议

1. 恢复/生成 `test1.ioc` 和 `test1.uvprojx` 后先 Keil Rebuild，目标 0 Error。
2. 烧录后激活设备，确认串口出现 `[MQTT] subscribed cargo/cmd`。
3. 在 dashboard 点击「暂留」，期望串口出现 `cloud action: HOLD`，LED2 常亮。
4. 点击「退货」，期望串口出现 `cloud action: RETURN`，蜂鸣器报警，LED2 快闪。
5. 点击「继续运输」，期望串口出现 `cloud action: CONTINUE`，LED2 恢复正常心跳闪烁。
6. 观察 MQTT payload 中 `lat/lon` 是否保留 6 位小数，例如 `31.230400` / `121.473700`。
