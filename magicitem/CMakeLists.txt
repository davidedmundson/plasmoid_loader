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
