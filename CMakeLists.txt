####
# VGMTrans
# cmake settings
####
cmake_minimum_required(VERSION 3.5.0)
project(vgmtrans)

############################################
# Compilers and platform-specific settings
####

# Libraries we have to link against
set(LIBS)
# We don't really need this right now,
# but we probably will in the future..
set(CMAKE_CXX_STANDARD 14)

# ...so check for a compiler that implements it
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND
  CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5.0)
  message(FATAL_ERROR "GCC >= 5.0 required (we've found \
                        ${CMAKE_CXX_COMPILER_VERSION})")

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND
  CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.4)
  message(FATAL_ERROR "Clang >= 3.4 required (we've found \
                        ${CMAKE_CXX_COMPILER_VERSION})")
endif()

# Set platform-specific flags
if(CMAKE_C_COMPILER_ID MATCHES "MSVC") # Untested
  add_compile_options(/permissive-)
  add_compile_options(/Zc:inline)
  add_compile_options(/Zc:throwingNew)
  add_compile_options(/volatile:iso)

  string(APPEND CMAKE_EXE_LINKER_FLAGS " /NXCOMPAT")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -Wtype-limits \
  -Wsign-compare -Wuninitialized")
  set(CMAKE_CXX_FLAGS_RELEASE "-w -Os -fomit-frame-pointer")

  # Stripping on OS X causes compilation to fail
  if(NOT APPLE)
    string(APPEND CMAKE_CXX_FLAGS_RELEASE " -s")
    if(NOT WIN32) # FIXME: zlib doesn't link under MinGW...
      string(APPEND CMAKE_CXX_FLAGS_RELEASE " -flto")
    endif()
  endif()
endif()

# Build for 'Release' unless specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
      "Build type (Release/Debug)" FORCE)
endif()

if(APPLE)
  set(CMAKE_OSX_ARCHITECTURES "x86_64")
  set(CMAKE_LIBRARY_ARCHITECTURE "x86_64-apple-darwin11")
  set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9")
  set(GUI_TYPE MACOSX_BUNDLE)
endif()

if(WIN32)
  set(CMAKE_LIBRARY_ARCHITECTURE "x86_64-w64-mingw32")
  string(APPEND CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ ")
  set(GUI_TYPE WIN32)
endif()

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH}
    "${CMAKE_SOURCE_DIR}/contrib/${CMAKE_LIBRARY_ARCHITECTURE}")
message(STATUS "PREFIX PATH: " ${CMAKE_PREFIX_PATH})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

##################################
# Dependency management

#TODO: Remove GLOB. It's compact, but not great.
file(GLOB_RECURSE MAIN_FILES
  "${PROJECT_SOURCE_DIR}/src/main/*.cpp" "${PROJECT_SOURCE_DIR}/src/main/*.h")
file(GLOB UI_FILES
  "${PROJECT_SOURCE_DIR}/src/ui/qt/*.cpp" "${PROJECT_SOURCE_DIR}/src/ui/qt/*.h")
file(GLOB_RECURSE LIB_FILES
  "${PROJECT_SOURCE_DIR}/lib/fluidsynth/mem_sfloader/*.c"
  "${PROJECT_SOURCE_DIR}/lib/fluidsynth/mem_sfloader/*.h"
  "${PROJECT_SOURCE_DIR}/lib/tinyxml/*.cpp"
  "${PROJECT_SOURCE_DIR}/lib/fluidsynth/*.h"
  "${PROJECT_SOURCE_DIR}/lib/minizip/*.c"
  "${PROJECT_SOURCE_DIR}/lib/minizip/*.h"
)

set(INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/src/main
  ${PROJECT_SOURCE_DIR}/src/main/formats
  ${PROJECT_SOURCE_DIR}/src/main/loaders
  ${PROJECT_SOURCE_DIR}/src/ui/qt
  ${PROJECT_SOURCE_DIR}/contrib/${CMAKE_LIBRARY_ARCHITECTURE}/include
  ${PROJECT_SOURCE_DIR}/contrib/${CMAKE_LIBRARY_ARCHITECTURE}/include/glib-2.0
  ${PROJECT_SOURCE_DIR}/contrib/${CMAKE_LIBRARY_ARCHITECTURE}/include/fluidsynth
  ${PROJECT_SOURCE_DIR}/lib/zlib/include
  ${PROJECT_SOURCE_DIR}/lib/tinyxml
  ${PROJECT_SOURCE_DIR}/lib/fluidsynth/mem_sfloader
)

