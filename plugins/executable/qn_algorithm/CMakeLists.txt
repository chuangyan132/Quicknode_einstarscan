# qn_algorithm
set(TARGET_NAME qn_algorithm)
set(FOLDER_NAME plugins)

# 以下是为了快速添加 Qt 风格可执行文件插件
if(120 EQUAL ${MSVC_TOOLSET_VERSION})
    set(QT_LIBS Qt5::Gui Qt5::Widgets Qt5::Core Qt5::concurrent)
elseif(141 EQUAL ${MSVC_TOOLSET_VERSION})
    set(QT_LIBS Qt5::Gui Qt5::Widgets Qt5::Core Qt5::Concurrent)
elseif(142 EQUAL ${MSVC_TOOLSET_VERSION})
    set(QT_LIBS Qt5::Gui Qt5::Widgets Qt5::Core Qt5::Concurrent)
endif()
file(GLOB_RECURSE source CONFIGURE_DEPENDS *.h *.cpp *.conf ../include/qtlike/*)
find_package(Qt5 COMPONENTS Widgets OpenGL Network Concurrent Test REQUIRED)
add_executable(${TARGET_NAME} ${source} )
#target_link_libraries(${TARGET_NAME} PUBLIC ${QT_LIBS})
#target_link_libraries(${TARGET_NAME} plugincore)
target_link_libraries(${TARGET_NAME} 
	PUBLIC 
	${QT_LIBS}
    plugincore
)
set_target_properties(${TARGET_NAME} PROPERTIES 
    FOLDER plugins
)
set_property(TARGET ${TARGET_NAME} PROPERTY AUTOMOC ON)
set(TARGET_NAME ${TARGET_NAME})
set_target_properties(${TARGET_NAME} 
    PROPERTIES
    COMPILE_FLAGS "/MP"
)

link_directories(../../../lib)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_BINARY_DIR}/../../../00_QuickNodes/zeno/.build/bin/plugins/executable/qn_einstarscan)
#以下内容包含部分当前电脑路径,此处先注释
#add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/../../../../zeno_2.0.0_test_build/bin/plugins/executable/${TARGET_NAME}"
#        #COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.conf D:/south_project/ZenoAbout/zeno_2.0.0_test_build/bin/plugins/executable/${TARGET_NAME}/${TARGET_NAME}.conf
#        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.conf "${CMAKE_CURRENT_SOURCE_DIR}/../../../../zeno_2.0.0_test_build/bin/plugins/executable/${TARGET_NAME}/${TARGET_NAME}.conf"
#        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${TARGET_NAME}> "${CMAKE_CURRENT_SOURCE_DIR}/../../../../zeno_2.0.0_test_build/bin/plugins/executable/${TARGET_NAME}"
#		)

#按实际目录展示在vs中
if (MSVC)
	set_target_properties(${TARGET_NAME} PROPERTIES 
        FOLDER ${FOLDER_NAME}
    )
endif()

