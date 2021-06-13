#pragma once

#include <sstream>


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


template<typename ValueType>
class BasicValueReader
{
public:
    virtual ~BasicValueReader() = default;
    
    operator bool() const {
        return !finished;
    }
    
    virtual BasicValueReader &operator>>(ValueType &val) = 0;

protected:
    bool finished = false;
};


template<typename ValueType>
class CmdValueReader: public BasicValueReader<ValueType>
{
public:
    CmdValueReader(int argc, char **argv):
        argc(argc),
        argv(argv)
    {
    }
    
    CmdValueReader &operator>>(ValueType &val) override {
        while (pos < argc) {
            if (!IsInteger(argv[pos])) {
                ++pos;
                continue;
            }
            val = ReadInteger<ValueType>(argv[pos]);
            ++pos;
            return *this;
        }
        this->finished = true;
        return *this;
    }
    
protected:
    int pos = 0;
    
    int argc;
    char **argv;
};

