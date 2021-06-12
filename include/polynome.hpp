#pragma once

#include <vector>
#include <cassert>


template<typename ValueType>
class Polynome
{
public:
    Polynome(const std::vector<ValueType> &coeffs):
        kCoeffs(coeffs),
        kDegree(findDegree(coeffs))
    {
    }
    virtual ~Polynome() = default;
    
    int degree() const {
        return kDegree;
    }
    
    const std::vector<ValueType> &coeffs() const {
        return kCoeffs;
    }
    
    size_t size() const {
        return kCoeffs.size();
    }
    
    const ValueType &operator[] (size_t index) const {
        assert(index < kCoeffs.size());
        return kCoeffs[index];
    }
    
    const ValueType &a() const {
        assert(kCoeffs.size() >= 3);
        return kCoeffs[2];
    }
    
    const ValueType &b() const {
        assert(kCoeffs.size() >= 2);
        return kCoeffs[1];
    }
    
    const ValueType &c() const {
        assert(kCoeffs.size() >= 1);
        return kCoeffs[0];
    }
    
private:

    const int kDegree;
    const std::vector<ValueType> kCoeffs;
    
    static bool isAlmostZero(const ValueType &value) {
        constexpr double kEPS = 1e-6;
        return -kEPS < value && value < kEPS;
    }
    
    static int findDegree(const std::vector<ValueType> &coeffs) {
        for (size_t i = 0; i < coeffs.size(); i++) {
            if (!isAlmostZero(coeffs[i])) {
                return coeffs.size() - i;
            }
        }
        return 0;
    }
};


template<typename ValueType>
std::ostream &operator<<(std::ostream &out, const Polynome<ValueType> &poly) {
    out << "(";
    for (int i = int(poly.size()) - 1; i >= 1; i--) {
        out << poly[i] << " ";
    }
    if (!poly.coeffs.empty()) {
        out << poly[0];
    }
    out << ")";
    return out;
}
