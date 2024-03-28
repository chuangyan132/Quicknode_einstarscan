cmake_minimum_required(VERSION 3.12)
cmake_policy(VERSION 3.12)
project(QNExcutablePlugins VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_INCLUDE_CURRENTDIR ON)

set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_BUILD_TYPE "" CACHE STRING "" FORCE)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QUICKEDITOR_PATH "" CACHE PATH "please input quickeditor.exe path")

include_directories(${CMAKE_SOURCE_DIR}/include)
include(SetConanPackageDir)
include(conan)
include(uni)

add_definitions(-DSDK_API_VERSION="v2.0")

option(RUN_SIGNTOOL "Run the signtool" OFF)

function(set_project_compile_definitions targetName compileFlagsInRelease)
	option(_using_debug_compile_definitions_${targetName} "对项目 ${targetName} 开启DEBUG模式" OFF)
	if(_using_debug_compile_definitions_${targetName})
		message("project: ${targetName} debug open.")
		set_target_properties(${targetName} PROPERTIES COMPILE_FLAGS "/Zi /Od")
		set_target_properties(${targetName} PROPERTIES LINK_FLAGS "/DEBUG /INCREMENTAL:NO /OPT:REF /OPT:ICF")
	else()
		set_target_properties(${targetName} PROPERTIES COMPILE_FLAGS ${compileFlagsInRelease})
	endif()
endfunction(set_project_compile_definitions)

#if(141 EQUAL ${MSVC_TOOLSET_VERSION})
#    conan_cmake_run(CONANFILE conanfile_vs2017.txt
#        BASIC_SETUP CMAKE_TARGETS UPDATE
#        BUILD never
#        GENERATORS cmake_multi
#    )
#else()
#	message(STATUS ${MSVC_TOOLSET_VERSION})
#    message(FATAL_ERROR "Visual Studio Version not supported!")
#endif()


include(SetConanPackageDir)
#set_package_root_postfix(Qt Qt5_DIR /lib/cmake/Qt5)
find_package(Qt5 COMPONENTS Core Gui Widgets Quick Qml QuickWidgets remoteobjects Concurrent REQUIRED CONFIG)

#OSG
#set_package_root_postfix(OSG OSG_DIR /cmake)
#find_package(OSG REQUIRED CONFIG)

##Qt
#set_package_root_postfix(Qt Qt5_DIR /lib/cmake/Qt5)
#find_package(Qt5 REQUIRED
#    COMPONENTS Core Quick Gui Qml
#)


#RenderKit
#set_package_root_postfix(Sn3D.RenderKit Sn3DRenderKit_DIR /cmake)
#find_package(Sn3DRenderKit REQUIRED CONFIG)

#set(RenderKit_DIR ${_renderkit_install_prefix})
#set(RenderKit_LIB_DIR ${RenderKit_DIR}/lib)
#set(RenderKit_INCLUDE_DIR ${RenderKit_DIR}/include)

#set(OSG_INCLUDE_DIR ${_osg_install_prefix}/include)
#set(OSG_LIB_DIR ${_osg_install_prefix}/lib)


get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(DEPLOYQT_EXECUTABLE NAMES windeployqt macdeployqt HINTS "${_qt_bin_dir}")
find_program(QMLPLUGINDUMP_EXECUTABLE NAMES qmlplugindump HINTS "${_qt_bin_dir}")

if(RUN_SIGNTOOL)
	find_program(SIGNTOOL_EXECUTABLE NAMES signtool REQUIRED)
endif()

#set(VERSIONINFO_RC "${CMAKE_BINARY_DIR}/VersionInforc")
#configure_file("${CMAKE_SOURCE_DIR}/VersionInfo.rc.in"
#	"${VERSIONINFO_RC}")
	
#install(DIRECTORY cmake_configs/ DESTINATION cmake)

add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_subdirectory(ScannerDemo)
#add_subdirectory(3rd)
add_subdirectory(plugins)
