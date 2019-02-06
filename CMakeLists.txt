cmake_minimum_required(VERSION 3.8)

# set up the project name
project(ArbitrageBot)

# specify the global variables
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -L/usr/local/opt/openssl/lib/ -lssl -lcrypto")
set(BOOST_ROOT /usr/local/opt/boost@1.67)

# add header files to be used
include_directories(${PROJECT_SOURCE_DIR}/include/nlohmann)
include_directories(${PROJECT_SOURCE_DIR}/include/trading)
include_directories(${PROJECT_SOURCE_DIR}/include/trading/strategy)
include_directories(${PROJECT_SOURCE_DIR}/include/arbitrage)
include_directories(${PROJECT_SOURCE_DIR}/include/utils)
include_directories(${PROJECT_SOURCE_DIR}/include/boost_1_69_0)
include_directories(/usr/local/Cellar/openssl/1.0.2o_2/include/)

add_subdirectory(src)
add_subdirectory(test)
