include(FindPkgConfig)
cmake_minimum_required(VERSION 2.8)
pkg_search_module(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
add_executable(tomator tomator.cc)
target_link_libraries(tomator ${GTKMM_LIBRARIES})
