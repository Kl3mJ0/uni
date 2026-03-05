cmake_minimum_required(VERSION 3.19)

project(WS6_GUI LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

# VTK (must be after cmake_minimum_required)
find_package(VTK REQUIRED)

qt_standard_project_setup()

qt_add_executable(WS6_GUI
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    ModelPart.cpp
    ModelPart.h
    ModelPartList.cpp
    ModelPartList.h
    icons.qrc
    option.h option.cpp option.ui
)

target_link_libraries(WS6_GUI
    PRIVATE
        Qt6::Core
        Qt6::Widgets
        ${VTK_LIBRARIES}
)

include(GNUInstallDirs)

install(TARGETS WS6_GUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET WS6_GUI
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

# Copy across OpenVR bindings that map controllers
# The program will expect to find these in the build dir when it runs
add_custom_target(VRBindings)
add_custom_command(TARGET VRBindings PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E
  copy_directory ${CMAKE_SOURCE_DIR}/vrbindings ${CMAKE_BINARY_DIR}/
)
