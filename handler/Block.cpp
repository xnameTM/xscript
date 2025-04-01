#include "Block.h"

Block::Block() {
    variables = std::vector<BlockVariableType>();
    functions = std::vector<Function*>();
}

var::VariableType Block::getVariableValue(std::string name) {
    for (const auto &variable : variables) {
        if (std::holds_alternative<var::String>(variable)) {
            var::String str = std::get<var::String>(variable);
            if (str.name == name) {
                return str.value;
            }
        } else if (std::holds_alternative<var::Number>(variable)) {
            var::Number num = std::get<var::Number>(variable);
            if (num.name == name) {
                if (std::holds_alternative<float>(num.value)) {
                    return std::get<float>(num.value);
                } else if (std::holds_alternative<var::NaN>(num.value)) {
                    return std::get<var::NaN>(num.value);
                } else if (std::holds_alternative<var::Infinity>(num.value)) {
                    return std::get<var::Infinity>(num.value);
                }
            }
        } else if (std::holds_alternative<var::Boolean>(variable)) {
            var::Boolean boolean = std::get<var::Boolean>(variable);
            if (boolean.name == name) {
                return boolean.value;
            }
        } else if (std::holds_alternative<var::Null>(variable)) {
            var::Null null = std::get<var::Null>(variable);
            if (null.name == name) {
                return nullptr;
            }
        }
    }
    return nullptr;
}