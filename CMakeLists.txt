cmake_minimum_required(VERSION 3.0)

project(movie-average VERSION 1.0.0.0)

# tclap
find_path(TCLAP_INCLUDE_DIR tclap/CmdLine.h)

# qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# libavcodec
find_path(AV_INCLUDE_DIR libavcodec/avcodec.h)

find_library(AV_CODEC_LIBRARY avcodec)
find_library(AV_FORMAT_LIBRARY avformat)
find_library(AV_RESAMPLE_LIBRARY avresample)
find_library(AV_UTIL_LIBRARY avutil)

set(AV_LIBRARIES
  ${AV_CODEC_LIBRARY}
  ${AV_FORMAT_LIBRARY}
  ${AV_RESAMPLE_LIBRARY}
  ${AV_UTIL_LIBRARY}
)

# libswscale
find_path(SW_SCALE_INCLUDE_DIR libswscale/swscale.h)
find_library(SW_SCALE_LIBRARY swscale)

# enable openmp
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

# Subfolders
add_subdirectory(movie-average)
