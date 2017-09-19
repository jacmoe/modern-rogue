#/*********************************************************************************************************
#* This file is part of the
#*
#* ███╗   ███╗ ██████╗ ██████╗ ███████╗██████╗ ███╗   ██╗      ██████╗  ██████╗  ██████╗ ██╗   ██╗███████╗
#* ████╗ ████║██╔═══██╗██╔══██╗██╔════╝██╔══██╗████╗  ██║      ██╔══██╗██╔═══██╗██╔════╝ ██║   ██║██╔════╝
#* ██╔████╔██║██║   ██║██║  ██║█████╗  ██████╔╝██╔██╗ ██║█████╗██████╔╝██║   ██║██║  ███╗██║   ██║█████╗  
#* ██║╚██╔╝██║██║   ██║██║  ██║██╔══╝  ██╔══██╗██║╚██╗██║╚════╝██╔══██╗██║   ██║██║   ██║██║   ██║██╔══╝  
#* ██║ ╚═╝ ██║╚██████╔╝██████╔╝███████╗██║  ██║██║ ╚████║      ██║  ██║╚██████╔╝╚██████╔╝╚██████╔╝███████╗
#* ╚═╝     ╚═╝ ╚═════╝ ╚═════╝ ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝      ╚═╝  ╚═╝ ╚═════╝  ╚═════╝  ╚═════╝ ╚══════╝
#*                                                                                                        
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
  container.cpp
  destructible.cpp
  engine.cpp
  gui.cpp
  map.cpp
  pickable.cpp
)

set_target_properties(mrogue PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED YES
  CXX_EXTENSIONS NO
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
