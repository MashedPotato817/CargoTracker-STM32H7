# Git 协作规范

> 三人团队 STM32H7 项目的 Git 工作流。目标：避免冲突、保护 `main` 分支、顺畅协作。

## 分支策略

```
main                       ← 始终可编译可烧录，保护分支
  ├── feat/i2c-driver      ← A: I2C1 + SHT31/PN532 真驱动
  ├── feat/spi-driver      ← A: SPI1 + W25Q128 真驱动
  ├── feat/gps-driver      ← B: USART2 + GPS 真驱动
  ├── feat/4g-driver       ← B: USART1 + Air780E 真驱动
  ├── feat/pb0-fix         ← C: PB0 冲突解决
  ├── feat/integration     ← C: 系统集成 + 状态机
  └── fix/*                ← Bug 修复
```

### 规则

1. **`main` 禁止直接提交。** 一切通过 feature 分支 + PR
2. **每个分支聚焦一个模块/一个问题**，不混改不相关的文件
3. **合并前 rebase 到最新 main**（`git rebase origin/main`），提前解决冲突
4. **合并后删除 feature 分支**，保持仓库整洁
5. **分支命名**：`feat/<模块>` 功能、`fix/<问题>` 修复、`cube/<操作>` CubeMX

## 首次设置（新成员 / 刚 clone）

> ⚠️ `Drivers/` 和 `Middlewares/` 不在 Git 中（已 gitignore）。

```bash
# 1. 安装 STM32CubeMX 6.17.0 + FW_H7 V1.13.0
# 2. clone 仓库
git clone <repo-url>
cd CargoTracker-STM32H7
# 3. 打开 test1/test1.ioc → Generate Code（生成 Drivers/ 和 Middlewares/）
# 4. Keil 打开 test1/MDK-ARM/test1.uvprojx → F7 编译
```

> 现有成员首次 pull 本次变更后，本地 `Drivers/` 和 `Middlewares/` 会被删除。**重新 CubeMX Generate Code 一次即可恢复。**

## 每日工作流

### 工作前
```bash
git checkout main
git pull                    # 同步最新代码
git checkout -b feat/xxx    # 创建今天的 feature 分支
```

### 工作时
```bash
git add <files>
git commit -m "模块: 描述"  # 频繁提交，小步快跑
```

### 收工前
```bash
git push -u origin feat/xxx # 推送分支
# 在 GitHub 创建 PR（可选：标记为 Draft）
```

## PR 流程

### 创建 PR

1. **先 rebase**：`git fetch && git rebase origin/main`
2. **解决冲突后**：`git push -f origin feat/xxx`
3. **在 GitHub 创建 PR**，描述清楚改了什么

### Review 清单

- [ ] **编译**：0 Error, 0 Warning（附构建截图或日志）
- [ ] **USER CODE**：未修改 BEGIN/END 段外代码
- [ ] **Drivers/**：未修改 HAL/CMSIS 库文件
- [ ] **CubeMX**：如涉及，仅 .ioc + 生成文件变更
- [ ] **文档**：API 变更时更新 CLAUDE.md/README.md
- [ ] **踩坑**：新问题已记录到 TROUBLESHOOTING.md

### 合并

- 至少 1 人 approve 后，用 GitHub "**Squash and merge**"
- 合并后：`git checkout main && git pull && git branch -d feat/xxx`

## CubeMX 冲突管理（最重要！）

`.ioc` 文件是 XML，**无法手动合并**。冲突 = 只能重做！

### 守门人模式（推荐）

**A 为 CubeMX 守门人。** 其他人需要修改外设时 → 告诉 A → A 统一操作 `.ioc`。

### 串行声明模式

任何人修改 CubeMX 前：
1. **在团队群里声明**："我在改 .ioc，大家先不要动 CubeMX"
2. `git commit` 当前状态 → 改 CubeMX → regenerate → 编译验证 → `git commit` → push
3. **声明完成**："CubeMX 改完了，可以 rebase 了"
4. 其他人：`git pull --rebase`

### 如果冲突了怎么办

1. 确认谁先提交 — 后提交的人重做 CubeMX 操作
2. **不要尝试手动编辑 .ioc**，用 CubeMX GUI 重新配置

## 提交信息规范

```
模块: 简要描述

详细说明（可选）
```

示例：
```
sht31: 启用真实 I2C 驱动，设置 SHT31_USE_HAL_I2C=1

CubeMX 已使能 I2C1（PB8/PB9, 400kHz）。
CRC8 校验通过，温度读数验证正常（~26°C）。
```

## 禁止事项

- ❌ 直接 push 到 main
- ❌ 不 rebase 就提 PR（冲突留给 reviewer）
- ❌ 手动修改 `Drivers/` 目录文件
- ❌ 同时修改 `.ioc` 不沟通
- ❌ PR 合并后不删 feature 分支
- ❌ 提交编译产物（.o, .axf, .hex, .map）
