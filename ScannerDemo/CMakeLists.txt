set(TARGET_NAME ScannerDemo)

set(HEADERS
    MeshDialog.h
	NewProjDlg.h
#	PublicClass.h
#	VideoThread.h
#	ScannerDemo.h
	#scanserviceimp.h
#    SharedMemoryParser.h	
	#e7plusfinishscandlg.h
	#e7plusmeshdlg.h
	#e7plusnewprojdlg.h
	#e7plusenterscandlg.h
	opencreateslndlg.h
	#e7plusfixscandlg.h
	#e7plusnewfixprojdlg.h
	sn3dfiledialog.h
	#e7plussharememory.h
	sdklogger.h
	#e7plussmceditcata.h
	newopenprojdialog.h
	device.h
	devicefactory.h
	#e3device.h
	#e3deviceh.h
	#e3devicehx.h
	#e7device.h
	sn3dmainwindow.h
	./subContrl/sn3dcamerwidget.h
	./subContrl/menuaction.h
	./subContrl/sn3ddialogback.h
	./subContrl/widgetcontent.h
	sn3dstyle.h
	e10device.h
)

set(SOURCES
    main.cpp
    MeshDialog.cpp
	NewProjDlg.cpp
#	ScannerDemo.cpp
#	VideoThread.cpp
	#scanserviceimp.cpp	
#	SharedMemoryParser.cpp
	#e7plusfinishscandlg.cpp
	#e7plusmeshdlg.cpp
	#e7plusnewprojdlg.cpp
	#e7plusenterscandlg.cpp
	opencreateslndlg.cpp
	#e7plusfixscandlg.cpp
	#e7plusnewfixprojdlg.cpp
	sn3dfiledialog.cpp
	#e7plussharememory.cpp
	sdklogger.cpp
	#e7plussmceditcata.cpp
	newopenprojdialog.cpp
	device.cpp
	devicefactory.cpp
	#e3device.cpp
	#e3deviceh.cpp
	#e3devicehx.cpp
	#e7device.cpp
	sn3dmainwindow.cpp
	./subContrl/sn3dcamerwidget.cpp
	./subContrl/menuaction.cpp
	./subContrl/sn3ddialogback.cpp
	./subContrl/widgetcontent.cpp
	sn3dstyle.cpp
	e10device.cpp
)
#link_directories(../3rd/sharedmemory/lib)
include_directories(./)
include_directories(../include)
link_directories(../lib)
link_libraries(sdk_adapter Qt5::Core Qt5::Widgets Qt5::RemoteObjects sn3d_smc.lib Sn3dE10SDK)
set(RESOURCES 
    images.qrc
)
qt5_add_resources(SCANDEMO_RCS images.qrc)
add_executable(${TARGET_NAME} ${HEADERS} ${SOURCES} ${SCANDEMO_RCS}
    MeshDialog.ui
#	ScannerDemo.ui
	NewProjDlg.ui
	#e7plusfinishscandlg.ui
	#e7plusmeshdlg.ui
	#e7plusnewprojdlg.ui
	#e7plusenterscandlg.ui
	opencreateslndlg.ui
	#e7plusfixscandlg.ui
	#e7plusnewfixprojdlg.ui
	sn3dmainwindow.ui
	./subContrl/sn3dcamerwidget.ui
	./subContrl/menuaction.ui
	./subContrl/sn3ddialogback.ui
	./subContrl/widgetcontent.ui
	newopenprojdialog.ui  ${VERSIONINFO_RC})
#target_link_libraries(${TARGET_NAME} libzmq-static)
#target_link_libraries(${TARGET_NAME} libzmq-v141-mt-4_3_2.lib)

install(TARGETS ${TARGET_NAME}
	RUNTIME DESTINATION examples
)

set_target_properties(${TARGET_NAME} PROPERTIES
    FOLDER "examples"
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_INSTALL_PREFIX}/examples"
    VS_DEBUGGER_ENVIRONMENT "PATH=${_qt_bin_dir};${CMAKE_INSTALL_PREFIX}/bin\nQT_QPA_PLATFORM_PLUGIN_PATH=${_qt_bin_dir}/../plugins/platforms"
    VS_DEBUGGER_COMMAND "${CMAKE_INSTALL_PREFIX}/examples/${TARGET_NAME}.exe"
)

add_custom_target(deploy_demo
    COMMAND ${DEPLOYQT_EXECUTABLE} "$<TARGET_FILE:${TARGET_NAME}>"
        --$<LOWER_CASE:$<CONFIG>>
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_INSTALL_PREFIX}/bin/ "$<TARGET_FILE_DIR:${TARGET_NAME}>"
)

set_target_properties(deploy_demo PROPERTIES
  FOLDER "examples/helpers"
)

# include_directories(../3rd/sharedmemory/include)


set_project_compile_definitions(${TARGET_NAME} "/MP")

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION .)
install(FILES zmq_sdk_config.ini DESTINATION .)
#install(FILES ../3rd/sharedmemory/bin/sn3d_smc.dll DESTINATION .)
install(DIRECTORY ../bin/ DESTINATION .)
#install(DIRECTORY ../3rd/ DESTINATION .)
                                    
