#include "TransportAlgorithm.hpp"

#define ROW true
#define COLUMN false

typedef std::queue<vertex_ivalue_pair> queue;

//public:
const double TransportAlgorithm::calculate(const Graph& graph){
    const auto& warehouses = graph.get_warehouses();
    const auto& shops = graph.get_shops();

    std::vector<ivector> M(warehouses.size(), ivector(shops.size()));
    std::set<vertex_ivalue_pair> B;
        
    TransportAlgorithm::find_first_solution(graph, M);
    TransportAlgorithm::find_base_set(M, B);

    constexpr int ITER_MAX = 100;
    for(uint iter = 0; iter < ITER_MAX; ++iter){
        std::vector<dvector> cB(warehouses.size(), dvector(shops.size(), 0.0));
        std::set<vertex_ivalue_pair> gamma1, gamma2;

#ifdef DEBUG
        transport_helper::print_matrix(M);
        transport_helper::print_set(B);
#endif

        TransportAlgorithm::find_zero_matrix(cB, B, graph.get_costs());
        vertex_ivalue_pair min_p = TransportAlgorithm::find_min(cB);
        
        if(TransportAlgorithm::is_optimal(cB, min_p))
            break;
        else
            B.insert(min_p);

        TransportAlgorithm::find_cycle(graph, B, min_p, gamma1, gamma2);
        TransportAlgorithm::update_base_set_and_matrix(M, B, gamma1, gamma2);
    }

#ifdef DEBUG
        std::cout << "--- OPTIMAL SOLUTION ---\n";
        transport_helper::print_matrix(M);
        std::cout << "^^^ OPTIMAL SOLUTION ^^^\n";
#endif

    graph.write_solution_to_file(M);
    return TransportAlgorithm::evaluate_solution(M, graph.get_costs());
}

void TransportAlgorithm::find_first_solution(const Graph& graph, std::vector<ivector>& M){ //using minimal matrix element algorithm
    auto warehouses = graph.get_warehouses(); //intentional copy
    auto shops = graph.get_shops(); //intentional copy
    const auto& cost_matrix = graph.get_costs();

    std::vector < std::pair<double, vertex_ivalue_pair> > temp;
    for(uint i = 0; i < warehouses.size(); ++i)
        for(uint j = 0; j < shops.size(); ++j)
            temp.emplace_back(cost_matrix.at(i).at(j), std::make_pair(i, j));

    std::sort(temp.begin(), temp.end());

    for(const auto& p : temp){
        int warehouse_indx = p.second.first;
        int shop_indx = p.second.second;
        int supply = warehouses.at(warehouse_indx).second;
        int demand = shops.at(shop_indx).second;

        if(supply >= demand){
            M.at(warehouse_indx).at(shop_indx) = demand;
            warehouses.at(warehouse_indx).second -= demand;
            shops.at(shop_indx).second = 0;
        }
        else{
            M.at(warehouse_indx).at(shop_indx) = supply;
            warehouses.at(warehouse_indx).second = 0;
            shops.at(shop_indx).second -= supply;
        }
    }
}

const double TransportAlgorithm::evaluate_solution(const std::vector<ivector>& M, const std::vector<dvector>& cost_matrix){
    double res = 0;

    for(uint i = 0; i < M.size(); ++i)
        for(uint j = 0; j < M.at(i).size(); ++j)
            res += M.at(i).at(j) * cost_matrix.at(i).at(j);
    
    return res;
}

void TransportAlgorithm::find_base_set(const std::vector<ivector>& M, std::set<vertex_ivalue_pair>& B){
    for(uint i = 0; i < M.size(); ++i)
        for(uint j = 0; j < M.at(i).size(); ++j)
            if(M.at(i).at(j) > 0) 
                B.insert({i, j});
}

bool TransportAlgorithm::find_zero_matrix(std::vector<dvector>& cB, const std::set<vertex_ivalue_pair>& B, const std::vector<dvector>& cost_matrix){
    dvector u(cB.size());
    dvector v(cB.at(0).size());
    bvector u_solved(u.size(), false);
    bvector v_solved(v.size(), false);

    queue Q(std::deque(B.begin(), B.end()));

    vertex_ivalue_pair no_pattern(-1, -1);
    vertex_ivalue_pair rep = no_pattern;
    u.at(0) = 0;
    u_solved.at(0) = true;
    while(!Q.empty()){
        const auto p = Q.front();
        int i = p.first;
        int j = p.second;
        Q.pop();

        if(u_solved.at(i)){
            v.at(j) = -(cost_matrix.at(i).at(j) + u.at(i));
            v_solved.at(j) = true;
            rep = no_pattern;
        }
        else if(v_solved.at(j)){
            u.at(i) = -(cost_matrix.at(i).at(j) + v.at(j));
            u_solved.at(i) = true;
            rep = no_pattern;
        }
        else{
            Q.push(p);
            if(p == rep)   
                return false; //system of equations cannot be solved
            else if(rep == no_pattern)
                rep = p;
        }
    } //can be more efficent

    for(uint i = 0; i < u.size(); ++i)
        for(uint j = 0; j < v.size(); ++j)
            cB.at(i).at(j) = cost_matrix.at(i).at(j) + u.at(i) + v.at(j);

#ifdef DEBUG
    transport_helper::print_vector(u);
    transport_helper::print_vector(v);
    transport_helper::print_matrix(cB);
#endif

    return true;
}

