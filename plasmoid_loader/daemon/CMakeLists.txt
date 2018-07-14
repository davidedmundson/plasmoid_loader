
set(DAEMON_SOURCES
    main.cpp
    abstractappletfactory.cpp
    abstractappletwrapper.cpp
    plasma5appletfactory.cpp
    plasma5appletwrapper.cpp
    )
add_library(compositorplugin ${DAEMON_SOURCES})
target_link_libraries(compositorplugin Qt5::Gui Qt5::Quick KF5::WaylandServer KF5::WaylandClient KF5::WindowSystem  Qt5::X11Extras EGL GL)
