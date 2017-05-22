# ==================================================
# Target: movie-average
# ==================================================
add_executable(${MOVIE_AVERAGE}
  src/movie_average.cpp
)
#target_link_libraries(${MOVIE_AVERAGE}
#
#)
install(TARGETS ${MOVIE_AVERAGE})
