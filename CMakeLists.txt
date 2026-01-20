cmake_minimum_required(VERSION 3.10)

project(RogueCMI
  VERSION 0.0.1
)

include(GNUInstallDirs)

find_package(gf REQUIRED)

set(RogueCMI_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/RogueCMI")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

# TODO : ajouter les executables
add_executable(RogueCMI
  code/main.cc
  code/bits/Map.cc
  code/bits/Character.cc
  code/bits/Comportment.cc
  code/bits/Actions.cc
  code/bits/RogueCMI.cc
  code/bits/WorldScene.cc
  code/bits/BaseScene.cc
  code/bits/DungeonGenerator.cc
  code/bits/bsp.cc
  code/bits/GameData.cc
  code/bits/MenuScene.cc
  code/bits/InventoryScene.cc
  code/bits/Inventory.cc
  code/bits/Item.cc
  code/bits/WorldEntity.cc
  code/bits/ItemManager.cc
)

target_include_directories(RogueCMI
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

# TODO : ajouter box2d::box2d si utilisé
target_link_libraries(RogueCMI
  PUBLIC
    gf::graphics
)

install(
  TARGETS RogueCMI
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/RogueCMI"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)