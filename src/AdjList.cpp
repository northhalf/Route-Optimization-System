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
#include <utility>
#include <vector>

// 考虑可能多次调用，设为inline
inline bool VexNode::insert_point_to(
    size_t end_index, size_t length, size_t people_num
) {
    // edge_node指向边表
    auto edge_node = edge_list;
    // 首先判断边表是否存在
    if (edge_node == NULL) {
        // 如果边表不存在则构造边表
        edge_list = std::make_shared<EdgeNode>(length, people_num, end_index);
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
        std::make_shared<EdgeNode>(length, people_num, end_index);
    return true;
}

// 查找可行路径,bfs
Path AdjList::possible_evacuation(
    VexType start, VexType end, size_t max_density, size_t max_one_path_density
) {
    // 路径节点，记录这个节点的索引，从起点到这个顶点的路径，
    // 起点到这个节点的长度，以及起点到这个节点的路径人数
    // 一个无序set记录经历过的节点
    struct PathNode {
        size_t index;
        std::vector<size_t> path;
        size_t dis;
        size_t people_num;
        std::unordered_set<size_t> visited;
    };
    // 记录起点和终点在顶点数组的下标
    size_t start_index = vex_index[start];
    size_t end_index = vex_index[end];
    // 队列记录待搜索的顶点
    // 起点加入队列，无父亲节点，且设置距离和人数都为0，起点设置为已经访问
    std::queue<PathNode> que(std::deque<PathNode>{
        {start_index, {start_index}, 0, 0, {start_index}}});

    while (!que.empty()) {
        // 获取队首顶点
        PathNode node(std::move(que.front()));
        que.pop();
        // 遍历与这个顶点相连的边
        for (auto edge = vexs[node.index].edge_list; edge != nullptr;
             edge = edge->next_edge) {
            // 如果这条边的人流密度大于限制密度则不考虑
            if (edge->people_num / edge->length > max_one_path_density)
                continue;
            // 如果这条边的目标地点位于路径中，则跳过，避免重复路径
            if (node.visited.find(edge->endvex) != node.visited.end()) {
                continue;
            }

            // 如果到达了终点，则判断人流量密度是否满足要求，并获取路径信息
            size_t pp = node.people_num + edge->people_num,
                   dis = node.dis + edge->length;
            if (edge->endvex == end_index) {
                // 如果路径人流量密度大于要求值，则跳过这条路径
                if (pp / dis > max_density) {
                    continue;
                }
                // 获取路径
                Path path{dis, pp, std::move(node.path)};
                // 将终点加入路径
                path.pass_nodes.emplace_back(end_index);
                // 将路径中的索引改为顶点
                std::transform(
                    path.pass_nodes.begin(), path.pass_nodes.end(),
                    path.pass_nodes.begin(),
                    [this](const size_t node_index) {
                        return this->vexs[node_index].vertex;
                    }
                );
                return path;
            }
            // 将这条边到达的顶点视为新顶点，初始化
            PathNode new_node{edge->endvex, node.path, dis, pp, node.visited};
            // 将这个顶点加入路径
            new_node.path.emplace_back(edge->endvex);
            new_node.visited.emplace(edge->endvex);
            // 顶点加入队列中
            que.push(std::move(new_node));
        }
    }
    return Path{};
}

// 实现插入一条边
void AdjList::insert_edge(
    VexType start, VexType end, size_t length, size_t people_num
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
        vex_index[end], length, people_num
    );
    // 如果添加了新边则边数自增1
    if (edge_flag) edge_num++;
}

// 实现对于图的从起点到不同顶点的最短距离数组，对应人流量数组，以及每条最短路径终点的
// 父节点数组的查找，并将这三个数据以元组形式打包返回
std::tuple<std::vector<size_t>, std::vector<size_t>, std::vector<size_t>>
AdjList::__get_dist_passFlow_parent(size_t start_index) {
    struct Dist_vex {
        size_t vex_index;
        size_t dist;
    };
    // 初始化记录起点到对应顶点的最小距离的dist数组
    std::vector<size_t> dist(vex_num, SIZE_MAX);
    dist[start_index] = 0;  // 起点到自身的距离为0
    // 记录与dist数组同步的路径人数信息
    std::vector<size_t> people_num(vex_num);

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
                // 更新对应的人数信息
                people_num[endvex] =
                    people_num[now_vex_index] + edge->people_num;
                // 更新这个节点的父亲节点
                parent_index[endvex] = now_vex_index;
                pq.emplace(endvex, now_dist);
            }
        }
    }
    return {dist, people_num, parent_index};
}

// 查找从指定起点出发的所有最短路径
std::unordered_map<VexType, Path> AdjList::min_dist_multi_path_Dijkstra(
    VexType start
) {
    // 首先查找顶点是否存在
    if (vex_index.find(start) == vex_index.end())
        return std::unordered_map<VexType, Path>{};

    // 顶点存在则查找顶点对应的顶点数组中的下标
    size_t start_index = vex_index[start];

    // dist:        记录起点到对应顶点的最小距离dist数组
    // people_num:   记录与dist数组同步的人流量信息
    // parend_index:记录最短路径的每个节点的父亲索引
    auto [dist, people_num, parent_index] =
        __get_dist_passFlow_parent(start_index);

    // 记录路径
    std::unordered_map<VexType, Path> path_map;
    // 枚举终点
    for (size_t index = 0; index < vex_num; index++) {
        // 如果路径不存在则跳过循环
        if (dist[index] == SIZE_MAX) continue;
        // 记录一条路径
        Path path{dist[index], people_num[index], {}};
        // 从终点往回回溯
        for (size_t now_index = index; now_index != SIZE_MAX;
             now_index = parent_index[now_index]) {
            path.pass_nodes.emplace_back(vexs[now_index].vertex);
        }
        // 将路径逆转，使得起点位于开头
        std::reverse(path.pass_nodes.begin(), path.pass_nodes.end());
        // 将路径加入哈希表中
        path_map[vexs[index].vertex] = std::move(path);
    }
    return path_map;
}

// Dijkstra最短路径查找，返回一条指定的最短路径
Path AdjList::min_dist_one_path_Dijkstra(VexType start, VexType end) {
    // 用来记录待搜索的顶点和到起点的距离的结构体
    struct Dist_vex {
        size_t vex_index;
        size_t dist;
    };

    // 首先查找顶点是否存在
    if (vex_index.find(start) == vex_index.end() ||
        vex_index.find(end) == vex_index.end())
        return Path{};
    // 判断起点是否和终点重合
    if (start == end) {
        Path path{0, 0, {start}};
        return path;
    }

    // 顶点存在则查找顶点对应的顶点数组中的下标
    size_t start_index = vex_index[start];
    size_t end_index = vex_index[end];

    // dist:        记录起点到对应顶点的最小距离dist数组
    // people_num:   记录与dist数组同步的人流量信息
    // parend_index:记录最短路径的每个节点的父亲索引
    auto [dist, people_num, parent_index] =
        __get_dist_passFlow_parent(start_index);

    // 定义记录这个顶点是否被搜索过的无序集
    // 判断是否存在路径，不存在则返回空路径
    if (dist[end_index] == SIZE_MAX) return Path{};
    // 记录路径
    Path path{dist[end_index], people_num[end_index], {}};
    // 从终点往回回溯
    for (size_t now_index = end_index; now_index != SIZE_MAX;
         now_index = parent_index[now_index]) {
        path.pass_nodes.emplace_back(vexs[now_index].vertex);
    }
    // 翻转数组，使得起点在开头
    std::reverse(path.pass_nodes.begin(), path.pass_nodes.end());
    return path;
}
