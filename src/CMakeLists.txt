# Set ES2-GSTTS source files.
# Source file list is used by the shared and static libraries.
# Please add any additions alphabetically to avoid duplication and make
# finding things easier for future maintainers.

set(
        estts-testbed-src
        fapi_rx/eps.cpp
        fapi_rx/telemetry_handler.cpp
        socket_handler.cpp
        sapi_rx/frame_constructor.cpp
        sapi_rx/frame_destructor.cpp
        sapi_rx/info_field.cpp
        utils/helper.cpp
)

set(
        include_directories
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/fapi_rx
        ${CMAKE_CURRENT_SOURCE_DIR}/sapi_rx
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
)

set(estts_include_directories ${include_directories} PARENT_SCOPE)

# Configure build targets
add_library(
        estts-testbed
        SHARED
        ${estts-testbed-src}
)
target_include_directories(
        estts-testbed
        PUBLIC
        ${include_directories}
)

target_link_libraries(estts-testbed PRIVATE spdlog::spdlog)

if (BUILD_DEBUG)
    target_compile_definitions(
            estts-testbed
            PRIVATE
    )
    target_compile_options(
            estts-testbed
            PRIVATE
            -Wall
    )
else ()
    target_compile_definitions(
            estts-testbed
            PRIVATE
    )
    target_compile_options(
            estts-testbed
            PRIVATE
            -w
    )
endif ()