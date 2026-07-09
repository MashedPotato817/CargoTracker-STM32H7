# ~~前端页面优化计划~~（已过期）

> ⚠️ **本文件为早期规划草稿，已过时。** 实际执行和状态见：
> - `doc-workflow/task3.3.md` — 任务追踪
> - `doc-workflow/log3.3.md` — 执行日志
> - `dashboard.html` — 实际代码
>
> 仅保留作为设计思路参考。2026-07-09

> 负责人：C | 三个页面统一风格 + 体验打磨，为答辩演示做准备

## 当前状态

| 页面 | 用途 | 状态 |
|------|------|:---:|
| `index.html` | 硬件接线指南 | 基础可用 |
| `dashboard.html` | 云端控制台 | 基础可用 |
| `report.html` | 设计报告 | 刚创建 |

## 问题诊断

### 全局问题

| # | 问题 | 影响 |
|---|------|------|
| 1 | **三个页面互相没有导航** | 用户看完接线不知道怎么去看报告/控制台 |
| 2 | **配色接近但不统一** | index 用 `--text: #9aa0b4`，dashboard 用 `--text: #c8ccd4`，report 用 `--text: #c8ccd4` |
| 3 | **移动端顶部浪费** | 标题占一整行，没利用空间放导航 |

### index.html

| # | 问题 | 优先级 |
|---|------|:---:|
| 1 | 没有返回到其他页面的入口 | 🟡 |
| 2 | 表格太宽时手机横滚不明显 | 🟢 |

### dashboard.html

| # | 问题 | 优先级 |
|---|------|:---:|
| 1 | 断线后不会自动重连 | 🔴 |
| 2 | 连接成功/断线没视觉反馈（toast） | 🟡 |
| 3 | 数据为空时显示 `--` 不够友好 | 🟡 |
| 4 | 指令按钮发完后没反馈（看不出是否成功） | 🟡 |
| 5 | CSQ 显示 `--`（设备没发 csq 字段，但用户不知道） | 🟡 |

### report.html

| # | 问题 | 优先级 |
|---|------|:---:|
| 1 | TOC 目录不跟随滚动 | 🟡 |
| 2 | 打印时暗色主题费墨 | 🔴 提交 PDF 需要 |
| 3 | 照片占位符太单调 | 🟡 |

---

## 优化方案

### Phase 1: 答辩必做（🔴 决赛前必须）

#### 1.1 三页面统一导航栏

每个页面顶部加同一导航：

```
┌──────────────────────────────────────┐
│ CargoTracker  📡接线  📊控制台  📄报告 │
└──────────────────────────────────────┘
```

实现：每个页面 `<head>` 中加相同的 `<nav>` 组件，当前页高亮。

```html
<nav class="top-nav">
  <a href="index.html" class="nav-brand">CargoTracker</a>
  <div class="nav-links">
    <a href="index.html">接线</a>
    <a href="dashboard.html">控制台</a>
    <a href="report.html">报告</a>
  </div>
</nav>
```

#### 1.2 dashboard 自动重连

MQTT 断线后自动尝试重连，带退避策略：

```js
client.on('close', () => {
  dot.className = 'dot offline';
  statusText.textContent = '重连中...';
  setTimeout(() => client.reconnect(), 3000);  // 3s 后退避重连
});
```

#### 1.3 report 打印样式

加 `@media print` 切换到白底黑字，隐藏导航：

```css
@media print {
  body { background: #fff; color: #000; }
  .top-nav, .toc { display: none; }
  pre { background: #f5f5f5; border: 1px solid #ccc; }
}
```

### Phase 2: 锦上添花（🟡 时间允许）

#### 2.1 统一 CSS 变量

提取共享变量到每个页面：

```css
:root {
  --bg: #0f1117; --card: #161822; --border: #252836;
  --text: #9aa0b4; --head: #e0e4f0; --accent: #4da6ff;
  --good: #34d399; --warn: #f59e0b; --bad: #ef4444; --dim: #6b7280;
}
```

#### 2.2 dashboard 指令按钮反馈

点击后按钮短暂变色 + 日志显示：

```js
function sendCmd(cmd) {
  const btn = event.target;
  btn.style.opacity = '0.5';
  setTimeout(() => btn.style.opacity = '1', 300);
  client.publish(TOPIC_CMD, JSON.stringify({ cmd, ts: Date.now()/1000|0 }));
  addLog('cmd', '下发: ' + cmd);
}
```

#### 2.3 dashboard 空状态提示

当 CSQ/bat 无数据时，用灰色文字代替 `--`：

```
"等待设备上报" 而不是 "--"
```

#### 2.4 report TOC 跟随

```css
.toc { position: sticky; top: 8px; z-index: 5; }
```

### Phase 3: 可选（🟢 有多余时间）

| 事项 | 说明 |
|------|------|
| PWA 离线缓存 | Service Worker 缓存，断网也能看 |
| 暗色/亮色切换 | toggle 按钮 |
| 移动端表格横滚提示 | 箭头或阴影提示可横滚 |

---

## 实施顺序

```
1. 统一导航栏（三个文件各加 ~15 行）
2. dashboard 自动重连（~5 行 JS）
3. report 打印样式（~15 行 CSS）
4. dashboard 按钮反馈（~5 行 JS）
5. 统一 CSS 变量
```

预计总工作量：~1 小时。

---

## 文件清单

| 文件 | 改动 |
|------|------|
| `index.html` | +导航栏，+统一 CSS 变量 |
| `dashboard.html` | +导航栏，+自动重连，+按钮反馈，+空状态提示 |
| `report.html` | +导航栏，+打印样式，+TOC sticky |
