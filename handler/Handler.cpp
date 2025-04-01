#include "Handler.h"
#include "../reader/Reader.h"

Handler::Handler(std::vector<Tree*> &nodes) {
    block = Block();

    for (const auto &node : nodes) {
        if (node == nullptr) continue;

        printTree(node);
        VariableType var = handleLine(*node);
    }

    // print variables from block
    std::cout << "Variables in block:" << std::endl;

    for (const auto &variable : block.variables) {
        std::cout << "   ";

        if (std::holds_alternative<var::String>(variable)) {
            var::String str = std::get<var::String>(variable);
            std::cout << (str.isProtected() ? "const " : "let ");
            std::cout << str.name << ": (String) \"" << var::convertToString(str.value) << "\"" << std::endl;
        } else if (std::holds_alternative<var::Number>(variable)) {
            var::Number num = std::get<var::Number>(variable);
            std::cout << (num.isProtected() ? "const " : "let ");
            std::cout << num.name << ": ";
            if (std::holds_alternative<float>(num.value)) {
                std::cout << "(Number) " << std::get<float>(num.value) << std::endl;
            } else if (std::holds_alternative<var::NaN>(num.value)) {
                std::cout << "(Number) NaN" << std::endl;
            } else if (std::holds_alternative<var::Infinity>(num.value)) {
                std::cout << "(Number) " << std::get<var::Infinity>(num.value).toString() << std::endl;
            }
        } else if (std::holds_alternative<var::Boolean>(variable)) {
            var::Boolean boolean = std::get<var::Boolean>(variable);
            std::cout << (boolean.isProtected() ? "const " : "let ");
            std::cout << boolean.name << ": (Boolean) " << var::convertToString(boolean.value) << std::endl;
        } else if (std::holds_alternative<var::Null>(variable)) {
            var::Null null = std::get<var::Null>(variable);
            std::cout << (null.isProtected() ? "const " : "let ");
            std::cout << null.name << ": (Null) Null" << std::endl;
        }
    }
}

VariableType Handler::handleLine(Tree& node) {
    if (node.value == "ASSIGN" || node.value == "ASSIGN_PERMANENT") {
        std::string name = node.left->value;
        var::VariableProtector protector = node.value == "ASSIGN_PERMANENT" ? var::VariableProtector::CONSTANT : var::VariableProtector::MUTABLE;
        VariableType value = handleLine(*node.right);

        if (std::holds_alternative<std::string>(value))
            block.variables.push_back(var::String(name, std::get<std::string>(value), protector));
        if (std::holds_alternative<float>(value))
            block.variables.push_back(var::Number(name, std::get<float>(value), protector));
        if (std::holds_alternative<bool>(value))
            block.variables.push_back(var::Boolean(name, std::get<bool>(value), protector));
        if (std::holds_alternative<void*>(value))
            block.variables.push_back(var::Null(name, protector));
        if (std::holds_alternative<var::NaN>(value))
            block.variables.push_back(var::Number(name, std::get<var::NaN>(value), protector));
        if (std::holds_alternative<var::Infinity>(value))
            block.variables.push_back(var::Number(name, std::get<var::Infinity>(value), protector));

        return nullptr;
    } else if (node.value == "ADD") {
        VariableType left = handleLine(*node.left);
        VariableType right = handleLine(*node.right);

        return var::Variable::add(left, right);
    } else if (node.value == "SUBTRACT") {
        VariableType left = handleLine(*node.left);
        VariableType right = handleLine(*node.right);

        return var::Variable::subtract(left, right);
    } else if (node.value == "MULTIPLY") {
        VariableType left = handleLine(*node.left);
        VariableType right = handleLine(*node.right);

        return var::Variable::multiply(left, right);
    } else if (node.value == "DIVIDE") {
        VariableType left = handleLine(*node.left);
        VariableType right = handleLine(*node.right);

        return var::Variable::divide(left, right);
    }

    std::string value = node.value;
    if (value == "true") {
        return true;
    } else if (value == "false") {
        return false;
    } else if (value == "null") {
        return nullptr;
    } else if (value == "NaN") {
        return var::NaN();
    } else if (value == "Infinity") {
        return var::Infinity(true);
    } else if (value == "-Infinity") {
        return var::Infinity(false);
    } else if (std::all_of(value.begin(), value.end(), [](char c) { return std::isdigit(c) || c == '.' || c == '-'; })) {
        return std::stof(value);
    } else if (value.size() > 1 && ((value[0] == '"' && value[value.size() - 1] == '"') || (value[0] == '\'' && value[value.size() - 1] == '\''))) {
        return value.substr(1, value.size() - 2);
    }

    if (Reader::isValidVariableName(node.value)) {
        auto value = block.getVariableValue(node.value);
        return value;
    }

    return nullptr;
}