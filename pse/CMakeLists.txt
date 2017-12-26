# SFML

set(SFML_ROOT ${CMAKE_CURRENT_LIST_DIR}/SFML)

set(BUILD_SHARED_LIBS true)
add_subdirectory(${SFML_ROOT})

set(SFML_INCLUDE_DIR ${SFML_ROOT}/include)

#
# SFML linking for SFML Engine
#

if (UNIX)
    set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)
endif (UNIX)

if (WIN32)
    set(SFML_LIBRARIES
            debug sfml-graphics-d
            debug sfml-window-d
            debug sfml-system-d
            optimized sfml-graphics
            optimized sfml-window
            optimized sfml-system
            )
endif (WIN32)

if (TGUI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPSE_TGUI" PARENT_SCOPE)
    add_definitions(-DPSE_TGUI)

    add_subdirectory(TGUI-0.7.2)
    FILE(COPY TGUI-0.7.2/widgets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    FILE(COPY TGUI-0.7.2/fonts DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    if (UNIX)
        set(SFML_LIBRARIES tgui ${SFML_LIBRARIES})
    endif (UNIX)

    if (WIN32)
        set(SFML_LIBRARIES debug tgui-d optimized tgui ${SFML_LIBRARIES})
    endif (WIN32)
endif (TGUI)

#
# SFML Engine
#

file(GLOB pseSrc
        *.cpp *.hpp)

if (UNIX)
    set(type SHARED)
elseif (WIN32)
    set(type STATIC)
endif ()

add_library(pse ${type} ${pseSrc})
target_include_directories(pse PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${SFML_INCLUDE_DIR})
target_link_libraries(pse putils ${SFML_LIBRARIES})
