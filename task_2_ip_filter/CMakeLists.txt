cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(ip_filter VERSION ${PROJECT_VERSION})

option(WITH_BOOST_TEST "Whether to build Boost test" ON)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/parser.cpp
    src/utils.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

if(WITH_BOOST_TEST)
    set(TEST_SUITE_NAME test_ip_filter)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(${TEST_SUITE_NAME}
        src/parser.cpp
        src/utils.cpp
        tests/test_main.cpp
        tests/test_parser.cpp
        tests/test_utils.cpp
    )

    set_target_properties(${TEST_SUITE_NAME} PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(${TEST_SUITE_NAME} PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )

    target_include_directories(${TEST_SUITE_NAME}
        PRIVATE src/
    )
endif()

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4
    )
    if(WITH_BOOST_TEST)
        target_compile_options(${TEST_SUITE_NAME} PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_BOOST_TEST)
        target_compile_options(${TEST_SUITE_NAME} PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_PACKAGE_CONTACT @lilSpeedwagon)
include(CPack)

if(WITH_BOOST_TEST)
    add_test(test_suite ${TEST_SUITE_NAME})
    enable_testing()
endif()
