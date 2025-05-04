/*utf-8*/
// 临接表类
#ifndef ADJLIST_HPP
#define ADJLIST_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
typedef size_t VexType;  // 顶点用无符号整数表示
// 路径类，存储路径长度和依次经过的节点
class Path {
public:
    size_t distance;                 // 路径总距离
    size_t pass_flow;                // 路径总人流量
    std::vector<size_t> pass_nodes;  // 路径经过的节点,用节点对应的数字表示
};

// 临接表边节点
class EdgeNode {
public:
    // 构造函数
    // 传入路径长度，人流量，终点位于顶点数组的下标，下一边节点
    EdgeNode(
        int len = 0, int flow = 0, int end = 0,
        std::shared_ptr<EdgeNode> next = nullptr
    )
        : next_edge(next), endvex(end), pass_flow(flow), length(len) {}
    // 设置友元类便于访问
    friend class VexNode;
    friend class AdjList;

private:
    std::shared_ptr<EdgeNode> next_edge;  // 指向下一边节点
    size_t endvex;     // 有向边指向顶点在临接表顶点数组的下标
    size_t pass_flow;  // 人流量
    size_t length;     // 边的长度
};

// 临接表的顶点节点
class VexNode {
public:
    VexNode(VexType vex, std::shared_ptr<EdgeNode> edge_list = nullptr)
        : vertex(vex), edge_list(edge_list) {}
    // 让该顶点增加一条指向另外一个顶点的边，即增加边节点
    // 传入另外一个顶点在顶点数组的下标，以及人流量和边长度信息
    // return:
    // 返回是否添加了一条边，如果添加了则返回true，如果边已经存在则返回false
    bool insert_point_to(size_t end_index, size_t length, size_t pass_flow = 0);

    friend class AdjList;

private:
    VexType vertex;                       // 顶点对应的数字
    std::shared_ptr<EdgeNode> edge_list;  // 指向边表第一个节点
};

class AdjList {
public:
    // 临接表构造函数，传入顶点和边个数
    AdjList(size_t vn = 0, size_t en = 0) : vex_num(vn), edge_num(en) {
        // 提前使数组预留空间
        vexs.reserve(vn);
    }
    // 给临接表插入一条边
    // 传入开始地点和到达地点，路径距离，人流量
    void insert_edge(
        VexType start, VexType end, size_t length, size_t pass_flow = 0
    );
    // Dijkstra最短路径查找，返回得到的路径
    Path min_dist_Dijkstra(VexType start, VexType end);
    // Floyed算法查找所有最短路径
    // 返回两个哈希表嵌套，实现像是path[1][2]查找路径
    std::unordered_map<VexType, std::unordered_map<VexType, Path>>
    min_dist_Floyed();
    // 查找从指定起点和终点的所有可能路径
    std::vector<Path> find_all_path(VexType start, VexType end);

private:
    std::vector<VexNode> vexs;                      // 顶点数组
    std::unordered_map<VexType, size_t> vex_index;  // 顶点和数组下标的映射
    size_t vex_num, edge_num;  // 顶点个数和边的个数
};

#endif