cmake_minimum_required(VERSION 3.0.0)
project(myCTW VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(sources
    main.cpp
    SRC/arithmeticCoder.cpp
    SRC/ctwBinaryTree.cpp
    SRC/ctwEngine.cpp
)

set(headers
    SRC/arithmeticCoder.h
    SRC/ctwBinaryTree.h
    SRC/ctwEngine.h
)

add_executable(myCTW ${sources} ${headers})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
