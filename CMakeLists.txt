cmake_minimum_required(VERSION 3.12)

project(Traced)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories(${CMAKE_SOURCE_DIR}/third_party/SDL2/include)

file(GLOB LIBRARIES "third_party/SDL2/lib/*.dylib")

file(GLOB SOURCE_FILES "src/*.cpp" "src/*.h")


add_executable(Traced ${SOURCE_FILES} test/unit_tests/test_example.cpp)
target_link_libraries(Traced ${LIBRARIES})

