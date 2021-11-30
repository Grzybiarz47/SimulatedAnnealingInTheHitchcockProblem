#pragma once

#include "debug.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <exception>

#ifdef DEBUG
#define PATHOUT "bin/output.txt"
#define PATHIN "example_data/example_input.txt"
#else
#define PATHOUT "output.txt"
#define PATHIN  "input.txt"
#endif

typedef std::pair<int, double> vertex_dvalue_pair;
typedef std::pair<int, int> vertex_ivalue_pair;
typedef std::vector<double> dvector;
typedef std::vector<int> ivector;
typedef std::vector<bool> bvector;
typedef unsigned int uint;

class Graph{
public:
    Graph(std::ifstream& fin, std::ofstream& fout, const std::string& path) : fin{fin}, fout{fout}, filename{path}{
        std::remove(PATHOUT);
        additional_vertex = -1;

        if(filename.empty())
            filename = std::string(PATHIN);
    }

    void read_from_file();
    void write_paths_to_file(const dvector&, const ivector&, const int) const;
    void write_solution_to_file(const std::vector<ivector>&) const;

    void add_crossroad(const int indx, const int vertex, const double value) { _crossroads.at(indx).emplace_back(vertex, value); }
    void add_shop(const int vertex, const int value) { _shops.emplace_back(vertex, value); }
    void add_warehouse(const int vertex, const int value) { _warehouses.emplace_back(vertex, value); }
    void fill_costs(const dvector&);

    const std::vector<vertex_ivalue_pair>& get_shops() const { return _shops; }
    const std::vector<vertex_ivalue_pair>& get_warehouses() const { return _warehouses; } 
    const std::vector<dvector>& get_costs() const { return _costs; }
    const std::vector<vertex_dvalue_pair>& get_vertex(const int vertex) const { return _crossroads.at(vertex); }
    const int size() const { return _crossroads.size(); }

private:
    int check_data_content(std::string) const;
#ifdef DEBUG
    void print_control_data() const;
    void print_control_costs() const;
#endif

    std::vector<std::vector<vertex_dvalue_pair>> _crossroads;
    std::vector<vertex_ivalue_pair> _shops, _warehouses;
    std::vector<dvector> _costs;
    int additional_vertex; 

    std::ifstream& fin;
    std::ofstream& fout;
    std::string filename;

    enum Datatype{
        VERTEX_START,
        VERTEX_NUM,
        EMPTY_LINE,
        WRONG_DATA
    };
};
