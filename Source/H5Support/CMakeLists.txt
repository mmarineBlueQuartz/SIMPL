# ============================================================================
# Copyright (c) 2009-2015 BlueQuartz Software, LLC
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# Redistributions of source code must retain the above copyright notice, this
# list of conditions and the following disclaimer.
#
# Redistributions in binary form must reproduce the above copyright notice, this
# list of conditions and the following disclaimer in the documentation and/or
# other materials provided with the distribution.
#
# Neither the name of BlueQuartz Software, the US Air Force, nor the names of its
# contributors may be used to endorse or promote products derived from this software
# without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# The code contained herein was partially funded by the followig contracts:
#    United States Air Force Prime Contract FA8650-07-D-5800
#    United States Air Force Prime Contract FA8650-10-D-5210
#    United States Prime Contract Navy N00173-07-C-2068
#
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

# set project's name
PROJECT( H5Support VERSION ${SIMPLProj_VERSION_MAJOR}.${SIMPLProj_VERSION_MINOR})

set(PROJECT_CODE_DIR ${PROJECT_SOURCE_DIR}/Source)
if(NOT PROJECT_RESOURCES_DIR)
  set(PROJECT_RESOURCES_DIR ${H5Support_SOURCE_DIR}/Resources)
endif()
if(NOT DEFINED CMP_SOURCE_DIR)
  set(CMP_SOURCE_DIR ${H5Support_SOURCE_DIR}/Support/cmp)
endif()

set(PROJECT_PREFIX "H5Support" CACHE STRING "The Prefix to be used for Preprocessor definitions")
set(H5Support_INSTALL_FILES 0)
# -----------------------------------------------------------------------
# The project generally does NOT want to install headers. See if another
# project has over ridden that property
get_property(CMP_PROJECT_INSTALL_HEADERS GLOBAL PROPERTY CMP_PROJECT_INSTALL_HEADERS)
if("${CMP_PROJECT_INSTALL_HEADERS}" STREQUAL "")
  set_property(GLOBAL PROPERTY CMP_PROJECT_INSTALL_HEADERS "0")
  get_property(CMP_PROJECT_INSTALL_HEADERS GLOBAL PROPERTY CMP_PROJECT_INSTALL_HEADERS)
endif()
set(H5Support_INSTALL_FILES "${CMP_PROJECT_INSTALL_HEADERS}")

set(H5Support_BUILT_AS_DYNAMIC_LIB)
if(BUILD_SHARED_LIBS)
    set(H5Support_BUILT_AS_DYNAMIC_LIB 1)
endif(BUILD_SHARED_LIBS)

#-------------------------------------------------------------------------------
# Include the cmp project to perform all the basic configuration tests for the
# project.
#-------------------------------------------------------------------------------
set(CMP_HEADER_DIR ${${PROJECT_NAME}_BINARY_DIR})
set(CMP_CONFIGURATION_FILE_NAME "${PROJECT_NAME}Configuration.h")
set(CMP_EXPORT_FILE_NAME "${PROJECT_NAME}DLLExport.h")
set(CMP_TYPES_FILE_NAME "${PROJECT_NAME}Types.h")
set(CMP_VERSION_HEADER_FILE_NAME "${PROJECT_NAME}Version.h")
set(CMP_VERSION_SOURCE_FILE_NAME "${PROJECT_NAME}Version.cpp")
set(CMP_TOP_HEADER_FILE "${PROJECT_NAME}.h")
set(CMP_PROJECT_NAMESPACE "${PROJECT_NAME}")
set(CMP_PROJECT_NAME "${PROJECT_NAME}")
set(CMP_ENABLE_PLUGINS "0")
set(CMP_LIB_SEARCH_DIRS "")
set(CMP_GENERATE_VERSION_STRING 1)
set(PROJECT_INSTALL_HEADERS "${CMP_PROJECT_INSTALL_HEADERS}")

INCLUDE (${CMP_SOURCE_DIR}/cmpProject.cmake)

#-------------------------------------------------------------------------------
# Set our H5Lite codes to also compile the Qt API version of H5Lite
#-------------------------------------------------------------------------------
set(H5Support_USE_QT 0)
option(H5Support_INCLUDE_QT_API "Include support for using Qt classes with H5Lite" ON)
if(H5Support_INCLUDE_QT_API)
  set(H5Support_USE_QT 1)
endif()
MARK_AS_ADVANCED(H5Support_INCLUDE_QT_API)
MARK_AS_ADVANCED(H5Support_USE_QT)

set(H5Support_SRCS
    ${H5Support_SOURCE_DIR}/H5Lite.cpp
    ${H5Support_SOURCE_DIR}/H5Utilities.cpp
    ${H5Support_SOURCE_DIR}/H5ScopedSentinel.cpp
    ${H5Support_SOURCE_DIR}/H5ScopedErrorHandler.cpp
  )

set(H5Support_HDRS
    ${H5Support_SOURCE_DIR}/H5Lite.h
    ${H5Support_SOURCE_DIR}/H5Utilities.h
    ${H5Support_SOURCE_DIR}/H5ScopedSentinel.h
    ${H5Support_SOURCE_DIR}/H5ScopedErrorHandler.h
    ${H5Support_SOURCE_DIR}/H5Macros.h
    ${H5Support_SOURCE_DIR}/H5SupportDLLExport.h
)

