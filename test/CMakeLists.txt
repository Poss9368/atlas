cmake_minimum_required(VERSION 3.14)
project(AtlasTest VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)
file(GLOB SOURCES "*.cpp" "others/*.cpp" "coupons/*.cpp" "instruments/*.cpp" "visitors/*.cpp" "rates/*.cpp" "parsers/*.cpp")
file(GLOB INCLUDES "*.hpp" "visitors/*.hpp")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(${PROJECT_NAME} ${INCLUDES} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC gtest_main)

find_package(Boost REQUIRED)
find_package(QuantLib REQUIRED)

link_directories("${CMAKE_SOURCE_DIR}/build/${CMAKE_BUILD_TYPE}")

target_link_libraries(${PROJECT_NAME} PUBLIC Atlas)
target_link_libraries(${PROJECT_NAME} PUBLIC QuantLib::QuantLib Boost::boost)

include_directories("${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/src")

gtest_discover_tests(${PROJECT_NAME})