cmake_minimum_required(VERSION 3.30)
project(XScript)

set(CMAKE_CXX_STANDARD 20)

add_executable(XScript main.cpp
        reader/reader.cpp
        handler/Block.cpp
        variables/String.cpp
        variables/Number.cpp
        variables/Bigint.cpp
        variables/Boolean.cpp
        variables/Undefined.cpp
        variables/Null.cpp
        variables/Object.cpp
        variables/Variable.cpp
        utils/string.cpp
        utils/Tree.cpp
        functions/Function.cpp
        handler/Handler.cpp
)
