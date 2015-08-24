cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(enfolderizeplugin)

set(QT_MIN_VERSION "5.0.0")
set(KF5_MIN_VERSION "5.0.0")
set(ECM_MIN_VERSION "0.0.11")

find_package(ECM ${ECM_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(KDEFrameworkCompilerSettings)
include(ECMMarkNonGuiExecutable)


find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    Widgets
)

find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    I18n
    KIO
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)

set(CMAKE_AUTOMOC ON)

# add_library(common OBJECT common.cpp)
# set_target_properties(common PROPERTIES POSITION_INDEPENDENT_CODE ON)  

set(enfolderizeplugin_SRCS
    enfolderizeplugin.cpp
     common.cpp)
#     $<TARGET_OBJECTS:common>)
set(enfolderizeplugin_dnd_SRCS
    enfolderizeplugin_dnd.cpp
     common.cpp)
#      $<TARGET_OBJECTS:common>)
         
add_library(enfolderizeplugin MODULE ${enfolderizeplugin_SRCS})
add_library(enfolderizeplugin_dnd MODULE ${enfolderizeplugin_dnd_SRCS})

kcoreaddons_desktop_to_json(enfolderizeplugin enfolderizeplugin.desktop)
kcoreaddons_desktop_to_json(enfolderizeplugin_dnd enfolderizeplugin_dnd.desktop)

set(myactionplugin_SRCS myactionplugin.cpp)

target_link_libraries(enfolderizeplugin
    KF5::KIOWidgets
)
target_link_libraries(enfolderizeplugin_dnd
    KF5::KIOWidgets
)

install(FILES enfolderizeplugin.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})
install(FILES enfolderizeplugin_dnd.desktop DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

install(TARGETS enfolderizeplugin DESTINATION ${KDE_INSTALL_PLUGINDIR})
install(TARGETS enfolderizeplugin_dnd DESTINATION ${KDE_INSTALL_PLUGINDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
