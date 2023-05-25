#include "reader.h"

int main() {
    auto a= parse_file("edges.txt");
    //std::cout << "Size of graph: " << a.first.size() << " nodes, " << a.second.size() << " edges" << std::endl;
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
        if(a.first.size()==c.size()){
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


/*
 auto hej2=DIKSTRAS(5,19);
    for (const auto& edge : hej2) {
        if(edge.weight!=999999) {
            std::cout << "Edge: " << edge.n1 << " " << " weight: " << edge.weight << "\n";
        }
    }
*/
    return 0;
}
