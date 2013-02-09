include(FindPkgConfig)
cmake_minimum_required(VERSION 2.8)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify>=0.7.0)

include_directories(${GTKMM_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIRS})
add_definitions(-std=c++0x -DLIBNOTIFY4=${LIBNOTIFY4})
add_executable(tomator tomator.cc states.cc config.cc icons.cc)
target_link_libraries(tomator ${GTKMM_LIBRARIES} ${LIBNOTIFY_LIBRARIES})
