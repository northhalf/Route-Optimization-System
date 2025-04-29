/*gbk*/
// �ٽӱ���
#ifndef ADJLIST_HPP
#define ADJLIST_HPP
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// �ٽӱ�߽ڵ�
class EdgeNode {
public:
    // ���캯��
    // ����·�����ȣ����������յ�λ�ڶ���������±꣬��һ�߽ڵ�
    EdgeNode(int len = 0, int flow = 0, int end = 0, EdgeNode* next = nullptr)
        : length(len), pass_flow(flow), endvex(end), next_edge(next) {}
    // ������Ԫ����ڷ���
    friend class VexNode;
    friend class AdjList;

private:
    std::unique_ptr<EdgeNode> next_edge;  // ָ����һ�߽ڵ�
    int endvex;     // �����ָ�򶥵����ٽӱ���������±�
    int pass_flow;  // ������
    int length;     // �ߵĳ���
};

// �ٽӱ�Ķ���ڵ�
class VexNode {
public:
    void insert_point_to(int end_index);

private:
    std::string vertex;                   // ��������
    std::unique_ptr<EdgeNode> edge_list;  // ָ��߱��һ���ڵ�
};

class AdjList {
public:
    // �ٽӱ��캯�������붥��ͱ߸���
    AdjList(int vn = 0, int en = 0) : vex_num(vn), edge_num(en) {
        // ��ǰʹ����Ԥ���ռ�
        vexs.reserve(vn);
    }
    // ���ٽӱ����һ����
    // ���뿪ʼ�ص�͵���ص㣬·������
    void insert_edge(std::string start, std::string end, int dist);
    // ���·������
    std::vector<std::string> min_dist_Dijkstra(
        std::string start, std::string end
    );
    std::vector<std::vector<std::vector<std::string>>> min_dist_Floyed();

private:
    std::vector<VexNode> vexs;                       // ��������
    std::unordered_map<std::string, int> vex_index;  // ����������±��ӳ��
    int vex_num, edge_num;  // ��������ͱߵĸ���
};

#endif