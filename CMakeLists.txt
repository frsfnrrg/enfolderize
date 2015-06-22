cmake_minimum_required(VERSION 2.8.8)
project(enfolderizeplugin)

find_package(KDE4 REQUIRED)
find_package(LibKonq REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIR})
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${LIBKONQ_INCLUDE_DIR} ${QIMAGEBLITZ_INCLUDES} )

set(CMAKE_AUTOMOC ON)

add_library(common OBJECT common.cpp)
SET_TARGET_PROPERTIES(common PROPERTIES
  COMPILE_FLAGS "-fPIC"
)

set(enfolderizeplugin_SRCS
    enfolderizeplugin.cpp
    $<TARGET_OBJECTS:common>)
set(enfolderizeplugin_dnd_SRCS
    enfolderizeplugin_dnd.cpp
    $<TARGET_OBJECTS:common>)

kde4_add_plugin(enfolderizeplugin  ${enfolderizeplugin_SRCS})
kde4_add_plugin(enfolderizeplugin_dnd  ${enfolderizeplugin_dnd_SRCS})

target_link_libraries(enfolderizeplugin ${KDE4_KIO_LIBS} ${LIBKONQ_LIBRARY})
target_link_libraries(enfolderizeplugin_dnd ${KDE4_KIO_LIBS} ${LIBKONQ_LIBRARY})

install(FILES enfolderizeplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES enfolderizeplugin_dnd.desktop DESTINATION ${SERVICES_INSTALL_DIR})

install(TARGETS enfolderizeplugin DESTINATION ${PLUGIN_INSTALL_DIR})
install(TARGETS enfolderizeplugin_dnd DESTINATION ${PLUGIN_INSTALL_DIR})
