#include "debug.hpp"
#include "Graph.hpp"
#include "CostCalculation.hpp"
#include "TransportAlgorithm.hpp"
#include "SimulatedAnnealing.hpp"
#include <iomanip>

int main(int argc, char *argv[])
{
    try{

        std::ifstream fin;
        std::ofstream fout;
        Graph G(fin, fout, (argc == 2 ? std::string(argv[1]) : std::string("")));

        G.read_from_file();
        CostCalculation::calculate(G);
        const double best_result = TransportAlgorithm::calculate(G);
        std::cout << "OPTIMAL: " << std::setprecision(20) << best_result << std::endl;
        const double approx_result = SimulatedAnnealing::calculate(G);
        std::cout << "APPROX : " << std::setprecision(20) << approx_result << std::endl;

    }catch(const std::exception& e){
        std::cout << e.what() << std::endl;
    }

    return 0;
}