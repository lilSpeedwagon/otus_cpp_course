cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(LIB_VERSION 0.0.${PATCH_VERSION})

project(lib_version VERSION ${LIB_VERSION})

configure_file(src/version_consts.hpp.in src/version_consts.hpp)

add_library(lib_version src/version.cpp)

target_include_directories(lib_version
    PRIVATE src/ include/ "${CMAKE_BINARY_DIR}"
)

if(WITH_BOOST_TEST)
    find_package(Boost COMPONENTS unit_test_framework REQUIRED)
    add_executable(test_version src/test_version.cpp)

    set_target_properties(test_version PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED ON
    )

    set_target_properties(test_version PROPERTIES
        COMPILE_DEFINITIONS BOOST_TEST_DYN_LINK
        INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR}
    )

    target_link_libraries(test_version
        ${Boost_LIBRARIES}
        lib_version
    )

    target_include_directories(test_version
        PRIVATE include/ "${CMAKE_BINARY_DIR}"
    )
endif()

if (MSVC)
    target_compile_options(lib_version PRIVATE
        /W4
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            /W4
        )
    endif()
else ()
    target_compile_options(lib_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    if(WITH_BOOST_TEST)
        target_compile_options(test_version PRIVATE
            -Wall -Wextra -pedantic -Werror
        )
    endif()
endif()

if(WITH_BOOST_TEST)
    add_test(test_version test_version)
endif()
