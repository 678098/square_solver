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
    
    EquationSolution<ValueType> solve(const Polynome<ValueType> &polynome) const {
        if (canSolve(polynome)) {
            return solveImpl(polynome);
        }
        if (nextSolver) {
            return nextSolver->solve(polynome);
        } else {
            //todo consider throwing an exception here instead of can't solve solution
            return SpecificSolution::kCannotSolve;
        }
    }
    
    template<template <typename NextSolverValueType> typename NextSolverType>
    EquationSolver *chain() {
        if (nextSolver) {
            nextSolver->template chain<NextSolverType>();
        } else {
            nextSolver = std::make_unique<NextSolverType<ValueType> >();
        }
        return this;
    }
    
protected:
    virtual bool canSolve(const Polynome<ValueType> &polynome) const = 0;
    virtual EquationSolution<ValueType> solveImpl(const Polynome<ValueType> &polynome) const = 0;
    
    std::unique_ptr<EquationSolver<ValueType> > nextSolver;
};


template<typename ValueType>
class QuadraticSolver: public EquationSolver<ValueType>
{
protected:
    bool canSolve(const Polynome<ValueType> &polynome) const override {
        return polynome.degree() == 2;
    }
    
    EquationSolution<ValueType> solveImpl(const Polynome<ValueType> &polynome) const override {
        const ValueType &a = polynome.a();
        const ValueType &b = polynome.b();
        const ValueType &c = polynome.c();
        
        ValueType d = b * b - 4 * a * c;
        if (d < 0) {
            return SpecificSolution::kNoRoots;
        }
        
        ValueType sqrt_d = std::sqrt(d);
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
    bool canSolve(const Polynome<ValueType> &polynome) const override {
        return polynome.degree() == 1;
    }
    
    EquationSolution<ValueType> solveImpl(const Polynome<ValueType> &polynome) const override {
        const ValueType &b = polynome.b();
        const ValueType &c = polynome.c();
        
        ValueType x = -c / b;
        
        return std::vector<ValueType>({x});
    }
};


template<typename ValueType>
class ConstantSolver: public EquationSolver<ValueType>
{
protected:
    bool canSolve(const Polynome<ValueType> &polynome) const override {
        return polynome.degree() == 0;
    }
    
    EquationSolution<ValueType> solveImpl(const Polynome<ValueType> &polynome) const override {
        const ValueType &c = polynome.c();
        
        if (isAlmostZero(c)) {
            return SpecificSolution::kAnyNumberIsRoot;
        } else {
            return SpecificSolution::kNoRoots;
        }
    }
};


template<typename ValueType>
std::unique_ptr<EquationSolver<ValueType> > buildSolver() {
    std::unique_ptr<EquationSolver<ValueType> > res = std::make_unique<QuadraticSolver<ValueType> >();
    res->template chain<LinearSolver>()->template chain<ConstantSolver>();
    return res;
}

