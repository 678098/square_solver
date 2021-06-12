#pragma once

#include <functional>
#include <sstream>
//todo remove iostream
#include <iostream>

#include <polynome.hpp>


inline bool isInteger(const char *str) {
    const char *ptr = str;
    while (*ptr != '\0') {
        if (std::isdigit(*ptr) ||
            (ptr == str && *ptr == '-')) 
        {
            ptr++;
            continue;
        }
        return false;
    }
    return true;
}

template<typename ValueType>
ValueType readInteger(const char *str) {
    //todo for integer arithmetic types make conversion without float values
    if (std::is_arithmetic<ValueType>()) {
        return std::stod(str);
    } else {
        ValueType coeff;
        //todo read complex types without stringstream
        std::stringstream stream(str);
        stream >> coeff;        
        return coeff;
    }
}


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
            if (!isInteger(lexemes[currentLexemeId])) {
                currentLexemeId++;
                continue;
            }
            
            ValueType coeff = readInteger<ValueType>(lexemes[currentLexemeId]);
            
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
