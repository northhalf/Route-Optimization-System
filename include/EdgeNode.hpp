/*UTF-8*/
#ifndef EDGENODE_HPP
#define EDGENODE_HPP

#include <iostream>
#include <memory>
#include <vector>

typedef size_t VexType;  // 顶点用无符号整数表示

class AdjList;

class EdgeNode {
public:
    // 构造函数
    // 传入路径长度，人流量，终点位于顶点数组的下标，下一边节点
    EdgeNode(
        int len = 0, int people_num = 0, int end = 0,
        std::shared_ptr<EdgeNode> next = nullptr
    )
        : next_edge(next), endvex(end), people_num(people_num), length(len) {}
    // 设置友元类便于访问
    friend class VexNode;
    friend class AdjList;
    friend std::ostream& operator<<(std::ostream& os, AdjList& adj);

private:
    std::shared_ptr<EdgeNode> next_edge;  // 指向下一边节点
    size_t endvex;  // 有向边指向顶点在临接表顶点数组的下标
    size_t people_num;  // 人数
    size_t length;      // 边的长度
};

#endif