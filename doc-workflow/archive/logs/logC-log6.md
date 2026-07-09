# C组应用集成开发日志 6

> 日期：2026-07-09  
> 范围：云端指令声光动作异步化

## 本次问题

现场现象：点击“暂留”后，LD2 黄灯可常亮，但继续点击“退货”或“继续运输”没有明显反应。

## 根因

云端命令由 `Task_4G_MQTT` 放入 `queue_cloud_cmd`，但真正执行动作依赖状态机进入 `STATE_WAIT_CLOUD` 后再读取队列。

这个设计有两个问题：

1. `STATE_WAIT_CLOUD` 实际窗口只有 2s，用户在窗口外点击命令时不会立即执行。
2. 状态机每次上传前会清空 `queue_cloud_cmd`，窗口外收到的命令可能被清掉。

因此暂留以后再次点击其他按钮时，MQTT 可能已经收到命令，但应用层声光动作没有稳定落地。

## 修复内容

文件：

- `test1/Core/Src/app/app.c`
- `doc-workflow/task3.3.md`

代码侧：

- `App_Task4GMQTT()` 收到 `HOLD` / `RETURN` / `CONTINUE` 后立即调用 `handle_cloud_command()`。
- `STATE_WAIT_CLOUD` 不再消费 `queue_cloud_cmd`，只保留状态显示和 NFC 关机检查。
- 删除上传前清空 `queue_cloud_cmd` 的动作，避免误删窗口外收到的命令。
- 传感器采样、GPS 定位、MQTT 上报、Flash 缓存流程保持不变。

声光动作：

- `HOLD`：LD2 黄灯常亮，蜂鸣器关闭。
- `RETURN`：LD2 黄灯快闪，蜂鸣器响 15s。
- `CONTINUE`：LD2 黄灯恢复正常心跳闪烁，蜂鸣器关闭。

## 验证建议

用户本轮自行 Keil 编译。烧录后建议按以下顺序看串口和硬件：

1. NFC 激活，等待 `[MQTT] subscribed cargo/cmd`。
2. dashboard 点击“暂留”：期望打印 `cloud action: HOLD`，LD2 黄灯常亮。
3. 不重新刷 NFC，直接点击“退货”：期望打印 `cloud action: RETURN`，LD2 快闪且蜂鸣器响 15s。
4. 再点击“继续运输”：期望打印 `cloud action: CONTINUE`，LD2 恢复正常心跳闪烁，蜂鸣器关闭。
5. 观察 SHT31 / GPS / MQTT telemetry 日志仍按原周期继续输出。
