cmake_minimum_required(VERSION 3.12)
project(chatstream VERSION 0.1.0 LANGUAGES C CXX)

find_package(RdKafka REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLIB REQUIRED glib-2.0)
include_directories(${GLIB_INCLUDE_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})

file(GLOB SOURCES
    *.h
    *.cpp
)

add_executable(chatstream ${SOURCES})
target_link_libraries(chatstream PRIVATE RdKafka::rdkafka RdKafka::rdkafka++)

add_definitions(${GLIB_CFLAGS_OTHER})
target_link_libraries(chatstream PRIVATE ${GLIB_LIBRARIES})
