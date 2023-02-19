cmake_minimum_required(VERSION 3.2)

set(LIBRARY_NAME "lib_async")

add_library(${LIBRARY_NAME}
    src/async.cpp
    src/context/context_holder.cpp
    src/sinks/sink_holder.cpp
    src/utils/file.cpp
    src/utils/time.cpp
)

target_include_directories(${LIBRARY_NAME} PRIVATE
    src/
    include/
)

set_target_properties(${LIBRARY_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(${LIBRARY_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${LIBRARY_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()
