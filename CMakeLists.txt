cmake_minimum_required(VERSION 3.14)

project(fesutils)

set(CMAKE_CXX_STANDARD 17)

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
        set(COVERAGE_LCOV_EXCLUDES '/usr/*' '*/unit_tests/*' '*/cli_fontend.cpp')
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
                src/file_reader/PlumedDatHeader.hpp
                src/file_reader/read_file.cpp
                src/file_reader/read_file.hpp
                src/common/graph_node_type.hpp
                src/file_reader/read_file_header.cpp
                src/file_reader/read_file_header.hpp src/common/fs_utils.cpp src/common/fs_utils.hpp)

add_executable(fesutils src/cli_frontend.cpp ${SOURCE_LIST})
target_compile_definitions(fesutils PUBLIC BOOST_LOG_DYN_LINK)



target_link_libraries(fesutils ${Boost_LIBRARIES} tbb )



enable_testing()

set(TEST_SOURCE_LIST
        src/unit_tests/main_test_suite.cpp
        src/unit_tests/common/extract_from_string_ts.cpp
        src/unit_tests/file_reader/file_header_reader_ts.cpp
        )


add_executable(complete_test_suite ${TEST_SOURCE_LIST}  ${SOURCE_LIST})
target_compile_definitions(complete_test_suite PUBLIC BOOST_LOG_DYN_LINK)

target_link_libraries(complete_test_suite ${Boost_LIBRARIES} tbb )


add_test(fesutils_unittest complete_test_suite --catch_system_error=yes --log_level=all )
