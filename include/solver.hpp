#pragma once

#include <polynome.hpp>

#include <variant>
#include <cmath>
#include <ostream>


enum class SpecificEquationRoot {
    kNoRoots,
    kAnyNumberIsRoot,
    kCannotSolve
};


template<typename ValueType>
using EquationRoots = std::variant<std::vector<ValueType>, SpecificEquationRoot>;


std::ostream &operator<<(std::ostream &out, const SpecificEquationRoot &root) {
    switch (root) {
        case SpecificEquationRoot::kNoRoots:
            out << "no roots";
            break;
        case SpecificEquationRoot::kAnyNumberIsRoot:
            out << "any number";
            break;
        case SpecificEquationRoot::kCannotSolve:
            out << "cannot solve";
            break;
        default:
            throw std::runtime_error("Unexpected SpecificEquationRoot found");
    }
    return out;
}

template<typename ValueType>
std::ostream &operator<<(std::ostream &out, const EquationRoots<ValueType> &roots) {
    std::size_t index = roots.index();
    if (index == 0) {
        const std::vector<ValueType> &vals = std::get<std::vector<ValueType> >(roots);
        out << "(";
        for (int i = int(vals.size()) - 1; i > 0; i--) {
            out << vals[i] << " ";
        }
        if (!vals.empty()) {
            out << vals[0];
        }
        out << ")";
    } else {
        out << std::get<SpecificEquationRoot>(roots);
    }
    return out;
}


template<typename ValueType>
class EquationSolver
{
public:    
    EquationRoots<ValueType> solve(const Polynome<ValueType> &polynome) const {
        const ValueType &a = polynome.a();
        const ValueType &b = polynome.b();
        const ValueType &c = polynome.c();
        
        if (polynome.degree() < 2) {
            //todo solve
            return SpecificEquationRoot::kCannotSolve;
        }
        
        ValueType d = b * b - 4 * a * c;
        if (d < 0) {
            return SpecificEquationRoot::kNoRoots;
        }
        
        ValueType sqrt_d = std::sqrt(d);
        //todo optimize
        ValueType a2 = 2 * a;
        ValueType x1 = (-b + sqrt_d) / a2;
        ValueType x2 = (-b - sqrt_d) / a2;
        
        return std::vector<ValueType>({x1, x2});
    }
    
};
