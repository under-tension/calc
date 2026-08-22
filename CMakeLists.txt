cmake_minimum_required(VERSION 3.25)

project(calc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)

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

set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(
    boost
    URL
    https://github.com/boostorg/boost/releases/download/boost-1.88.0/boost-1.88.0-cmake.tar.xz
)

FetchContent_MakeAvailable(libmath json spdlog googletest boost)

add_executable(calc
    src/main.cpp
    src/Calculator.cpp
    src/Checker.cpp
    src/application/ConsoleApplication.cpp
    src/application/ServiceRunner.cpp
    src/printer/ConsolePrinter.cpp
    src/parser/JsonParser.cpp
    src/logger/SpdLogger.cpp
    src/signal/SignalHandler.cpp
    src/db/repository/OperationRepository.cpp
    src/cache/HashTableCache.cpp
    src/cache/PgCacheHydrator.cpp
)

target_link_libraries(calc PRIVATE
    libmath
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    PostgreSQL::PostgreSQL
    Boost::asio
    Threads::Threads
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
  src/parser/JsonParser.cpp
  tests/CalculatorTest.cpp
  tests/CheckerTest.cpp
  tests/parser/JsonParserTest.cpp
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

option(ENABLE_VALGRIND "Run tests under Valgrind" OFF)

if(ENABLE_VALGRIND)
    find_program(VALGRIND_EXECUTABLE valgrind REQUIRED)
    message(STATUS "Found Valgrind: ${VALGRIND_EXECUTABLE}")
        
    set(VALGRIND_FLAGS
        --leak-check=full
        --show-leak-kinds=definite,indirect
        --error-exitcode=1
        --track-origins=yes
        --soname-synonyms=somalloc=nouserintercepts
    )
    
    add_test(
        NAME tests_memcheck
        COMMAND ${VALGRIND_EXECUTABLE} ${VALGRIND_FLAGS} $<TARGET_FILE:tests>
    )
    
    set_tests_properties(tests_memcheck PROPERTIES TIMEOUT 300)
    
    message(STATUS "Valgrind test enabled: run with 'ctest -R tests_memcheck'")
endif()