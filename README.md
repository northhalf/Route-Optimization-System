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

```
{ 
    BasedOnStyle: Google, 
    AlignAfterOpenBracket: BlockIndent,
    IndentWidth: 4,
    AccessModifierOffset: -4
}
```

目前尚需实现的功能：

- [x] `VexNode::insert_point_to()` 实现为顶点节点插入边节点（正在编写）
- [x] `AdjList::insert_edge()` 实现为临接表插入一条边
- [x] `AdjList::min_dist_multi_path_Dijkstra()` 实现Dijkstra算法查找从起点到其他所有顶点的最短路径
- [x] `AdjList::min_dist_one_path_Dijkstra()` 实现Dijkstra算法查找从起点到某一个顶点的最短路径
- [ ] `AdjList::min_dist_Floyed()` 实现Floyed算法查找所有最短路径
- [ ] `AdjList::find_all_path()` 实现查找从指定起点到终点所有可能路径