bool TransportAlgorithm::is_optimal(const std::vector<dvector>& cB, const vertex_ivalue_pair& p){
    return !(cB.at(p.first).at(p.second) < 0);
}

vertex_ivalue_pair TransportAlgorithm::find_min(const std::vector<dvector>& cB){
    vertex_ivalue_pair p(0, 0);
    for(uint i = 0; i < cB.size(); ++i)
        for(uint j = 0; j < cB.at(0).size(); ++j)
            if(cB.at(p.first).at(p.second) > cB.at(i).at(j))
                p = std::make_pair(i, j);

    return p;
}

void TransportAlgorithm::find_cycle(const Graph& graph, const std::set<vertex_ivalue_pair>& B, const vertex_ivalue_pair& p,
                                    std::set<vertex_ivalue_pair>& gamma1, std::set<vertex_ivalue_pair>& gamma2){
    std::vector<ivector> rows(graph.get_warehouses().size());
    std::vector<ivector> columns(graph.get_shops().size());
    bvector visited_rows(graph.get_warehouses().size(), false);
    bvector visited_columns(graph.get_shops().size(), false);
    ivector vertices;
    
    for(const auto& edge : B)
        if(edge != p){ //cut off egde p to prevent creating too short cycle
            rows.at(edge.first).push_back(edge.second);
            columns.at(edge.second).push_back(edge.first);
        }

    vertices.push_back(p.first);
    find_cycle(vertices, rows, columns, visited_rows, visited_columns, COLUMN, p.second, p.first);

    for(uint i = 1; i < vertices.size() - 1; i += 2) //first half-cycle
        gamma1.insert({vertices.at(i + 1), vertices.at(i)});
    gamma1.insert(p);

    for(uint i = 0; i < vertices.size(); i += 2) //second half-cycle
        gamma2.insert({vertices.at(i), vertices.at(i + 1)});

#ifdef DEBUG
    transport_helper::print_vector(vertices);
    transport_helper::print_set(gamma1);
    transport_helper::print_set(gamma2);
#endif
}

void TransportAlgorithm::update_base_set_and_matrix(std::vector<ivector>& M, std::set<vertex_ivalue_pair>& B, 
                                                    const std::set<vertex_ivalue_pair>& gamma1, const std::set<vertex_ivalue_pair>& gamma2){
    vertex_ivalue_pair q(*gamma2.begin());
    for(auto p : gamma2){
        int i = p.first, iq = q.first;
        int j = p.second, jq = q.second;
        if(M.at(i).at(j) < M.at(iq).at(jq))
            q = p; 
    }

    const int value = M.at(q.first).at(q.second);
    for(uint i = 0; i < M.size(); ++i)
        for(uint j = 0; j < M.at(0).size(); ++j){
            vertex_ivalue_pair p(i, j);
            
            if(std::find(gamma1.begin(), gamma1.end(), p) != gamma1.end())
                M.at(i).at(j) += value;
            if(std::find(gamma2.begin(), gamma2.end(), p) != gamma2.end())
                M.at(i).at(j) -= value;
        }

    B.erase(q);
}

//private:
bool TransportAlgorithm::find_cycle(ivector& vertices, std::vector<ivector>& rows, std::vector<ivector>& columns, 
                                    bvector& v_row, bvector& v_col, bool is_row, const int indx, const int end){
    auto& vec = is_row == ROW ? rows : columns;
    auto& visited = is_row == ROW ? v_row : v_col;

    if(indx == end && is_row == ROW)
        return true; //cycle found

    if(visited.at(indx))
        return false; //already visited - no cycle
    else
        visited.at(indx) = true; //set to visited

    for(const int& x : vec.at(indx))
        if(find_cycle(vertices, rows, columns, v_row, v_col, !is_row, x, end)){
            vertices.push_back(indx);
            return true;
        }

    return false;
}