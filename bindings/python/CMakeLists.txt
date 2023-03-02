cmake_minimum_required(VERSION 3.12)
project(abacus_python)

find_package(Python REQUIRED COMPONENTS Interpreter Development)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# pybind11 dependency
add_subdirectory("${STEINWURF_RESOLVE}/pybind11-source" pybind11)

# Define library
file(GLOB_RECURSE abacus_python_sources ./src/*.cpp)
pybind11_add_module(abacus_python ${abacus_python_sources})
set_target_properties(abacus_python PROPERTIES OUTPUT_NAME abacus)

# Get all steinwurf object libraries and link directly with them.
get_property(steinwurf_object_libraries GLOBAL
             PROPERTY steinwurf::object_libraries)
target_link_libraries(abacus_python PRIVATE ${steinwurf_object_libraries})
target_link_libraries(abacus_python PRIVATE steinwurf::abacus)

# Install library
install(FILES $<TARGET_FILE:abacus_python>
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
