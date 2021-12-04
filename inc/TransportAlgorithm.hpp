#pragma once
#include "Graph.hpp"
#include "transport_helpers.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <queue>
#include <deque>

class TransportAlgorithm{
public:
    static const double calculate(const Graph&);
    static const double evaluate_solution(const std::vector<ivector>&, const std::vector<dvector>&);
    static void find_zero_matrix(std::vector<dvector>&, const std::set<vertex_ivalue_pair>&, const std::vector<dvector>&);
    static vertex_ivalue_pair find_min(const std::vector<dvector>&);
    static bool is_optimal(const std::vector<dvector>&, const vertex_ivalue_pair&);
    static void find_base_set(const std::vector<ivector>&, std::set<vertex_ivalue_pair>&);

    static void find_first_solution(std::vector<ivector>&, std::vector<vertex_ivalue_pair>, std::vector<vertex_ivalue_pair>,
                                    const std::vector<dvector>&);

    static void make_base_set_valid(const std::vector<ivector>&, const std::vector<vertex_ivalue_pair>&, const std::vector<vertex_ivalue_pair>&,
                                    const std::vector<dvector>&, std::set<vertex_ivalue_pair>&);

    static bool find_cycle(const Graph&, const std::set<vertex_ivalue_pair>&, const vertex_ivalue_pair& p,
                           std::set<vertex_ivalue_pair>&, std::set<vertex_ivalue_pair>&);

    static void update_base_set_and_matrix(std::vector<ivector>&, std::set<vertex_ivalue_pair>&, 
                                           const std::set<vertex_ivalue_pair>&, const std::set<vertex_ivalue_pair>&);
                           
private:
    static bool find_cycle(ivector&, std::vector<ivector>&, std::vector<ivector>&, bvector&, bvector&, bool, const int, const int);
};