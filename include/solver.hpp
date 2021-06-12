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
        SpecificEquationRoot rt = std::get<SpecificEquationRoot>(roots);
        switch (rt) {
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
    }
    return out;
}


template<typename ValueType>
class EquationSolver
{
public:    
    EquationRoots<ValueType> solve(const Polynome<ValueType> &polynome) const {
        const ValueType &a = polynome.coeffs[2];
        const ValueType &b = polynome.coeffs[1];
        const ValueType &c = polynome.coeffs[0];
        
        if (std::abs(a) < 1e-6) {
            //todo solve
            return SpecificEquationRoot::kCannotSolve;
        }
        
        ValueType d = b*b - 4 * a * c;
        if (d < 0) {
            return SpecificEquationRoot::kNoRoots;
        }
        
        ValueType sqrt_d = std::sqrt(d);
        //todo optimize
        ValueType x1 = (-b + sqrt_d) / (2 * a);
        ValueType x2 = (-b - sqrt_d) / (2 * a);
        
        return std::vector<ValueType>({x1, x2});
    }
    
    
};
