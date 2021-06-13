#pragma once

#include <functional>
#include <sstream>

#include <polynome.hpp>


inline bool IsInteger(const char *str) {
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
ValueType ReadInteger(const char *str) {
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
        int currentDegree = kPolynomeDegree - 1;
        std::vector<ValueType> currentCoeffs = std::vector<ValueType>(kPolynomeDegree, ValueType(0));
        
        while (currentLexemeId < lexemesNum) {
            if (!IsInteger(lexemes[currentLexemeId])) {
                currentLexemeId++;
                continue;
            }
            
            currentCoeffs[currentDegree--] = ReadInteger<ValueType>(lexemes[currentLexemeId]);
            
            if (currentDegree < 0) {
                //todo move coeffs optimal
                callback(std::move(Polynome(currentCoeffs)));
                currentCoeffs = std::vector<ValueType>(Degree, ValueType(0));
                currentDegree = kPolynomeDegree - 1;
            }
            
            currentLexemeId++;
        }
        
        if (currentDegree < kPolynomeDegree - 1) {
            callback(std::move(Polynome(currentCoeffs)));
            currentCoeffs = std::vector<ValueType>(Degree, ValueType(0));
            currentDegree = kPolynomeDegree - 1;
        }
    }
    
private:
    const unsigned int kPolynomeDegree = Degree;
    
};
