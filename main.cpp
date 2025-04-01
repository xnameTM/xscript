#include <iostream>
#include "handler/Handler.h"
#include "reader/Reader.h"

int main() {
    Reader reader("/Users/xname/Documents/projects/XScript/file.ts");
    Handler handler(reader.nodes);

    // std::cout << var::convertToString(100000.12f) << std::endl;

    // for (const auto& node : reader.nodes) {
    //     printTree(node);
    // }

    return 0;
}
