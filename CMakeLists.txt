set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_LIST_DIR}/CMakeModules)

add_subdirectory(connection)
add_subdirectory(pluginManager)

if(PUTILS_BUILD_MEDIATOR)
    add_subdirectory(mediator)
endif()

if(PUTILS_BUILD_HTTP)
    add_subdirectory(http)
endif()

if(PUTILS_BUILD_LUA)
    add_subdirectory(lua)
endif()

if(PUTILS_BUILD_PYTHON)
    add_subdirectory(python)
endif()

if (PUTILS_BUILD_PSE)
    add_subdirectory(pse)
endif ()

if (PUTILS_BUILD_POGRE)
    add_subdirectory(pogre)
endif ()

file(GLOB src *.hpp)
add_library(putils INTERFACE)
target_include_directories(putils INTERFACE .)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} PARENT_SCOPE)
