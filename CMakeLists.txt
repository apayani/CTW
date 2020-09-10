cmake_minimum_required(VERSION 3.0.0)
project(myCTW VERSION 0.1.0)

# include(CTest)
# enable_testing()

set(sources
    main.cpp
    Source/arithmeticCoder.cpp
    Source/ctwBinaryTree.cpp
    Source/ctwEngine.cpp
)

set(headers
    Source/arithmeticCoder.h
    Source/ctwBinaryTree.h
    Source/ctwEngine.h
)

add_executable(myCTW ${sources} ${headers})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
