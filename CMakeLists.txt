cmake_minimum_required(VERSION 3.22)
project(CUniGen VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

find_package(COLA REQUIRED)
find_package(ROOT REQUIRED)

include(CMakePackageConfigHelpers)

set(CMAKE_INSTALL_PREFIX ${COLA_DIR})
set(COLA_MODULE_NAME CUniGen)

set(srcs src/CUniGenFactory.cc src/CUniGenWriter.cc UniGen/src/UEvent.cxx UniGen/src/UParticle.cxx UniGen/src/URun.cxx)
add_library(CUniGen SHARED ${srcs})

target_include_directories(CUniGen PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/UniGen/include>
        $<INSTALL_INTERFACE:include/${COLA_MODULE_NAME}>)

set_target_properties(CUniGen PROPERTIES PUBLIC_HEADER "include/CUniGen.hh;include/CUniGenFactory.hh;include/CUniGenWriter.hh;\
UniGen/include/UEvent.hh;UniGen/include/UParticle.hh;UniGen/include/URun.hh")

# Generate dictionaries for I/O classes
ROOT_GENERATE_DICTIONARY(
        Dict_CUniGen UniGen/include/URun.hh UniGen/include/UEvent.hh UniGen/include/UParticle.hh
        LINKDEF UniGen/include/LinkDef.hh
        MODULE CUniGen
)

target_link_libraries(CUniGen PUBLIC COLA ROOT::RIO ${ROOT_LIBRARIES})

# Configure config files
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/data/${COLA_MODULE_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${COLA_MODULE_NAME}
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}ConfigVersion.cmake
        COMPATIBILITY AnyNewerVersion)

# Install library
install(TARGETS CUniGen
        EXPORT CUniGenExport
        LIBRARY DESTINATION lib/${COLA_MODULE_NAME}
        PUBLIC_HEADER DESTINATION include/${COLA_MODULE_NAME}
        INCLUDES DESTINATION include/${COLA_MODULE_NAME})

# Install export file and config files
install(EXPORT CUniGenExport
        DESTINATION lib/cmake/${COLA_MODULE_NAME})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}ConfigVersion.cmake"
        DESTINATION lib/cmake/${COLA_MODULE_NAME})

# Install ROOT dicts for I/O
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lib${COLA_MODULE_NAME}_rdict.pcm"
        "${CMAKE_CURRENT_BINARY_DIR}/lib${COLA_MODULE_NAME}.rootmap"
        DESTINATION lib/${COLA_MODULE_NAME})