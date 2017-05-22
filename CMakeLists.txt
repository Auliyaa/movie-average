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
find_path(AV_CODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AV_CODEC_LIBRARY avcodec)
find_library(AV_DEVICE_LIBRARY avdevice)
find_library(AV_FILTER_LIBRARY avfilter)
find_library(AV_FORMAT_LIBRARY avformat)
find_library(AV_RESAMPLE_LIBRARY avresample)
find_library(AV_UTIL_LIBRARY avutil)

aio_dump_variable(TCLAP_INCLUDE_DIR)
aio_dump_variable(AV_CODEC_INCLUDE_DIR)

aio_dump_variable(AV_CODEC_LIBRARY)
aio_dump_variable(AV_DEVICE_LIBRARY)
aio_dump_variable(AV_FILTER_LIBRARY)
aio_dump_variable(AV_FORMAT_LIBRARY)
aio_dump_variable(AV_RESAMPLE_LIBRARY)
aio_dump_variable(AV_UTIL_LIBRARY)

# ==================================================
# Global compiler directives
# ==================================================

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
