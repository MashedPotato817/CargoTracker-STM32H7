# AGENTS.md

This file provides guidance to Codex when working with code in this repository.

> **共享项目信息请见 [CLAUDE.md](./CLAUDE.md)**。本文件仅记录 Codex 特有差异，避免双份维护。

## Codex 特有配置

- Codex 项目记忆目录：`.codex/projects/`（如不存在请创建）
- Codex 设置文件：`.codex/settings.json`

## 与 Claude Code 的差异

| 项目 | Claude Code | Codex |
|------|-------------|-------|
| 配置文件 | `.claude/settings.json` | `.codex/settings.json` |
| 项目记忆 | `.claude/projects/` | `.codex/projects/` |
| 会话历史 | `.claude/projects/<project>/` | `.codex/projects/<project>/` |

## 协作规范

- CLAUDE.md 是全项目唯一权威上下文文件，Codex 也应首先读取 CLAUDE.md
- 代码修改前请参照 CLAUDE.md 中的引脚分配、架构、约束
- Git 协作规范见 `doc-workflow/GIT_WORKFLOW.md`
