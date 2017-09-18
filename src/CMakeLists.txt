#/*********************************************************************************************************
#* This file is part of the
#*
#* ███╗   ███╗ ██████╗ ██████╗ ███████╗██████╗ ███╗   ██╗      ██████╗  ██████╗ ██╗   ██╗ ██████╗ ███████╗
#* ████╗ ████║██╔═══██╗██╔══██╗██╔════╝██╔══██╗████╗  ██║      ██╔══██╗██╔═══██╗██║   ██║██╔════╝ ██╔════╝
#* ██╔████╔██║██║   ██║██║  ██║█████╗  ██████╔╝██╔██╗ ██║█████╗██████╔╝██║   ██║██║   ██║██║  ███╗█████╗  
#* ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝  ██╔══██╗██║╚██╗██║╚════╝██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝  
#* ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗██║  ██║██║ ╚████║      ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗
#* ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝      ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝
#*
#* project : https://github.com/jacmoe/modern-rogue
#*
#* Copyright 2017 Jacob Moen
#*
#**********************************************************************************************************/

add_executable(mrogue main.cpp
  actor.cpp
  ai.cpp
  attacker.cpp
  destructible.cpp
  engine.cpp
  gui.cpp
  map.cpp
)

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
