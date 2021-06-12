#include <iostream>

#include <polynome.hpp>
#include <polynome_reader.hpp>


void OnPolynomeRead(Polynome<double> poly) {
    std::cout << poly << std::endl;
    
}

int main(int argc, char **argv) {
    
    PolynomeReader<double, 3> reader;
    reader.Read(--argc, ++argv, OnPolynomeRead);
    
    return 0;
}
