#pragma once

#include <polynome.hpp>
#include <solution.hpp>

#include <cmath>



template<typename ValueType>
class EquationSolver
{
public:    
    EquationSolution<ValueType> solve(const Polynome<ValueType> &polynome) const {
        const ValueType &a = polynome.a();
        const ValueType &b = polynome.b();
        const ValueType &c = polynome.c();
        
        if (polynome.degree() < 2) {
            //todo solve
            return SpecificSolution::kCannotSolve;
        }
        
        ValueType d = b * b - 4 * a * c;
        if (d < 0) {
            return SpecificSolution::kNoRoots;
        }
        
        ValueType sqrt_d = std::sqrt(d);
        //todo optimize
        ValueType a2 = 2 * a;
        ValueType x1 = (-b + sqrt_d) / a2;
        ValueType x2 = (-b - sqrt_d) / a2;
        
        return std::vector<ValueType>({x1, x2});
    }
    
};
