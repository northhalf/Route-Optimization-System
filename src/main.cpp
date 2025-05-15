// -*- coding: utf-8 -*-
#include <AdjList.hpp>
#include <algorithm>
#include <fstream>
#include <iostream>
int main() {
    std::ifstream is(
        "D:/computerscience/programme/route-optimization-system/data/"
        "test_data.txt"
    );
    AdjList adj;
    is >> adj;
    auto path = adj.min_dist_Floyed();
    auto path1 = path[66][18];
    auto path2 = path[91][23];
    printf(
        "distance:%llu, density of people:%lf\n", path1.distance,
        static_cast<double>(path1.people_num) /
            static_cast<double>(path1.distance)
    );
    for (auto i = path1.pass_nodes.begin(); *i != path1.pass_nodes.back();
         i++) {
        printf("%llu->", *i);
    }
    printf("%llu\n", path1.pass_nodes.back());
    printf(
        "distance:%llu, density of people:%lf\n", path2.distance,
        static_cast<double>(path2.people_num) /
            static_cast<double>(path2.distance)
    );
    for (auto i = path2.pass_nodes.begin(); *i != path2.pass_nodes.back();
         i++) {
        printf("%llu->", *i);
    }
    printf("%llu\n", path2.pass_nodes.back());
    return 0;
}