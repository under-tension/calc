cmake_minimum_required(VERSION 3.10)

project(calc)

include(FetchContent)

FetchContent_Declare(
    libmath
    GIT_REPOSITORY git@github.com:under-tension/libmath.git
    GIT_TAG master
)

FetchContent_MakeAvailable(libmath)

add_executable(calc
    src/app.cpp
    src/main.cpp
)

target_link_libraries(calc
    PRIVATE
        math
)

target_include_directories(math
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)
