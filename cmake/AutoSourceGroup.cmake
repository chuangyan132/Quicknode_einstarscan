#=====================================================================================================
#函数: 根据文件目录结构，自动设置VS Filter的层次结构
#auto_source_group 函数名称
#filelist 待处理文件列表,文件路径必须为绝对路径
#groupprefix 对文件分组时,是否添加前缀,比如"Header Files",不可为空,如果不需要前缀,填写"_noprefix"
#shouldinclude 是否将文件所在目录添加到包含目录,true 或 false
#
#用法示例
# file(GLOB_RECURSE headers *.h)
# auto_source_group("${headers}" "Header Files" true)
# auto_source_group("${headers}" "_noprefix" true)

function(auto_source_group filelist groupprefix shouldinclude)
    foreach(_source ${filelist})
		if (${shouldinclude})
			get_filename_component(_include_path "${_source}" PATH)
			include_directories(${_include_path})
		endif()
		
		if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
			#set(_source_rel "${_source}")
        endif()

        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
		string(COMPARE EQUAL ${groupprefix} "_noprefix" _dontneedprefix)
		if (NOT ${_dontneedprefix})
			string(PREPEND	_source_path_msvc ${groupprefix}\\)
		endif()
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction()
#=====================================================================================================
