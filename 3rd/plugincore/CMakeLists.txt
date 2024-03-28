# QNAPI

set(TARGET_NAME plugincore)

set(Source_Files
	qn_plugin.cpp
	rpcmanager.cpp
	../../include/jsoncpp/jsoncpp.cpp

)

set(Header_Files
	rpcmanager.h
	../../include/fifo.h
	# ./include/log.h
	./include/logging.h
	../../include/qn_plugin.h
	../../include/qn_defines.h
	../../include/jsoncpp/json/json.h
	../../include/jsoncpp/json/json-forwards.h
)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

find_package_from_conan_V1(mosquitto pthread)

#set_package_root_postfix(glog glog_DIR lib/cmake/glog)
#find_package(glog 0.6.0 REQUIRED)
#include_directories(${glog_INCLUDE_DIR})

#set_package_root_postfix(gflags gflags_DIR lib/cmake/gflags)
#find_package(gflags 2.2.2 REQUIRED)
#include_directories(${gflags_INCLUDE_DIR})

set(CMAKE_DEBUG_POSTFIX "d")

add_library(${TARGET_NAME} SHARED ${Source_Files} ${Header_Files})
target_compile_definitions(${TARGET_NAME} PRIVATE QUICKNODES_DLLEXPORT LIBMOSQUITTO_STATIC)

include_directories(${mosquitto_INCLUDE_DIR}/include)

target_link_libraries(${TARGET_NAME} PRIVATE mosquitto_static ws2_32 pthread_static)
	

#get_filename_component (glog_BINARY "${glog_DIR}/../../../bin" ABSOLUTE)
#add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${glog_BINARY} ${CMAKE_BINARY_DIR}/bin
#    )
#	
#get_filename_component (gflags_BINARY "${gflags_DIR}/../../../bin" ABSOLUTE)
#add_custom_command(TARGET ${TARGET_NAME} PRE_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory ${gflags_BINARY} ${CMAKE_BINARY_DIR}/bin
#    )
			

