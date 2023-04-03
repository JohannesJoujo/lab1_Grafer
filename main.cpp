#include "reader.h"
int main() {
    auto a= parse_file("edges.txt");
    std::cout << "Size of graph: " << a.first.size() << " nodes, " << a.second.size() << " edges" << std::endl;
    std::vector<node_id_t>count;
    //print_graph(a);
    //complete_DFS();
    //DFS(1);

    auto edges=connnected_nodes(a);
    //BFS(1);

    for(int i=0;i<a.first.size();i++){
        auto c=DFS(i);
        for (const auto& edge : c) {
            std::cout<<edge.n1<<", ";
        }
        std::cout<<"\nThe size: "<<c.size();
        if(a.first.size()==c.size()+1){
            std::cout<<"\nAll the nodes are connected for node: "<<i<<"\n";
        }else{
            std::cout<<"\nAll the nodes are not connected for node: "<<i<<"\n";
            count.push_back(i);
            //break;
        }
    }
    std::cout<<"\nThese are the nodes that does not have a complete connection to each node in the adjacent list using DFS\n";
    for (const auto& edge : count) {
        std::cout<<edge<<" ";
    }



    //std::cout<<complete_DFS()<<", ";
/*
    std::cout<<"\n---------------------------"<<std::endl;
    auto c= has_edge(2,a);
    std::cout<<"size: "<<c.size()<<'\n';
    std::cout<<"---------------------------"<<std::endl;
    for(int i; i < c.size();i++) {
        std::cout << c[i]<<" ";
        //std::cout << c[rand()%c.size()]<<" ";
    }
    */
    // Print sorted adjacency list

    return 0;
}



/*
bool DFS(node_id_t n1){
    auto a= parse_file("edges.txt");
    std::stack<node_id_t> stack;
    node current;
    current.n1=n1;
    current.visited=true;
    stack.push(current.n1);
    std::vector<node_id_t> dir;

    std::vector<node_id_t> all_nodes;
    for (const auto& meta2 : a.first) {
        all_nodes.push_back(meta2.first);
    }

    while(!stack.empty()){
        dir=has_edge(current.n1,a);
        //auto random=rand()%dir.size();

        current.n1=dir.back();
        stack.push(current.n1);
        current.visited= true;

        if(!dir.empty()){
            stack.pop();
            stack.push(current.n1);
            //auto random2=rand()%dir.size();
            node temp;
            temp.n1=dir.back();
            dir.pop_back();
            temp.visited= true;
            stack.push(temp.n1);

        } else{
            stack.pop();
            std::cout<<"gg";
        }

        //auto it = all_nodes.find(current.n1);
        auto iter = std::find(all_nodes.begin(), all_nodes.end(), current.n1);
        if (iter != all_nodes.end()) {
            all_nodes.erase(iter);
        }
    }
    if (all_nodes.empty()) {
        std::cout << "All nodes have been visited" << std::endl;
        return true;
    } else {
        std::cout << "Not all nodes have been visited" << std::endl;
    }
    return false;
}*/

