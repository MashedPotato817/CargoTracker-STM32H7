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

- 地图高度：240px（桌面）/ 200px（手机）
- 触摸拖动正常
- 不拦截页面滚动（`dragging: false` 时 fallback）

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
// 初始化地图（默认视野中国居中）
const map = L.map('map-container', {
  attributionControl: false,  // 简洁，去掉 OSM 版权
  zoomControl: true,
}).setView([35.86, 104.19], 5);

L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
  maxZoom: 19,
}).addTo(map);

// 设备 marker
let deviceMarker = null;
let firstFix = true;

function updateMap(lat, lon, valid) {
  if (!valid) {
    // 定位无效：marker 变灰
    if (deviceMarker) deviceMarker.setOpacity(0.4);
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
    map.setView(pos, 15);  // 首次定位：飞到设备位置
    firstFix = false;
  }
}
```

### 在现有 updateUI() 中接入

```js
function updateUI(d) {
  // ... 现有温湿度、CSQ 更新不变 ...

  if (d.lat !== undefined && d.lon !== undefined) {
    const valid = (d.gps_v !== undefined ? d.gps_v : d.gps_valid);
    updateMap(d.lat, d.lon, valid === 1 || valid === true);
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
  height: 240px;
  border-radius: 8px;
  overflow: hidden;
}

/* 暗色瓦片：使用 CartoDB dark 或 CSS filter */
.leaflet-tile {
  filter: brightness(0.6) saturate(0.5) invert(1) hue-rotate(180deg);
}
/* 或者换暗色 tile 源 */
```

> 简易方案：CSS `filter` 反色，效果足够。想要更好可以换 CartoDB dark tiles（`https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png`），同样免费无需 Key。

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
| `dashboard.html` | 修改 | +60 行（HTML 容器 + JS 逻辑 + CSS） |

无需新建文件，全部在 dashboard.html 内完成。

## 验证

```
1. 打开 dashboard.html → 地图显示中国全景 + "等待定位…"
2. 硬件上电 → MQTT 上报 lat/lon → 地图自动飞到设备位置 + 蓝色标记
3. 设备移动（GPS 坐标变化）→ marker 跟随移动
4. gps_valid=0 → marker 变灰
5. 手机端打开 → 地图可触摸拖动，不卡页面滚动
```

## 参考

- Leaflet.js 文档：https://leafletjs.com/reference.html
- OpenStreetMap Tile Usage：https://operations.osmfoundation.org/policies/tiles/
- CartoDB Dark Matter tiles：`https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png`
