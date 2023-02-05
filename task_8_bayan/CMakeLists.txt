cmake_minimum_required(VERSION 3.2)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})
set(OPENSSL_USE_STATIC_LIBS TRUE)

project(bayan VERSION ${PROJECT_VERSION})

find_package(Boost COMPONENTS system filesystem regex program_options REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(${PROJECT_NAME}
    src/file_comparator/file_comparator.cpp
    src/file_reader/factory.cpp
    src/file_reader/file_reader.cpp
    src/file_scanner/file_scanner.cpp
    src/hash/default.cpp
    src/hash/factory.cpp
    src/hash/rs.cpp
    src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    src/
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

target_link_libraries(${PROJECT_NAME} 
    Boost::filesystem
    Boost::program_options
    Boost::regex
    OpenSSL::Crypto
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
