cmake_minimum_required(VERSION 3.23)
project(XMLB LANGUAGES CXX)

add_library(XMLB INTERFACE)
target_include_directories(XMLB INTERFACE .)




#add_library(XMLB::XMLB ALIAS XMLB)