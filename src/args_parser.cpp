#include <args_parser.hpp>

#include <iostream>


void PrintUsage() {
    std::cout << "Usage: square_solver [PARAMETERS] [COEFFICIENTS: a1 b1 c1 ... aN [bN [cN]]]" << std::endl;
    std::cout << "Solving multiple square equations a*x^2 + b*x + c = 0 with given" << std::endl;
    std::cout << "COEFFICIENTS a b c. Missing trailing coefficients b, c are" << std::endl;
    std::cout << "considered to be 0. Non-numeric data in arguments is filtered out." << std::endl;
    std::cout << std::endl;
    std::cout << "\t--help        print usage" << std::endl;
    std::cout << "\t--silent      don't print found solutions" << std::endl;
    std::cout << "\t--perf        measure request processing time" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "\t./square_solver 3 4 5 0 4 -4 11 22 123 45 42 42 67 1001" << std::endl;
    std::cout << "\t./square_solver 1 -2 -3 0 4 -4 11 22" << std::endl;
    std::cout << "\t./square_solver 1 2 1 1 4 1 1 -3 2 0 1 -1 0 0 1 0 0 0" << std::endl;
    std::cout << "\t./square_solver --silent --perf `cat task.txt`" << std::endl;
}


Configuration ParseCmdArgs(int argc, char **argv, int &argc_flags_offset_out) {
    //executable name should be skipped
    int offset = 1;
    
    Configuration config;
    
    while (offset < argc) {
        std::string flag(argv[offset]);
        if (flag == "--help") {
            config.printUsage = true;
            ++offset;
            continue;
        }
        if (flag == "--silent") {
            config.silent = true;
            ++offset;
            continue;
        }
        if (flag == "--perf") {
            config.measurePerformance = true;
            ++offset;
            continue;
        }
        break;
    }
    if (argc == offset) {
        config.printUsage = true;
    }
    
    if (config.printUsage) PrintUsage();
    
    argc_flags_offset_out = offset;
    return config;
}
