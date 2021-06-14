#pragma once

#include <polynome.hpp>
#include <value_reader.hpp>


//PolynomeReader provides stream-like interface for reading Polynome(s) with specified Degree and ValueType
//from cmd or stream sources
template<typename ValueType, unsigned int Degree>
class PolynomeReader
{
public:
    PolynomeReader(int argc, char **argv) :
        reader(std::make_unique<CmdValueReader<ValueType> >(argc, argv))
    {
    }
    PolynomeReader(std::istream &inp) :
        reader(std::make_unique<StreamReader<ValueType> >(inp))
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
                val = Polynome<ValueType>(coeffs);
                return *this;
            }
        }
        
        //we can have not completed Polynome with some coeffs missing
        //we want to return it anyway
        if (degree < kPolynomeDegree - 1) {
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
