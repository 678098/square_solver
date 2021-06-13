#pragma once

#include <vector>
#include <cassert>

template<typename ValueType>
bool IsAlmostZero(const ValueType &value) {
    constexpr double kEPS = 1e-6;
    return -kEPS < value && value < kEPS;
}


template<typename ValueType>
class Polynome
{
public:
    Polynome(const std::vector<ValueType> &coeffs):
        coeffs(coeffs),
        degree(FindDegree(coeffs))
    {
    }
    Polynome():
        degree(0)
    {
    }
    Polynome(const Polynome &other):
        coeffs(other.coeffs),
        degree(other.degree)
    {
    }
    virtual ~Polynome() = default;
    
    int Degree() const {
        return degree;
    }
    
    const std::vector<ValueType> &Coeffs() const {
        return coeffs;
    }
    
    size_t Size() const {
        return coeffs.size();
    }
    
    bool Empty() const {
        return coeffs.empty();
    }
    
    const ValueType &operator[] (size_t index) const {
        assert(index < coeffs.size());
        return coeffs[index];
    }
    
    const ValueType &A() const {
        assert(coeffs.size() >= 3);
        return coeffs[2];
    }
    
    const ValueType &B() const {
        assert(coeffs.size() >= 2);
        return coeffs[1];
    }
    
    const ValueType &C() const {
        assert(coeffs.size() >= 1);
        return coeffs[0];
    }
    
private:

    std::vector<ValueType> coeffs;
    int degree;
    
    static int FindDegree(const std::vector<ValueType> &coeffs) {
        for (size_t i = 0; i < coeffs.size(); i++) {
            if (!IsAlmostZero(coeffs[i])) {
                return coeffs.size() - i - 1;
            }
        }
        return 0;
    }
};


template<typename ValueType>
std::ostream &operator<<(std::ostream &out, const Polynome<ValueType> &poly) {
    out << "(";
    for (int i = int(poly.Size()) - 1; i >= 1; i--) {
        out << poly[i] << " ";
    }
    if (!poly.Empty()) {
        out << poly[0];
    }
    out << ")";
    return out;
}
