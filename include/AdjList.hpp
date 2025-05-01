/*utf-8*/
// 临接表类
#ifndef ADJLIST_HPP
#define ADJLIST_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
// 路径类，存储路径长度和依次经过的节点
class Path {
public:
    size_t distance;                      // 路径总距离
    size_t pass_flow;                     // 路径总人流量
    std::vector<std::string> pass_nodes;  // 路径经过的节点
};

// 临接表边节点
class EdgeNode {
public:
    // 构造函数
    // 传入路径长度，人流量，终点位于顶点数组的下标，下一边节点
    EdgeNode(int len = 0, int flow = 0, int end = 0, EdgeNode* next = nullptr)
        : length(len), pass_flow(flow), endvex(end), next_edge(next) {}
    // 设置友元类便于访问
    friend class VexNode;
    friend class AdjList;

private:
    std::shared_ptr<EdgeNode> next_edge;  // 指向下一边节点
    int endvex;     // 有向边指向顶点在临接表顶点数组的下标
    int pass_flow;  // 人流量
    int length;     // 边的长度
};

// 临接表的顶点节点
class VexNode {
public:
    // 让该顶点增加一条指向另外一个顶点的边，即增加边节点
    // 传入另外一个顶点在顶点数组的下标
    void insert_point_to(int end_index);

    friend class AdjList;

private:
    std::string vertex;                   // 顶点名字
    std::shared_ptr<EdgeNode> edge_list;  // 指向边表第一个节点
};

class AdjList {
public:
    // 临接表构造函数，传入顶点和边个数
    AdjList(int vn = 0, int en = 0) : vex_num(vn), edge_num(en) {
        // 提前使数组预留空间
        vexs.reserve(vn);
    }
    // 给临接表插入一条边
    // 传入开始地点和到达地点，路径距离
    void insert_edge(std::string start, std::string end, int dist);
    // Dijkstra最短路径查找，返回得到的路径
    Path min_dist_Dijkstra(std::string start, std::string end);
    // Floyed算法查找所有最短路径
    // 返回两个哈希表嵌套，实现像是path["A"]["B"]查找路径
    std::unordered_map<std::string, std::unordered_map<std::string, Path>>
    min_dist_Floyed();
    // 查找从指定起点和终点的所有可能路径
    std::vector<Path> find_all_path(std::string start, std::string end);

private:
    std::vector<VexNode> vexs;                       // 顶点数组
    std::unordered_map<std::string, int> vex_index;  // 顶点和数组下标的映射
    int vex_num, edge_num;  // 顶点个数和边的个数
};

#endif