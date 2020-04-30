cmake_minimum_required(VERSION 3.14)

project(fesutils)

set(CMAKE_CXX_STANDARD 17)

option(SANITIZER_BUILD "Enable UB and adress sanitizer." OFF)
set(SANITIZER_BUILD 0)

if(SANITIZER_BUILD)
    message("Sanitizer build enabled")
    SET (CMAKE_CXX_COMPILER             "clang")
    SET (CMAKE_CXX_COMPILER             "clang++")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=address")
    set (CMAKE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=undefined -fsanitize=address")
else()
    message("Sanitizer build disabled")
endif()

set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
            STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
            "Debug" "Release" "RelWithDebInfo")
endif()


option(COVERAGE_BUILD "Create a GCov-enabled build." OFF)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeModules)

if(COVERAGE_BUILD)
    message("Enabling code coverage ...")
    if(CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        APPEND_COVERAGE_COMPILER_FLAGS()
        set(COVERAGE_LCOV_EXCLUDES '/usr/*' '*/bundled-deps/*' '*/unit_tests/*' '*/cli_fontend.cpp')
        SETUP_TARGET_FOR_COVERAGE_LCOV(
                NAME ${PROJECT_NAME}_coverage                    # New target name
                EXECUTABLE complete_test_suite # Executable in PROJECT_BINARY_DIR
                DEPENDENCIES complete_test_suite                     # Dependencies to build first
        )
    endif()
endif()


find_package(Boost 1.67 REQUIRED COMPONENTS program_options log_setup log unit_test_framework iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(./bundled-deps/indicators/include/)

include_directories(./bundled-deps/deps-install/include)
link_directories(./bundled-deps/deps-install/lib)
link_directories(./bundled-deps/deps-install/lib64)



set(PCH_HEADER_LIST
        src/file_reader/PlumedDatHeader.hpp)

set(PCH_SYST_HEADER_LIST
        <string>
        <vector>
        <memory>
        <map>
        <variant>
        <fmt/format.h>
        <boost/log/trivial.hpp>
        <Eigen/Dense>
        <stdexcept>)

set(SOURCE_LIST src/GeneralOptions.cpp
                src/GeneralOptions.h
                src/final_bias_reweight/final_bias_reweight.hpp
                src/final_bias_reweight/final_bias_reweight.cpp
                src/frontend_common.h
                src/help/help.hpp
                src/help/help.cpp
                src/common/extract_from_string.cpp
                src/common/extract_from_string.hpp
                src/file_reader/PlumedDatHeader.cpp
                src/file_reader/read_file.cpp
                src/common/graph_node_type.hpp
                src/file_reader/read_file_header.cpp
                src/common/fs_utils.cpp
                src/common/fs_utils.hpp
                src/common/conversions.hpp
                src/common/print_lines_node.cpp
                src/common/print_lines_node.hpp
                src/file_reader/parse_space_separated_values_node.cpp
                src/file_reader/parse_space_separated_values_node.hpp
                src/common/dummy_sink_node.hpp
                src/grid/grid_utils.cpp
                src/grid/grid_utils.hpp
                src/grid/Grid.cpp
                src/grid/Grid.hpp src/common/limiter_decrementer_node.hpp src/grid/grid_filler_node.cpp src/grid/grid_filler_node.hpp src/progress/progress_printer.cpp src/progress/progress_printer.hpp src/progress/progress_printer_nodes.hpp)

set(LIBRARY_LINK_LIST tbb ${Boost_LIBRARIES} fmt)


add_executable(fesutils src/cli_frontend.cpp ${SOURCE_LIST})
target_compile_definitions(fesutils PUBLIC BOOST_LOG_DYN_LINK)

target_link_libraries(fesutils  ${LIBRARY_LINK_LIST})

target_compile_options(fesutils PRIVATE -Wno-ignored-attributes)

if(NOT SANITIZER_BUILD)
target_precompile_headers(fesutils
        PUBLIC
        ${PCH_HEADER_LIST}
        ${PCH_SYST_HEADER_LIST}
        )
endif()

add_custom_command(
        TARGET fesutils POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/system_tests/st_final_bias_reweight.py
        ${CMAKE_CURRENT_BINARY_DIR}/st_final_bias_reweight.py)


enable_testing()

set(TEST_SOURCE_LIST
        src/unit_tests/main_test_suite.cpp
        src/unit_tests/common/extract_from_string_ts.cpp
        src/unit_tests/file_reader/file_header_reader_ts.cpp
        src/unit_tests/file_reader/read_file_ts.cpp
        src/unit_tests/TempTextFile.hpp
        src/unit_tests/TempTextFile.cpp
        src/unit_tests/file_reader/parse_space_separated_values_ts.cpp src/unit_tests/grid/grid_utils_ts.cpp src/unit_tests/grid/grid_ts.cpp src/unit_tests/MiscFixture.cpp src/unit_tests/MiscFixture.hpp src/grid/NDArray.cpp src/grid/NDArray.hpp src/unit_tests/grid/ndarray_ts.cpp src/unit_tests/common/misc_ts.cpp src/unit_tests/grid/grid_filler_node_ts.cpp src/unit_tests/progress/progress_printer_ts.cpp)


add_executable(complete_test_suite ${TEST_SOURCE_LIST}  ${SOURCE_LIST})
target_compile_definitions(complete_test_suite PUBLIC BOOST_LOG_DYN_LINK)

target_link_libraries(complete_test_suite ${LIBRARY_LINK_LIST})

if(NOT SANITIZER_BUILD)
    target_precompile_headers(complete_test_suite
            PUBLIC
            ${PCH_HEADER_LIST}
            ${PCH_SYST_HEADER_LIST}
            )
endif()


target_compile_options(complete_test_suite PRIVATE -Wno-ignored-attributes)

add_custom_command(
        TARGET complete_test_suite POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/system_tests/st_final_bias_reweight.py
        ${CMAKE_CURRENT_BINARY_DIR}/st_final_bias_reweight.py)


add_test(fesutils_unittest complete_test_suite --catch_system_error=yes --log_level=all )

add_test(NAME fesutils_systemtest_final_bias_reweight
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND python3 st_final_bias_reweight.py)