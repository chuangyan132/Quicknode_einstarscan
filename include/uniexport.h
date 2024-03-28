/*!
* @brief   
* @project UniCommon
* @file    uniexport.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-13 11:39
*/
//==============================================================================
#ifndef UNICOMMON_UNIEXPORT_H
#define UNICOMMON_UNIEXPORT_H
//==============================================================================

/*!
* 正确定义导入导出宏
* 需要根据不同平台和不同编译器，定义UNI_PROPER_EXPORT_FLAG和UNI_PROPER_IMPORT_FLAG
*/

#if defined (WIN32) || defined (_WINDOWS_) || defined __CYGWIN__
#	define UNI_PROPER_EXPORT_FLAG __declspec (dllexport)
#	define UNI_PROPER_IMPORT_FLAG __declspec (dllimport)
#elif ( __SUNPRO_CC > 0x540 )
#	define UNI_PROPER_EXPORT_FLAG __symbolic
#	define UNI_PROPER_IMPORT_FLAG __global
#elif (__GNUC__ >= 4)
#		define UNI_PROPER_EXPORT_FLAG	__attribute__ ((visibility("default")))
#		define UNI_PROPER_IMPORT_FLAG	__attribute__ ((visibility("default")))
#else
#	define UNI_PROPER_EXPORT_FLAG
#	define UNI_PROPER_IMPORT_FLAG
#endif

//==============================================================================
#endif // UNICOMMON_UNIEXPORT_H
//==============================================================================