if(H5Support_USE_QT)
  set(H5Support_SRCS
    ${H5Support_SRCS}
    ${H5Support_SOURCE_DIR}/QH5Lite.cpp
    ${H5Support_SOURCE_DIR}/QH5Utilities.cpp
  )
  set(H5Support_HDRS
    ${H5Support_HDRS}
    ${H5Support_SOURCE_DIR}/QH5Lite.h
    ${H5Support_SOURCE_DIR}/QH5Utilities.h
  )
endif()


#---------------------------------
# Now configure the main header file.
configure_file(${H5Support_SOURCE_DIR}/H5SupportConfiguration.h.in
               ${H5Support_BINARY_DIR}/${CMP_TOP_HEADER_FILE})

cmp_IDE_SOURCE_PROPERTIES( "" "${H5Support_HDRS}" "${H5Support_SRCS}" ${PROJECT_INSTALL_HEADERS})

# --------------------------------------------------------------------
# Generate a Header file with Compile Version variables
# --------------------------------------------------------------------
# set(VERSION_GEN_NAME "H5Support")
# set(VERSION_GEN_NAMESPACE "H5Support")
# set(PROJECT_PREFIX "H5Support")
# set(VERSION_GEN_COMPLETE  "1.0.0")
# set(VERSION_GEN_VER_MAJOR "1")
# set(VERSION_GEN_VER_MINOR "0")
# set(VERSION_GEN_VER_PATCH "0")
# set(VERSION_GEN_HEADER_FILE_NAME "H5SupportVersion.h")

# configure_file(${CMP_CONFIGURED_FILES_SOURCE_DIR}/cmpVersion.h.in
#                ${H5Support_BINARY_DIR}/${PROJECT_NAME}/H5SupportVersion.h  )
# configure_file(${CMP_CONFIGURED_FILES_SOURCE_DIR}/cmpVersion.cpp.in
#                ${H5Support_BINARY_DIR}/${PROJECT_NAME}/H5SupportVersion.cpp  )


set(PROJECT_SRCS
    ${H5Support_SRCS}
    ${H5Support_HDRS}
    ${H5Support_BINARY_DIR}/${CMP_VERSION_SOURCE_FILE_NAME}
)

add_library(${PROJECT_NAME} ${LIB_TYPE} ${PROJECT_SRCS})
CMP_AddDefinitions(TARGET ${PROJECT_NAME})
LibraryProperties( ${PROJECT_NAME}  ${EXE_DEBUG_EXTENSION} )
CMP_MODULE_INCLUDE_DIRS (TARGET ${PROJECT_NAME} LIBVARS HDF5 Qt5Core)

get_filename_component(TARGET_BINARY_DIR_PARENT ${H5Support_BINARY_DIR} PATH)
get_filename_component(TARGET_SOURCE_DIR_PARENT ${H5Support_SOURCE_DIR} PATH)


TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC
                              $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>
                              $<BUILD_INTERFACE:${TARGET_SOURCE_DIR_PARENT}>
                              $<BUILD_INTERFACE:${TARGET_BINARY_DIR_PARENT}>
                              )

if(BUILD_SHARED_LIBS)
  if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PUBLIC "-DH5Support_BUILT_AS_DYNAMIC_LIB")
  endif(WIN32)
endif(BUILD_SHARED_LIBS)
target_link_libraries(${PROJECT_NAME} ${HDF5_C_TARGET_NAME} Qt5::Core )


set(install_dir "bin")
set(lib_install_dir "lib")

if(APPLE)
  get_property(DREAM3D_PACKAGE_DEST_PREFIX GLOBAL PROPERTY DREAM3D_PACKAGE_DEST_PREFIX)
  set(install_dir "${DREAM3D_PACKAGE_DEST_PREFIX}bin")
  set(lib_install_dir "${DREAM3D_PACKAGE_DEST_PREFIX}lib")
elseif(WIN32)
  set(install_dir ".")
  set(lib_install_dir ".")
endif()


INSTALL(TARGETS ${PROJECT_NAME}
    EXPORT H5SupportTargets
    COMPONENT Applications
    RUNTIME DESTINATION ${install_dir}
    LIBRARY DESTINATION ${lib_install_dir}
    ARCHIVE DESTINATION lib
    BUNDLE DESTINATION "."
)


# --------------------------------------------------------------------
# Allow the generation and installation of a CMake configuration file
# which makes using SIMPL from another project easier.
# --------------------------------------------------------------------
export(EXPORT H5SupportTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}Targets.cmake"
  NAMESPACE H5Support::
)

if (H5Support_INSTALL_FILES)
  install(EXPORT H5SupportTargets
    FILE
      ${PROJECT_NAME}Targets.cmake
    NAMESPACE
      ${PROJECT_NAME}::
    DESTINATION
      ${ConfigPackageLocation}
  )

  install(FILES 
      ${CMP_HEADER_DIR}/${CMP_TOP_HEADER_FILE}
      ${CMP_HEADER_DIR}/${CMP_TYPES_FILE_NAME}
      ${CMP_HEADER_DIR}/${CMP_CONFIGURATION_FILE_NAME}
      ${CMP_HEADER_DIR}/${CMP_VERSION_HEADER_FILE_NAME}
    DESTINATION
      include/${PROJECT_NAME}
  )

endif()


# ------- Enable the CTest testing. ---------------
# -------------------------------------------------------------------- 
# If Testing is enabled, turn on the Unit Tests 
if(SIMPL_BUILD_TESTING)
    include(${H5Support_SOURCE_DIR}/Test/CMakeLists.txt)
endif()


