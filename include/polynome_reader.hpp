#pragma once

#include <polynome.hpp>
#include <value_reader.hpp>


template<typename ValueType, unsigned int Degree>
class PolynomeReader
{
public:
    PolynomeReader(int argc, char **argv) :
        reader(std::make_unique<CmdValueReader<ValueType> >(argc, argv))
    {
    }
    virtual ~PolynomeReader() = default;
    
    operator bool() const {
        return !finished;
    }
    
    PolynomeReader &operator>>(Polynome<ValueType> &val) {
        int degree = kPolynomeDegree - 1;
        
        std::vector<ValueType> coeffs(kPolynomeDegree, ValueType(0));
        while ((*reader) >> coeffs[degree]) {
            if (--degree < 0) {
                //todo move coeffs optimal
                val = Polynome<ValueType>(coeffs);
                return *this;
            }
        }
        
        if (degree < kPolynomeDegree - 1) {
            //todo move coeffs optimal
            val = Polynome<ValueType>(coeffs);
            return *this;
        }
        
        this->finished = true;
        return *this;
    }
    
private:
    const unsigned int kPolynomeDegree = Degree;
    
    bool finished = false;
    
    std::unique_ptr<BasicValueReader<ValueType> > reader;
    
};
