#pragma once
#include "Graph.hpp"
#include "TransportAlgorithm.hpp"
#include "transport_helpers.hpp"
#include <iostream>
#include <set>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

class SimulatedAnnealing{
public:
    static double calculate(const Graph&);
    static void build_sets(const std::vector<ivector>&, std::set<vertex_ivalue_pair>&);
    static vertex_ivalue_pair draw_pair(std::set<vertex_ivalue_pair>&);
    static bool accept_worse_solution(const double, const double);

};