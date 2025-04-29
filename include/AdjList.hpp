/*gbk*/
// 临接表类
#ifndef ADJLIST_HPP
#define ADJLIST_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

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
    std::unique_ptr<EdgeNode> next_edge;  // 指向下一边节点
    int endvex;     // 有向边指向顶点在临接表顶点数组的下标
    int pass_flow;  // 人流量
    int length;     // 边的长度
};

// 临接表的顶点节点
class VexNode {
public:
    void insert_point_to(int end_index);

private:
    std::string vertex;                   // 顶点名字
    std::unique_ptr<EdgeNode> edge_list;  // 指向边表第一个节点
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
    // 最短路径查找
    std::vector<std::string> min_dist_Dijkstra(
        std::string start, std::string end
    );
    std::vector<std::vector<std::vector<std::string>>> min_dist_Floyed();

private:
    std::vector<VexNode> vexs;                       // 顶点数组
    std::unordered_map<std::string, int> vex_index;  // 顶点和数组下标的映射
    int vex_num, edge_num;  // 顶点个数和边的个数
};

#endif