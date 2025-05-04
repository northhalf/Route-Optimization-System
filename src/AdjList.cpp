/*utf-8*/
// AdjList.cpp 实现类的方法具体实现
#include <AdjList.hpp>
#include <algorithm>
#include <climits>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// 考虑可能多次调用，设为inline
inline bool VexNode::insert_point_to(
    size_t end_index, size_t length, size_t pass_flow
) {
    // edge_node指向边表
    auto edge_node = edge_list;
    // 首先判断边表是否存在
    if (edge_node == NULL) {
        // 如果边表不存在则构造边表
        edge_list = std::make_shared<EdgeNode>(length, pass_flow, end_index);
        return true;
    }
    // 遍历当前的所有边，直到最后一个节点
    while (edge_node->next_edge != NULL) {
        // 如果在遍历的时候发现这条边已经存在，则直接返回
        if (edge_node->endvex == end_index) return false;
        edge_node = edge_node->next_edge;
    }
    // 检测最后一个边节点是否为要求的边
    if (edge_node->endvex == end_index) return false;
    // 则这条边为新的边，添加到最后
    edge_node->next_edge =
        std::make_shared<EdgeNode>(length, pass_flow, end_index);
    return true;
}

// 实现插入一条边
void AdjList::insert_edge(
    VexType start, VexType end, size_t length, size_t pass_flow
) {
    // 查找起点是否已经处于顶点数组中
    if (vex_index.find(start) == vex_index.end()) {
        // 这个顶点不在顶点数组中
        // 将这个顶点加入顶点数组
        vexs.emplace_back(start);
        // 更新顶点和数组下标的映射
        vex_index[start] = vexs.size() - 1;
        // 判断顶点个数是否需要更新
        if (vexs.size() > vex_num) vex_num = vexs.size();
    }
    // 判断起点是否和终点重合，不允许自身的回路
    if (start == end) return;
    // 同样查找终点是否位于顶点数组中
    if (vex_index.find(end) == vex_index.end()) {
        vexs.emplace_back(end);
        vex_index[end] = vexs.size() - 1;
        if (vexs.size() > vex_num) vex_num = vexs.size();
    }
    // 设置起点顶点数据，同时记录是否添加了一条新的边
    bool edge_flag = vexs[vex_index[start]].insert_point_to(
        vex_index[end], length, pass_flow
    );
    // 如果添加了新边则边数自增1
    if (edge_flag) edge_num++;
}

// Dijkstra最短路径查找，返回得到的路径
Path AdjList::min_dist_Dijkstra(VexType start, VexType end) {
    // 用来记录待搜索的顶点和到起点的距离的结构体
    struct Dist_vex {
        size_t vex_index;
        size_t dist;
    };

    // 首先查找顶点是否存在，以及终点是否与起点重合
    if (vex_index.find(start) == vex_index.end() ||
        vex_index.find(end) == vex_index.end())
        return Path{};
    // 判断起点是否和终点重合
    if (start == end) {
        Path path{};
        path.pass_nodes.emplace_back(start);
        return path;
    }

    // 顶点存在则查找顶点对应的顶点数组中的下标
    size_t start_index = vex_index[start];
    size_t end_index = vex_index[end];

    // 初始化记录起点到对应顶点的最小距离的dist数组
    std::vector<size_t> dist(vex_num, SIZE_MAX);
    dist[start_index] = 0;  // 起点到自身的距离为0
    // 记录与dist数组同步的人流量信息
    std::vector<size_t> pass_flow(vex_num);

    // 记录最短路径的每个节点的父亲索引
    std::vector<size_t> parent_index(vex_num, SIZE_MAX);

    // 定义记录这个顶点是否被搜索过的无序集
    std::unordered_set<size_t> visited_vex_set;
    // 定义一个优先队列，将此时与起点距离最小的顶点放于堆顶
    auto comparator = [](const Dist_vex& vex1, const Dist_vex& vex2) {
        return vex1.dist > vex2.dist;
    };
    std::priority_queue<Dist_vex, std::vector<Dist_vex>, decltype(comparator)>
        pq(comparator);
    pq.emplace(start_index, 0);  // 先将起点放入优先队列中

    // 循环直到队列为空
    while (!pq.empty()) {
        // 获取队前顶点和距离
        size_t now_vex_index = pq.top().vex_index;
        size_t now_vex_dis = pq.top().dist;
        pq.pop();
        // 如果这个距离不是最短距离则跳过
        if (now_vex_dis > dist[now_vex_index]) continue;
        // 搜索与这个顶点相连的边
        for (auto edge = vexs[now_vex_index].edge_list; edge != NULL;
             edge = edge->next_edge) {
            size_t endvex = edge->endvex;
            // 如果这条边的终点已经访问过，则跳过
            if (visited_vex_set.find(endvex) != visited_vex_set.end()) continue;

            // 如果从这个顶点走的距离更短则更新dist数组，并将这组数据加入队列中
            if (size_t now_dist = now_vex_dis + edge->length;
                now_dist < dist[endvex]) {
                // 更新最短距离
                dist[endvex] = now_dist;
                // 更新对应的人流量信息
                pass_flow[endvex] = pass_flow[now_vex_index] + edge->pass_flow;
                // 更新这个节点的父亲节点
                parent_index[endvex] = now_vex_index;
                pq.emplace(endvex, now_dist);
            }
        }
    }
    // 判断是否存在路径，不存在则返回空路径
    if (dist[end_index] == SIZE_MAX) return Path{};
    // 记录路径
    Path path;
    path.distance = dist[end_index];
    path.pass_flow = pass_flow[end_index];
    // 从终点往回回溯
    for (size_t now_index = end_index; now_index != start_index;
         now_index = parent_index[now_index]) {
        path.pass_nodes.emplace_back(vexs[now_index].vertex);
    }
    path.pass_nodes.emplace_back(start);
    std::reverse(path.pass_nodes.begin(), path.pass_nodes.end());
    return path;
}
