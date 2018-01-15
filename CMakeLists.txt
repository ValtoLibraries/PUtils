set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/CMakeModules)

add_subdirectory(mediator)
add_subdirectory(connection)
add_subdirectory(pluginManager)
add_subdirectory(http)
add_subdirectory(lua)

if (PUTILS_BUILD_PSE)
    add_subdirectory(pse)
endif ()

if (PUTILS_BUILD_POGRE)
    add_subdirectory(pogre)
endif ()

file(GLOB src *.hpp)
add_library(putils INTERFACE)
target_include_directories(putils INTERFACE .)

if (PUTILS_TEST)
    set(test_name putils_test)
    add_subdirectory(googletest)
    file(GLOB testSrc
            test/*.cpp test/*.hpp
            test/*/*.cpp test/*/*.hpp)
    add_executable(${test_name} ${testSrc})
    target_link_libraries(${test_name} putils mediator connection pluginManager gtest_main)

    if (CMAKE_COMPILER_IS_GNUCXX)
        include(CodeCoverage)
        append_coverage_compiler_flags()
        find_program(lcov_found NAMES lcov)
        if (lcov_found)
            setup_target_for_coverage(NAME ${test_name}_coverage
                    EXECUTABLE ${test_name}
                    coverage)
        endif()
    endif ()
endif ()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
