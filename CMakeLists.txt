cmake_minimum_required(VERSION 3.12)
project(abacus)

find_package(Python COMPONENTS Interpreter)
include(FetchContent)

# Use waf to resolve dependencies
if(NOT DEFINED STEINWURF_RESOLVE)
  message(STATUS "Resolving dependencies...")
  execute_process(
    COMMAND ${Python_EXECUTABLE} waf resolve ${STEINWURF_RESOLVE_OPTIONS}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE STATUS)

  if(STATUS AND NOT STATUS EQUAL 0)
    message(FATAL_ERROR "Failed: ${STATUS}")
  endif()

  set(STEINWURF_RESOLVE "${CMAKE_CURRENT_SOURCE_DIR}/resolve_symlinks")
endif()

# bourne dependency
if(NOT TARGET steinwurf::bourne)
  add_subdirectory("${STEINWURF_RESOLVE}/bourne" 
  ${CMAKE_CURRENT_BINARY_DIR}/bourne 
  EXCLUDE_FROM_ALL)
endif()

# endian dependency
if(NOT TARGET steinwurf::endian)

  add_subdirectory("${STEINWURF_RESOLVE}/endian" 
  ${CMAKE_CURRENT_BINARY_DIR}/endian 
  EXCLUDE_FROM_ALL)
endif()

# protobuf
if(NOT TARGET protobuf::libprotobuf)
  add_subdirectory(
    "${STEINWURF_RESOLVE}/protobuf" 
    ${CMAKE_CURRENT_BINARY_DIR}/protobuf)
endif()

# Define library
file(GLOB_RECURSE abacus_sources ./src/*.cpp ./src/*.cc)

# Is this the top-level steinwurf project?
if(${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
  # Create static library
  add_library(abacus STATIC ${abacus_sources})

  # Get all steinwurf object libraries and link directly with them.
  get_property(steinwurf_object_libraries GLOBAL
    PROPERTY steinwurf::object_libraries)
  target_link_libraries(abacus PRIVATE ${steinwurf_object_libraries})

  # Install library
  install(FILES $<TARGET_FILE:abacus> DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
else()
  # Create object library
  add_library(abacus OBJECT ${abacus_sources})

  # Add this library to a global list of steinwurf object libraries
  set_property(GLOBAL APPEND PROPERTY steinwurf::object_libraries
    steinwurf::abacus)

  # Link object dependencies
  target_link_libraries(abacus PRIVATE steinwurf::bourne)
endif()

target_link_libraries(abacus PRIVATE steinwurf::endian)
target_link_libraries(abacus PUBLIC protobuf::libprotobuf)

target_include_directories(abacus PUBLIC src)
target_compile_features(abacus PUBLIC cxx_std_14)
add_library(steinwurf::abacus ALIAS abacus)

# Install headers excluding "detail" as these are internal to the library.
install(
  DIRECTORY ./src/abacus
  DESTINATION ${CMAKE_INSTALL_PREFIX}/include
  FILES_MATCHING
  PATTERN *.hpp
  PATTERN ./src/abacus/detail EXCLUDE)

# Is top level project?
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
  # Build executables
  add_executable(metrics_simple examples/metrics_simple.cpp)
  target_link_libraries(metrics_simple abacus)
  target_link_libraries(metrics_simple steinwurf::endian)
  target_link_libraries(metrics_simple protobuf::libprotobuf)

  enable_testing()


  # Google Test dependency
  add_subdirectory("${STEINWURF_RESOLVE}/gtest")

  # Build test executable
  file(GLOB_RECURSE abacus_tests_sources ./test/*.cpp)
  add_executable(abacus_tests ${abacus_tests_sources})
  target_link_libraries(abacus_tests steinwurf::gtest)
  target_link_libraries(abacus_tests
    steinwurf::abacus
    steinwurf::endian
    steinwurf::bourne
    protobuf::libprotobuf)

  add_test(abacus_tests abacus_tests)

  # Google Benchmark dependency
  set(BENCHMARK_ENABLE_TESTING
    OFF
    CACHE INTERNAL "Disable gbenchmark test" FORCE)

  if(NOT TARGET benchmark)
    add_subdirectory("${STEINWURF_RESOLVE}/gbenchmark-source")
  endif()

  # Build benchmark executable
  file(GLOB_RECURSE abacus_benchmark_sources ./benchmark/main.cpp)
  add_executable(abacus_benchmark ${abacus_benchmark_sources})
  target_link_libraries(abacus_benchmark benchmark)
  target_link_libraries(abacus_benchmark
    steinwurf::abacus
    steinwurf::endian
    steinwurf::bourne
    protobuf::libprotobuf)
endif()
