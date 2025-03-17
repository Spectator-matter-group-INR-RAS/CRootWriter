##
# CRootWriter - COLA Library Module for ROOT data storage support.
# Copyright (C) 2025 Savva Savenkov
#
# This file is part of CRootWriter
#
# CRootWriter is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CRootWriter is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with CRootWriter.  If not, see <https://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.22)
project(CRootWriter VERSION 1.1)

set(CMAKE_CXX_STANDARD 17)

find_package(COLA REQUIRED)
find_package(ROOT REQUIRED)

include(CMakePackageConfigHelpers)

set(CMAKE_INSTALL_PREFIX ${COLA_DIR})
set(COLA_MODULE_NAME CRootWriter)

set(srcs CRootWriter/CRootWriterFactory.cc CRootWriter/CRootWriter.cc UniGen/src/UEvent.cxx UniGen/src/UParticle.cxx UniGen/src/URun.cxx
        CUniGenWriter/CUniGenWriter.cc CNativeRootWriter/CNativeRootWriter.cc CAAMCCWriter/CAAMCCWriter.cc)
add_library(CRootWriter SHARED ${srcs})

target_include_directories(CRootWriter PUBLIC 
        "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/CRootWriter;${PROJECT_SOURCE_DIR}/UniGen/include;${PROJECT_SOURCE_DIR}/CUniGenWriter;${PROJECT_SOURCE_DIR}/CNativeRootWriter;${PROJECT_SOURCE_DIR}/CAAMCCWriter>"
        $<INSTALL_INTERFACE:include/${COLA_MODULE_NAME}>)

set_target_properties(CRootWriter PROPERTIES PUBLIC_HEADER "CRootWriter/CRootWriter.hh;CRootWriter/CRootWriterFactory.hh;\
UniGen/include/UEvent.hh;UniGen/include/UParticle.hh;UniGen/include/URun.hh;CUniGenWriter/CUniGenWriter.hh;CNativeRootWriter/CNativeRootWriter.hh;\
CAAMCCWriter/CAAMCCWriter.hh")

# Generate dictionaries for I/O classes
ROOT_GENERATE_DICTIONARY(
        Dict_CRootWriter UniGen/include/URun.hh UniGen/include/UEvent.hh UniGen/include/UParticle.hh
        LINKDEF LinkDef.hh
        MODULE CRootWriter
)

target_link_libraries(CRootWriter PUBLIC COLA ROOT::RIO ${ROOT_LIBRARIES})

# Configure config files
configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${COLA_MODULE_NAME}Config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}Config.cmake"
        INSTALL_DESTINATION lib/cmake/${COLA_MODULE_NAME}
)

write_basic_package_version_file(
        ${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}ConfigVersion.cmake
        COMPATIBILITY AnyNewerVersion)

# Install library
install(TARGETS CRootWriter
        EXPORT CRootWriterExport
        LIBRARY DESTINATION lib/${COLA_MODULE_NAME}
        PUBLIC_HEADER DESTINATION include/${COLA_MODULE_NAME}
        INCLUDES DESTINATION include/${COLA_MODULE_NAME})

# Install export file and config files
install(EXPORT CRootWriterExport
        DESTINATION lib/cmake/${COLA_MODULE_NAME})

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}Config.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/${COLA_MODULE_NAME}ConfigVersion.cmake"
        DESTINATION lib/cmake/${COLA_MODULE_NAME})

# Install ROOT dicts for I/O
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lib${COLA_MODULE_NAME}_rdict.pcm"
        "${CMAKE_CURRENT_BINARY_DIR}/lib${COLA_MODULE_NAME}.rootmap"
        DESTINATION lib/${COLA_MODULE_NAME})