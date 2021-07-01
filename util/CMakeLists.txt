add_library(util SHARED timeout_guard.cpp)
target_include_directories(util PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
