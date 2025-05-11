# 城市道路优化系统

总体目录结构:

```shell
├─data
├─include
└─src
```

- `data`：存放测试数据，即道路和节点数据。
- `include`：存放头文件，包含类的定义
- `src`：存放`cpp`源文件

***

`Clang-format`格式：

```plaintext
{ 
    BasedOnStyle: Google, 
    AlignAfterOpenBracket: BlockIndent,
    IndentWidth: 4,
    AccessModifierOffset: -4
}
```

已实现的功能:

- 可行路径查找(根据人流量)
- 冗余路径查找
- 单起点、单目标的Dijkstra算法
- 单起点、多目标的Dijkstra算法
- 多起点、多目标的Floyed算法
