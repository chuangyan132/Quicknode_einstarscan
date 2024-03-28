//==============================================================================
#ifndef UNIERRORS_UNIERRDEF_H
#define UNIERRORS_UNIERRDEF_H
//==============================================================================

/*!
*  QN_RESULT are 32 bit values layed out as follows:
*
*   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
*   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
*	 +-+-------------+---------------+-----------------------------+
*   |S|  Reserved	 |    Facility   |            code             |
*   +-+-------------+---------------+-----------------------------+
*
*  where
*
*      S - Severity - indicates success/fail
*
*          0 - Success
*          1 - Fail( <0 )
*			
*
*      Reserved(24-30, 7bit) - reserved portion.
*
*      Facility(16-23, 8bit) - is the facility code	
*
*      code(0-15) - is the facility's status code
*/


/// Severity values

#include <qn_basicTypes.h>

#define QN_SUCCESS_MASK    0
#define QN_ERROR_MASK 1

#define QN_ERR_FACILITY_MASK	0x80FF0000

/// Facility表示在对用什么技术，例如由于gRPC/ZMQ产生的错误
/// 不要以模块划分，否则会导致很多含义相同的错误码 
#define QN_FACILITY_NULL                   0	///	一般性Facility
#define QN_FACILITY_PLUGIN	               1	/// QtPlugin Facility
#define QN_FACILITY_RPC                    2	///	远程通讯Facility
#define QN_FACILITY_STL                    4	///	STL Facility



#define MAKE_QN_ERROR_RESULT(fac,code) \
	((QN_RESULT)(((QN_INT32)(QN_ERROR_MASK) << 31) | ((QN_INT32)(fac) << 16) | ((QN_INT32)(code))))

/// Success
#define QN_OK			((QN_RESULT)0x00000000)
#define QN_S_FALSE		((QN_RESULT)0x00000001) /// 该错误码表示一种中间正确状态，已经处理，但没有影响
#define QN_REDUNDANCY_OK	((QN_RESULT)0x00000002) /// 该错误码表示主备都可以正确处理，客户端不用处理该错误码，服务端自动转为QN_OK返回

/// 判断是否成功
#define QN_SUCCEEDED(r)	(((QN_RESULT)(r)) >= 0)


//==============================================================================
//	通常错误码
//==============================================================================
/// An unspecified failure has occurred.
#define QN_FAIL ((QN_RESULT)0x80000001) //MAKE_QN_ERROR_RESULT( QN_FACILITY_NULL, 1 )

/// The operation was aborted because of an unspecified error.
#define QN_ABORT ((QN_RESULT)0x80000002) 

/// One or more arguments are invalid.
#define QN_INVALIDARG ((QN_RESULT)0x80000003)

/// The method failed to allocate necessary memory.
#define QN_OUTOFMEMORY ((QN_RESULT)0x80000004)

/// The data necessary to complete the operation is not yet available.
#define QN_PENDING ((QN_RESULT)0x80000005)

/// An invalid pointer was used.
#define QN_E_POINTER ((QN_RESULT)0x80000006)

/// A catastrophic failure has occurred.
#define QN_UNEXPECTED ((QN_RESULT)0x80000007)

/// A general access-denied error.
#define QN_ACCESSDENIED ((QN_RESULT)0x80000008)

/// An invalid handle was used.
#define QN_E_HANDLE ((QN_RESULT)0x80000009)

/// not supported yet
#define QN_NOT_SUPPORTED ((QN_RESULT)0x8000000a)

///  object already exist
#define QN_ALREADY_EXIST ((QN_RESULT)0x8000000b)

///  object already exist
#define QN_NOT_FIND ((QN_RESULT)0x8000000c)

/// unknown service type
#define QN_SERVICE_UNKNOWN_TYPE ((QN_RESULT)0x8000000d)

/// unknown service type
#define QN_SERVICE_NOT_EXIST ((QN_RESULT)0x8000000e)

/// buffer too small
#define QN_BUFFER_SMALL ((QN_RESULT)0x8000000f)

/// some overflow
#define QN_OVERFLOW ((QN_RESULT)0x80000010)

/// Unknown data type
#define QN_INVALID_DATATYPE ((QN_RESULT)0x80000011)

/// exceeds size limit
#define QN_EXCEEDS_LIMIT ((QN_RESULT)0x80000012)

/// object out of date
#define QN_OUT_OF_DATE ((QN_RESULT)0x80000013)

/// Library used incorrectly
#define QN_MISUSE ((QN_RESULT)0x80000014)

/// Session invalid
#define QN_INVALID_SESSION ((QN_RESULT)0x80000015)

/// Timeout
#define QN_TIMEOUT ((QN_RESULT)0x80000016)

/// License not find or some error
#define QN_LICENSE_FAILED ((QN_RESULT)0x80000017)

/// license limited
#define QN_LICENSE_LIMITED ((QN_RESULT)0x80000018)

/// busy
#define QN_BUSY ((QN_RESULT)0x80000019)

/// offline, devices...
#define QN_OFFLINE ((QN_RESULT)0x8000001a)

/// The program issued a command but the command length is incorrect..
#define QN_BAD_LENGTH ((QN_RESULT)0x8000001b)


//==============================================================================
//	服务相关错误码
//==============================================================================
/// Service not available, usually network failed
#define QN_SVC_NOT_AVAILABLE ((QN_RESULT)0x80030000)

/// Start up required.
#define QN_NOT_STARTUP ((QN_RESULT)0x80030001)

/// Service Exception cached
#define QN_EXCEPTION_REMOTE ((QN_RESULT)0x80030002)

/// Local Exception cached
#define QN_EXCEPTION_LOCAL ((QN_RESULT)0x80030003)

/// running is slaver mode
#define QN_STAND_BY ((QN_RESULT)0x80030004)

/// Login Denied
#define QN_LOGIN_DENIED ((QN_RESULT)0x80030005)

/// System Constraint error
#define QN_SYSTEM_CONSTRAINT ((QN_RESULT)0x80030006)


/// <std 异常错误码>
#define STL_UNKNOWN						  ((QN_RESULT)0x80040000)
#define STL_BAD_ALLOC	                      ((QN_RESULT)0x80040001)
#define STL_BAD_CAST	                      ((QN_RESULT)0x80040002)
#define STL_BAD_TYPEID	                      ((QN_RESULT)0x80040003)
#define STL_DOMAIN_ERROR	                  ((QN_RESULT)0x80040004)
#define STL_INVALID_ARGUMENT                  ((QN_RESULT)0x80040005)
#define STL_LENGTH_ERROR	                  ((QN_RESULT)0x80040006)
#define STL_OUT_OF_RANGE	                  ((QN_RESULT)0x80040007)
#define STL_IOS_BASE_FAILURE                  ((QN_RESULT)0x80040008)
#define STL_RANGE_ERROR		                ((QN_RESULT)0x80040009)
#define STL_OVERFLOW_ERROR	                  ((QN_RESULT)0x8004000a)
#define STL_UNDERFLOW_ERROR	                  ((QN_RESULT)0x8004000b)
#define STL_BAD_EXCEPTION	                  ((QN_RESULT)0x8004000c)

//==============================================================================
#endif // UNIERRORS_UNIERRDEF_H
//==============================================================================
