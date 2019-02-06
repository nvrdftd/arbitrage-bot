# create executables
add_executable(ArbitrageBot main.cpp)

# link static libraries
target_link_libraries(ArbitrageBot trading)
target_link_libraries(ArbitrageBot arbitrage)
target_link_libraries(ArbitrageBot utils)
