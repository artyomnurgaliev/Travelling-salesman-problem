//
// Created by artum on 27.04.2019.
//

#include "tsp_mst.h"
#include <stack>

Node::Node(int root): root(root), num(root){}

Edge::Edge(double w, Node *&b, Node *&e):w(w), b(b), e(e){}

bool Compare(Edge& edge1, Edge& edge2){
    return edge1.w > edge2.w;
}

int GetRoot(Node*& node){
    if (node->root != -1)
        return node->root;
    else {
        while (true){
            if (node->prev->root != -1){
                return node->prev->root;
            }
            else {
                if (node->prev->prev->root !=-1){
                    node->prev = node->prev->prev;
                    return node->prev->root;
                }
                else{
                    node->prev->prev = node->prev->prev->prev;
                    node->prev = node->prev->prev;
                }
            }
        }
    }
}
void EdgeCheck(Edge& edge, int& cnt, std::vector<std::vector<int>>& next){
    if (GetRoot(edge.b) != GetRoot(edge.e)){
        cnt++;
        if (edge.b->root != -1){
            if (edge.e->root !=-1){
                edge.b->prev = edge.e;
            }
            else{
                edge.b->prev = edge.e->prev;
            }
            edge.b->root =-1;
        }
        else{
            edge.b->prev->root = -1;
            if (edge.e->root !=-1){
                edge.b->prev->prev = edge.e;
            }
            else{
                edge.b->prev->prev = edge.e->prev;
            }
        }
        next[edge.b->num].emplace_back(edge.e->num);
        next[edge.e->num].emplace_back(edge.b->num);
    }
}

void Solution(std::vector<Edge> edges, int n, std::vector<std::vector<int>>& next){
    std::make_heap(edges.begin(), edges.end(), Compare);
    int cnt = 0;
    while (cnt != n - 1) {
        std::pop_heap(edges.begin(), edges.end(), Compare);
        EdgeCheck(edges.back(), cnt, next);
        edges.pop_back();
    }
}

double count_distance(std::pair<double, double > &node1, std::pair<double, double> &node2){
    return std::sqrt(std::abs(node1.first - node2.first)*std::abs(node1.first - node2.first)
                     + std::abs(node1.second - node2.second)*std::abs(node1.second - node2.second));
}


Graph::Graph(std::vector<std::pair<double, double>> &input) : distance(input.size(), std::vector<double>(input.size(), 0)){
    edges.clear();
    nodes.clear();
    for (int i = 0; i < input.size(); ++i){
        nodes.push_back(new Node(i));
    }
    for (int i=0; i<input.size(); ++i){
        for (int j=i+1; j<input.size(); ++j) {
            distance[i][j] = count_distance(input[i], input[j]);
            distance[j][i] = distance[i][j];
            auto edge = Edge(distance[i][j], nodes[i], nodes[j]);
            edges.push_back(edge);
        }
    }
}
Graph::~Graph() {
    for (auto node: nodes){
        delete node;
    }
}

std::vector<std::vector<int>> Graph::find_MST() {
    std::vector<std::vector<int>> next(nodes.size());
    Solution(edges, nodes.size(), next);
    return next;
}
void Graph::find_tsp_with_bust(std::vector<int> vec, double &min, int ind) {
    vec.push_back(ind);
    if (vec.size() == nodes.size()){
        double res = 0;
        for (int j=1; j<vec.size();++j){
            res+=distance[vec[j-1]][vec[j]];
        }
        res+=distance[vec[vec.size()-1]][vec[0]];
        if (res < min){
            min = res;
        }
        return;
    }
    else{
        for (int i=0; i<nodes.size(); ++i){
            if (std::find(vec.begin(), vec.end(), i) == vec.end()){
                find_tsp_with_bust(vec, min, i);
            }
        }
    }
}

double Graph::find_tsp_with_mst(std::vector<std::vector<int>>& next) {
    std::vector<int> visited(nodes.size(), 0);
    std::vector<int> previdx(nodes.size(), 0);
    std::vector<int> idxs(nodes.size(), -1);

    std::vector<int> vec;
    int curr = 0;
    vec.push_back(curr);
    idxs[curr] = 0;

    visited[curr] = 1;
    while (vec.size() != next.size()) {
        int t = -1;
        for (int item: next[curr]){
            if (!visited[item]){
                t = item;
                break;
            }
        }
        if (t != -1){
            vec.push_back(t);
            idxs[t] = vec.size()-1;
            previdx[t] = idxs[curr];
            curr = t;
            visited[curr] = 1;
        }
        else{
            curr = vec[previdx[curr]];
        }
    }
    double res = 0;
    for (int j=1; j<vec.size();++j){
        res+=distance[vec[j-1]][vec[j]];
    }
    res+=distance[vec[vec.size()-1]][vec[0]];
    return res;
}
bool cmp(const SimpleEdge &a, const SimpleEdge &b){
    return a.w>b.w;
}

double Graph::find_tsp_with_busting_couples_and_eulerian_cycle() {
    std::vector<std::vector<int>> next = find_MST();
    std::vector<int> odd_vertices;
    for (int i=0; i<next.size();++i){
        if (next[i].size() % 2 != 0){
            odd_vertices.push_back(i);
        }
    }
    std::vector<SimpleEdge> edges_between_odd_vert;
    for (int i=0; i<odd_vertices.size(); ++i){
        for (int j=i+1; j<odd_vertices.size(); ++j){
            edges_between_odd_vert.emplace_back(odd_vertices[i], odd_vertices[j], distance[odd_vertices[i]][odd_vertices[j]]);
        }
    }
    std::make_heap(edges_between_odd_vert.begin(), edges_between_odd_vert.end(), cmp);

    std::vector<int> visited(nodes.size(), 0);
    int cnt = 0;
    while (cnt != odd_vertices.size()){
        std::pop_heap(edges_between_odd_vert.begin(), edges_between_odd_vert.end(), cmp);
        auto &i = edges_between_odd_vert.back();
        int b = i.b;
        int e = i.e;

        if (!visited[b] && !visited[e]) {
            visited[b] = true;
            visited[e] = true;
            if (std::find(next[b].begin(), next[b].end(), e) == next[b].end())
                next[b].emplace_back(e);
            if (std::find(next[e].begin(), next[e].end(), b) == next[e].end())
                next[e].emplace_back(b);
            cnt += 2;
        }

        edges_between_odd_vert.pop_back();
    }

    return find_tsp_with_mst(next);
}
