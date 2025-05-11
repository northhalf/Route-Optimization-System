/*UTF-8*/
/* VexNode_public.cpp 实现VexNode类方法*/
#include "VexNode.hpp"

#include <memory>

#include "EdgeNode.hpp"
// 考虑可能多次调用，设为inline
bool VexNode::insert_point_to(
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