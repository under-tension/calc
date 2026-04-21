cmake_minimum_required(VERSION 3.10)

project(calc)

include(FetchContent)

FetchContent_Declare(
    libmath
    GIT_REPOSITORY git@github.com:under-tension/libmath.git
    GIT_TAG master
)

FetchContent_MakeAvailable(libmath)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(calc
    src/main.cpp
    src/Calculator.cpp
    src/Checker.cpp
    src/Runner.cpp
    src/printers/ConsolePrinter.cpp
    src/parsers/JsonParser.cpp
)

target_link_libraries(calc PRIVATE libmath)
target_link_libraries(calc PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(calc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)

option(ENABLE_CLANG_FORMAT "Enable clang-format" OFF)
if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_PATH NAMES clang-format-19 clang-format REQUIRED)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_PATH}")

    file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
        include/*.h
        src/*.cpp
    )

    add_custom_target(calc-clang-format-target
        COMMAND
            ${CLANG_FORMAT_PATH} -style=file -i ${CLANG_FORMAT_SRC}
        WORKING_DIRECTORY
            ${CMAKE_SOURCE_DIR}
        COMMENT
            "Formatting with ${CLANG_FORMAT_PATH} ..."
    )

    add_dependencies(calc calc-clang-format-target)
endif()

option(ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
if(ENABLE_CLANG_TIDY)
    find_program(CLANG_TIDY_PATH NAMES clang-tidy-19 clang-tidy REQUIRED)
    message(STATUS "Found clang-tidy: ${CLANG_TIDY_PATH}")

    set(CLANG_TIDY_COMMAND ${CLANG_TIDY_PATH} "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")
    set_target_properties(calc PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
endif()

