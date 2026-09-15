cmake_minimum_required(VERSION 3.25)

project(calc VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)

option(ENABLE_TSAN "Build with ThreadSanitizer" OFF)

if(ENABLE_TSAN)
    message(STATUS "ThreadSanitizer enabled")

    add_compile_options(-fsanitize=thread -g -O1 -fno-omit-frame-pointer)
    add_link_options(-fsanitize=thread)
endif()

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

# Общий код: доступен и серверу, и клиенту, и тестам.
add_library(calc_core STATIC
    src/Calculator.cpp
    src/Checker.cpp
    src/application/ConsoleApplication.cpp
    src/application/ServiceRunner.cpp
    src/application/Daemon.cpp
    src/application/Options.cpp
    src/application/PidFile.cpp
    src/printer/ConsolePrinter.cpp
    src/parser/JsonParser.cpp
    src/protocol/Messages.cpp
    src/config/ServerConfig.cpp
    src/config/AppConfig.cpp
    src/config/DotEnv.cpp
    src/logger/SpdLogger.cpp
    src/signal/SignalHandler.cpp
    src/net/Server.cpp
    src/net/Session.cpp
    src/net/Client.cpp
    src/db/repository/OperationRepository.cpp
    src/cache/HashTableCache.cpp
    src/cache/PgCacheHydrator.cpp
)

target_link_libraries(calc_core PUBLIC
    libmath
    nlohmann_json::nlohmann_json
    spdlog::spdlog
    PostgreSQL::PostgreSQL
    Boost::asio
    Threads::Threads
)

target_include_directories(calc_core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(calc_core PUBLIC CALC_VERSION="${PROJECT_VERSION}")

add_executable(calc src/main.cpp)
target_link_libraries(calc PRIVATE calc_core)

add_executable(calc_client src/client_main.cpp)
target_link_libraries(calc_client PRIVATE calc_core)

install(TARGETS calc calc_client
    RUNTIME DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}
)

install(TARGETS calc calc_client
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    COMPONENT service
    EXCLUDE_FROM_ALL
)

install(FILES systemd/calc.service
    DESTINATION lib/systemd/system
    COMPONENT service
    EXCLUDE_FROM_ALL
)

install(FILES .env.example
    DESTINATION /etc/calc
    RENAME calc.env
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ
    COMPONENT service
    EXCLUDE_FROM_ALL
)

install(DIRECTORY migrations/
    DESTINATION ${CMAKE_INSTALL_DATADIR}/calc/migrations
    COMPONENT service
    EXCLUDE_FROM_ALL
    FILES_MATCHING PATTERN "*.sql"
)

enable_testing()

add_executable(
  tests
  tests/CalculatorTest.cpp
  tests/CheckerTest.cpp
  tests/parser/JsonParserTest.cpp
  tests/protocol/MessagesTest.cpp
  tests/cache/HashTableCacheTest.cpp
  tests/config/DotEnvTest.cpp
  tests/net/ServerTest.cpp
)

target_link_libraries(tests PRIVATE
    calc_core
    GTest::gtest_main
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

# Сборка deb-пакета: cpack --config build/CPackConfig.cmake
set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_NAME "calc")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "Calculator TCP service with PostgreSQL storage")

set(CPACK_PACKAGE_CONTACT "calc maintainers <noname@example.com>"
    CACHE STRING "Package maintainer")

set(CPACK_PACKAGING_INSTALL_PREFIX "/usr")

set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_COMPONENTS_ALL service)

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_CONTACT}")
set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")

set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)

set(CPACK_DEBIAN_PACKAGE_RECOMMENDS "postgresql-client")

set(CPACK_DEBIAN_PACKAGE_CONTROL_STRICT_PERMISSION ON)

set(CPACK_DEBIAN_SERVICE_PACKAGE_NAME "calc")
set(CPACK_DEBIAN_SERVICE_FILE_NAME "DEB-DEFAULT")

set(CALC_CONFFILES "${CMAKE_CURRENT_BINARY_DIR}/debian/conffiles")
file(WRITE "${CALC_CONFFILES}" "/etc/calc/calc.env\n")

set(CPACK_DEBIAN_SERVICE_PACKAGE_CONTROL_EXTRA
    "${CALC_CONFFILES}"
    "${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst"
    "${CMAKE_CURRENT_SOURCE_DIR}/debian/prerm"
    "${CMAKE_CURRENT_SOURCE_DIR}/debian/postrm"
)

include(CPack)