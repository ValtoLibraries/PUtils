set(LUA_PC_CFG "lua;lua5.3;lua5.2;lua5.1" CACHE STRING "pkg-config Lua configuration file (or files, separated by ;)")

find_package(Lua QUIET)

if(LUA_FOUND)
    add_library(lua INTERFACE)
    target_link_libraries(lua INTERFACE ${LUA_LIBRARIES})
    target_include_directories(lua INTERFACE ${LUA_INCLUDE_DIR} .)
endif()
