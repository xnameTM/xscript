#include "Reader.h"

Reader::Reader(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
    }
    std::string line;
    while (std::getline(file, line)) {
        nodes.push_back(interpret_line(line));
    }
}

Tree* Reader::interpret_line(std::string &line) {
    if (line.empty() || ltrim(line).starts_with("//")) return nullptr;
    line = trim(line);

    std::vector<std::string> tokens = split_preserving_quotes(line);

    Tree *tree(parse_expression(tokens));

    // std::cout << "#####################################" << std::endl;
    // printTree(tree);

    return tree;
}

Tree* Reader::parse_expression(const std::vector<std::string> &tokens) {
    Tree *tree = new Tree("NOT_HANDLED");

    if (tokens.empty())
        return tree;

    if (tokens[0] == "const" || tokens[0] == "let") {
        if (tokens.size() < 4) {
            std::cerr << "Invalid assignment" << std::endl;
            return tree;
        }
        if (tokens[2] != "=" || !isValidVariableName(tokens[1])) {
            std::cerr << "Invalid assignment" << std::endl;
            return tree;
        }

        tree->value = tokens[0] == "const" ? "ASSIGN_PERMANENT" : "ASSIGN";
        tree->left = new Tree(tokens[1]);
        tree->right = parse_expression(std::vector(tokens.begin() + 3, tokens.end()));

        return tree;
    }

    std::vector<std::string> rpn_tokens = tokens_to_rpn(tokens);
    std::stack<Tree *> ast_stack;

    for (const auto &token: rpn_tokens) {
        if (token == "+" || token == "-" || token == "*" || token == "/") {
            if (ast_stack.size() < 2) {
                std::cerr << "Błędna notacja RPN: niewystarczająca liczba operandów dla operatora " << token <<
                        std::endl;
                return tree;
            }
            Tree *right = ast_stack.top();
            ast_stack.pop();
            Tree *left = ast_stack.top();
            ast_stack.pop();
            std::string action = token == "+"
                                     ? "ADD"
                                     : token == "-"
                                           ? "SUBTRACT"
                                           : token == "*"
                                                 ? "MULTIPLY"
                                                 : token == "/"
                                                       ? "DIVIDE"
                                                       : "NOT_HANDLED";
            Tree *node = new Tree(action);
            node->left = left;
            node->right = right;
            ast_stack.push(node);
        } else {
            ast_stack.push(new Tree(token));
        }
    }
    if (!ast_stack.empty()) {
        tree = ast_stack.top();
        ast_stack.pop();
    }

    return tree;
}

std::vector<std::string> Reader::tokens_to_rpn(const std::vector<std::string> &tokens) {
    std::vector<std::string> output;
    std::vector<std::string> stack;

    for (const std::string &token: tokens) {
        if (token == "(") {
            stack.push_back(token);
        } else if (token == ")") {
            while (!stack.empty() && stack.back() != "(") {
                output.push_back(stack.back());
                stack.pop_back();
            }
            stack.pop_back();
        } else if (token == "+" || token == "-") {
            while (!stack.empty() && (stack.back() == "+" || stack.back() == "-" || stack.back() == "*" || stack.back() == "/")) {
                output.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(token);
        } else if (token == "*" || token == "/") {
            while (!stack.empty() && (stack.back() == "*" || stack.back() == "/")) {
                output.push_back(stack.back());
                stack.pop_back();
            }
            stack.push_back(token);
        } else {
            output.push_back(token);
        }
    }

    while (!stack.empty()) {
        output.push_back(stack.back());
        stack.pop_back();
    }

    return output;
}

std::vector<std::string> Reader::split_preserving_quotes(const std::string &s) {
    std::vector<std::string> tokens;
    std::string token;
    char activeQuote = '\0';

    for (char c: s) {
        if (c == '"' || c == '\'' || c == '`') {
            if (activeQuote == '\0') {
                activeQuote = c;
                token.push_back(c);
                continue;
            } else if (activeQuote == c) {
                token.push_back(c);
                activeQuote = '\0';
                continue;
            }
        }
        if (c == ';' && activeQuote == '\0') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            continue;
        }
        if (std::isspace(static_cast<unsigned char>(c)) && activeQuote == '\0') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if ((c == '(' || c == ')') && activeQuote == '\0') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(std::string(1, c));
        } else {
            token.push_back(c);
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

bool Reader::isValidVariableName(const std::string &name) {
    if (name.empty()) return false;

    if (std::isdigit(static_cast<unsigned char>(name[0])))
        return false;

    for (char c: name) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') {
            return false;
        }
    }
    return true;
}