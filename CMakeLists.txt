cmake_minimum_required(VERSION 3.23)

set(XMLB_VERSION "0.1")
project(xmlb VERSION ${XMLB_VERSION} LANGUAGES CXX)

set(XMLB_ALIAS "xmlb::xmlb")
set(XMLB_STATIC "xmlb_static")
set(XMLB_SHARED "xmlb_shared")
set(XMLB_STATIC_ALIAS "xmlb::xmlb_static")
set(XMLB_SHARED_ALIAS "xmlb::xmlb_shared")

file(GLOB XMLB_HEADERS *.h)

add_library(${PROJECT_NAME} INTERFACE)
add_library(${XMLB_ALIAS} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ./include/)