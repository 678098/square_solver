#pragma once

#include <polynome.hpp>
#include <solution.hpp>

#include <cmath>
#include <memory>


template<typename ValueType>
class EquationSolver
{
public:
    virtual ~EquationSolver() = default;
    
    EquationSolution<ValueType> Solve(const Polynome<ValueType> &polynome) const {
        if (CanSolve(polynome)) {
            return SolveImpl(polynome);
        }
        if (nextSolver) {
            return nextSolver->Solve(polynome);
        } else {
            //todo consider throwing an exception here instead of can't solve solution
            return SpecificSolution::kCannotSolve;
        }
    }
    
    template<template <typename NextSolverValueType> typename NextSolverType>
    EquationSolver *Chain() {
        if (nextSolver) {
            nextSolver->template Chain<NextSolverType>();
        } else {
            nextSolver = std::make_unique<NextSolverType<ValueType> >();
        }
        return this;
    }
    
protected:
    virtual bool CanSolve(const Polynome<ValueType> &polynome) const = 0;
    virtual EquationSolution<ValueType> SolveImpl(const Polynome<ValueType> &polynome) const = 0;
    
    std::unique_ptr<EquationSolver<ValueType> > nextSolver;
};


template<typename ValueType>
class QuadraticSolver: public EquationSolver<ValueType>
{
protected:
    bool CanSolve(const Polynome<ValueType> &polynome) const override {
        return polynome.Degree() == 2;
    }
    
    EquationSolution<ValueType> SolveImpl(const Polynome<ValueType> &polynome) const override {
        const ValueType &a = polynome.A();
        const ValueType &b = polynome.B();
        const ValueType &c = polynome.C();
        
        ValueType d = b * b - 4 * a * c;
        if (d < 0) {
            return SpecificSolution::kNoRoots;
        }
        
        ValueType sqrt_d = std::sqrt(static_cast<double>(d));
        //todo optimize
        ValueType a2 = 2 * a;
        ValueType x1 = (-b + sqrt_d) / a2;
        ValueType x2 = (-b - sqrt_d) / a2;
        
        return std::vector<ValueType>({x1, x2});
    }
};


template<typename ValueType>
class LinearSolver: public EquationSolver<ValueType>
{
protected:
    bool CanSolve(const Polynome<ValueType> &polynome) const override {
        return polynome.Degree() == 1;
    }
    
    EquationSolution<ValueType> SolveImpl(const Polynome<ValueType> &polynome) const override {
        const ValueType &b = polynome.B();
        const ValueType &c = polynome.C();
        
        ValueType x = -c / b;
        
        return std::vector<ValueType>({x});
    }
};


template<typename ValueType>
class ConstantSolver: public EquationSolver<ValueType>
{
protected:
    bool CanSolve(const Polynome<ValueType> &polynome) const override {
        return polynome.Degree() == 0;
    }
    
    EquationSolution<ValueType> SolveImpl(const Polynome<ValueType> &polynome) const override {
        const ValueType &c = polynome.C();
        
        return IsAlmostZero(c) ? SpecificSolution::kAnyNumberIsRoot : SpecificSolution::kNoRoots;
    }
};


template<typename ValueType>
std::unique_ptr<EquationSolver<ValueType> > BuildSolver() {
    std::unique_ptr<EquationSolver<ValueType> > res = std::make_unique<QuadraticSolver<ValueType> >();
    res->template Chain<LinearSolver>()->template Chain<ConstantSolver>();
    return res;
}
