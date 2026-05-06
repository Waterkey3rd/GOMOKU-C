# 简易五子棋对弈系统

> **C++ 线性表专题设计** | 大学生 C++ 专题作业
>
> 基于前后端分离架构的图形化五子棋人-人对弈系统，核心数据结构使用自实现的线性表（vector），并通过 OOP 设计为人-机对弈预留可扩展接口。

---

## 目录

1. [项目概述](#1-项目概述)
2. [系统架构](#2-系统架构)
3. [技术栈与依赖](#3-技术栈与依赖)
4. [项目文件结构](#4-项目文件结构)
5. [核心数据结构——自实现 vector](#5-核心数据结构自实现-vector)
6. [后端逻辑层详解](#6-后端逻辑层详解)
   - 6.1 [数据类型定义](#61-数据类型定义)
   - 6.2 [Chessboard 类（棋盘）](#62-chessboard-类棋盘)
   - 6.3 [Player 类（选手）](#63-player-类选手)
7. [前端 UI 层详解](#7-前端-ui-层详解)
   - 7.1 [UI::Chessboard 类（棋盘绘制）](#71-uichessboard-类棋盘绘制)
   - 7.2 [UI::GameHistory 类（信息面板）](#72-uigamehistory-类信息面板)
   - 7.3 [游戏主循环](#73-游戏主循环)
8. [胜负判定算法](#8-胜负判定算法)
9. [OOP 设计与人-机对弈接口](#9-oop-设计与人-机对弈接口)
10. [编译与运行](#10-编译与运行)
11. [C++ 知识点总结](#11-c-知识点总结)

---

## 1. 项目概述

### 1.1 问题描述

本项目实现一个用于人-人五子棋对弈的简易棋盘系统，棋盘规格为 **15×15**，无禁手规则。当一方获胜（即率先有五颗棋子在纵、横、斜的任意一条直线上无间断连成一线）时，棋盘自动提示获胜信息并终止本次对弈。

### 1.2 功能清单

| 编号 | 功能 | 说明 |
|------|:-----|------|
| 1 | 创建、绘制棋盘 | 基于 raylib 图形库绘制 15×15 棋盘网格 |
| 2 | 对弈功能 | 鼠标选点 + 二次确认落子，黑棋先行交替落子 |
| 3 | 最后一手标记 | 红色高亮标记当前局面的最后一手棋 |
| 4 | 实时信息显示 | 显示总手数、下一手颜色、落子历史记录 |
| 5 | 胜负判别 | 四方向（横、纵、左斜、右斜）五子连珠检测 |
| 6 | 平局判定 | 棋盘下满 225 格自动判定平局 |
| 7 | 重新开始 | 获胜后提供 "Restart Game" 按钮重新对弈 |
| 8 | OOP 扩展接口 | Player 类设计为可替换基类，预留人-机对弈接口 |

### 1.3 坐标系统

```
     A  B  C  D  E  F  G  H  I  J  K  L  M  N  O
 15  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·
 14  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·
  ...
  2  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·
  1  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·  ·
```

- **X 轴**：从左到右，对应数组下标 0~14
- **Y 轴**：从上到下，对应数组下标 0~14

> 注：当前实现中坐标直接使用数字下标，未做字母映射。字符坐标映射可作为后续扩展。

---

## 2. 系统架构

本项目采用 **前后端分离** 架构，后端负责棋盘数据与游戏逻辑，前端负责图形渲染与用户交互，两者通过明确的接口通信。

```
┌─────────────────────────────────────────────────────────┐
│                      main.cpp                           │
│              游戏初始化 · 主循环驱动                       │
└──────────────┬──────────────────────┬───────────────────┘
               │                      │
       ┌───────▼───────┐      ┌───────▼───────┐
       │   UI 前端层    │      │  GOMOKU 后端层 │
       │               │      │               │
       │  UI::Chessboard│◄────►│  Chessboard  │
       │  UI::GameHistory│    │  Player       │
       │               │      │  vector<T>    │
       └───────┬───────┘      └───────┬───────┘
               │                      │
       ┌───────▼───────┐      ┌───────▼───────┐
       │   raylib      │      │  线性数据结构   │
       │   图形渲染引擎  │      │  (二维数组+vector)│
       └───────────────┘      └───────────────┘
```

### 架构优势

1. **关注点分离**：前端专注渲染与交互，后端专注数据与逻辑
2. **可测试性**：后端逻辑可脱离 UI 独立测试
3. **可扩展性**：替换前端（如改为字符界面）不影响后端；替换 Player（如改为 AI）不影响棋盘逻辑
4. **代码复用**：Player 抽象层使得人-人和人-机对弈共享同一套棋盘逻辑

---

## 3. 技术栈与依赖

| 技术 | 用途 | 说明 |
|------|------|------|
| **C++17** | 主语言 | 使用 `constexpr`、`enum class`、designated initializer 等特性 |
| **raylib** | 图形库 | 轻量级 2D/3D 游戏开发库，负责窗口管理、图形绘制、鼠标输入 |
| **raygui** | GUI 扩展 | raylib 的即时模式 GUI 库，提供按钮等控件 |
| **自实现 vector\<T\>** | 线性数据结构 | 课程要求的核心数据结构，替代 `std::vector` |

### raylib 简介

[raylib](https://www.raylib.com/) 是一个用 C 语言编写的简单易用的游戏编程库，提供：

- 窗口创建与管理（`InitWindow`、`CloseWindow`）
- 2D 图形绘制（`DrawLine`、`DrawCircle`、`DrawRectangle`、`DrawText`）
- 鼠标输入处理（`IsMouseButtonPressed`、`GetMousePosition`）
- 帧率控制（`SetTargetFPS`）
- 颜色系统（内置 `BLACK`、`RAYWHITE`、`BEIGE`、`RED`、`DARKGRAY` 等常量）

---

## 4. 项目文件结构

```
Gomoku/
├── App/                        # 应用源代码目录
│   ├── main.cpp                # 程序入口，初始化与主循环
│   ├── Chessboard.h            # 后端：棋盘类（数据存储 + 胜负判定）
│   ├── Player.h                # 后端：玩家类（落子提交 + 确认机制）
│   ├── vector.h                # 自实现 vector 模板类（线性表核心）
│   ├── ui.h                    # 前端：UI 类声明（棋盘绘制 + 信息面板）
│   └── ui.cpp                  # 前端：UI 实现 + 游戏主循环逻辑
├── include/
│   └── raylib/                 # raylib 图形库
│       ├── include/
│       │   ├── raylib.h        # raylib 主头文件
│       │   ├── raygui.h        # raygui GUI 扩展头文件
│       │   └── raymath.h       # raylib 数学工具
│       └── lib/
│           ├── libraylib.a     # raylib 静态库
│           ├── libraylibdll.a  # raylib 动态库导入库
│           └── raylib.dll      # raylib 动态链接库
├── design.md                   # 设计文档（早期设计思路）
└── README.md                   # 本文档
```

### 命名空间划分

| 命名空间 | 职责 | 包含的类/结构 |
|----------|------|---------------|
| `GOMOKU` | 后端游戏逻辑 | `Chessboard`、`Player`、`Color`、`placeAction`、`Event` |
| `UI` (全局) | 前端图形界面 | `UI::Chessboard`、`UI::GameHistory`、`init()`、`update()`、`Stop()` |

---

## 5. 核心数据结构——自实现 vector

> **线性表专题设计要求**：核心算法必须用到线性数据结构。

本项目自实现了一个泛型动态数组 `vector<T>`，替代 STL 的 `std::vector`，用于存储棋盘的落子历史记录（`Event` 序列）。

### 5.1 设计思路

动态数组（顺序表）是线性表最基本的实现方式。其核心思想是：

- 使用一块 **连续内存** 存储元素
- 通过三个指针管理内存状态：`head`（首元素）、`tail`（末尾后一位）、`cap`（容量边界）
- 当空间不足时，分配 **2 倍** 新空间并拷贝旧数据（倍增策略）

```
内存布局示意：

  head        tail           cap
   │           │              │
   ▼           ▼              ▼
   [e0][e1][e2][  ][  ][  ][  ]
   ├─── 已使用 ───┤├── 空闲 ──┤
   ├───────── 容量 ────────── ┤
```

### 5.2 完整接口

```cpp
template<typename T>
class vector {
private:
    T* head;    // 指向首元素
    T* tail;    // 指向最后一个元素的下一个位置（即 size）
    T* cap;     // 指向容量末尾（即 capacity）
public:
    // ========== 构造与析构 ==========
    vector();
    // 默认构造：创建空 vector，三个指针均为空
    // 时间复杂度：O(1)
    ~vector();
    // 析构函数：释放 head 指向的动态数组
    // 时间复杂度：O(1)
    vector(const vector& other);
    // 拷贝构造：深拷贝 other 的所有元素到新分配的内存
    // 时间复杂度：O(n)，n = other.size()
    vector& operator=(const vector& other);
    // 拷贝赋值运算符：先释放自身内存，再深拷贝 other
    // 包含自赋值检查（this != &other）
    // 时间复杂度：O(n)
    // ========== 容量相关 ==========
    size_t size() const;
    // 返回当前元素个数，即 tail - head
    // 时间复杂度：O(1)
    size_t capacity() const;
    // 返回当前容量，即 cap - head
    // 时间复杂度：O(1)
    // ========== 元素访问 ==========
    T& operator[](size_t index);
    const T& operator[](size_t index) const;
    // 下标访问运算符：返回第 index 个元素的引用
    // 无边界检查（与 std::vector 的 operator[] 行为一致）
    // 时间复杂度：O(1)
    // ========== 修改操作 ==========
    void push_back(const T& data);
    // 在尾部追加元素
    // 若 size == capacity，则触发扩容：
    //   - 容量为 0 时扩展为 1
    //   - 否则扩展为当前容量的 2 倍
    // 扩容步骤：new 新数组 → 拷贝旧数据
    //→ delete 旧数组 → 更新指针
    // 均摊时间复杂度：O(1)
    void clear();
    // 清空所有元素：仅将 tail 回移到 head，不释放内存
    // 时间复杂度：O(1)
};
```

### 5.3 关键实现细节

#### 倍增扩容策略

```cpp
void push_back(const T& data) {
    if (tail == cap) {                          
        // 空间已满，需要扩容
        size_t old_c = cap - head;              
        // 旧容量
        size_t new_c = (old_c == 0) ? 1 : old_c * 2; 
        // 新容量：0→1，否则翻倍
        T* new_head = new T[new_c];   
        // 分配新内存
        for (size_t i = 0; i < old_c; ++i) {
            // 拷贝旧数据
            new_head[i] = head[i];
        }
        delete[] head;
        // 释放旧内存
        head = new_head;
        // 更新三指针
        tail = head + old_c;
        cap = head + new_c;
    }
    *tail = data;
    // 写入新元素
    ++tail;
    // 移动尾指针
}
```

**为什么选择倍增策略？**

- 若每次扩容固定增加 1 个空间，则 N 次 `push_back` 需要 O(N^2) 的拷贝操作
- 倍增策略下，N 次 `push_back` 的总拷贝次数为 `1 + 2 + 4 + ... + N ≈ 2N`，均摊 O(1)
- 这是经典的空间-时间权衡，也是 STL `std::vector` 的标准做法

#### 拷贝语义（深拷贝）

```cpp
vector(const vector& other) {
    size_t n = other.size();
    if (n == 0) {
        head = tail = cap = nullptr;            // 空 vector
    } else {
        head = new T[n];                        // 分配恰好 n 个元素的空间
        for (size_t i = 0; i < n; ++i)
            head[i] = other.head[i];            // 逐元素拷贝
        tail = cap = head + n;                  // capacity == size（无冗余空间）
    }
}
```

**拷贝构造 vs 拷贝赋值**的区别：

- 拷贝构造：对象首次创建，无需释放旧资源
- 拷贝赋值：对象已存在，必须先 `delete[] head` 释放旧内存，且需检查自赋值

---

## 6. 后端逻辑层详解

### 6.1 数据类型定义

#### Color 枚举——棋子颜色

```cpp
enum class Color 
    : uint8_t {
    None  = 0,  
    // 空（无棋子）
    Black = 1,  
    // 黑棋
    White = 2   
    // 白棋
};
```

- 使用 `enum class`（强类型枚举）而非普通 `enum`，避免隐式转换和命名冲突
- 底层类型为 `uint8_t`（1 字节），节省内存且与棋盘的 `memset` 初始化兼容
- 三个值恰好为 0、1、2，可直接用 `memset(board, 0, ...)` 将整个棋盘初始化为 `None`

#### placeAction 结构体——落子动作

```cpp
struct placeAction {
    int x, y;      
    // 落子坐标（数组下标）
    Color color;    
    // 落子颜色
};
```

使用 C++20 的 designated initializer（指定初始化器）语法：

```cpp
placeAction action = {
    .x = 3,
    .y = 5,
    .color = Color::Black
};
```

#### Event 结构体——历史事件

```cpp
struct Event {
    int id;                     // 事件编号（第几手）
    union {                     // 匿名联合体，当前仅包含落子动作
        placeAction placeaction;
    };
};
```

- 使用 `union` 而非单独的 `placeAction`，是为未来扩展预留：可添加认输事件、悔棋事件等
- `union` 中所有成员共享同一块内存，同一时刻只有一个成员有效

### 6.2 Chessboard 类（棋盘）

`Chessboard` 是后端核心类，负责棋盘数据存储、落子操作和胜负判定。

#### 6.2.1 类定义与成员变量

```cpp
class Chessboard {
private:
    static constexpr int H = 15;                // 棋盘高度（行数）
    static constexpr int W = 15;                // 棋盘宽度（列数）
    Color board[H][W];                          // 棋盘二维数组（核心存储）
    vector<Event> history;                      // 落子历史记录（线性表）
    placeAction lastPlaceAction;                // 最后一手棋
    Color Winner = Color::None;                 // 获胜方
    bool white_ready;                           // 白棋选手是否就绪
    bool black_ready;                           // 黑棋选手是否就绪
    int step;                                   // 当前总手数
};
```

**成员变量说明**：

| 成员 | 类型 | 作用 |
|------|------|------|
| `board[15][15]` | `Color` 二维数组 | 棋盘的核心存储，`board[y][x]` 表示坐标 (x,y) 的棋子状态 |
| `history` | `vector<Event>` | 自实现线性表，按顺序存储每一步落子事件 |
| `lastPlaceAction` | `placeAction` | 缓存最后一手棋，用于胜负判定时只需检查最后一手 |
| `Winner` | `Color` | 记录获胜方，`None` 表示未结束 |
| `white_ready` / `black_ready` | `bool` | 双方选手注册标志，双方就绪后才允许落子 |
| `step` | `int` | 已落子总数，用于判断平局（step == 225） |

#### 6.2.2 构造与重置

```cpp
Chessboard() {
    reset();    // 构造时自动初始化
}

// 禁用拷贝构造（棋盘是游戏全局状态，不应被拷贝）
Chessboard(const Chessboard&) = delete;

~Chessboard() = default;

void reset() {
    // 用 memset 将整个 board 数组填充为 0（即 Color::None）
    std::memset(board, (int)(Color::None), sizeof(board));
    Winner = Color::None;
    step = 0;
    lastPlaceAction = {.x = 0, .y = 0, .color = Color::None};
    history.clear();    // 清空历史记录（不释放内存）
}
```

**`memset` 初始化棋盘的原理**：

- `Color::None = 0`，`Color::Black = 1`，`Color::White = 2`
- `memset` 按字节填充，每个 `Color` 占 1 字节（`uint8_t`）
- 填充 0 即将所有格点设为 `None`，这是安全的初始化方式

#### 6.2.3 落子接口

```cpp
// 接口 1：分别传入坐标和颜色
bool place_piece(int x, int y, Color color) {
    // 检查：目标位置必须为空 且 双方选手已就绪
    if (board[y][x] != Color::None || !isready()) return false;

    lastPlaceAction = {.x = x, .y = y, .color = color};
    board[y][x] = color;                        // 写入棋盘
    step++;                                     // 手数 +1
    history.push_back({step, lastPlaceAction}); // 记入历史
    return true;
}

// 接口 2：传入 placeAction 结构体
bool place_piece(placeAction action) {
    if (board[action.y][action.x] != Color::None || !isready()) return false;

    board[action.y][action.x] = action.color;
    lastPlaceAction = action;
    step++;
    history.push_back({step, action});
    return true;
}

// 检查某位置是否可落子（为空）
bool check_piece(int x, int y) {
    return board[y][x] == Color::None;
}
```

#### 6.2.4 选手注册

```cpp
bool registerPlayer(Color color) {
    if (color == Color::None) return false;    
    // 不能注册"无色"

    if (color == Color::Black && !black_ready) {
        black_ready = true;
        return true;
    }
    if (color == Color::Black && black_ready)
        return false;                        
    // 黑棋已注册，重复注册失败

    if (color == Color::White && !white_ready) {
        white_ready = true;
        return true;
    }
    if (color == Color::White && white_ready)
        return false;   
    // 白棋已注册，重复注册失败

    return false;
}
```

注册机制确保：
- 双方必须分别注册后才能开始对弈
- 同一颜色不能重复注册
- `isready()` 返回 `white_ready && black_ready`

#### 6.2.5 查询接口

```cpp
inline bool isready()        { return white_ready && black_ready; }
inline bool isfull()         { return step == W * H; }       
inline const vector<Event>& gethistory()  { return history; }
inline const placeAction& get_last_move() { return lastPlaceAction; }
inline Color get_piece_at(int x, int y)   { return board[y][x]; }
inline const Color* getBoardData()   { return &board[0][0]; }
inline const int getBoardSize()      { return W * H; }
inline const int& getBoardH()        { return H; }
inline const int& getBoardW()        { return W; }
inline const int& getStep()          { return step; }
```

**`getBoardData()` 的设计意图**：

- 返回 `&board[0][0]`，即二维数组的首地址
- 前端 UI 层可通过这个一维指针遍历整个棋盘（`board[i * 15 + j]`）
- 避免暴露二维数组的具体类型，实现前后端解耦

### 6.3 Player 类（选手）

`Player` 类实现了 **"提交-确认" 两阶段落子机制**，这是本项目的一个关键设计。

#### 6.3.1 设计理念

```
用户交互流程：

  鼠标点击选点 ──► commitMove() ──► 预选标记（红色圆圈）
        │                                  │
        │                          再次点击同一位置
        │                                  │
        ▼                                  ▼
   取消/选择其他点                     comfirmSelection()
                                              │
                                              ▼
                                      落子写入棋盘
```

两阶段设计的意义：
1. **防误触**：第一次点击仅标记预选，第二次确认才真正落子
2. **可修改**：预选后可移动鼠标选择其他位置
3. **接口统一**：人类玩家通过鼠标交互完成 commit + confirm，AI 玩家可直接调用同一接口

#### 6.3.2 类定义

```cpp
class Player {
private:
    Chessboard* bindChessboard = nullptr;   // 绑定的棋盘指针
    bool isbind = false;                     // 是否已绑定
    Color selfcolor;                         // 自身颜色（黑/白）
    placeAction selfaction;                  // 暂存的落子动作
    bool haveAction = false;                 // 是否有待确认的落子
};
```

#### 6.3.3 构造与绑定

```cpp
Player(Chessboard* board, Color color) {
    setColor(color);
    bindBoard(board);
}

inline void setColor(Color color) {
    selfcolor = color;
}

inline void bindBoard(Chessboard* board) {
    bindChessboard = board;
    isbind = true;
    board->registerPlayer(selfcolor);   // 向棋盘注册自身颜色
}
```

**为什么用指针绑定而非引用？**

- `Player` 需要调用 `Chessboard` 的非 const 方法（`place_piece`）
- 指针允许在构造后重新绑定（虽然当前未使用）
- 体现了 C++ 中"组合"（has-a）关系的设计

#### 6.3.4 落子提交（commitMove）

```cpp
// 接口 1：传入 placeAction（前端直接构造动作）
bool commitMove(placeAction action) {
    if (haveAction) return false;   // 已有未确认的动作，拒绝新提交
    selfaction = action;
    return true;
}

// 接口 2：传入坐标（内部构造动作，自动填充自身颜色）
bool commitMove(int x, int y) {
    if (!bindChessboard->check_piece(x, y)) return false;  // 目标位置非空
    selfaction = {.x = x, .y = y, .color = selfcolor};
    haveAction = true;
    return true;
}
```

#### 6.3.5 落子确认（comfirmSelection）

```cpp
// 接口 1：传入坐标，检查是否与预选一致
bool comfirmSelection(placeAction a) {
    if (!haveAction) return false;              // 没有待确认的动作
    bool isSuccess = true;
    if (haveAction && a.x == selfaction.x && a.y == selfaction.y) {
        bindChessboard->place_piece(selfaction); // 同一位置 → 确认落子
        haveAction = false;
    } else {
        isSuccess = false;                      // 不同位置 → 取消
        haveAction = false;
    }
    return isSuccess;
}

// 接口 2：布尔确认（供 AI 或键盘确认使用）
bool comfirmSelection(bool iscomfirm) {
    if (!haveAction) return false;
    bool isSuccess = true;
    if (iscomfirm) {
        bindChessboard->place_piece(selfaction); // 确认 → 落子
        haveAction = false;
    } else {
        haveAction = false;                      // 取消
    }
    return isSuccess;
}
```

#### 6.3.6 查询接口

```cpp
const Color& getColor() const { return selfcolor; }

const placeAction getAction() const {
    if (haveAction)
        return selfaction;
    return {.x = 0, .y = 0, .color = Color::None};  // 无动作时返回空
}
```

---

## 7. 前端 UI 层详解

前端基于 raylib 图形库实现，位于 `UI` 命名空间中。

### 7.1 UI::Chessboard 类（棋盘绘制）

负责棋盘网格绘制、棋子渲染和鼠标输入处理。

```cpp
class Chessboard {
private:
    size_t width;           // 棋盘绘制区域的像素宽度
    size_t start_x;         // 棋盘左上角 X 坐标（像素）
    size_t start_y;         // 棋盘左上角 Y 坐标（像素）
    size_t pixelnumber;     // 格点数量（15）
    bool isStart = false;
};
```

#### 7.1.1 棋盘绘制

```cpp
void drawBoard() {
    size_t space = width / pixelnumber;     // 每格像素间距
    ClearBackground(BEIGE);                 // 清除背景为米色

    for (int i = 0; i < pixelnumber; i++) {
        // 画水平线（X 方向）
        DrawLine(start_x, start_y + i * space,
                 start_x + (pixelnumber - 1) * space,
                 start_x + i * space, DARKGRAY);
        // 画垂直线（Y 方向）
        DrawLine(start_x + i * space, start_y,
                 start_x + i * space,
                 start_x + (pixelnumber - 1) * space, DARKGRAY);
    }
}
```

**绘制原理**：

- 15×15 的棋盘需要 15 条水平线和 15 条垂直线
- 线与线的间距 = `width / pixelnumber`
- 使用 `DrawLine(x1, y1, x2, y2, color)` 绘制

#### 7.1.2 棋子渲染

```cpp
void drawPiece(const GOMOKU::Color* board) {
    size_t space = width / pixelnumber;
    for (int i = 0; i < pixelnumber; i++) {         // 遍历行（Y）
        for (int j = 0; j < pixelnumber; j++) {     // 遍历列（X）
            if (board[i * pixelnumber + j] == GOMOKU::Color::White) {
                // 白棋：在交叉点画白色实心圆
                DrawCircle(start_x + j * space, start_y + i * space,
                          (size_t)(space / 2), RAYWHITE);
            }
            if (board[i * pixelnumber + j] == GOMOKU::Color::Black) {
                // 黑棋：在交叉点画黑色实心圆
                DrawCircle(start_x + j * space, start_y + i * space,
                          (size_t)(space / 2), BLACK);
            }
        }
    }
}
```

**坐标映射**：

- 二维棋盘 `board[y][x]` 在一维指针中索引为 `y * 15 + x`
- 屏幕像素坐标 = `start + index * space`

#### 7.1.3 预选标记

```cpp
void drawPreSelect(size_t tx, size_t ty) {
    size_t space = width / pixelnumber;
    // 在预选位置画红色半透明圆圈
    DrawCircle(tx * space + start_x, ty * space + start_y,
              (size_t)(space / 2), RED);
}
```

#### 7.1.4 鼠标输入处理

```cpp
GOMOKU::placeAction getPiece() {
    GOMOKU::placeAction action = {.x = -1, .y = -1, .color = GOMOKU::Color::None};
    size_t space = width / pixelnumber;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {     // 检测鼠标左键点击
        Vector2 m = GetMousePosition();                 // 获取鼠标像素坐标
        // 将像素坐标转换为棋盘格点坐标（带四舍五入对齐）
        action.x = (m.x - start_x + (int)(space / 2)) / space;
        action.y = (m.y - start_y + (int)(space / 2)) / space;
    }
    return action;
}
```

**坐标转换公式**：

```
格点坐标 = (鼠标像素坐标 - 起始偏移 + 半格间距) / 格间距
```

加 `space/2` 是为了实现"就近吸附"——鼠标点击在格点附近时自动对齐到最近的格点。当鼠标未点击时，返回 `x = -1` 表示无输入。

### 7.2 UI::GameHistory 类（信息面板）

在棋盘右侧显示实时游戏信息。

```cpp
class GameHistory {
private:
    int startx, starty;     // 面板起始坐标
    int height, width;      // 面板尺寸
    int displayCount;       // 最多显示的历史记录条数（5 条）
};
```

#### 7.2.1 历史记录显示

```cpp
void updatehistory(const vector<GOMOKU::Event>& hist) {
    int dis = 0;
    // 从最新的记录开始，最多显示 displayCount 条
    for (int i = (int)hist.size() - 1; i >= 0 && dis < displayCount; i--, dis++) {
        auto act = hist[i].placeaction;
        // 显示格式：#手数: (x, y)
        DrawText(TextFormat("#%d: (%d, %d)", hist[i].id, act.x, act.y),
                startx, starty + 120 + dis * 25, 16, DARKGRAY);
    }
}
```

#### 7.2.2 回合信息显示

```cpp
void updateTurn(int step, GOMOKU::Color c) {
    // 绘制半透明背景面板
    DrawRectangle(startx, starty, width, height, Fade(BLACK, 0.1f));
    // 显示当前手数
    DrawText(TextFormat("Step: %d", step), startx, 50 + starty, 20, DARKGRAY);
    // 显示当前回合颜色
    DrawText(TextFormat("Turn: %s", (c == GOMOKU::Color::Black ? "BLACK" : "WHITE")),
            startx, 80 + starty, 20, BLACK);
}
```

#### 7.2.3 胜利画面

```cpp
bool updateWin(GOMOKU::Color c) {
    if (c == GOMOKU::Color::None)
        return false;                           // 游戏未结束
    else {
        // 绘制半透明白色遮罩覆盖整个窗口
        Vector2 pos = GetWindowPosition();
        DrawRectangle(pos.x, pos.y, GetScreenWidth(), GetScreenHeight(),
                     Fade(RAYWHITE, 0.8f));
        // 显示获胜信息（红色大字）
        DrawText(TextFormat("%s WIN!",
                c == GOMOKU::Color::Black ? "BLACK" : "WHITE"),
                300, 300, 40, RED);
        // 绘制重新开始按钮（使用 raygui）
        if (GuiButton({350, 380, 200, 40}, "Restart Game"))
            return true;                        // 按钮被点击，返回 true 触发重置
    }
    return false;
}
```

### 7.3 游戏主循环

`ui.cpp` 中实现了完整的游戏初始化、更新和关闭流程。

#### 7.3.1 全局变量

```cpp
bool isGameover = false;    // 游戏是否结束
bool have_selected = false; // 是否已预选落子点
int Gamecount = 0;          // 已完成的对局数

// 后端对象
GOMOKU::Chessboard chessboard;                                          // 棋盘（全局唯一）
GOMOKU::Player blackPlayer = GOMOKU::Player(&chessboard, GOMOKU::Color::Black);  // 黑棋选手
GOMOKU::Player whitePlayer = GOMOKU::Player(&chessboard, GOMOKU::Color::White);  // 白棋选手
GOMOKU::Player* turnPlayer = &whitePlayer;                              // 当前回合选手指针

// 前端 UI 对象
Chessboard chessboard_ui = Chessboard(40, 40, 600, 15);                 // 棋盘 UI
GameHistory gamehistory_ui = GameHistory(620, 0, 650, 280, 5);          // 信息面板 UI
```

#### 7.3.2 初始化流程

```cpp
void UI::init() {
    InitWindow(900, 650, "Gomoku Game");        // 创建 900×650 窗口
    SetWindowState(FLAG_WINDOW_RESIZABLE);       // 允许调整窗口大小
    SetTargetFPS(60);                            // 锁定 60 FPS
    isGameover = false;
    chessboard.reset();                          // 重置棋盘数据
    chessboard.registerPlayer(GOMOKU::Color::Black);   // 注册黑棋
    chessboard.registerPlayer(GOMOKU::Color::White);   // 注册白棋
}
```

#### 7.3.3 主更新循环（核心逻辑）

```cpp
bool UI::update() {
    if (WindowShouldClose()) { UI::Stop(); return false; }  // 关闭窗口退出

    BeginDrawing();                                 // 开始绘制帧
    ClearBackground(BEIGE);

    GOMOKU::placeAction inputaction;

    if (!isGameover) {
        // ===== 输入处理 =====
        inputaction = chessboard_ui.getPiece();     // 获取鼠标点击

        if (inputaction.x != -1) {                  // 有鼠标点击
            if (have_selected) {
                // 已有预选 → 尝试确认
                if (turnPlayer->comfirmSelection(inputaction) == true) {
                    have_selected = false;
                    // 切换回合：白→黑 或 黑→白
                    turnPlayer = turnPlayer == &whitePlayer
                                 ? &blackPlayer : &whitePlayer;
                } else {
                    // 点击不同位置 → 提交新的预选
                    if (turnPlayer->commitMove(inputaction.x, inputaction.y))
                        chessboard_ui.drawPreSelect(inputaction.x, inputaction.y);
                }
            } else {
                // 无预选 → 提交预选
                if (turnPlayer->commitMove(inputaction.x, inputaction.y)) {
                    chessboard_ui.drawPreSelect(inputaction.x, inputaction.y);
                    have_selected = true;
                }
            }
        } else {
            // 无鼠标点击但有预选 → 持续显示预选标记
            if (have_selected) {
                chessboard_ui.drawPreSelect(
                    turnPlayer->getAction().x,
                    turnPlayer->getAction().y);
            }
        }
    }

    // ===== 渲染 =====
    chessboard_ui.drawBoard();                              // 绘制棋盘网格
    chessboard_ui.drawPiece(chessboard.getBoardData());     // 绘制所有棋子
    gamehistory_ui.updatehistory(chessboard.gethistory());  // 更新历史记录
    gamehistory_ui.updateTurn(chessboard.getStep(),
                              turnPlayer->getColor());     // 更新回合信息

    // ===== 胜负检测 =====
    isGameover = gamehistory_ui.updateWin(chessboard.checkWin());

    if (isGameover) {
        printf("GameCount: %d\n", ++Gamecount);
        // 重置游戏状态，准备下一局
        isGameover = false;
        chessboard.reset();
        chessboard.registerPlayer(GOMOKU::Color::Black);
        chessboard.registerPlayer(GOMOKU::Color::White);
        turnPlayer = &whitePlayer;  // 白棋重置（黑棋先行，由 turnPlayer 初始为 white + 首次切换决定）
    }

    EndDrawing();   // 结束绘制帧
    return true;
}
```

#### 7.3.4 程序入口

```cpp
#define RAYGUI_IMPLEMENTATION       // raygui 实现宏（必须在包含 raygui.h 之前定义）
#include "ui.h"

int main() {
    UI::init();                     // 初始化窗口和游戏
    printf("GAMESTART\n");
    while (UI::update()) {          // 主循环：每帧调用 update()
        // 渲染和逻辑都在 update() 中完成
    }
    printf("GAMEOVER\n");
    return 0;
}
```

---

## 8. 胜负判定算法

胜负判定是五子棋的核心算法。本项目采用 **以最后一手为中心的四方向扫描法**。

### 8.1 算法思路

不需要扫描整个棋盘，只需检查 **最后落子位置** 在四个方向上是否形成五子连珠：

```
方向定义（以最后一手为中心）：

     左斜 ↗        纵 |        右斜 ↘
         \          |          /
          \         |         /
    ───────●──────●──────●───────  横 ─
          /         |         \
         /          |          \
```

四个方向：
1. **横向（─）**：同一行，X 变化
2. **纵向（│）**：同一列，Y 变化
3. **右斜（↘）**：X+1, Y+1 同时变化
4. **左斜（↗）**：X-1, Y+1 同时变化

### 8.2 算法实现

```cpp
Color checkWin() {
    if (lastPlaceAction.color == Color::None)
        return Color::None;     // 无落子，无需检查

    int x = lastPlaceAction.x;
    int y = lastPlaceAction.y;
    Color color = lastPlaceAction.color;

    // ========== 方向 1：横向检查 ==========
    uint8_t count = 1;  // 从 1 开始（最后一手本身算一颗）

    // 向左扫描（最多 4 步，不超过边界）
    for (int i = x - 1; i >= x - 4 && i >= 0; i--) {
        if (color == board[y][i])
            count++;
        else
            break;      // 遇到不同颜色或空位即中断
    }
    // 向右扫描
    for (int i = x + 1; i <= x + 4 && i < W; i++) {
        if (color == board[y][i])
            count++;
        else
            break;
    }
    if (count == 5) return color;   // 五子连珠！

    // ========== 方向 2：纵向检查 ==========
    count = 1;
    for (int i = y - 1; i >= y - 4 && i >= 0; i--) {      // 向上
        if (color == board[i][x]) count++;
        else break;
    }
    for (int i = y + 1; i <= y + 4 && i < H; i++) {       // 向下
        if (color == board[i][x]) count++;
        else break;
    }
    if (count == 5) return color;

    // ========== 方向 3：右斜检查（↘） ==========
    count = 1;
    for (int i = 1; i <= 4 && x + i < W && y + i < H; i++) {   // 右下
        if (color == board[y + i][x + i]) count++;
        else break;
    }
    for (int i = 1; i <= 4 && x - i >= 0 && y - i >= 0; i++) { // 左上
        if (color == board[y - i][x - i]) count++;
        else break;
    }
    if (count == 5) return color;

    // ========== 方向 4：左斜检查（↗） ==========
    count = 1;
    for (int i = 1; i <= 4 && x - i >= 0 && y + i < H; i++) {  // 左下
        if (color == board[y + i][x - i]) count++;
        else break;
    }
    for (int i = 1; i <= 4 && x + i < W && y - i >= 0; i++) {  // 右上
        if (color == board[y - i][x + i]) count++;
        else break;
    }
    if (count == 5) return color;

    return Color::None;     // 四个方向均未形成五连
}
```

### 8.3 算法复杂度分析

| 指标 | 值 |
|------|-----|
| 时间复杂度 | O(1)——最多扫描 4 方向 × 8 步 = 32 次比较 |
| 空间复杂度 | O(1)——仅使用常数额外空间 |
| 优势 | 不需扫描整个棋盘（O(N^2)），只检查最后一手的局部 |

### 8.4 算法正确性说明

- **为什么最多只扫描 4 步？** 五子连珠最多跨越 5 个格点，最后一手占据其中 1 个，左右各最多 4 个
- **为什么从 1 开始计数？** `count = 1` 表示最后一手本身已是一颗棋子
- **为什么遇到不同颜色就 break？** 五子连珠要求"无间断"，中间不能有对方棋子
- **为什么遇到空位也 break？** 同理，空位会中断连珠

---

## 9. OOP 设计与人-机对弈接口

### 9.1 面向对象设计思想

本项目体现了以下 OOP 核心概念：

#### 封装（Encapsulation）

- `Chessboard` 将棋盘数据（`board[15][15]`）封装为 `private`，外部只能通过公开接口操作
- `Player` 将落子状态（`selfaction`、`haveAction`）封装，外部无需关心内部实现
- 使用 `inline` 标注简单的 getter/setter，提示编译器内联优化

#### 组合（Composition）

- `Player` **拥有** `Chessboard*`（has-a 关系），而非继承
- 组合优于继承：Player 不是"一种棋盘"，而是"使用棋盘"的对象

#### 命名空间隔离（Namespace）

- `GOMOKU` 命名空间隔离后端逻辑，`UI` 隔离前端
- 避免命名冲突（如 `Chessboard` 在两个命名空间中各有定义）

#### 禁用拷贝

```cpp
Chessboard(const Chessboard&) = delete;    // 棋盘不可拷贝
```

- 棋盘是游戏的全局状态，拷贝会导致状态不一致
- 使用 `= delete` 显式禁止，编译期报错

### 9.2 人-机对弈接口设计

本项目通过 `Player` 类的抽象层，天然支持替换人类玩家为 AI 玩家。

#### 扩展示例：AI 玩家

```cpp
// 假设的 AI 玩家类（继承 Player 或独立实现）
class AIPlayer {
private:
    GOMOKU::Chessboard* bindChessboard;
    GOMOKU::Color selfcolor;

public:
    AIPlayer(GOMOKU::Chessboard* board, GOMOKU::Color color)
        : bindChessboard(board), selfcolor(color) {
        board->registerPlayer(color);
    }

    // AI 自主决定落子位置
    bool makeAIMove() {
        // === AI 算法在此实现 ===
        // 例如：遍历棋盘找最优位置
        int bestX = 7, bestY = 7;  // 示例：默认下中心

        // 使用与人类玩家相同的接口
        GOMOKU::placeAction action = {
            .x = bestX, .y = bestY, .color = selfcolor
        };
        return bindChessboard->place_piece(action);  // 直接落子，无需确认
    }

    const GOMOKU::Color& getColor() const { return selfcolor; }
};
```

#### 接口复用的关键

```
                    ┌──────────────┐
                    │  Chessboard  │
                    │  place_piece │
                    └──────┬───────┘
                           │
              ┌────────────┼────────────┐
              │                         │
     ┌────────▼────────┐     ┌─────────▼─────────┐
     │  Player (人类)   │     │   AIPlayer (AI)    │
     │  commitMove()   │     │   makeAIMove()     │
     │  confirmSelect()│     │   （直接调用place） │
     └─────────────────┘     └────────────────────┘
```

- 人类玩家：两阶段（commit → confirm），支持鼠标预览
- AI 玩家：一阶段（直接 place），无需预览确认
- 两者共享同一个 `Chessboard` 实例和 `place_piece` 接口

---

## 10. 编译与运行

### 10.1 环境要求

- **编译器**：支持 C++17 的编译器（g++ 8+、MSVC 2019+、Clang 7+）
- **依赖库**：raylib（已包含在 `include/raylib/` 目录）

### 10.2 编译命令

#### Windows (MinGW g++)

```bash
g++ -std=c++17 -I include/raylib/include \
    App/main.cpp App/ui.cpp \
    -L include/raylib/lib \
    -lraylib -lopengl32 -lgdi32 -lwinmm \
    -o gomoku.exe
```

#### Windows (MSVC)

```bash
cl /std:c++17 /I include\raylib\include \
   App\main.cpp App\ui.cpp \
   /link /LIBPATH:include\raylib\lib \
   raylib.lib opengl32.lib gdi32.lib winmm.lib \
   /OUT:gomoku.exe
```

### 10.3 运行

```bash
./gomoku.exe
```

### 10.4 操作说明

| 操作 | 说明 |
|------|------|
| 鼠标左键（第一次） | 选中落子位置，显示红色预选标记 |
| 鼠标左键（第二次，同一位置） | 确认落子，棋子落定 |
| 鼠标左键（第二次，不同位置） | 更改预选位置 |
| "Restart Game" 按钮 | 获胜后重新开始对弈 |
| 关闭窗口按钮 | 退出程序 |

---

## 11. C++ 知识点总结

本项目综合运用了以下 C++ 知识点，体现了面向对象程序设计的学习成果：

### 11.1 基础语法

| 知识点 | 项目中的应用 |
|--------|-------------|
| `enum class` 强类型枚举 | `Color` 枚举，避免隐式转换 |
| `struct` 聚合类型 | `placeAction`、`Event` 数据结构 |
| 二维数组 | `Color board[15][15]` 棋盘存储 |
| `constexpr` 常量 | `H`、`W` 编译期常量 |
| `uint8_t` 固定宽度整型 | `Color` 底层类型，节省内存 |
| Designated Initializer | `.x = 0, .y = 0, .color = Color::None` |

### 11.2 面向对象编程

| 知识点 | 项目中的应用 |
|--------|-------------|
| 类与对象 | `Chessboard`、`Player`、`UI::Chessboard` |
| 构造函数 / 析构函数 | 默认构造、带参构造、`delete` 禁用拷贝 |
| 访问控制 | `private` 数据 + `public` 接口 |
| `this` 指针 | 拷贝赋值中的自赋值检查 |
| 组合关系 | `Player` 拥有 `Chessboard*` |
| 命名空间 | `GOMOKU`、`UI` 命名空间隔离 |
| `inline` 函数 | 简单 getter 的内联优化 |

### 11.3 模板编程

| 知识点 | 项目中的应用 |
|--------|-------------|
| 类模板 | `vector<T>` 泛型动态数组 |
| 模板实例化 | `vector<Event>` 存储历史记录 |

### 11.4 内存管理

| 知识点 | 项目中的应用 |
|--------|-------------|
| `new[]` / `delete[]` | `vector` 内部动态数组管理 |
| 深拷贝 vs 浅拷贝 | `vector` 拷贝构造与拷贝赋值 |
| 三/五法则 | `vector` 的拷贝构造 + 拷贝赋值 + 析构 |
| `memset` 内存操作 | 棋盘初始化 |
| 指针语义 | `Player` 通过指针绑定 `Chessboard` |

### 11.5 线性数据结构

| 知识点 | 项目中的应用 |
|--------|-------------|
| 顺序表（动态数组） | 自实现 `vector<T>` |
| 倍增扩容策略 | `push_back` 的 O(1) 均摊复杂度 |
| 线性表遍历 | 历史记录的正向/反向遍历 |
| 线性表清空 | `clear()` 的 O(1) 实现 |

### 11.6 算法设计

| 知识点 | 项目中的应用 |
|--------|-------------|
| 方向扫描法 | 四方向五子连珠判定 |
| 坐标变换 | 像素坐标 ↔ 格点坐标转换 |
| 边界检查 | 防止数组越界的循环条件 |

---

## 附录：接口一览表

### GOMOKU::Chessboard 公开接口

| 接口签名 | 返回值 | 说明 |
|----------|--------|------|
| `reset()` | `void` | 重置棋盘，清空所有数据 |
| `place_piece(int x, int y, Color color)` | `bool` | 在 (x,y) 落子，返回是否成功 |
| `place_piece(placeAction action)` | `bool` | 通过结构体落子，返回是否成功 |
| `check_piece(int x, int y)` | `bool` | 检查 (x,y) 是否可落子 |
| `registerPlayer(Color color)` | `bool` | 注册选手颜色 |
| `isready()` | `bool` | 双方是否就绪 |
| `isfull()` | `bool` | 棋盘是否已满 |
| `checkWin()` | `Color` | 检查胜负，返回获胜方（`None` 表示未结束） |
| `get_piece_at(int x, int y)` | `Color` | 获取指定位置的棋子状态 |
| `get_last_move()` | `const placeAction&` | 获取最后一手棋 |
| `gethistory()` | `const vector<Event>&` | 获取历史记录 |
| `getBoardData()` | `const Color*` | 获取棋盘数据首指针 |
| `getBoardSize()` | `const int` | 获取棋盘格点总数 |
| `getBoardH()` / `getBoardW()` | `const int&` | 获取棋盘高度/宽度 |
| `getStep()` | `const int&` | 获取当前总手数 |

### GOMOKU::Player 公开接口

| 接口签名 | 返回值 | 说明 |
|----------|--------|------|
| `Player(Chessboard* board, Color color)` | — | 构造函数，绑定棋盘和颜色 |
| `setColor(Color color)` | `void` | 设置自身颜色 |
| `bindBoard(Chessboard* board)` | `void` | 绑定棋盘并注册 |
| `commitMove(placeAction action)` | `bool` | 提交落子动作（结构体版本） |
| `commitMove(int x, int y)` | `bool` | 提交落子坐标（自动填充颜色） |
| `comfirmSelection(placeAction a)` | `bool` | 通过坐标确认落子 |
| `comfirmSelection(bool iscomfirm)` | `bool` | 通过布尔值确认落子 |
| `getColor()` | `const Color&` | 获取自身颜色 |
| `getAction()` | `const placeAction` | 获取当前预选动作 |

### 自实现 vector\<T\> 接口

| 接口签名 | 返回值 | 说明 |
|----------|--------|------|
| `vector()` | — | 默认构造，创建空容器 |
| `~vector()` | — | 析构，释放内存 |
| `vector(const vector& other)` | — | 拷贝构造（深拷贝） |
| `operator=(const vector& other)` | `vector&` | 拷贝赋值（深拷贝） |
| `push_back(const T& data)` | `void` | 尾部追加，自动扩容 |
| `size()` | `size_t` | 返回元素个数 |
| `capacity()` | `size_t` | 返回当前容量 |
| `operator[](size_t index)` | `T&` / `const T&` | 下标访问 |
| `clear()` | `void` | 清空元素（不释放内存） |

### UI 命名空间接口

| 接口签名 | 返回值 | 说明 |
|----------|--------|------|
| `UI::init()` | `void` | 初始化窗口和游戏状态 |
| `UI::update()` | `bool` | 主循环每帧调用，返回 `false` 退出 |
| `UI::Stop()` | `void` | 关闭窗口 |
| `UI::Chessboard::drawBoard()` | `void` | 绘制棋盘网格 |
| `UI::Chessboard::drawPiece(const Color*)` | `void` | 绘制棋子 |
| `UI::Chessboard::drawPreSelect(size_t, size_t)` | `void` | 绘制预选标记 |
| `UI::Chessboard::getPiece()` | `placeAction` | 获取鼠标点击输入 |
| `UI::GameHistory::updatehistory(const vector<Event>&)` | `void` | 更新历史记录显示 |
| `UI::GameHistory::updateTurn(int, Color)` | `void` | 更新回合信息 |
| `UI::GameHistory::updateWin(Color)` | `bool` | 显示胜利画面，返回是否重开 |
