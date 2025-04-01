#pragma once
#include "Variable.cpp"

namespace var {
    class BigInt : public Variable {
    public:
        long int value;
        BigInt(std::string name, VariableProtector protector, long int value) : Variable(std::move(name), &protector), value(value) {}

        std::string toString() override {
            return std::to_string(value);
        }
    };
}