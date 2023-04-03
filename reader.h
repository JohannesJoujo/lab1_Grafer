//
// Created by Johannes Joujo on 2023-03-29.
//

#ifndef LAB1_GRAFER_READER_H
#define LAB1_GRAFER_READER_H
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>

using node_id_t = int;
using weight_t = double;
using meta_t = std::map<node_id_t,std::string>;

struct edge{
    node_id_t n1;
    node_id_t n2;
    weight_t weight;
    std::string description;
};
struct node{
    node_id_t n1=0;
    bool visited;
};

using edge_list_t = std::vector<edge>;

using adjacency_list_t = std::pair<meta_t,edge_list_t>;
void print_graph(const adjacency_list_t& graph);
adjacency_list_t parse_file(std::string filename);
std::vector<std::pair<std::pair<node_id_t, node_id_t>,double>> connnected_nodes(const adjacency_list_t& graph);
std::vector<std::pair<node_id_t,double>> has_edge(node_id_t n1, const adjacency_list_t& graph);
//bool DFS(node_id_t n1);
std::vector<node> DFS(node_id_t start);
std::vector<node> BFS(node_id_t start);
bool complete_DFS();
#endif //LAB1_GRAFER_READER_H
