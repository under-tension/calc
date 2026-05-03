cmake_minimum_required(VERSION 3.25)

project(calc)

include(FetchContent)

FetchContent_Declare(
    libmath
    GIT_REPOSITORY git@github.com:under-tension/libmath.git
    GIT_TAG master
)

FetchContent_Declare(
    json
    URL
    https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(GOOGLETEST_USE_INSTALL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

FetchContent_MakeAvailable(libmath json spdlog googletest)

add_executable(calc
    src/main.cpp
    src/Calculator.cpp
    src/Checker.cpp
    src/application/ConsoleApplication.cpp
    src/printers/ConsolePrinter.cpp
    src/parsers/JsonParser.cpp
    src/loggers/SpdLogger.cpp
)

target_link_libraries(calc PRIVATE
    libmath
    nlohmann_json::nlohmann_json
    spdlog::spdlog
)

target_include_directories(calc
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

install(TARGETS calc
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)

enable_testing()

add_executable(
  tests
  src/Calculator.cpp
  src/Checker.cpp
  src/parsers/JsonParser.cpp
  tests/CalculatorTest.cpp
  tests/CheckerTest.cpp
  tests/parsers/JsonParserTest.cpp
)

target_include_directories(tests
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE
    libmath
    GTest::gtest_main
    nlohmann_json::nlohmann_json
)

include(GoogleTest)
gtest_discover_tests(tests)

option(ENABLE_CLANG_FORMAT "Enable clang-format" OFF)
if(ENABLE_CLANG_FORMAT)
    find_program(CLANG_FORMAT_PATH NAMES clang-format-19 clang-format REQUIRED)
    message(STATUS "Found clang-format: ${CLANG_FORMAT_PATH}")

    file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
        include/*.hpp
        src/*.cpp
        tests/*.cpp
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

