cmake_minimum_required(VERSION 3.10)

project(BRFD
  VERSION 0.1.0
)

include(GNUInstallDirs)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-chipmunk/cmake")

find_package(gf REQUIRED COMPONENTS graphics)
find_package(Chipmunk REQUIRED)

set(BRFD_DATA_DIR "${CMAKE_INSTALL_PREFIX}/share/games/brfd")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(bank_robbery_for_dummies
  code/bank_robbery_for_dummies.cc
  code/local/Car.cc
  code/local/Level.cc
  code/local/Story.cc
  vendor/gf-chipmunk/gfcp/Physics.cc
)

target_include_directories(bank_robbery_for_dummies
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/gf-chipmunk
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(bank_robbery_for_dummies
  PRIVATE
    gf::graphics
    Chipmunk::Chipmunk
)

install(
  TARGETS bank_robbery_for_dummies
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
)

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/brfd"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
)
