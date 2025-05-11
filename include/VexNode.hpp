/*UTF-8*/
#ifndef VEXNODE_HPP
#define VEXNODE_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "EdgeNode.hpp"
// 临接表的顶点节点
class VexNode {
public:
    VexNode(VexType vex, std::shared_ptr<EdgeNode> edge_list = nullptr)
        : vertex(vex), edge_list(edge_list) {}
    // 让该顶点增加一条指向另外一个顶点的边，即增加边节点
    // 传入另外一个顶点在顶点数组的下标，以及人数和边长度信息
    // @return:
    // 返回是否添加了一条边，如果添加了则返回true，如果边已经存在则返回false
    bool insert_point_to(
        size_t end_index, size_t length, size_t people_num = 0
    );

    friend class AdjList;
    friend std::ostream& operator<<(std::ostream& os, AdjList& adj);

private:
    VexType vertex;                       // 顶点对应的数字
    std::shared_ptr<EdgeNode> edge_list;  // 指向边表第一个节点
};

#endif