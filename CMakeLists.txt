cmake_minimum_required(VERSION 3.0)

# ==================================================
# Project definition
# ==================================================
project(movie-average VERSION 1.0.0.0)

# ==================================================
# Include CMake module
# ==================================================
set(CMAKE_PREFIX_PATH
  ${CMAKE_PREFIX_PATH}
  ${CMAKE_SOURCE_DIR}/cmake/src
)

find_package(AIO REQUIRED)

# ==================================================
# Global dependencies
# ==================================================
# tclap
find_path(TCLAP_INCLUDE_DIR tclap/CmdLine.h)

# libavcodec
find_path(AV_INCLUDE_DIR libavcodec/avcodec.h)

find_library(AV_CODEC_LIBRARY avcodec)
find_library(AV_DEVICE_LIBRARY avdevice)
find_library(AV_FILTER_LIBRARY avfilter)
find_library(AV_FORMAT_LIBRARY avformat)
find_library(AV_RESAMPLE_LIBRARY avresample)
find_library(AV_UTIL_LIBRARY avutil)

set(AV_LIBRARIES
  ${AV_CODEC_LIBRARY}
  ${AV_DEVICE_LIBRARY}
  ${AV_FILTER_LIBRARY}
  ${AV_FORMAT_LIBRARY}
  ${AV_RESAMPLE_LIBRARY}
  ${AV_UTIL_LIBRARY}
)

# libswscale
find_path(SW_SCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SW_SCALE_LIBRARY swscale)

# Qt
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Gui REQUIRED)

# ==================================================
# Global compiler directives
# ==================================================
include_directories(${CMAKE_SOURCE_DIR}/libs/ctpl)

# ==================================================
# Targets built by this project
# ==================================================
set(MOVIE_AVERAGE movie-average)

# ==================================================
# Subfolders
# ==================================================
add_subdirectory(movie-average)

# ==================================================
# Include module installer
# ==================================================
