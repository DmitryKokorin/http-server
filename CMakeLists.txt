cmake_minimum_required (VERSION 2.8)

set (PROJECT http-server)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set (SOURCES
    main.cpp
    util.cpp
    options.cpp
    http.cpp
)

set (HEAEDRS
    util.hpp
    options.hpp
    http.hpp
)


add_executable (${PROJECT} ${SOURCES} ${HEADERS})
