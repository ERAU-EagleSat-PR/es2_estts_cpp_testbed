#
# estts-demo1 build configuration.
#

# Add primary executable for estts-demo1
add_executable(testbed-config-test src/main.cpp)

target_include_directories(
        testbed-config-test
        PUBLIC
        ${estts_include_directories}
)

# Link GSTTS library to build
target_link_libraries(testbed-config-test PUBLIC estts-testbed)