/*!
* @brief   
* @project QNCOMMON
* @file    qn_basicTypes.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-20 15:30
*/
//==============================================================================
#ifndef QNCOMMON_QN_BASICTYPES_H
#define QNCOMMON_QN_BASICTYPES_H
//==============================================================================

#include <stdint.h>
#include <stddef.h>

///	bool
typedef	bool		QN_BOOL;
#define	QN_TRUE	    true
#define	QN_FALSE	false

/// Integer
typedef int8_t		QN_INT8;
typedef uint8_t		QN_UINT8;
typedef QN_UINT8	QN_BYTE;
typedef	int16_t		QN_INT16;
typedef	uint16_t	QN_UINT16;
typedef int32_t		QN_INT32;
typedef uint32_t	QN_UINT32;
typedef int64_t		QN_INT64;
typedef uint64_t	QN_UINT64;

#if 0
#if defined (WIN32) || defined (_WINDOWS_) || defined __CYGWIN__
#	if !defined (__MINGW32__)
typedef signed __int64	 QN_INT64;
typedef unsigned __int64 QN_UINT64;
#	else
typedef signed long long QN_INT64;
typedef unsigned long long QN_UINT64;
#	endif
#else
#	if defined (__LP64__) || defined (_LP64) || defined (__sparcv9)
typedef long			QN_INT64;
typedef unsigned long	QN_UINT64;
#	else
typedef signed long long	QN_INT64;
typedef unsigned long long	QN_UINT64;
#	endif
#endif
#endif

// char
typedef	char			    QN_CHAR;
typedef	wchar_t			    QN_WCHAR;

/// string
typedef	QN_CHAR*			QN_STR;
typedef	const QN_CHAR*		QN_CSTR;
typedef QN_WCHAR*			QN_WSTR;
typedef	const QN_WCHAR*	    QN_CWSTR;

/// float
typedef float               QN_FLOAT;
typedef double              QN_DOUBLE;
typedef long double         QN_LONGDOUBLE;

/// time
typedef QN_INT64            QN_TIME_VALUE;
#if defined (WIN32) || defined (_WINDOWS_) || defined __CYGWIN__
#	ifdef _USE_32BIT_TIME_T
#		define QN_MAX_TIME_VALUE 2147483647000000L // 2038-01-19 03:14:07:000
#	else
#		define QN_MAX_TIME_VALUE 32535215999000000L // 3000-12-31 23:59:59:000
#	endif 
#else
#	if defined (__LP64__) || defined (_LP64) || defined (__sparcv9)
#		define QN_MAX_TIME_VALUE 32535215999000000L // 2038-01-19 03:14:07:000
#	else
#		define QN_MAX_TIME_VALUE 2147483647000000L // 3000-12-31 23:59:59:000
#	endif
#endif

/// pointer
typedef	void*		QN_LPVOID;
typedef	QN_LPVOID	QN_HANDLE;

/// binary
typedef QN_BYTE*	QN_BINARY;
typedef	QN_LPVOID	QN_BLOB;

/// pointer operator
typedef	intptr_t	QN_INTPTR_T;
typedef	uintptr_t	QN_UINTPTR_T;	//uintptr_t	is more	safe than	intptr_t --	solaris
typedef	ptrdiff_t	QN_PTRDIFF_T;

///	result
typedef	QN_INT32	QN_RESULT;


//==============================================================================
#endif // QNCOMMON_QN_BASICTYPES_H
//==============================================================================
