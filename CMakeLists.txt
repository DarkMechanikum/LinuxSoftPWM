cmake_minimum_required(VERSION 3.10)
project(linuxsoftpwm VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Create the library
add_library(linuxsoftpwm SHARED ${CMAKE_CURRENT_SOURCE_DIR}/src/linuxsoftpwm.cpp)

# Optional: Define include directory for external use
target_include_directories(linuxsoftpwm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}../)

# Link libgpiod
find_library(GPIOD_LIBRARY gpiod REQUIRED)
target_link_libraries(linuxsoftpwm PRIVATE ${GPIOD_LIBRARY})

# Install headers and library
install(TARGETS linuxsoftpwm
        EXPORT linuxsoftpwmTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include)

install(FILES linuxsoftpwm.h
        DESTINATION include)

# Export targets for downstream use
install(EXPORT linuxsoftpwmTargets
        FILE linuxsoftpwmTargets.cmake
        NAMESPACE linuxsoftpwm::
        DESTINATION lib/cmake/linuxsoftpwm)

# Create and install the version file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/linuxsoftpwmConfigVersion.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/linuxsoftpwmConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/linuxsoftpwmConfig.cmake"
        INSTALL_DESTINATION lib/cmake/linuxsoftpwm
)

install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/linuxsoftpwmConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/linuxsoftpwmConfigVersion.cmake"
        DESTINATION lib/cmake/linuxsoftpwm
)

# Export from the build tree
export(EXPORT linuxsoftpwmTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/linuxsoftpwmTargets.cmake"
        NAMESPACE linuxsoftpwm::)

export(PACKAGE linuxsoftpwm)
