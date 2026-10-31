# FragGraph — 碎片化知识图谱管理系统

> 用 C 语言实现的数据结构课程作业：以单链表管理笔记，以邻接表把「标签相同」的笔记连成知识图谱，并支持 BFS 遍历、孤立节点发现与文件持久化。

## 功能

| 功能 | 说明 |
|---|---|
| 笔记管理 | 添加 / 删除 / 修改 / 列表，ID 自增且删除后不复用 |
| 标签搜索 | 按标签子串匹配，输出命中笔记 |
| 自动建图 | 标签相同的两条笔记之间建立双向边，重建时先清空旧图 |
| BFS 遍历 | 从指定笔记出发，按邻接表广度优先遍历并输出 `[ID] 标题` |
| 孤立笔记 | 列出没有任何边的笔记 |
| 文件持久化 | 保存/加载 `kb_data.txt`，包含笔记段与 `[EDGES]` 边段 |

## 数据结构

```c
typedef struct Note {          // 笔记：单链表
    int id;
    char title[100];
    char body[500];
    char tag[50];
    struct Note *next;
} Note;

typedef struct AdjNode {       // 图：邻接表（头插）
    int vid;
    struct AdjNode *next;
} AdjNode;
```

- 笔记使用**尾插法**维护链表，保持输入顺序；
- 图谱以笔记 ID 作为顶点下标，邻接表采用**头插法**；
- 建边规则：`strcmp(p1->tag, p2->tag) == 0` 且 ID 不同时，建立 `p1 → p2` 与 `p2 → p1` 两条边（无向图）。

## 数据文件格式

```text
NOTE|1|C语言入门|学习变量、循环|C语言
NOTE|2|指针讲解|指针与地址|C语言
[EDGES]
1->2
2->1
```

- 笔记行：`NOTE|ID|标题|正文|标签`；
- 边段以 `[EDGES]` 开头，每行 `起点->终点`；
- 程序读取和写入的是**当前工作目录**下的 `kb_data.txt`。

## 编译与运行

需要 C11 编译器（GCC / MinGW / MSVC）。

```bash
# 在仓库根目录编译
gcc -std=c11 -Wall -Wextra src/main.c -o kg

# 程序从当前目录读写 kb_data.txt，因此进入 data/ 运行
cd data
../kg          # Linux / macOS
..\kg.exe      # Windows
```

也可以直接使用示例数据体验：仓库已附带 `data/kb_data.txt`（4 条笔记、6 条边）。首次运行后选择 `10` 加载，再选择 `7` 做 BFS、`8` 查看孤立笔记。

> 源码与示例数据均为 UTF-8。Windows 控制台若显示乱码，请先执行 `chcp 65001`，或在 VS Code 等 UTF-8 终端中运行。

## 目录结构

```text
├── src/main.c              # 最终版（t6：主菜单整合）
├── data/kb_data.txt        # 示例数据（UTF-8）
└── stages/                 # 各阶段迭代源码，保留开发过程
    ├── t0_basics.c          # 笔记原型：链表 + 标签数组
    ├── t1_adjacency_list.c  # 引入邻接表
    ├── t2_file_io.c         # 文件读写
    ├── t3_tag_processing.c  # 标签处理与匹配
    ├── t4_graph_edges.c     # 自动建边
    └── t5_bfs.c             # BFS 遍历
```

| 阶段 | 主题 | 主要产出 |
|---|---|---|
| t0 | 基础原型 | 笔记链表、标签数组、基础 CRUD |
| t1 | 邻接表 | 图的存储结构 |
| t2 | 文件 IO | 数据保存与加载 |
| t3 | 标签处理 | 标签匹配与搜索 |
| t4 | 建边逻辑 | 相同标签自动连边 |
| t5 | 图算法 | BFS 遍历 |
| t6 | 主菜单整合 | 合并为完整系统（`src/main.c`） |

## 已知限制

- 顶点数组固定为 `MAX_VERTEX = 200`，笔记 ID 需小于 200；
- 每条笔记只有一个标签，标签内的空格会在 `sscanf` 解析时截断；
- 标题/正文/标签中不能包含 `|`（与文件分隔符冲突）；
- 删除笔记后 ID 不复用，长期使用需注意上限。

## 说明

本项目为郑州大学数据结构课程作业的整理版本。原始开发过程保留在 `stages/` 中，最终版位于 `src/main.c`。

## License

[MIT](LICENSE)
