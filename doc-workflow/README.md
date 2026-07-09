# doc-workflow/ — 开发工作流文档

> 🎯 **当前入口**：[`last-tasks.md`](last-tasks.md) — 未完成任务清单

## 目录结构

```
doc-workflow/
├── README.md                  ← 本文件
├── last-tasks.md              ← 🎯 当前未完成任务（每日更新）
│
├── task2.md                   ← 硬件集成（当前阶段）
├── task3.md                   ← 云平台对接总览
│   ├── task3.1.md             ←   EMQX Cloud 注册（✅ 已完成）
│   ├── task3.3.md             ←   云指令下发（⚠️ 进行中）
│   └── task3.5.md             ←   地图实时定位（✅ 已完成）
├── task4.md                   ← 低功耗优化
├── task5.md                   ← 整机测试 + 产出物
│
├── GIT_WORKFLOW.md            ← Git 协作规范（必读！）
├── TROUBLESHOOTING.md         ← 踩坑记录（遇到问题先查）
├── hardware_map.md            ← 硬件-代码引脚映射
├── current_status.md          ← 项目当前状态快照
│
└── archive/                   ← 历史记录
    ├── logs/                  ←   全部执行日志
    ├── plans/                 ←   过期计划文档
    └── debug/                 ←   调试产物
```

## 使用指南

### 新人/队友首次打开

1. 先看 `last-tasks.md` — 了解当前要做什么
2. 再看 `GIT_WORKFLOW.md` — 了解 Git 协作规则
3. 遇到问题查 `TROUBLESHOOTING.md`

### 日常开发

- 每个 task 执行前读对应 `task*.md`
- 完成后更新 task 文档中的勾选状态
- 写日志放到 `archive/logs/`

### C（文档负责人）

- `last-tasks.md` 是任务入口，每次有新进展或阻塞时更新
- 新增 task 子文档命名：`taskX.Y.md`

## 规则

- 根目录只放**活跃文档**（task + 参考）
- 所有历史日志、过期计划放 `archive/`
- 构建产物、调试文件放 `archive/debug/`
- 不要往根目录新增无关文件
