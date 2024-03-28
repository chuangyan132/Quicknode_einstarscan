# plugins

# 以下是为了快速添加 Qt 风格可执行文件插件
if(120 EQUAL ${MSVC_TOOLSET_VERSION})
    set(QT_LIBS Qt5::Core)
elseif(141 EQUAL ${MSVC_TOOLSET_VERSION})
    set(QT_LIBS Qt5::Core)
elseif(142 EQUAL ${MSVC_TOOLSET_VERSION})
    set(QT_LIBS Qt5::Core)
endif()
macro(add_qn_plugin_exe plugin_name)
    file(GLOB_RECURSE source CONFIGURE_DEPENDS ./*.h ./*.cpp ./*.conf)
    find_package(Qt5 COMPONENTS Widgets OpenGL Network Test REQUIRED)
	
    add_executable(${plugin_name} ${source} )

    target_link_libraries(${plugin_name} 
	    PUBLIC 
	    ${QT_LIBS}
        plugincore
    )
    set_target_properties(${plugin_name} PROPERTIES 
        FOLDER plugins
    )
    set_property(TARGET ${plugin_name} PROPERTY AUTOMOC ON)
    set(TARGET_NAME ${plugin_name})
    set_target_properties(${TARGET_NAME} 
        PROPERTIES
        COMPILE_FLAGS "/MP"
    )
    add_custom_command(TARGET ${plugin_name} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/bin/plugins/executable/${plugin_name}"
        #COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${plugin_name}> "${CMAKE_BINARY_DIR}/bin/plugins/executable/${plugin_name}"
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/${plugin_name}.conf" "${CMAKE_BINARY_DIR}/bin/plugins/executable/${plugin_name}/${plugin_name}.conf"
    )
endmacro()

add_qn_plugin_exe(qn_einstarscan)