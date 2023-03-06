cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(OPENSSL_USE_STATIC_LIBS TRUE)

project(bulk_server VERSION ${PROJECT_VERSION})

find_package(Boost REQUIRED)

add_executable(${PROJECT_NAME}
    src/logging/logger.cpp
    src/handlers/handle_commands.cpp
    src/main.cpp
    src/tcp/tcp_session.cpp
    src/tcp/tcp_server.cpp
    src/sinks/sink_holder.cpp
    src/utils/file.cpp
    src/utils/time.cpp
    src/workers/workers.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
    )
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT @lilSpeedwagon)
include(CPack)
