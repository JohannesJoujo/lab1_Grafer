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
void print_graph(const adjacency_list_t& graph) {
    std::cout << "Printing graph:" << std::endl;

    // Print metadata
    std::cout << "Metadata:" << std::endl;
    for (const auto& meta1 : graph.first) {
        std::cout << "Node " << meta1.first << ": " << meta1.second << std::endl;
    }
    std::cout << std::endl;

    // Sort adjacency list by node ID
    auto sorted_adj_list = graph.second;
    std::sort(sorted_adj_list.begin(), sorted_adj_list.end(),
              [](const edge& e1, const edge& e2) { return e1.n1 < e2.n1; });

    // Print edges
    std::cout << "Edges:" << std::endl;
    for (const auto& edge : graph.second) {
        std::cout << "Node " << edge.n1 << " -> Node " << edge.n2
                  << ", weight: " << edge.weight
                  << ", description: " << edge.description << std::endl;
    }
}

std::vector<node> DFS(node_id_t start) {
    auto a = parse_file("edges.txt");
    auto edges = has_edge(start, a);
    std::vector<node> all_visited_nodes;
    std::stack<node> stack;
    node temp;
    temp.n1 = start;
    temp.visited = false;
    stack.push(temp);

    std::set<node_id_t> visited_nodes;

    while (!stack.empty()) {
        node temp2 = stack.top();
        stack.pop();

        if (!temp2.visited && visited_nodes.count(temp2.n1) == 0) {
            temp2.visited = true;
            visited_nodes.insert(temp2.n1);
            all_visited_nodes.push_back(temp2);
            edges = has_edge(temp2.n1, a);
            if (!edges.empty()) {
                for (auto edge : edges) {
                    node temp3;
                    temp3.n1 = edge.first;
                    temp3.visited = false;
                    stack.push(temp3);
                }
            }
        }
    }
    return all_visited_nodes;

}

