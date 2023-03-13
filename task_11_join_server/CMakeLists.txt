cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VERSION})

include(FetchContent)
FetchContent_Declare(sqlite3 URL "https://www.sqlite.org/snapshot/sqlite-snapshot-202205041843.tar.gz")
FetchContent_MakeAvailable(sqlite3)
if (NOT sqlite3_POPULATED)
  FetchContent_Populate(sqlite3)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    src/logging/logger.cpp
    src/tcp/server.cpp
    src/tcp/session.cpp
    src/utils/time.cpp
    ${sqlite3_SOURCE_DIR}/sqlite3.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src
    ${sqlite3_SOURCE_DIR}
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
