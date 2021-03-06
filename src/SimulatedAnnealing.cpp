#include "SimulatedAnnealing.hpp"

#define START_TEMP 1000.0
#define ITER_MAX 50
#define ALPHA_CONST 0.05
#define MIN_CHANGE 1e-3

double SimulatedAnnealing::calculate(const Graph& graph){
    const auto& warehouses = graph.get_warehouses();
    const auto& shops = graph.get_shops();
    const auto& cost_matrix = graph.get_costs();

    srand(time(NULL));

    std::vector<ivector> M(warehouses.size(), ivector(shops.size()));
    std::set<vertex_ivalue_pair> paths;

    TransportAlgorithm::find_first_solution(M, warehouses, shops, cost_matrix);
    SimulatedAnnealing::build_sets(M, paths);

    double T = START_TEMP;
    double eval_prev = 0.0;
    double eval = TransportAlgorithm::evaluate_solution(M, cost_matrix);
    int count_min_change = 0;

    if(shops.size() > 1 && warehouses.size() > 1){
        uint iter_per_temp = warehouses.size() * shops.size();

        for(uint iter = 0; iter < ITER_MAX; ++iter){
            for(uint j = 0; j < iter_per_temp; ++j){
                const auto& f_path = SimulatedAnnealing::draw_pair(paths);
                const auto& s_path = SimulatedAnnealing::draw_pair(paths);

                if(f_path.first == s_path.first || f_path.second == s_path.second)
                    continue;

                int cargo = std::min(M.at(f_path.first).at(f_path.second), M.at(s_path.first).at(s_path.second));
                double old_cost = cost_matrix.at(f_path.first).at(f_path.second) + cost_matrix.at(s_path.first).at(s_path.second);
                double new_cost = cost_matrix.at(f_path.first).at(s_path.second) + cost_matrix.at(s_path.first).at(f_path.second);

                if(new_cost < old_cost || SimulatedAnnealing::accept_worse_solution(cargo * (new_cost - old_cost), T)){
                    M.at(f_path.first).at(f_path.second) -= cargo;
                    M.at(s_path.first).at(s_path.second) -= cargo;

                    if(M.at(f_path.first).at(f_path.second) == 0)
                        paths.erase({f_path.first, f_path.second});
                    if(M.at(s_path.first).at(s_path.second) == 0)
                        paths.erase({s_path.first, s_path.second});

                    M.at(f_path.first).at(s_path.second) += cargo;
                    M.at(s_path.first).at(f_path.second) += cargo;
                    paths.insert({f_path.first, s_path.second});
                    paths.insert({s_path.first, f_path.second});
                }
            }
            
            if(std::abs(eval_prev - eval) < MIN_CHANGE)
                ++count_min_change;
            else
                count_min_change = 0;
        
            if(count_min_change == 2)
                break;
            else{
                eval_prev = eval;
                eval = TransportAlgorithm::evaluate_solution(M, cost_matrix);
                T = T / (1 + ALPHA_CONST);
            }
        }
    }

#ifdef DEBUG
        std::cout << "--- APPROX SOLUTION ---\n";
        transport_helper::print_matrix(M);
        std::cout << "^^^ APPROX SOLUTION ^^^\n";
#endif

    graph.write_solution_to_file(M);
    return TransportAlgorithm::evaluate_solution(M, cost_matrix);
}

void SimulatedAnnealing::build_sets(const std::vector<ivector>& M, std::set<vertex_ivalue_pair>& paths){
    for(uint i = 0; i < M.size(); ++i)
        for(uint j = 0; j < M.at(0).size(); ++j)
            if(M.at(i).at(j) > 0)
                paths.insert({i, j});
}

vertex_ivalue_pair SimulatedAnnealing::draw_pair(std::set<vertex_ivalue_pair>& paths){
    int indx = rand() % paths.size();
    auto it = paths.begin();
    std::advance(it, indx);
    return *it;
}

bool SimulatedAnnealing::accept_worse_solution(const double x, const double T){
    return ((double)rand() / (RAND_MAX)) < (exp(-x/T));
}
