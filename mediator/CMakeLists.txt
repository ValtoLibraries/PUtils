file(GLOB src *.cpp *.hpp)

if(UNIX)
    set(type SHARED)
elseif(WIN32)
    set(type STATIC)
endif()

add_library(mediator ${type} ${src})
target_include_directories(mediator PUBLIC .)
target_link_libraries(mediator putils)
include(cotire)
cotire(mediator)

if (UNIX)
    target_link_libraries(mediator pthread)
endif()
