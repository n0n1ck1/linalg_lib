 #Include gtest via FetchContent
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



# Macro for creating a test
macro(package_add_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    target_link_libraries(${TEST_NAME} matrix gtest gtest_main)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_target_properties(${TEST_NAME} PROPERTIES FOLDER tests)
endmacro()


file(GLOB TEST_SRC_FILES "test_*.cpp")

# Create test for each file in directory
foreach(TEST_SRC_FILE ${TEST_SRC_FILES})
    get_filename_component(TEST_NAME ${TEST_SRC_FILE} NAME_WE)
    package_add_test(${TEST_NAME} ${TEST_SRC_FILE})
endforeach()

# Create helper executable that runs all tests
add_executable(all_tests ${TEST_SRC_FILES})
target_link_libraries(all_tests matrix gtest gtest_main)
add_test(NAME all_tests COMMAND all_tests)
set_target_properties(all_tests PROPERTIES FOLDER tests)
