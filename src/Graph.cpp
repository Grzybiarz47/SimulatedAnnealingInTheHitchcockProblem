#include "Graph.hpp"

#define GRAPH 0
#define SHOPS 1
#define WAREHOUSES 2
#define INF 1E+9

//public:
void Graph::read_from_file(){
    fin.open(filename);
    if(!fin)
        throw std::invalid_argument("File cannot be opened");

    int flag = GRAPH, indx;
    int vertex, ivalue;
    double dvalue;

    fin >> indx;
    _crossroads.resize(indx);

    while(!fin.eof()){
        std::string str;
        fin >> str;

        switch (check_data_content(str)){
            case VERTEX_START:
                str.erase(str.end() - 1);
                indx = std::stoi(str) - 1;
                break;

            case VERTEX_NUM:
                vertex = std::stoi(str) - 1;

                if(flag == GRAPH){
                    fin >> dvalue;
                    add_crossroad(indx, vertex, dvalue);
                }
                else if(flag == SHOPS){
                    fin >> ivalue;
                    add_shop(vertex, ivalue);
                }
                else{
                    fin >> ivalue;
                    add_warehouse(vertex, ivalue);
                }
                break;

            case WRONG_DATA:
                throw std::invalid_argument("Invalid data format at: " + str);

            case EMPTY_LINE:
                ++flag;
        }
    }

    double supply = 0, demand = 0;
    for(const auto& x : _warehouses)
        supply += x.second;
    for(const auto& x : _shops)
        demand += x.second;
    
    if(demand != supply){ //check if problem is open
        int size = _crossroads.size();
        _crossroads.resize(size + 1);
        //add_crossroad(0, size, INF);
        //add_crossroad(size, 0, INF);

        if(supply > demand){
            for(const auto& warehouse : _warehouses){
                add_crossroad(warehouse.first, size, INF);
                add_crossroad(size, warehouse.first, INF);
            }
            add_shop(size, supply - demand);
        }
        else{
            for(const auto& shop : _shops){
                add_crossroad(shop.first, size, INF);
                add_crossroad(size, shop.first, INF);
            }

            add_warehouse(size, demand - supply);
        }
            

        additional_vertex = size;
    }

#ifdef DEBUG
    print_control_data();
#endif

    fin.close();
}

void Graph::write_paths_to_file(const dvector& d, const ivector& path, const int warehouse) const {
    if(additional_vertex == warehouse)
        return;

    fout.open(PATHOUT, std::ios_base::app);

    for(const auto& p : _shops){
        int shop = p.first;

        if(additional_vertex == shop)
            continue;

        fout << "path s-w (" <<  shop + 1 << "," << warehouse + 1 << ")\n";
        do{
            fout << shop + 1 << " ";
            shop = path.at(shop);
        } while(shop != warehouse);
        fout << warehouse + 1 << " ";

        fout << d.at(p.first) << "\n";
    }
    fout.close();
}

void Graph::write_solution_to_file(const std::vector<ivector>& solution) const{
    fout.open(PATHOUT, std::ios_base::app);

    fout << "---\n";
    for(uint j = 0; j < _shops.size(); ++j)
        for(uint i = 0; i < _warehouses.size(); ++i){
            int warehouse = _warehouses.at(i).first;
            int shop = _shops.at(j).first;
            
            if(additional_vertex == warehouse || additional_vertex == shop)
                continue;

            fout << "cargo s-w (" <<  _shops.at(j).first + 1 << "," << _warehouses.at(i).first + 1 << ")\n";
            fout << solution.at(i).at(j) << "\n";
        }
    fout << "---\n";
    fout.close();
}

void Graph::fill_costs(const dvector& d){
    dvector temp;
    for(uint i = 0; i < _shops.size(); ++i)
        temp.push_back(d.at(_shops.at(i).first));

    _costs.emplace_back(temp);
}

//private:
int Graph::check_data_content(std::string str) const{
    if(std::find(str.begin(), str.end(), ':') != str.end())
        return VERTEX_START;
    if(std::find(str.begin(), str.end(), '-') != str.end())
        return EMPTY_LINE;
    
    for(const char& ch : str)
        if(!std::isdigit(ch))
            return WRONG_DATA;
    
    return VERTEX_NUM;
}

#ifdef DEBUG
void Graph::print_control_data() const{
    int indx = 1;
    for(const auto& v : _crossroads){
        std::cout << indx << ": ";
        for(const auto& p : v){
            std::cout << "(" << p.first << " " << p.second << ")";
        }
        std::cout << std::endl;
        ++indx;
    }

    for(const auto& elem : _shops){
        std::cout << "(" << elem.first << " " << elem.second << ")";
    }
    std::cout << std::endl;
    for(const auto& elem : _warehouses){
        std::cout << "(" << elem.first << " " << elem.second << ")";
    }
    std::cout << std::endl;
}
#endif