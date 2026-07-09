# Task 3.5 执行日志 — 地图实时定位显示

> 2026-07-09 创建 | 负责人：C

## 目标

dashboard.html 嵌入 Leaflet 地图，接收硬件 MQTT 上报的 GPS 经纬度后实时标记设备位置。

## 方案选型

| 对比 | Leaflet + CartoDB | 高德/百度 |
|------|:---:|:---:|
| API Key | 不需要 | 需要注册 |
| 坐标系 | WGS-84（GPS 原生） | GCJ-02（需转换，有偏移） |
| JS 体积 | ~42KB | ~500KB+ |
| GitHub Pages | 即用 | 需 Key 绑定域名 |

选 Leaflet.js + CartoDB Dark Matter 瓦片：零注册、坐标直出、暗色主题匹配 dashboard。

## 实现

改动全部在 `dashboard.html`（单文件，+88 行）：

### 依赖

```html
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
```

### 地图配置

| 参数 | 值 | 说明 |
|------|-----|------|
| 初始视野 | `[31.2, 120.5]` zoom 9 | 长三角居中 |
| 范围锁定 | `[[27,116],[35.5,123]]` | 华东（江浙沪+缓冲区） |
| 最小缩放 | `minZoom: 7` | 不缩到全国 |
| 边界弹性 | `maxBoundsViscosity: 0.8` | 拖出界有阻力，不硬卡 |
| 闪烁修复 | `fadeAnimation: false` | 瓦片直接显示，无白色闪块 |
| 即加载 | `updateWhenIdle: false` | 拖动时立即加载瓦片 |
| 范围外 | `noWrap: true` | 不加载边界外图块 |

### 地图行为

- 无 GPS 数据 → 默认华东视野
- 首次收到有效坐标 → 飞行到设备位置（zoom 15）+ 蓝色 CircleMarker
- 后续更新 → marker `setLatLng()` 平滑跟踪
- `gps_valid=0` → marker 变灰色
- hover → tooltip "设备位置"

### 暗色适配

- CartoDB Dark Matter 瓦片（`dark_all`）
- Leaflet zoom 控件底色覆盖为 `var(--card)`
- attribution 文字暗色处理

## 验证

- [ ] 打开 dashboard.html → 地图显示华东范围，暗色主题
- [ ] 硬件上电 → MQTT 上报 lat/lon → 地图自动飞到设备位置 + 蓝色标记
- [ ] gps_valid=0 → marker 变灰
- [ ] 拖拽/缩放 → 瓦片无闪烁延迟
- [ ] 手机端 → 地图可触摸拖动，不卡页面滚动

## 文件变更

| 文件 | 操作 | 说明 |
|------|:---:|------|
| `dashboard.html` | 修改 | +88 行（CDN + CSS + HTML + JS） |
| `doc-workflow/task3.5.md` | 新建 | 方案设计与实现文档 |
| `doc-workflow/task3.md` | 修改 | 新增 Task 3.5 条目 |

## 参考

- 分支：`feat/dashboard-map`
- Leaflet 文档：https://leafletjs.com/reference.html
- CartoDB Dark Matter：https://carto.com/basemaps/

---

## 2026-07-09 地图显示优化收口

### 背景

实际页面截图中，Leaflet 地图右侧和底部出现未铺满色块，地图瓦片只覆盖左上区域。页面其余 MQTT 数据和连接状态正常，问题集中在 dashboard 地图渲染。

### 修复

文件：`dashboard.html`

改动：
- 地图容器改为 `width: 100%; height: 260px`，增加轻边框，确保卡片内铺满。
- 瓦片源从 CartoDB Dark Matter retina 写法改为 OpenStreetMap 标准瓦片。
- 使用 `.leaflet-tile` CSS filter 做暗色适配，避免瓦片源/retina 参数导致区域缺块。
- 增加 `refreshMapSize()`，在页面加载、窗口 resize 和坐标更新时调用 `map.invalidateSize()`。
- telemetry 未带 `gps_v/gps_valid` 时，只要 `lat/lon` 有效即允许更新地图。
- 后续坐标更新时使用 `map.panTo()`，保持 marker 跟随体验。

### 验证结果

- [x] 打开 dashboard.html 后，地图完整显示长三角区域。
- [x] 原先右侧/下方色块消失，瓦片铺满地图容器。
- [x] 页面连接状态保持 `已连接`。
- [x] 温湿度和最后上报时间继续正常刷新。
- [x] `gps_valid=0` 时显示 `定位 No`，地图保留默认视野，不误放 marker。
- [ ] 真实 GPS 有效定位后 marker 移动仍需硬件输出非 0 经纬度后最终确认。

### 结论

Task 3.5 的页面显示问题已修复，dashboard 地图可用于答辩截图和云端控制台演示。后续只需等待 GPS 输出有效经纬度，再做 marker 跟随的硬件闭环验证。
