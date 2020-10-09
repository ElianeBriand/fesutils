cmake_minimum_required(VERSION 3.14)

project(fesutils)

set(CMAKE_CXX_STANDARD 17)

option(SANITIZER_BUILD "Enable UB and adress sanitizer." OFF)
set(SANITIZER_BUILD 0)

if(SANITIZER_BUILD)
    message("Sanitizer build enabled")
    SET (CMAKE_CXX_COMPILER             "clang")
    SET (CMAKE_CXX_COMPILER             "clang++")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
    set (CMAKE_LINKER_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
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
        set(COVERAGE_LCOV_EXCLUDES '/usr/*' '*/bundled-deps/*'  'bundled-deps/*' '*/unit_tests/*' '*/cli_fontend.cpp')
        SETUP_TARGET_FOR_COVERAGE_LCOV(
                NAME ${PROJECT_NAME}_coverage                    # New target name
                EXECUTABLE complete_test_suite # Executable in PROJECT_BINARY_DIR
                DEPENDENCIES complete_test_suite                     # Dependencies to build first
        )
    endif()
endif()


find_package(Boost 1.67 COMPONENTS program_options log_setup log unit_test_framework iostreams REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
include_directories(${EIGEN3_INCLUDE_DIR})

include_directories(./bundled-deps/indicators/include/)

include_directories(./bundled-deps/deps-install/include)
link_directories(./bundled-deps/deps-install/lib)
link_directories(./bundled-deps/deps-install/lib64)



set(PCH_HEADER_LIST
        src/file_reader/PlumedDatHeader.hpp
        src/log_utils.hpp)

set(PCH_SYST_HEADER_LIST
        <string>
        <vector>
        <memory>
        <map>
        <tuple>
        <algorithm>
        <optional>
        <variant>
        <fmt/format.h>
        <boost/log/trivial.hpp>
        <boost/filesystem.hpp>
        <Eigen/Dense>
        <stdexcept>
        <yaml-cpp/yaml.h>
        )

set(SOURCE_LIST
                src/GeneralOptions.h
                src/final_bias_reweight/final_bias_reweight.hpp
                src/final_bias_reweight/final_bias_reweight.cpp
                src/frontend_common.h
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
                src/common/limiter_decrementer_node.hpp
                src/file_reader/grid_file/grid_filler_node.cpp
                src/file_reader/grid_file/grid_filler_node.hpp
                src/progress/progress_printer.cpp
                src/progress/progress_printer.hpp
                src/progress/progress_printer_nodes.hpp
                src/reconstruct_from_fragments/reconstruct_from_fragments.cpp
                src/reconstruct_from_fragments/reconstruct_from_fragments.hpp
                src/file_reader/field_function_heuristics.cpp
                src/file_reader/field_function_heuristics.hpp
                src/reconstruct_from_fragments/reconstruction_verynaive.cpp
                src/reconstruct_from_fragments/reconstruction_verynaive.hpp
                src/reconstruct_from_fragments/reconstruction_utils.cpp
                src/reconstruct_from_fragments/reconstruction_utils.hpp
                src/log_utils.hpp src/export/simple_opendx.cpp
                src/export/simple_opendx.hpp
                src/runfile/yaml_frontend.hpp
                src/runfile/yaml_frontend.cpp
                src/runfile/runfile_general_section.cpp
                src/runfile/runfile_general_section.hpp
                src/GeneralOptions.cpp
                src/runfile/runfile_search_directory_section.cpp
                src/runfile/runfile_search_directory_section.hpp
                src/runfile/runfile_steps.cpp
                src/runfile/runfile_steps.hpp
                src/runfile/runfile_fes_data.cpp
                src/runfile/runfile_fes_data.hpp
                src/runfile/runfile_parsed_tree.cpp
                src/runfile/runfile_parsed_tree.hpp
                src/runfile/runfile_reweight.cpp
                src/runfile/runfile_reweight.hpp
                src/common/find_in_search_directory.cpp
                src/common/find_in_search_directory.hpp
                src/common/yaml_utils.cpp
                src/common/yaml_utils.hpp
                src/runfile/runfile_steps_executor.cpp
                src/runfile/runfile_steps_executor.hpp
                src/steps/step_fes_data.cpp
                src/steps/step_fes_data.hpp
                src/frontend_common.cpp
                src/file_reader/cv_file/CVData.cpp
                src/file_reader/cv_file/CVData.hpp
                src/file_reader/cv_file/ingest_cv_data.cpp
                src/file_reader/cv_file/ingest_cv_data.hpp
                src/export/array_bindump.cpp
                src/export/array_bindump.hpp
                src/file_reader/cv_file/CVData_filler_node.cpp
                src/file_reader/cv_file/CVData_filler_node.hpp
                src/file_reader/hills_file/HillsData.cpp
                src/file_reader/hills_file/HillsData.hpp
                src/file_reader/grid_file/GridData.cpp
                src/file_reader/grid_file/GridData.hpp
                src/file_reader/grid_file/ingest_grid_data.cpp
                src/file_reader/grid_file/ingest_grid_data.hpp
                src/common/NotImplementedError.cpp
                src/common/NotImplementedError.hpp
                src/file_reader/grid_file/GridAccessTracker.cpp
                src/file_reader/grid_file/GridAccessTracker.hpp)

set(LIBRARY_LINK_LIST tbb ${Boost_LIBRARIES} fmt yaml-cpp)


add_executable(fesutils src/entrypoint.cpp ${SOURCE_LIST})
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

set(SYSTEM_TEST_FILENAMES_STEMS st_final_bias_reweight st_reconstruct_from_fragments st_load_fes_data st_utils)

foreach(SysTestFile IN LISTS SYSTEM_TEST_FILENAMES_STEMS)
    add_custom_command(
            TARGET fesutils POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/system_tests/${SysTestFile}.py
            ${CMAKE_CURRENT_BINARY_DIR}/${SysTestFile}.py)
endforeach()




enable_testing()

set(TEST_SOURCE_LIST
        src/unit_tests/main_test_suite.cpp
        src/unit_tests/common/extract_from_string_ts.cpp
        src/unit_tests/file_reader/file_header_reader_ts.cpp
        src/unit_tests/file_reader/read_file_ts.cpp
        src/unit_tests/TempTextFile.hpp
        src/unit_tests/TempTextFile.cpp
        src/unit_tests/file_reader/parse_space_separated_values_ts.cpp
        src/unit_tests/grid/grid_utils_ts.cpp
        src/unit_tests/grid/grid_ts.cpp
        src/unit_tests/MiscFixture.cpp
        src/unit_tests/MiscFixture.hpp
        src/unit_tests/common/misc_ts.cpp
        src/unit_tests/grid/grid_filler_node_ts.cpp
        src/unit_tests/progress/progress_printer_ts.cpp
        src/unit_tests/file_reader/field_function_heuristics_ts.cpp
        src/unit_tests/grid/ingest_grid_ts.cpp
        src/unit_tests/TempDirectory.cpp
        src/unit_tests/TempDirectory.hpp
        src/unit_tests/common/find_in_search_directory_ts.cpp
        src/unit_tests/dummy_template_ts.cpp
        src/unit_tests/common/limiter_decrementer_node_ts.cpp
        src/unit_tests/common/yaml_utils_ts.cpp
        src/unit_tests/frontend_common_ts.cpp
        src/unit_tests/general_option_ts.cpp
        src/unit_tests/export/array_bindump_ts.cpp
        src/unit_tests/file_reader/cv_file/CVData_ts.cpp
        src/unit_tests/file_reader/cv_file/CVData_filler_node_ts.cpp
        src/unit_tests/file_reader/cv_file/ingest_cv_data_ts.cpp
        src/unit_tests/file_reader/grid_file/GridData_ts.cpp
        src/unit_tests/progress/progress_printer_node_ts.cpp
        src/unit_tests/file_reader/grid_file/grid_filler_node_ts.cpp
        src/unit_tests/file_reader/grid_file/ingest_grid_data_ts.cpp
        src/unit_tests/common/not_implemented_error_ts.cpp
        src/unit_tests/runfile/runfile_fes_data_ts.cpp
        src/unit_tests/runfile/runfile_general_ts.cpp
        src/unit_tests/runfile/runfile_parsed_tree_ts.cpp
        src/unit_tests/runfile/runfile_reweight_ts.cpp
        src/unit_tests/runfile/runfile_search_directory_ts.cpp
        src/unit_tests/runfile/runfile_steps_ts.cpp
        src/unit_tests/runfile/read_yaml_runfile_ts.cpp
        src/unit_tests/file_reader/grid_file/GridAccessTracker_ts.cpp)


add_executable(complete_test_suite ${TEST_SOURCE_LIST}  ${SOURCE_LIST})
target_compile_definitions(complete_test_suite PUBLIC BOOST_LOG_DYN_LINK)
add_dependencies(complete_test_suite fesutils)
target_link_libraries(complete_test_suite ${LIBRARY_LINK_LIST})

if(NOT SANITIZER_BUILD)
    target_precompile_headers(complete_test_suite
            PUBLIC
            ${PCH_HEADER_LIST}
            ${PCH_SYST_HEADER_LIST}
            )
endif()


target_compile_options(complete_test_suite PRIVATE -Wno-ignored-attributes)

foreach(SysTestFile IN LISTS SYSTEM_TEST_FILENAMES_STEMS)
    add_custom_command(
            TARGET complete_test_suite POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/system_tests/${SysTestFile}.py
            ${CMAKE_CURRENT_BINARY_DIR}/${SysTestFile}.py)
    add_test(NAME fesutils_${SysTestFile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMAND python3 ${SysTestFile}.py)
endforeach()


add_test(fesutils_unittest complete_test_suite --catch_system_error=yes --log_level=all )

