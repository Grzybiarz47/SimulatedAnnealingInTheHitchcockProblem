#pragma once
#include "Graph.hpp"
#include <vector>
#include <queue>
#include <utility>

class CostCalculation{
public:
    static void calculate(Graph&);
    static void dijkstra(Graph&, const int);
    
    struct DijkstraPair{
        int vertex;
        double cost;
    };

    class Comparator{
    public:
        bool operator() (DijkstraPair a, DijkstraPair b){
            return a.cost > b.cost;
        }
    };
};