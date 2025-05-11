/*UTF-8*/
/* AdjList_private.cpp 实现私有方法的定义*/
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <vector>

#include "AdjList.hpp"

std::vector<Path> AdjList::__find_path(
    VexType start, VexType end, bool multi_flag, size_t max_density,
    size_t max_one_path_density
) {
    std::vector<Path> all_path;
    // 路径节点，记录这个节点的索引，从起点到这个顶点的路径，
    // 起点到这个节点的长度，以及起点到这个节点的路径人数
    // 一个无序set记录经历过的节点
    struct PathNode {
        size_t index;              // 顶点在顶点数组中的下标
        std::vector<size_t> path;  // 具体路径，即依次通过的顶点索引
        size_t dis;                // 这条路径的距离
        size_t people_num;         // 这条路径的总人数
        std::unordered_set<size_t>
            visited;  // 这条路径经过的节点，用于避免重复路径
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
                all_path.push_back(std::move(path));
                if (!multi_flag) return all_path;
                // 寻找多条路径时，遇到终点则不进行将终点加入路径的行为
                continue;
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
    return all_path;
}