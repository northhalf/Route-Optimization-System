/*UTF-8*/
/* AdjList_others.cpp 实现其他方法*/
#include <iostream>

#include "AdjList.hpp"
#include "EdgeNode.hpp"
#include "VexNode.hpp"

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

std::istream& operator>>(std::istream& is, AdjList& adj) {
    VexType start, end;
    size_t length, people_num;
    while (is >> start >> end >> length >> people_num) {
        adj.insert_edge(start, end, length, people_num);
    }
    return is;
}
std::ostream& operator<<(std::ostream& os, AdjList& adj) {
    for (size_t index = 0; index < adj.vex_num; index++) {
        for (auto edge = adj.vexs[index].edge_list; edge != nullptr;
             edge = edge->next_edge) {
            os << "start:" << adj.vexs[index].vertex
               << "\tend:" << adj.vexs[edge->endvex].vertex
               << "\tlength:" << edge->length
               << "\tpeople_num:" << edge->people_num << std::endl;
        }
    }
    return os;
}