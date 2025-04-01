#include "Block.h"

Block::Block() {
    variables = std::vector<var::Variable*>();
    functions = std::vector<Function*>();
}