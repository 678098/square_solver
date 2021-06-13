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
        kCoeffs(coeffs),
        kDegree(FindDegree(coeffs))
    {
    }
    virtual ~Polynome() = default;
    
    int Degree() const {
        return kDegree;
    }
    
    const std::vector<ValueType> &Coeffs() const {
        return kCoeffs;
    }
    
    size_t Size() const {
        return kCoeffs.size();
    }
    
    bool Empty() const {
        return kCoeffs.empty();
    }
    
    const ValueType &operator[] (size_t index) const {
        assert(index < kCoeffs.size());
        return kCoeffs[index];
    }
    
    const ValueType &A() const {
        assert(kCoeffs.size() >= 3);
        return kCoeffs[2];
    }
    
    const ValueType &B() const {
        assert(kCoeffs.size() >= 2);
        return kCoeffs[1];
    }
    
    const ValueType &C() const {
        assert(kCoeffs.size() >= 1);
        return kCoeffs[0];
    }
    
private:

    const std::vector<ValueType> kCoeffs;
    const int kDegree;
    
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
