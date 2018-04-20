project(asdf)
set(PROJECT_VERSION "5.1.90")
set(PROJECT_VERSION_MAJOR 5)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION "5.3.0")

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)
include(ECMInstallIcons)
include(ECMMarkAsTest)
include(ECMMarkNonGuiExecutable)
include(FeatureSummary)
include(CheckIncludeFiles)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
Core
Quick
X11Extras
)

set(KF5_MIN_VERSION "5.2.0")
find_package(KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS
    Wayland
    WindowSystem
)

include_directories(${Qt5Core_PRIVATE_INCLUDE_DIRS})
include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

set(FOO_SOURCES
    main.cpp
    compositor.cpp
    embeddedwindow.cpp
    embeddedprocessloader.cpp
    proxywindow.cpp)
add_library(compositorplugin ${FOO_SOURCES})
target_link_libraries(compositorplugin Qt5::Gui Qt5::Quick KF5::WaylandServer KF5::WaylandClient KF5::WindowSystem  Qt5::X11Extras EGL GL)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

install(TARGETS compositorplugin DESTINATION ${KDE_INSTALL_QMLDIR}/org/dave)
install(FILES qmldir DESTINATION ${KDE_INSTALL_QMLDIR}/org/dave)
