cmake_minimum_required(VERSION 3.9)
#set(CMAKE_SUPPRESS_REGENERATION true)

project(DesignPatterns)

if (UNIX)
    SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fPIC -m64 -fvisibility=hidden")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -m64 -fvisibility=hidden -std=c++17")
endif (UNIX)

add_definitions(-Wall)
#add_definitions(-W4)

add_subdirectory(Factory)

#Visual Studio file filters
source_group(""    FILES ${SRC})
source_group("ref" FILES ${SRC_REF})

#set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Factory)
#install(TARGETS Factory DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/inst/")
