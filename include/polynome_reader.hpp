#pragma once

#include <functional>
#include <sstream>
//todo remove iostream
#include <iostream>

#include <polynome.hpp>


template<typename ValueType, unsigned int Degree>
class PolynomeReader
{
public:
    typedef std::function<void(Polynome<ValueType>)> OnPolynomeReadCallback;
    
    PolynomeReader() = default;
    virtual ~PolynomeReader() = default;
    
    void Read(int lexemesNum, char **lexemes, OnPolynomeReadCallback callback) const {
        int currentLexemeId = 0;
        std::vector<ValueType> currentCoeffs;
        
        while (currentLexemeId < lexemesNum) {
            ValueType coeff;
            //todo read without stringstream
            //todo get rid of garbage
            std::stringstream str(lexemes[currentLexemeId]);
            str >> coeff;
            
            currentCoeffs.push_back(coeff);
            
            if (currentCoeffs.size() == kPolynomeDegree) {
                //todo reverse coeffs order
                //todo move coeffs optimal
                callback(std::move(Polynome(currentCoeffs)));
                currentCoeffs.clear();
            }
            
            currentLexemeId++;
        }
        
        if (!currentCoeffs.empty()) {
            while (currentCoeffs.size() < kPolynomeDegree) {
                currentCoeffs.push_back(ValueType(0));
            }
            callback(std::move(Polynome(currentCoeffs)));
            currentCoeffs.clear();
        }
    }
    
private:
    const unsigned int kPolynomeDegree = Degree;
    
};