if(UNIX)
    set(type SHARED)
elseif(WIN32)
    set(type STATIC)
endif()

file(GLOB src lua/src/*)
add_library(lua ${type} ${src})
target_include_directories(lua PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/lua/src)
