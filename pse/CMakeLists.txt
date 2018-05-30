# SFML

set(SFML_ROOT ${CMAKE_CURRENT_LIST_DIR}/SFML)

set(BUILD_SHARED_LIBS true)
add_subdirectory(${SFML_ROOT})

set(SFML_INCLUDE_DIR ${SFML_ROOT}/include)

#
# SFML linking for SFML Engine
#

#if (UNIX)
	set(SFML_LIBRARIES sfml-graphics sfml-window sfml-system)
#endif (UNIX)

#if (WIN32)
#    set(SFML_LIBRARIES
#            debug sfml-graphics-d
#            debug sfml-window-d
#            debug sfml-system-d
#            optimized sfml-graphics
#            optimized sfml-window
#            optimized sfml-system
#            )
#endif (WIN32)

set(TGUI_DIR TGUI-0.7.7)

if (TGUI)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DPSE_TGUI" PARENT_SCOPE)

    set(SFML_ROOT "${CMAKE_CURRENT_LIST_DIR}/SFML")
    add_subdirectory(${TGUI_DIR})
    FILE(COPY ${TGUI_DIR}/widgets DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    FILE(COPY ${TGUI_DIR}/fonts DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    set(SFML_LIBRARIES tgui ${SFML_LIBRARIES})
endif (TGUI)

#
# SFML Engine
#

file(GLOB pseSrc
        imgui/imgui.cpp imgui/imgui_draw.cpp
        imgui/imgui-SFML.cpp imgui/imgui_demo.cpp
        *.cpp *.hpp)

if (UNIX)
    set(type SHARED)
elseif (WIN32)
    set(type STATIC)
endif ()

add_library(pse ${type} ${pseSrc})
target_include_directories(pse PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${SFML_INCLUDE_DIR} imgui)
target_compile_definitions(pse PUBLIC PSE_TGUI)
target_link_libraries(pse putils ${SFML_LIBRARIES})

if (TGUI)
    target_include_directories(pse PUBLIC ${TGUI_DIR}/include)
endif()
