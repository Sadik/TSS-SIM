project(TSS-SIM)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0 -std=c++11 -lboost_system")

add_subdirectory(src)

aux_source_directory(. SRC_LIST)

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

FIND_PACKAGE(Boost COMPONENTS system thread filesystem REQUIRED)

add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} src ${Boost_LIBRARIES})
