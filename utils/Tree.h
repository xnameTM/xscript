#pragma once
#include <iostream>
#include <string>

class Tree {
public:
    std::string value{};
    Tree* left;
    Tree* right;

    explicit Tree(const std::string& value);
};

void printTree(Tree *node, std::string prefix = "", bool isLast = true);