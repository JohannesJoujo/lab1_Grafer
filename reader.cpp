//
// Created by Johannes Joujo on 2023-03-29.
//

#include <set>
#include <queue>
#include <stack>
#include "reader.h"



enum token{
    COMMENT, META, EDGE, END_OF_FILE
};

token get_line_type(std::istream& is){
    switch(is.peek()){
        case std::istream::traits_type::eof():      return END_OF_FILE;
        case '#':                                   return COMMENT;
        case 'M':                                   return META;
    };
    return EDGE;
}

meta_t meta;

edge read_edge(std::istream& is){
    edge e;
    is >> e.n1 >> e.n2 >> e.weight;
    std::getline(is, e.description);
    return e;
}

void read_meta(std::istream& is){
    char discard;
    node_id_t vertex_id;
    std::string name;
    is >> discard >> vertex_id;
    std::getline(is, name);
    meta[vertex_id] = name;
}

adjacency_list_t parse_file(std::string filename){
    std::ifstream in(filename);
    token l;
    edge_list_t edge_list;
    while((l = get_line_type(in)) != END_OF_FILE){
        edge e;
        switch(l){
            case token::EDGE:
                e = read_edge(in);
                edge_list.push_back(e);
                break;
            case token::META:
                read_meta(in);
                break;
            default:
                std::string comment;
                std::getline(in, comment);
        }
    }
    return adjacency_list_t{meta, edge_list};
}

std::vector<node> DFS(node_id_t start) {
    auto file_content = parse_file("edges.txt");
    auto edges = has_edge(start, file_content);
    std::vector<node> all_visited_nodes;
    std::stack<node> stack;
    node temp;
    temp.n1 = start;
    stack.push(temp);

    std::set<node_id_t> visited_nodes;

    while (!stack.empty()) {
        node temp2 = stack.top();
        stack.pop();

        if (visited_nodes.count(temp2.n1) == 0) {
            visited_nodes.insert(temp2.n1);
            all_visited_nodes.push_back(temp2);
            edges = has_edge(temp2.n1, file_content);
            if (!edges.empty()) {
                for (auto edge : edges) {
                    node temp3;
                    temp3.n1 = edge.first;
                    stack.push(temp3);
                }
            }
        }
    }

    return all_visited_nodes;
}

std::vector<node> BFS(node_id_t start) {
    auto file_content = parse_file("edges.txt");
    std::vector<node> all_visited_nodes;
    std::set<node_id_t> visited_nodes;
    std::queue<node> node_queue;

    node temp;
    temp.n1 = start;
    node_queue.push(temp);

    while (!node_queue.empty()) {
        temp = node_queue.front();
        node_queue.pop();

        if (visited_nodes.count(temp.n1) == 0) {
            visited_nodes.insert(temp.n1);
            all_visited_nodes.push_back(temp);

            auto edges = has_edge(temp.n1, file_content);
            for (auto edge : edges) {
                node neighbor;
                neighbor.n1 = edge.first;
                node_queue.push(neighbor);
            }
        }
    }

    return all_visited_nodes;
}

std::vector<node> DIKSTRAS(node_id_t start, node_id_t end) {
    auto file_content = parse_file("edges.txt");
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;
    std::set<std::pair<node_id_t, double>> shortest_path;
    std::vector<node> all_nodes(file_content.first.size());
    for (int i = 0; i < all_nodes.size(); i++) {
        all_nodes.at(i).weight = 999999;
        all_nodes.at(i).n1 = i;
    }
    all_nodes.at(start).weight = 0;

    pq.emplace(0.0, start);
    while (!pq.empty()) {
        int temp_node_int = pq.top().second;
        double temp_node_weight = pq.top().first;
        pq.pop();
        auto start_neighbours = has_edge(temp_node_int, file_content);

        for (auto edge : start_neighbours) {
            double new_weight = temp_node_weight + edge.second;
            if (new_weight < all_nodes.at(edge.first).weight) {
                pq.emplace(new_weight, edge.first);
                all_nodes.at(edge.first).weight = new_weight;
                all_nodes.at(edge.first).prev = temp_node_int;
                shortest_path.emplace(edge.first, new_weight);
            }
        }
    }

    std::cout<< "\nThe shortest path from "<<start<<" to "<<end<<" is "<<all_nodes.at(end).weight<<"m\n";
    std::cout<<"Path: ";
    std::vector<node_id_t> path;
    node_id_t curr_node = end;
    while (curr_node != start) {
        path.push_back(curr_node);
        curr_node = all_nodes.at(curr_node).prev;
    }
    path.push_back(start);
    //std::reverse(path.begin(), path.end());
    for(auto e:path){
        if(e!=end) {
            std::cout << e << "-> ";
        }else{
            std::cout<<e;
        }
    }
    std::cout<<"\n";
    std::cout<<"\n";
    return all_nodes;
}

std::vector<std::pair<std::pair<node_id_t, node_id_t>,double>> connnected_nodes(const adjacency_list_t& graph){
    std::vector<std::pair<std::pair<node_id_t, node_id_t>,double>> node_pair_vec;
    for (const auto& edge : graph.second) {
        auto par=std::make_pair(edge.n1,edge.n2);

        node_pair_vec.emplace_back(par,edge.weight);
    }
    return node_pair_vec;
}
std::vector<std::pair<node_id_t,double>> has_edge(node_id_t n1, const adjacency_list_t& graph) {
    auto node_pair_vec = connnected_nodes(graph);
    std::vector<std::pair<node_id_t,double>> edges;
    for (const auto& node_pair : node_pair_vec) {
        if ((node_pair.first.first == n1)){
            edges.emplace_back(node_pair.first.second,node_pair.second);
        }
    }
    return edges;
}
