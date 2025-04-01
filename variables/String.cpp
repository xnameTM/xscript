#pragma once
#include <utility>
#include "Variable.cpp"

namespace var {
    class String : public Variable {
    public:
        std::string value;
        String(std::string name, VariableProtector protector, std::string value) : Variable(std::move(name), &protector), value(std::move(value)) {}

        std::string toString() override {
            return value;
        }


    };
}