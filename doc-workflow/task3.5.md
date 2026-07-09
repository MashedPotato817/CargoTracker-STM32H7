# Task 3.5: 地图实时定位显示

> 🟡 加分 | 负责人：C | 依赖：Task 3.3 已基本完成，GPS 数据经 MQTT 上报

## 目标

dashboard.html 中嵌入实时地图，每次收到 telemetry 数据时自动更新设备位置标记。

```
硬件 GPS → MQTT cargo/telemetry → dashboard 收到 lat/lon → 地图 marker 移动
```

## 方案选型

**Leaflet.js + OpenStreetMap**（选定）

| 对比 | Leaflet + OSM | 高德/百度 |
|------|:---:|:---:|
| API Key | ❌ 不需要 | ✅ 需要注册 |
| 坐标系统 | WGS-84（=GPS 原生） | GCJ-02（需转换） |
| JS 体积 | ~42KB | ~500KB+ |
| GitHub Pages | 即用 | 需 Key 绑定域名 |

> ATGM336H 输出 WGS-84，Leaflet 直接用，无偏移问题。

## 设计

### UI 布局

dashboard 现有卡片顺序：温湿度 → GPS 数字 → 连接状态 → 指令 → 日志

改动：**GPS 数字卡片替换为地图卡片**，经纬度数值内嵌到地图上方的 info bar。

```
┌─────────────────────────────┐
│ 实时环境   25°C  49%        │
├─────────────────────────────┤
│ GPS 定位   lat:31.xxxx      │
│            lon:121.xxxx     │
│ ┌─────────────────────────┐ │
│ │                         │ │
│ │       Leaflet 地图       │ │
│ │      📍 设备位置         │ │
│ │                         │ │
│ └─────────────────────────┘ │
├─────────────────────────────┤
│ 连接状态   CSQ 28  刚刚     │
├─────────────────────────────┤
│ 云端指令 [暂留][退货][继续] │
├─────────────────────────────┤
│ 数据日志                    │
└─────────────────────────────┘
```

### 地图行为

| 场景 | 行为 |
|------|------|
| 首次收到坐标 | 地图以该点为中心，zoom 15，放 marker |
| 后续坐标更新 | marker 平滑移动到新位置（`setLatLng`） |
| 坐标未变化 | marker 不动 |
| 坐标无效（gps_valid=0） | marker 变灰色，不移动 |
| 无坐标数据 | 地图显示默认视野（中国居中），提示"等待定位…" |

### marker 设计

```
默认：蓝色圆点 + 脉冲波纹（CSS animation）
有效定位：蓝色实心
无效定位：灰色空心
hover：显示 "设备位置" tooltip
```

### 移动端适配

- 地图宽度固定跟随卡片 `width: 100%`
- 地图高度：260px，保证答辩截图中地图信息足够完整
- 触摸拖动正常，不额外拦截页面滚动

## 实现要点

### 依赖引入

```html
<!-- Leaflet CSS + JS，从 CDN 加载 -->
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
```

> 约 42KB gzip，CDN 加载，无需本地文件。

### 核心 JS 逻辑

```js
// 初始化地图（默认视野长三角居中）
const map = L.map('map-container', {
  attributionControl: false,
  zoomControl: true,
  preferCanvas: true,
  fadeAnimation: false,
  maxBounds: [[27.0, 116.0], [35.5, 123.0]],
  maxBoundsViscosity: 0.8,
  minZoom: 7,
}).setView([31.2, 120.5], 9);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
  maxZoom: 19,
  updateWhenIdle: false,
  keepBuffer: 3,
  crossOrigin: true,
}).addTo(map);

// 设备 marker
let deviceMarker = null;
let firstFix = true;

function updateMap(lat, lon, valid) {
  if (!Number.isFinite(lat) || !Number.isFinite(lon)) {
    return;
  }

  if (!valid) {
    if (deviceMarker) deviceMarker.setStyle({ fillColor: '#6b7280', color: '#6b7280' });
    return;
  }

  const pos = [lat, lon];

  if (!deviceMarker) {
    // 首次：创建蓝色圆点 marker
    deviceMarker = L.circleMarker(pos, {
      radius: 8,
      fillColor: '#4da6ff',
      color: '#fff',
      weight: 2,
      fillOpacity: 0.9,
    }).addTo(map);
  } else {
    deviceMarker.setLatLng(pos);
    deviceMarker.setOpacity(0.9);
  }

  if (firstFix) {
    map.setView(pos, 15, { animate: true });
    firstFix = false;
  } else {
    map.panTo(pos, { animate: true, duration: 0.4 });
  }
}
```

