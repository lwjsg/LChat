# LChat

LChat 是一个基于 Qt/C++ 开发的即时聊天项目，包含客户端、网关服务、聊天服务、状态服务和验证服务等模块。

## 项目结构

```text
LChat/
├── LChat/                 # Qt 客户端
├── 服务器/
│   ├── GateServer/        # 网关服务
│   ├── ChatServer/        # 聊天服务
│   ├── ChatServer2/       # 第二聊天服务
│   ├── StatusServer/      # 状态服务
│   └── VarifyServer/      # 验证服务
└── .gitignore
