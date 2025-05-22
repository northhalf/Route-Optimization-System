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

测试数据格式为四元组：1.起点;2.终点;3.长度;4.人数。

已实现的功能:

- 可行路径查找(根据人流量)(`possible_evacuation`)
- 冗余路径查找(`find_all_path`)
- 单起点、单目标的Dijkstra算法(`min_dist_one_path_Dijksra`)
- 单起点、多目标的Dijkstra算法(`min_dist_multi_path_Dijkstra`)
- 多起点、多目标的Floyed算法(`min_dist_Floyed`)
