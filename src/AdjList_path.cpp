/*UTF-8*/
// AdjList_path.cpp 实现类查找路径方法具体实现
#include <algorithm>
#include <climits>
#include <functional>
#include <limits>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "AdjList.hpp"

// 查找可行路径,bfs
Path AdjList::possible_evacuation(
    VexType start, VexType end, size_t max_density, size_t max_one_path_density
) {
    auto all_path =
        __find_path(start, end, false, max_density, max_one_path_density);

    if (all_path.size() == 0) return Path{};

    return std::move(all_path.front());
}

// 查找所有可行路径
std::vector<Path> AdjList::find_all_path(
    VexType start, VexType end, size_t max_density, size_t max_one_path_density
) {
    auto all_path =
        __find_path(start, end, true, max_density, max_one_path_density);
    std::sort(
        all_path.begin(), all_path.end(),
        [](const Path& path_1, const Path& path_2) -> bool {
            return path_1.distance < path_2.distance;
        }
    );
    return all_path;
}

// 查找从指定起点出发的所有最短路径
std::unordered_map<VexType, Path> AdjList::min_dist_multi_path_Dijkstra(
    VexType start, size_t max_one_path_density
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
        __get_dist_passFlow_parent(start_index, max_one_path_density);

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
Path AdjList::min_dist_one_path_Dijkstra(
    VexType start, VexType end, size_t max_one_path_density
) {
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
        __get_dist_passFlow_parent(start_index, max_one_path_density);

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

std::unordered_map<VexType, std::unordered_map<VexType, Path>>
AdjList::min_dist_Floyed() {
    struct path_dist {
        size_t parent;            // 从哪个顶点到达这个顶点
        size_t dist, people_num;  // 路径距离和人数
    };

    // 距离矩阵
    std::vector<std::vector<path_dist>> dist(
        vex_num, std::vector<path_dist>(vex_num, {SIZE_MAX, SIZE_MAX, SIZE_MAX})
    );
    // 遍历临接表获取初始值
    for (size_t i = 0; i < vex_num; i++) {
        for (auto edge = vexs[i].edge_list; edge != nullptr;
             edge = edge->next_edge) {
            dist[i][edge->endvex] = {i, edge->length, edge->people_num};
        }
    }
    // 对角线元素距离为0
    for (size_t i = 0; i < vex_num; i++) dist[i][i] = {i, 0, 0};
    // 三层循环，遍历中间节点，起始节点和目标顶点
    for (size_t k = 0; k < vex_num; k++) {
        for (size_t i = 0; i < vex_num; i++) {
            for (size_t j = 0; j < vex_num; j++) {
                // 如果路径不存在则跳过
                if (dist[i][k].dist == SIZE_MAX || dist[k][j].dist == SIZE_MAX)
                    continue;

                // 路径存在

                // 如果路径比原来更短则选择
                if (size_t dis = dist[i][k].dist + dist[k][j].dist;
                    dis < dist[i][j].dist) {
                    dist[i][j] = {
                        k, dis, dist[i][k].people_num + dist[k][j].people_num};
                }
            }
        }
    }
    // 记录路径
    std::unordered_map<VexType, std::unordered_map<VexType, Path>> multi_path;
    // 回溯路径，枚举起点和终点
    for (size_t i = 0; i < vex_num; i++) {
        for (size_t j = 0; j < vex_num; j++) {
            // 如果路径不存在则跳过
            if (dist[i][j].dist == SIZE_MAX) continue;
            Path path;  // 记录路径
            path.distance = dist[i][j].dist;
            path.people_num = dist[i][j].people_num;
            // 回溯
            for (size_t now = j; now != i; now = dist[i][now].parent) {
                path.pass_nodes.push_back(vexs[now].vertex);
            }
            path.pass_nodes.push_back(vexs[i].vertex);
            std::reverse(path.pass_nodes.begin(), path.pass_nodes.end());
            multi_path[vexs[i].vertex][vexs[j].vertex] = std::move(path);
        }
    }
    return multi_path;
}