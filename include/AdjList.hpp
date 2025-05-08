/*utf-8*/
// 临接表类
#ifndef ADJLIST_HPP
#define ADJLIST_HPP

#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
typedef size_t VexType;  // 顶点用无符号整数表示
// 路径类，存储路径长度和依次经过的节点
class Path {
public:
    size_t distance;                 // 路径总距离
    size_t people_num;               // 路径总人数
    std::vector<size_t> pass_nodes;  // 路径经过的节点,用节点对应的数字表示
};

class AdjList;
// 临接表边节点
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

class AdjList {
public:
    // 临接表构造函数，传入顶点和边个数
    AdjList(size_t vex_num = 0, size_t edge_num = 0)
        : vex_num(vex_num), edge_num(edge_num) {
        // 提前使数组预留空间
        vexs.reserve(vex_num);
    }

    /*
     * @brief 给临接表插入一条边
     * @param start: 起点
     * @param end: 终点
     * @param length: 路径长度
     * @param people_num: 这条路有多少人
     **/
    void insert_edge(
        VexType start, VexType end, size_t length, size_t people_num = 0
    );

    /*
     * @brief 查找一条可能的疏散路径
     * @param start: 起点
     * @param end: 终点
     * @param max_density: 最大路径人密度，只有小于此密度的路径才会被考虑
     * @param max_one_path_density: 路人密度的阈值，小于此的路线才可行
     * @return 一条可行路径
     **/
    Path possible_evacuation(
        VexType start, VexType end, size_t max_density,
        size_t max_one_path_density
    );

    /*
     * @brief Dijkstra最短路径查找从一个顶点出发的所有最短路径
     * @param start: 起点
     * @return 映射终点和路径的哈希表
     **/
    std::unordered_map<VexType, Path> min_dist_multi_path_Dijkstra(VexType start
    );

    /*
     * @brief Dijkstra最短路径查找，返回得到的路径
     * @param start: 起点
     * @param end: 终点
     * @return 最短路径
     **/
    Path min_dist_one_path_Dijkstra(VexType start, VexType end);

    /*
     * @brief Floyed算法查找所有的最短路径
     * @return 哈希表嵌套，将起点和终点映射到一条路径
     **/
    std::unordered_map<VexType, std::unordered_map<VexType, Path>>
    min_dist_Floyed();

    /*
     * @brief 查找从指定起点和终点的所有可能路径
     * @param start: 起点
     * @param end: 终点
     * @param max_density: 总路径允许的最大人流密度
     * @param max_one_path_density: 一条边允许的最大人流密度
     * @return 包含所有可能路径的向量，按路径长度递增排序
     **/
    std::vector<Path> find_all_path(
        VexType start, VexType end, size_t max_density = SIZE_MAX,
        size_t max_one_path_density = SIZE_MAX
    );
    friend std::ostream& operator<<(std::ostream& os, AdjList& adj);

private:
    // 实现对于图的从起点到不同顶点的最短距离数组，对应人流量数组，以及每条最短路径终点的
    // 父节点数组的查找，并将这三个数据以元组形式打包返回
    std::tuple<std::vector<size_t>, std::vector<size_t>, std::vector<size_t>>
    __get_dist_passFlow_parent(size_t start_index);
    // 指定起点和终点查找路径，flag用于判断是否寻找多条路径
    std::vector<Path> __find_path(
        VexType start, VexType end, bool multi_flag, size_t max_density,
        size_t max_one_path_density
    );

    std::vector<VexNode> vexs;                      // 顶点数组
    std::unordered_map<VexType, size_t> vex_index;  // 顶点和数组下标的映射
    size_t vex_num, edge_num;  // 顶点个数和边的个数
};

std::istream& operator>>(std::istream& is, AdjList& adj);
std::ostream& operator<<(std::ostream& os, AdjList& adj);

#endif