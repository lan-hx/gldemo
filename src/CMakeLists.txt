cmake_minimum_required(VERSION 3.14)

cmake_policy(SET CMP0087 NEW)

FILE(GLOB_RECURSE GLDEMO_SOURCE
        ${PROJECT_SOURCE_DIR}/src/**/*.cpp
        ${PROJECT_SOURCE_DIR}/src/**/*.ui
        ${PROJECT_SOURCE_DIR}/src/**/*.h
        )
FILE(GLOB_RECURSE GLDEMO_SOURCE_MAIN
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/*.ui
        ${PROJECT_SOURCE_DIR}/src/*.h
        )
message(STATUS "find sources: ${GLDEMO_SOURCE} ${GLDEMO_SOURCE_MAIN}")
#ADD_EXECUTABLE(gldemo ${GLDEMO_SOURCE} ${GLDEMO_SOURCE_MAIN})
qt_add_executable(gldemo ${GLDEMO_SOURCE} ${GLDEMO_SOURCE_MAIN})

set(qt_deploy_script "${CMAKE_CURRENT_BINARY_DIR}/deploy_gldemo_$<CONFIG>.cmake")
file(GENERATE OUTPUT ${qt_deploy_script} CONTENT "
include(\"${QT_DEPLOY_SUPPORT}\")
qt_deploy_runtime_dependencies(
    EXECUTABLE \"$<TARGET_FILE_NAME:gldemo>\"
    BIN_DIR \"\"
    GENERATE_QT_CONF
)")
install(TARGETS gldemo
        DESTINATION .
        )
install(SCRIPT ${qt_deploy_script})

TARGET_LINK_LIBRARIES(gldemo PRIVATE Qt6::Core Qt6::Widgets Qt6::OpenGL Qt6::OpenGLWidgets Qt6::Gui tinyobjloader::tinyobjloader)
set_target_properties(gldemo PROPERTIES
        MACOSX_BUNDLE_GUI_IDENTIFIER huaji
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
        )
add_dependencies(gldemo clang_format shader)

#if(WIN32)
#    set(QT_DEPLOY_EXE "${QT_PREFIX_DIR}/bin/windeployqt.exe")
#    message("QT_DEPLOY_EXE: ${QT_DEPLOY_EXE}")
#    add_custom_command(TARGET gldemo POST_BUILD
#            COMMAND ${QT_DEPLOY_EXE} $<TARGET_FILE:gldemo>)
#endif()

add_custom_target(deploy
        COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --config $<CONFIG>
        )
add_dependencies(deploy gldemo)
