#pragma once
#include <iostream>
#include <string>

class Tree {
public:
    std::string value{};
    Tree* left;
    Tree* right;

    explicit Tree(const std::string& value) : value(value), left(nullptr), right(nullptr) {}
};

inline void printTree(Tree *node, std::string prefix = "", bool isLast = true) {
    if (!node) return;

    std::cout << prefix;
    std::cout << (isLast ? "└── " : "├── ");
    std::cout << node->value << std::endl;

    std::string newPrefix = prefix + (isLast ? "    " : "│   ");
    bool hasLeft = (node->left != nullptr);
    bool hasRight = (node->right != nullptr);

    // Jeśli istnieje lewy węzeł, wypisujemy go. Jako ostatni, jeśli brak prawego.
    if (hasLeft) {
        printTree(node->left, newPrefix, !hasRight);
    }

    // Jeśli istnieje prawy węzeł, wypisujemy go jako ostatni.
    if (hasRight) {
        printTree(node->right, newPrefix, true);
    }
}