set(RESOURCE_PATH
  "${PROJECT_SOURCE_DIR}/src/ui/qt/resources/resources.qrc"
  "${PROJECT_SOURCE_DIR}/src/ui/qt/resources/qdarkstyle/style.qrc"
)

source_group("ui" REGULAR_EXPRESSION .*src/ui/qt/.*)
source_group("main\\formats" REGULAR_EXPRESSION .*src/main/formats/.*)
source_group("main\\loaders" REGULAR_EXPRESSION .*src/main/loaders/.*)
source_group("main" REGULAR_EXPRESSION .*src/main/.*)
source_group("lib\\fluidsynth" REGULAR_EXPRESSION .*lib/fluidsynth/.*)
source_group("lib\\tinyxml" REGULAR_EXPRESSION .*lib/tinyxml/.*)
source_group("lib\\minizip" REGULAR_EXPRESSION .*lib/minizip/.*)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${INCLUDE_DIRS})

# Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
set_property(TARGET Qt5::Core PROPERTY INTERFACE_COMPILE_FEATURES "")
message(STATUS "Found Qt version ${Qt5Core_VERSION}")

set(CMAKE_AUTOMOC ON)
add_definitions(${Qt5Widgets_DEFINITIONS})
qt5_add_resources(UI_RESOURCES ${RESOURCE_PATH})

# Use the Widgets module from Qt 5.
list(APPEND LIBS Qt5::Core Qt5::Widgets)

find_package(ZLIB REQUIRED)
include_directories(${ZLIB_INCLUDE_DIRS})
list(APPEND LIBS ${ZLIB_LIBRARIES})

find_package(FluidSynth REQUIRED)
list(APPEND LIBS ${FLUIDSYNTH_LIBRARIES})

find_package(GLIB REQUIRED)
list(APPEND LIBS ${GLIB_LIBRARIES})

add_definitions(-DTIXML_USE_STL)

if(WIN32 OR APPLE)
  find_library(QTPCRE_LIBRARY qtpcre)
  list(APPEND LIBS ${QTPCRE_LIBRARY})

  find_library(INTL_LIBRARY NAMES intl)
  list(APPEND LIBS ${INTL_LIBRARY})

  find_library(ICONV_LIBRARIES NAMES iconv)
  list(APPEND LIBS ${ICONV_LIBRARIES})
endif()

if(WIN32)
  set_property(TARGET Qt5::Core APPEND PROPERTY
              INTERFACE_LINK_LIBRARIES ws2_32)
  find_library(QWINDOWS_LIBRARY qwindows)
  list(APPEND LIBS
              ${QWINDOWS_LIBRARY}

              imm32.lib
              winmm.lib
              dsound.lib)

  add_definitions(-DFLUIDSYNTH_NOT_A_DLL -DG_OS_WIN32)
endif()

if(APPLE)
  find_library(IOKIT_LIBRARY IOKit)
  find_library(APPKIT_LIBRARY AppKit)
  find_library(CARBON_LIBRARY Carbon)
  find_library(COREMIDI_LIBRARY CoreMIDI)
  find_library(COREAUDIO_LIBRARY CoreAudio)
  find_library(AUDIOUNIT_LIBRARY AudioUnit)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)
  find_library(CORESERVICES_LIBRARY CoreServices)

  find_library(QCOCOA_LIBRARY qcocoa)
  find_library(QT5PLATFORMSUPPORT_LIBRARY Qt5PlatformSupport)

  list(APPEND LIBS
              ${QCOCOA_LIBRARY}
              ${APPKIT_LIBRARY}
              ${CARBON_LIBRARY}
              ${COREMIDI_LIBRARY}
              ${COREAUDIO_LIBRARY}
              ${AUDIOUNIT_LIBRARY}
              ${COREFOUNDATION_LIBRARY}
              ${CORESERVICES_LIBRARY}

              ${QT5PLATFORMSUPPORT_LIBRARY}
              ${IOKIT_LIBRARY}
      )
endif()

# Project target

set(APP_BINARY vgmtrans)

add_executable(
  ${APP_BINARY} ${GUI_TYPE} ${MAIN_FILES} ${UI_FILES}
  ${LIB_FILES} ${UI_RESOURCES})
target_link_libraries(${APP_BINARY} ${LIBS})

add_custom_target(
  reformat
  COMMAND clang-format
  -style='{BasedOnStyle: Google, SortIncludes: false}'
  -i
  ${MAIN_FILES} ${UI_FILES}
)

