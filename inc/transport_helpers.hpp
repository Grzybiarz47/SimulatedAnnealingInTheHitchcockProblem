#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>

namespace transport_helper{
#ifdef DEBUG
    template <typename T>
    inline void print_matrix(const std::vector<std::vector<T>>& matrix){
        std::cout << "---MATRIX---\n";
        for(const auto& vec : matrix){
            for(const auto& x : vec)
                std::cout << std::left << std::setw(5) << x << " ";
            std::cout << std::endl;
        }
    }
    template <typename T, typename S>
    inline void print_vector(const std::vector<std::pair<T, S>>& vec){
        std::cout << "---PVEC---\n";
        for(const auto& p : vec)
            std::cout << "(" << p.first << "," << p.second << ")";
        std::cout << "\n";
    }
    template <typename T>
    inline void print_vector(const std::vector<T>& vec){
        std::cout << "---VEC---\n";
        for(const auto& x : vec)
            std::cout << x << " ";
        std::cout << "\n";
    }
    template <typename T, typename S>
    inline void print_set(const std::set<std::pair<T, S>>& vec){
        std::cout << "---PSET---\n";
        for(const auto& p : vec)
            std::cout << "(" << p.first << "," << p.second << ")";
        std::cout << "\n";
    }
    template <typename T>
    inline void print_set(const std::set<T>& vec){
        std::cout << "---SET---\n";
        for(const auto& x : vec)
            std::cout << x << " ";
        std::cout << "\n";
    }
#endif
}