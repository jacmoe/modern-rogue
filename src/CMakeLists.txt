
add_executable(mrogue main.cpp)

find_package (Libtcod REQUIRED)
find_package (Libtcodxx REQUIRED)

target_include_directories(mrogue PUBLIC
  ${LIBTCOD_INCLUDE_DIRS}
  ${LIBTCODXX_INCLUDE_DIRS}
)

target_link_libraries(mrogue
  ${LIBTCOD_LIBRARIES}
  ${LIBTCODXX_LIBRARIES}
)
