cmake_minimum_required(VERSION 2.8.3)
project(grid_sim)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++11)

#SFML(until 2.5 you need an extra FindSFML.cmake aside your project...)
#https: // github.com/SFML/SFML/wiki/Tutorial:-Build-your-SFML-project-with-CMake
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})
find_package(SFML 2.4 COMPONENTS graphics window system REQUIRED)

find_package(Threads)

find_package(tmxparser REQUIRED)

find_package(catkin REQUIRED capnzero alica_capnzero_proxy id_capnzero id_manager fsystem system_config cnc_geometry)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}_msgs ${PROJECT_NAME}_world
  CATKIN_DEPENDS capnzero alica_capnzero_proxy id_capnzero id_manager fsystem system_config cnc_geometry
  DEPENDS SFML
)

include_directories(
  include
  ${SFML_INCLUDE_DIR}
  ${catkin_INCLUDE_DIRS}
)

############### Simulator
add_executable(${PROJECT_NAME}
  src/Simulator.cpp
  src/GUI.cpp
  src/communication/Communication.cpp
        src/commands/SpawnCommandHandler.cpp
        src/commands/CommandHandler.cpp
        src/commands/MoveCommandHandler.cpp
        src/commands/ManipulationHandler.cpp
        )

target_link_libraries(${PROJECT_NAME}
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES}
  ${CMAKE_THREAD_LIBS_INIT}
  ${PROJECT_NAME}_msgs
  ${PROJECT_NAME}_world
  ${catkin_LIBRARIES}
  tmxparser
)

#copy the resource(e.g.texture files)
file(GLOB ResourceFiles
  ${CMAKE_SOURCE_DIR}/textures/*.png
  ${CMAKE_SOURCE_DIR}/textures/*.tmx
  ${CMAKE_SOURCE_DIR}/textures/*.tsx
)
add_custom_target(copy ALL
  DEPENDS
  ${ResourceFiles}
)
foreach(ResourceFile ${ResourceFiles})
  get_filename_component(file ${ResourceFile} NAME)
  add_custom_command(TARGET copy
    PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E
    copy ${ResourceFile} $<TARGET_FILE_DIR:grid_sim>/textures/${file}
    COMMAND ${CMAKE_COMMAND} -E
    make_directory $<TARGET_FILE_DIR:grid_sim>/textures/
  )
endforeach()

############### World
add_library(${PROJECT_NAME}_world
        src/world/World.cpp
        src/world/Cell.cpp
        src/container/Coordinate.cpp
        src/world/Object.cpp
        src/world/Door.cpp
        src/world/ServiceRobot.cpp
        src/world/Sensor.cpp
        src/world/Actuator.cpp
        src/world/Manipulation.cpp
        src/world/ObjectDetection.cpp
)

target_link_libraries(${PROJECT_NAME}_world
  tmxparser
  ${catkin_LIBRARIES}
)

############### Sim Messages
find_package(CapnProto REQUIRED)
set(CAPNPC_IMPORT_DIRS ${id_capnzero_SOURCE_PREFIX}/include)# include external msgs
set(${PROJECT_NAME}_msgs "${CMAKE_CURRENT_SOURCE_DIR}/include/srgsim/msgs")
file(GLOB_RECURSE capnp_messages ${${PROJECT_NAME}_msgs} *.capnp)
set(CAPNPC_SRC_PREFIX ${${PROJECT_NAME}_msgs})
set(CAPNPC_OUTPUT_DIR ${${PROJECT_NAME}_msgs})
capnp_generate_cpp(CAPNP_SRCS CAPNP_HDRS ${capnp_messages})

add_library(${PROJECT_NAME}_msgs
  src/container/ContainerUtils.cpp
  ${CAPNP_SRCS}
)
target_link_libraries(${PROJECT_NAME}_msgs
  ${catkin_LIBRARIES}
)
