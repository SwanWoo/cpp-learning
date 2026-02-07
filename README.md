# C++ Learning Project with CMake

一个面向工程实践的 C++ 学习项目，基于 CMake 构建，支持模块化目录结构、独立可执行文件、增量编译和一致的二进制输出结构。

---

## 1. 项目目标

本项目旨在：

- 构建一个可扩展的 C++ 学习工程框架
- 使用 CMake 管理构建流程
- 支持 `src` 目录按模块/章节组织
- 每个 `.cpp` 文件生成独立的可执行程序
- `bin` 目录结构与 `src` 保持一致
- 支持增量编译与按 target 构建

适用于：

- C++ 基础与进阶学习
- 系统编程 / 算法 / 并发 / 网络实验
- 构建个人 C++ 技术底座

---

## 2. 项目结构

```text
cpp-learning/
├── CMakeLists.txt      # CMake 构建配置
├── README.md           # 项目说明文档
├── src/                # 源码目录
│   ├── basic/
│   │   └── hello.cpp
│   ├── algo/
│   │   └── sort.cpp
│   └── system/
│       └── thread.cpp
├── build/              # 构建目录（中间文件，不入 Git）
└── bin/                # 可执行文件输出目录
    ├── basic/
    │   └── hello
    ├── algo/
    │   └── sort
    └── system/
        └── thread
```

---

## 3. 构建环境要求

| 工具 | 最低版本 | 备注 |
|------|----------|------|
| OS | - | Linux / macOS / WSL |
| GCC | >= 9 | 或 Clang >= 10 |
| CMake | >= 3.20 | 必需 |
| Ninja | - | 可选，推荐 |

检查版本：

```bash
g++ --version
cmake --version
```

---

## 4. 构建方式

### 4.1 标准构建（推荐）

```bash
cmake -S . -B build
cmake --build build
```

### 4.2 运行程序

```bash
./bin/basic/hello
./bin/algo/sort
./bin/system/thread
```

### 4.3 只编译指定 cpp（target）

**Target 命名规则：**

| 源文件路径 | Target 名称 |
|------------|-------------|
| `src/basic/hello.cpp` | `basic_hello` |
| `src/algo/sort.cpp` | `algo_sort` |

编译单个目标：

```bash
cmake --build build --target basic_hello
```

或使用 Make：

```bash
cd build
make basic_hello
```

### 4.4 使用 Ninja（高性能构建）

```bash
cmake -G Ninja -S . -B build
ninja -C build
ninja -C build basic_hello
```

---

## 5. 新增模块或 cpp 文件

示例：

```bash
mkdir -p src/net
touch src/net/socket.cpp
```

重新构建：

```bash
cmake --build build
```

生成：

```text
bin/net/socket
```

---

## 6. 设计理念

### 6.1 每个 cpp = 一个独立实验单元

适合：

- C++ 语言特性实验
- 算法练习
- 系统编程测试
- 并发模型验证

### 6.2 工程级目录组织

| 目录 | 用途 | 是否入 Git |
|------|------|------------|
| `src/` | 源码 | ✅ |
| `build/` | 构建缓存 | ❌ |
| `bin/` | 可执行文件 | ❌ |

### 6.3 CMake 自动映射目录结构

```text
src/.../.cpp  →  bin/.../
```

类似 Bazel / Cargo / Meson 的构建模式。

---

## 7. 推荐扩展方向（未来规划）

- [ ] 引入 static / shared library
- [ ] GoogleTest 单元测试
- [ ] Benchmark 性能测试
- [ ] clang-tidy / clang-format
- [ ] Sanitizer / Valgrind
- [ ] 模块化 CMakeLists.txt
- [ ] CI/CD（GitHub Actions）

---

## 8. License

MIT License
