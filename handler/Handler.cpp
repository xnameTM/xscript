#include "Handler.h"

Handler::Handler(std::vector<Tree*> &nodes) {
    block = Block();

    for (const auto &node : nodes) {
        if (node == nullptr) continue;

        printTree(node);
        VariableType var = handleLine(*node);

        // print variable
        // std::cout << "Variable: " << var::convertToString(var) << std::endl;
    }
}

VariableType Handler::handleLine(Tree& node) {
    if (node.value == "ASSIGN" || node.value == "ASSIGN_PERMANENT") {
        std::string name = node.left->value;
        var::VariableProtector protector = node.value == "ASSIGN_PERMANENT" ? var::VariableProtector::CONSTANT : var::VariableProtector::MUTABLE;
        VariableType value = handleLine(*node.right);

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

    return nullptr;
}