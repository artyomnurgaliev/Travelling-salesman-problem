#ifndef MOD_2_TASK_2_2_TSP_MST_H
#define MOD_2_TASK_2_2_TSP_MST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <list>
struct Node{
    int root = -1;
    int num = 0;
    Node* prev = nullptr;

    explicit Node(int root);
};
struct Edge{
    double w = 0.0;
    Node* b = nullptr;
    Node* e = nullptr;
    Edge(double w, Node*& b, Node*& e);
};
struct SimpleEdge{
    int b=0;
    int e=0;
    double w=0;
    SimpleEdge(int b, int e, double w): b(b), e(e), w(w){};
};



class Graph{
private:
    std::vector<Edge> edges;
    std::vector<Node*> nodes;
public:
    std::vector<std::vector<double>> distance;
    explicit Graph(std::vector<std::pair<double ,double>>& input);
    ~Graph();
    std::vector<std::vector<int>> find_MST();
    double find_tsp_with_mst(std::vector<std::vector<int>>& next);
    void find_tsp_with_bust(std::vector<int> vec, double& min, int ind);
    double find_tsp_with_busting_couples_and_eulerian_cycle();
};

#endif //MOD_2_TASK_2_2_TSP_MST_H


