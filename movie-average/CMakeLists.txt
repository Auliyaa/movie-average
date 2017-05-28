# ==================================================
# Target: movie-average
# ==================================================
include_directories(
  include

  ${AV_INCLUDE_DIR}
  ${SW_SCALE_INCLUDE_DIR}
  ${TCLAP_INCLUDE_DIR}

)

add_executable(${MOVIE_AVERAGE}
  src/movie_average.cpp
  src/movie_decoder.cpp
)

target_link_libraries(${MOVIE_AVERAGE}
  ${AV_LIBRARIES}
  ${SW_SCALE_LIBRARY}
  pthread
  Qt5::Gui
)

install(TARGETS ${MOVIE_AVERAGE})
