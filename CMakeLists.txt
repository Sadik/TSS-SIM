project(TSS-SIM)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(src)

aux_source_directory(. SRC_LIST)

set(BOOST_INCLUDEDIR ./thirdparty/boost)
set(BOOST_LIBRARYDIR ./thirdparty/boost/stage/lib)
set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)

find_package(Boost
             1.62.0
             COMPONENTS program_options
             REQUIRED)

#find_package(Boost 1.58.0 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} src ${Boost_LIBRARIES} )
