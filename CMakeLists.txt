cmake_minimum_required(VERSION 3.8)
project(parallel-cpp)


if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CLANG 1)
endif()

if (ASAN)
    if(NOT CLANG)
        message(FATAL_ERROR "Cannot enable ASAN unless using Clang")
    endif()
    message("Building with address sanitizer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize-address-use-after-scope -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    add_definitions(-DNO_TIMEOUT)
endif()

if (TSAN)
    if(NOT CLANG)
        message(FATAL_ERROR "Cannot enable TSAN unless using Clang")
    endif()
    message("Building with thread sanitizer")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=thread")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=thread")
    add_definitions(-DNO_TIMEOUT)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)
set(CMAKE_CXX_STANDARD             17)

function(register_test test_name)
    FILE(GLOB_RECURSE sources *.cpp)
    add_executable(${test_name} ${sources})
    target_link_libraries(${test_name} gtest_main util)
    include(GoogleTest)
    gtest_discover_tests(${test_name})
endfunction()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.11.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
if (WIN32)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(util)

# PS 0
add_subdirectory(is_prime)

# PS 1
add_subdirectory(hello_world)
add_subdirectory(reduce)
add_subdirectory(deadlock)
add_subdirectory(philosophers)

# PS 2
add_subdirectory(thread_safe_queue)
add_subdirectory(thread_safe_vector)
add_subdirectory(unbuffered_channel)

# PS 3
add_subdirectory(spin_lock)
add_subdirectory(rw_spin_lock)
add_subdirectory(ticket_lock)
add_subdirectory(mcs_lock)

# PS 4
add_subdirectory(mpsc_queue)
add_subdirectory(mpmc_queue)

