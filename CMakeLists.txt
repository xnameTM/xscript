cmake_minimum_required(VERSION 3.30)
project(XScript)

set(CMAKE_CXX_STANDARD 20)

add_executable(XScript main.cpp
        reader/reader.cpp
        handler/Block.cpp
        variables/String.h
        variables/Number.h
        variables/Boolean.h
        variables/Undefined.h
        variables/Null.h
        variables/Variable.cpp
        utils/string.cpp
        utils/Tree.cpp
        functions/Function.cpp
        handler/Handler.cpp
        variables/Variable.h
        utils/Tree.h
        utils/string.h
        reader/Reader.h
        handler/Handler.h
        handler/Block.h
        functions/Function.h
        variables/Boolean.cpp
        variables/Null.cpp
        variables/Number.cpp
        variables/String.cpp
        variables/Undefined.cpp
)