std::vector<node> BFS(node_id_t start) {
    auto a = parse_file("edges.txt");
    std::vector<node> all_visited_nodes;
    std::set<node_id_t> visited_nodes;
    std::queue<node> node_queue;

    node temp;
    temp.n1 = start;
    temp.visited = false;
    node_queue.push(temp);

    while (!node_queue.empty()) {
        temp = node_queue.front();
        node_queue.pop();

        if (!temp.visited && visited_nodes.count(temp.n1) == 0) {
            temp.visited = true;
            visited_nodes.insert(temp.n1);
            all_visited_nodes.push_back(temp);

            auto edges = has_edge(temp.n1, a);
            for (auto edge : edges) {
                node neighbor;
                neighbor.n1 = edge.first;
                neighbor.visited = false;
                node_queue.push(neighbor);
            }
        }
    }

    return all_visited_nodes;
}
/*
std::vector<node> DIKSTRAS(node_id_t start,node_id_t end) {
    auto a = parse_file("edges.txt");
    std::priority_queue<std::pair<double,int>, std::vector<std::pair<double,int>>, std::greater<>>pq;
    std::set<std::pair<node_id_t,double>>shortest_path;
    std::vector<node>all_nodes(a.first.size());
    for(int i=0;i<all_nodes.size();i++){
        all_nodes.at(i).weight=999999;
        all_nodes.at(i).n1=i;
    }
    all_nodes.at(start).weight=0;

    //shortest_path.emplace(start,0);
    pq.emplace(0,start);
    while(!pq.empty()){
        int temp_node_int=pq.top().second;
        double temp_node_weight=pq.top().first;
        pq.pop();
        auto start_neighbours = has_edge(temp_node_int, a);

        for(auto edge:start_neighbours){
            double new_weight = temp_node_weight + edge.second;
            if(new_weight < all_nodes.at(edge.first).weight){
                std::cout << "node: " << temp_node_int << " new_weight: " << new_weight << " ";
                pq.emplace(new_weight, edge.first);
                start_neighbours.pop_back();
                all_nodes.at(temp_node_int).weight = new_weight;
                shortest_path.emplace(edge.first, new_weight);
            }

        }

    }

    //while(shortest_path.size()<a.first.size()) {
    while(!pq.empty()) {
    //while(!pq.empty() && shortest_path.count(pq.top())==0) {
    //while(shortest_path.size()<a.first.size()) {
    //while(shortest_path.size()<a.first.size()) {
        auto temp_node_int=pq.top().second;
        auto temp_node_weight=pq.top().first;
        pq.pop();
        if(shortest_path.count(pq.top())==0) {
            auto start_neighbours = has_edge(temp_node_int, a);

            while (!start_neighbours.empty()) {
                double new_weight = 0;
                new_weight = temp_node_weight + start_neighbours.back().second;

                std::cout << "node: " << temp_node_int << " new_weight: " << new_weight << " ";
                pq.emplace(start_neighbours.back().second, new_weight);
                start_neighbours.pop_back();
                all_nodes.at(temp_node_int).weight = new_weight;
                shortest_path.emplace(start_neighbours.back().first, new_weight);

            }
        }

        if(temp_node_int==end){
            std::cout<<"break\n";
            break;
        }
    }

    std::cout<<"\nsize of shortest_path: "<<shortest_path.size()<<"\n";
    std::cout<<"size of neighbours: "<<all_nodes.size()<<"\n";


pq.emplace(12,12);
pq.emplace(2,2);
pq.emplace(1,2);
pq.emplace(0,12);

    while (!pq.empty()) {
        std::pair<double, int> top_element = pq.top();
        pq.pop();
        std::cout << top_element.first << " " << top_element.second << std::endl;
    }
    //std::cout<<"size of shortest_path: "<<shortest_path.size()<<"\n";
    return all_nodes;
}
*/
std::vector<node> DIKSTRAS(node_id_t start, node_id_t end) {
    auto a = parse_file("edges.txt");
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<>> pq;
    std::set<std::pair<node_id_t, double>> shortest_path;
    std::vector<node> all_nodes(a.first.size());
    for (int i = 0; i < all_nodes.size(); i++) {
        all_nodes.at(i).weight = 999999;
        all_nodes.at(i).n1 = i;
    }
    all_nodes.at(start).weight = 0;

    pq.emplace(0, start);
    while (!pq.empty()) {
        int temp_node_int = pq.top().second;
        double temp_node_weight = pq.top().first;
        pq.pop();
        auto start_neighbours = has_edge(temp_node_int, a);

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
    std::reverse(path.begin(), path.end());
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

/*
std::vector<node> BFS(node_id_t start) {
    auto a = parse_file("edges.txt");
    std::vector<node> all_visited_nodes;
    std::set<node_id_t> visited_nodes;
    std::vector<node> node_queue;

    node temp;
    temp.n1 = start;
    temp.visited = false;
    node_queue.push_back(temp);

    while (!node_queue.empty()) {
        temp = node_queue.back();
        node_queue.pop_back();

        if (!temp.visited && visited_nodes.count(temp.n1) == 0) {
            temp.visited = true;
            visited_nodes.insert(temp.n1);
            all_visited_nodes.push_back(temp);

            auto edges = has_edge(temp.n1, a);
            for (auto edge : edges) {
                node neighbor;
                neighbor.n1 = edge.first;
                neighbor.visited = false;
                node_queue.push_back(neighbor);
            }
        }
    }

    return all_visited_nodes;
}
}*/

/*
std::vector<std::pair<node_id_t, node_id_t>> connnected_nodes(const adjacency_list_t& graph){
    std::vector<std::pair<node_id_t, node_id_t>> node_pair_vec;
    for (const auto& edge : graph.second) {
        auto a=std::make_pair(edge.n1,edge.n2);
        node_pair_vec.emplace_back(a);
    }
    return node_pair_vec;
}*/
/*
std::vector<node_id_t> has_edge(node_id_t n1, const adjacency_list_t& graph) {
    auto node_pair_vec = connnected_nodes(graph);
    std::vector<node_id_t> edges;
    for (const auto& node_pair : node_pair_vec) {
        if ((node_pair.first == n1)){
            edges.emplace_back(node_pair.second);
            //std::cout << n1 << " -> " <<node_pair.second << std::endl;
            //return true;
        }
    }
    return edges;
}*/
std::vector<std::pair<std::pair<node_id_t, node_id_t>,double>> connnected_nodes(const adjacency_list_t& graph){
    std::vector<std::pair<std::pair<node_id_t, node_id_t>,double>> node_pair_vec;
    for (const auto& edge : graph.second) {
        auto a=std::make_pair(edge.n1,edge.n2);

        node_pair_vec.emplace_back(a,edge.weight);
    }
    return node_pair_vec;
}
std::vector<std::pair<node_id_t,double>> has_edge(node_id_t n1, const adjacency_list_t& graph) {
    auto node_pair_vec = connnected_nodes(graph);
    std::vector<std::pair<node_id_t,double>> edges;
    for (const auto& node_pair : node_pair_vec) {
        if ((node_pair.first.first == n1)){
            edges.emplace_back(node_pair.first.second,node_pair.second);
            //std::cout << n1 << " -> " <<node_pair.second << std::endl;
            //return true;
        }
    }
    return edges;
}
