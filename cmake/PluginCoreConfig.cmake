
if (CMAKE_VERSION VERSION_LESS 3.15.0)
    message(FATAL_ERROR "PluginCore requires at least CMake version 3.15.0")
endif()

get_filename_component(_quicknodes_ui_install_prefix "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

add_library(plugincore INTERFACE IMPORTED)

set_property(TARGET plugincore PROPERTY INTERFACE_LINK_LIBRARIES
    $<$<CONFIG:Release>:${_quicknodes_ui_install_prefix}/lib/plugincore.lib>
    $<$<CONFIG:Debug>:${_quicknodes_ui_install_prefix}/lib/plugincored.lib>
)

set_property(TARGET plugincore PROPERTY INTERFACE_INCLUDE_DIRECTORIES
    ${_quicknodes_ui_install_prefix}/include
)

set(QuickNodes_APP_PATH ${_quicknodes_ui_install_prefix}/app)

set(PluginCore_BIN $<$<CONFIG:Debug>:${_quicknodes_ui_install_prefix}/bin/plugincored.dll>$<$<CONFIG:Release>:${_quicknodes_ui_install_prefix}/bin/plugincore.dll>)
