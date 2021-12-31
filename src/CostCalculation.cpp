#include "CostCalculation.hpp"
#include "transport_helpers.hpp"

typedef std::priority_queue<CostCalculation::DijkstraPair, std::vector<CostCalculation::DijkstraPair>, CostCalculation::Comparator> pqueue;

namespace dijkstra_helper{
    inline void push(pqueue& Q, const std::vector<vertex_dvalue_pair>& connections, dvector& d, ivector& path, const CostCalculation::DijkstraPair& parent){
        for(const auto& p : connections){
            int vertex = p.first;
            double cost = p.second;
            if(cost + parent.cost < d.at(vertex)){
                d.at(vertex) = cost + parent.cost;
                path.at(vertex) = parent.vertex;
                Q.push(CostCalculation::DijkstraPair{vertex,  d.at(vertex)});
            }
        }
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

    dijkstra_helper::push(Q, graph.get_vertex(start), d, path, CostCalculation::DijkstraPair{start, 0});
    while(!Q.empty()){
        const auto elem = Q.top();
        Q.pop();
        
        if(elem.cost == d.at(elem.vertex)){
            dijkstra_helper::push(Q, graph.get_vertex(elem.vertex), d, path, elem);
        }
    }
    graph.write_paths_to_file(d, path, start);
    graph.fill_costs(d);
}

