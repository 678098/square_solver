#pragma once

#include <vector>


template<typename ValueType>
class Polynome
{
public:
    Polynome(const std::vector<ValueType> &coeffs): coeffs(coeffs) {
    }
    virtual ~Polynome() = default;
    
    
    std::vector<ValueType> coeffs;
    
    friend std::ostream &operator<<(std::ostream &out, const Polynome &poly) {
        out << "(";
        for (int i = int(poly.coeffs.size()) - 1; i >= 1; i--) {
            out << poly.coeffs[i] << " ";
        }
        if (!poly.coeffs.empty()) {
            out << poly.coeffs[0];
        }
        out << ")";
        return out;
    }
};
