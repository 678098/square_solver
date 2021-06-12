#include <iostream>

#include <polynome.hpp>
#include <solver.hpp>
#include <polynome_reader.hpp>


void OnPolynomeRead(Polynome<double> poly) {
    EquationSolver<double> solver;
    auto res = solver.solve(poly);
    
    std::cout << poly << " => " << res << std::endl;
    
}

int main(int argc, char **argv) {
    
    clock_t requestStartTime = clock();
    
    for (int i = 0; i < 100; i++) {
        PolynomeReader<double, 3> reader;
        reader.Read(--argc, ++argv, OnPolynomeRead);
    }
    
    clock_t requestEndTime = clock();
    //todo get rid of this in the final version
    std::cout << "Request processing time: " << (requestEndTime - requestStartTime) / double(CLOCKS_PER_SEC) << std::endl;
        
    return 0;
}
