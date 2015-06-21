cmake_minimum_required(VERSION 2.6)
project(enfolderizeplugin)

find_package(KDE4 REQUIRED)
find_package(LibKonq REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)


add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${KDEPIMLIBS_INCLUDE_DIR})
include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${LIBKONQ_INCLUDE_DIR} ${QIMAGEBLITZ_INCLUDES} )

set(enfolderizeplugin_SRCS
    enfolderizeplugin.cpp
    checkoutdialog.cpp
    commitdialog.cpp
    tagdialog.cpp
    pushdialog.cpp
    pulldialog.cpp
    gitwrapper.cpp
)

MESSAGE( STATUS "VARIABLES " ${SERVICES_INSTALL_DIR} ${KCFG_INSTALL_DIR} ${PLUGIN_INSTALL_DIR})

kde4_add_kcfg_files(enfolderizeplugin_SRCS enfolderizepluginsettings.kcfgc)
kde4_add_plugin(enfolderizeplugin  ${enfolderizeplugin_SRCS})
target_link_libraries(enfolderizeplugin ${KDE4_KIO_LIBS} ${LIBKONQ_LIBRARY})
install(FILES enfolderizeplugin.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install(FILES enfolderizepluginsettings.kcfg DESTINATION ${KCFG_INSTALL_DIR})
install(TARGETS enfolderizeplugin DESTINATION ${PLUGIN_INSTALL_DIR})