### 在现有 updateUI() 中接入

```js
function updateUI(d) {
  // ... 现有温湿度、CSQ 更新不变 ...

  if (d.lat !== undefined && d.lon !== undefined) {
    const valid = (d.gps_v !== undefined ? d.gps_v : d.gps_valid);
    const hasValidityFlag = valid !== undefined && valid !== null;
    updateMap(Number(d.lat), Number(d.lon), hasValidityFlag ? (valid === 1 || valid === true) : true);
    // 同时更新地图上方的经纬度文字
    document.getElementById('lat-val').textContent = d.lat.toFixed(4);
    document.getElementById('lon-val').textContent = d.lon.toFixed(4);
  }
}
```

### 暗色主题适配

Leaflet 默认白色底，需要 CSS 覆盖匹配 dashboard 暗色风格：

```css
/* 地图容器 */
#map-container {
  width: 100%;
  height: 260px;
  border-radius: 8px;
  overflow: hidden;
}

/* 暗色瓦片：使用 OSM 标准瓦片 + CSS filter */
.leaflet-tile {
  filter: brightness(0.55) contrast(1.15) saturate(0.55) invert(1) hue-rotate(180deg);
}
```

> 实测 CartoDB retina 瓦片写法在当前页面中容易出现右侧/下方色块；已改为 OpenStreetMap 标准瓦片并用 CSS 滤镜适配暗色主题。

### 地图刷新修复

Leaflet 在窄卡片内初始化时，可能先按旧容器尺寸计算瓦片，导致地图只加载左上部分。当前代码新增：

```js
function refreshMapSize() {
  requestAnimationFrame(() => {
    map.invalidateSize({ pan: false });
    setTimeout(() => map.invalidateSize({ pan: false }), 200);
  });
}

window.addEventListener('load', refreshMapSize);
window.addEventListener('resize', refreshMapSize);
```

在页面加载、窗口变化、坐标更新时刷新 Leaflet 尺寸，避免地图区域出现未铺满色块。

### 轨迹线（可选）

如果设备移动中上报多个点，可以用 `L.polyline` 画出历史轨迹：

```js
const trailPoints = [];
const trail = L.polyline([], { color: '#4da6ff', weight: 2, opacity: 0.6 }).addTo(map);

trailPoints.push(pos);
trail.setLatLngs(trailPoints);
```

> Phase 1 不做，留到后面。

## 文件变更

| 文件 | 操作 | 行数估计 |
|------|:---:|:---:|
| `dashboard.html` | 修改 | 地图 UI + Leaflet 尺寸刷新 + OSM 暗色滤镜 |

无需新建文件，全部在 dashboard.html 内完成。

## 验证

- [x] 打开 dashboard.html → 地图完整铺满卡片，无右侧/底部色块
- [x] MQTT 页面保持连接，截图中显示 `已连接`
- [x] Telemetry 上报温湿度后，实时环境和最后上报时间正常更新
- [x] `gps_valid=0` 时页面显示 `定位 No`，地图保留默认长三角视野
- [ ] 真实 GPS 有效定位后 marker 跳转/跟随移动（待硬件 GPS 输出有效经纬度）

> 2026-07-09 页面截图确认：地图已完整显示长三角区域，原先右侧/下方 Leaflet 背景色块消失。

## 参考

- Leaflet.js 文档：https://leafletjs.com/reference.html
- OpenStreetMap Tile Usage：https://operations.osmfoundation.org/policies/tiles/
