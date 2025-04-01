#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "../utils/string.h"
#include "../utils/Tree.h"

class Reader {
public:
    std::vector<Tree*> nodes;
    explicit Reader(const std::string &path);
    static bool isValidVariableName(const std::string &name);

private:
    static Tree* interpret_line(std::string &line);
    static Tree* parse_expression(const std::vector<std::string> &tokens);
    static std::vector<std::string> tokens_to_rpn(const std::vector<std::string> &tokens);
    static std::vector<std::string> split_preserving_quotes(const std::string &s);
};