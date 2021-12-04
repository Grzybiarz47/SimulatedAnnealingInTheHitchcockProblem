#include "CostCalculation.hpp"
#include "transport_helpers.hpp"

typedef std::priority_queue<CostCalculation::DijkstraPair, std::vector<CostCalculation::DijkstraPair>, CostCalculation::Comparator> pqueue;

namespace dijkstra_helper{
    inline void push(pqueue& Q, const std::vector<vertex_dvalue_pair>& connections, dvector& d, const int source, const double cost){
        for(const auto& p : connections)
            if(p.second + cost < d.at(p.first)) //not neccessary
                Q.push(CostCalculation::DijkstraPair{p.first,  p.second + cost, source});
    }
}

void CostCalculation::calculate(Graph& graph){
    auto& warehouses = graph.get_warehouses();

    for(uint i = 0; i < warehouses.size(); ++i)
        CostCalculation::dijkstra(graph, warehouses.at(i).first);
    
#ifdef DEBUG
        transport_helper::print_matrix(graph.get_costs());
#endif
}

void CostCalculation::dijkstra(Graph& graph, const int start){
    dvector d(graph.size(), INF);
    ivector path(graph.size(), -1);
    d.at(start) = 0; path.at(start) = start;
    pqueue Q;

    dijkstra_helper::push(Q, graph.get_vertex(start), d, start, 0);
    while(!Q.empty()){
        const int vertex = Q.top().vertex;
        const double cost = Q.top().cost;
        const int pred = Q.top().pred;
        Q.pop();
        
        if(cost < d.at(vertex)) {
            d.at(vertex) = cost; //new shortest path
            path.at(vertex) = pred; //predecessor of vertex
            dijkstra_helper::push(Q, graph.get_vertex(vertex), d, vertex, cost);
        }
    }
    graph.write_paths_to_file(d, path, start);
    graph.fill_costs(d);
}

