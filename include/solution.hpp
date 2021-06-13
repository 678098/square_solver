#pragma once

#include <variant>


enum class SpecificSolution {
    kNoRoots,
    kAnyNumberIsRoot,
    kCannotSolve
};


template<typename ValueType>
using EquationSolution = std::variant<std::vector<ValueType>, SpecificSolution>;


std::ostream &operator<<(std::ostream &out, const SpecificSolution &spec) {
    switch (spec) {
        case SpecificSolution::kNoRoots:
            out << "no roots";
            break;
        case SpecificSolution::kAnyNumberIsRoot:
            out << "any number";
            break;
        case SpecificSolution::kCannotSolve:
            out << "cannot solve";
            break;
        default:
            throw std::runtime_error("Unexpected SpecificSolution found");
    }
    return out;
}

template<typename ValueType>
std::ostream &operator<<(std::ostream &out, const EquationSolution<ValueType> &solution) {
    std::size_t index = solution.index();
    if (index == 0) {
        const std::vector<ValueType> &vals = std::get<std::vector<ValueType> >(solution);
        out << "(";
        for (int i = int(vals.size()) - 1; i > 0; i--) {
            out << vals[i] << " ";
        }
        if (!vals.empty()) {
            out << vals[0];
        }
        out << ")";
    } else {
        out << std::get<SpecificSolution>(solution);
    }
    return out;
}
