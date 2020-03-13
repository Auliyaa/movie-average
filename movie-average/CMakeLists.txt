# ==================================================
# Target: movie-average
# ==================================================
add_executable(movie-average
  src/movie_average.cpp
  src/movie_decoder.cpp
  src/frame_handlers.cpp
)

target_link_libraries(movie-average
  ${AV_LIBRARIES}
  ${SW_SCALE_LIBRARY}
  pthread
  Qt5::Widgets
)

target_include_directories(movie-average
  PRIVATE
  include
  ${AV_INCLUDE_DIR}
  ${SW_SCALE_INCLUDE_DIR}
  ${TCLAP_INCLUDE_DIR}
)
