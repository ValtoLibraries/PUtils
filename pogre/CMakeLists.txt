## Ogre

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR})

if(WIN32)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:\\CMake")
    message("Remember to install SDL2, Ogre and CEGUI")
    message("\tset the OGRE_HOME, CEGUI_HOME and SDL2_DIR environment variables")
    message("\tand add FindOGRE.cmake and all its dependencies to C:\\CMake")
endif()

find_package(OGRE REQUIRED)
find_package(CEGUI QUIET)

if(WIN32)
    foreach(dir in ${OGRE_INCLUDE_DIRS})
        set(OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS} ${dir}/OGRE)
        set(OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS} ${dir}/OGRE/Bites)
        set(OGRE_INCLUDE_DIRS ${OGRE_INCLUDE_DIRS} ${dir}/OGRE/RTShaderSystem)
    endforeach()

    message("Remember to copy all Ogre libraries \(including dependencies such as zlib\) into ${OGRE_PLUGIN_DIR_DBG}")
    file(GLOB ogre_libs
            ${CEGUI_BINARY_REL}
            ${CEGUI_BINARY_DBG}
            ${CEGUI_OgreRenderer_BINARY_REL}
            ${CEGUI_OgreRenderer_BINARY_DBG}
            ${OGRE_PLUGIN_DIR_DBG}/*
            ${OGRE_PLUGIN_DIR_REL}/*)

    file(COPY ${ogre_libs}
            DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    file(COPY ${ogre_libs}
            DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
    file(COPY ${ogre_libs}
            DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
endif()

if(UNIX)
    set(platform unix)
elseif(WIN32)
    set(platform win)
endif()

# Copy my own versions (with CEGUI resources)
file(COPY ${platform}/plugins.cfg ${platform}/resources.cfg resources
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${platform}/plugins.cfg ${platform}/resources.cfg resources
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
file(COPY ${platform}/plugins.cfg ${platform}/resources.cfg resources
        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})

## Pogre

file(GLOB src *.cpp *.hpp)

if(UNIX)
    set(type SHARED)
elseif(WIN32)
    set(type STATIC)
endif()

add_library(pogre ${type} ${src})
target_include_directories(pogre PUBLIC . ${OGRE_INCLUDE_DIRS} SDL2/include)
target_link_libraries(pogre PUBLIC putils
    ${OGRE_LIBRARIES} ${OGRE_Bites_LIBRARIES} ${OGRE_RTShaderSystem_LIBRARIES})

if(CEGUI_FOUND)
    target_include_directories(pogre PUBLIC ${CEGUI_INCLUDE_DIR})
    target_link_libraries(pogre PUBLIC ${CEGUI_LIBRARIES} ${CEGUI_OgreRenderer_LIBRARIES})
    target_compile_definitions(pogre PUBLIC POGRE_CEGUI)
endif()
