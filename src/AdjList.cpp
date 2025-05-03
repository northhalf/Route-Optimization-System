/*utf-8*/
// AdjList.cpp 实现类的方法具体实现
#include <AdjList.hpp>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// 考虑可能多次调用，设为inline
inline void VexNode::insert_point_to(
    size_t end_index, size_t length, size_t pass_flow
) {
    // edge_node指向边表
    auto edge_node = edge_list;
    // 首先判断边表是否存在
    if (edge_node == NULL) {
        // 如果边表不存在则构造边表
        edge_list = std::make_shared<EdgeNode>(length, pass_flow, end_index);
        return;
    }
    // 遍历当前的所有边，直到最后一个节点
    while (edge_node->next_edge != NULL) {
        // 如果在遍历的时候发现这条边已经存在，则直接返回
        if (edge_node->endvex == end_index) return;
        edge_node = edge_node->next_edge;
    }
    // 检测最后一个边节点是否为要求的边
    if (edge_node->endvex == end_index) return;
    // 则这条边为新的边，添加到最后
    edge_node->next_edge =
        std::make_shared<EdgeNode>(length, pass_flow, end_index);
}

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
    // 同样查找终点是否位于顶点数组中
    if (vex_index.find(end) == vex_index.end()) {
        vexs.emplace_back(end);
        vex_index[end] = vexs.size() - 1;
        if (vexs.size() > vex_num) vex_num = vexs.size();
    }
    // 设置起点顶点数据
    vexs[vex_index[start]].insert_point_to(vex_index[end], length, pass_flow);
}