/*!
* @brief   
* @project QuickNodes
* @file    qn_errors.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-20 17:10
*/
//==============================================================================
#ifndef QUICKNODES_QN_ERRORS_H
#define QUICKNODES_QN_ERRORS_H
//==============================================================================

#include <qn_errDef.h>

///	WSTR
#ifndef WSTR
#   define WSTR(x) L##x
#endif

class CQnResultToString {
public:
    static const QN_WCHAR *QnResultToString(QN_RESULT res) {
#define RETURN_WIDE_CODE(x) \
    case x: return WSTR(#x)
      switch (res) {
          /// 通常错误
          RETURN_WIDE_CODE(QN_OK);
          RETURN_WIDE_CODE(QN_S_FALSE);
          RETURN_WIDE_CODE(QN_FAIL);
          RETURN_WIDE_CODE(QN_ABORT);
          RETURN_WIDE_CODE(QN_INVALIDARG);
          RETURN_WIDE_CODE(QN_OUTOFMEMORY);
          RETURN_WIDE_CODE(QN_PENDING);
          RETURN_WIDE_CODE(QN_E_POINTER);
          RETURN_WIDE_CODE(QN_UNEXPECTED);
          RETURN_WIDE_CODE(QN_ACCESSDENIED);
          RETURN_WIDE_CODE(QN_E_HANDLE);
          RETURN_WIDE_CODE(QN_NOT_SUPPORTED);
          RETURN_WIDE_CODE(QN_ALREADY_EXIST);
          RETURN_WIDE_CODE(QN_NOT_FIND);
          RETURN_WIDE_CODE(QN_SERVICE_UNKNOWN_TYPE);
          RETURN_WIDE_CODE(QN_SERVICE_NOT_EXIST);
          RETURN_WIDE_CODE(QN_BUFFER_SMALL);
          RETURN_WIDE_CODE(QN_OVERFLOW);
          RETURN_WIDE_CODE(QN_INVALID_DATATYPE);
          RETURN_WIDE_CODE(QN_EXCEEDS_LIMIT);
          RETURN_WIDE_CODE(QN_OUT_OF_DATE);
          RETURN_WIDE_CODE(QN_MISUSE);
          RETURN_WIDE_CODE(QN_INVALID_SESSION);
          RETURN_WIDE_CODE(QN_TIMEOUT);
          RETURN_WIDE_CODE(QN_LICENSE_FAILED);
          RETURN_WIDE_CODE(QN_LICENSE_LIMITED);
          RETURN_WIDE_CODE(QN_BUSY);
          RETURN_WIDE_CODE(QN_OFFLINE);
          RETURN_WIDE_CODE(QN_BAD_LENGTH);

          /// 服务调用错误
          RETURN_WIDE_CODE(QN_SVC_NOT_AVAILABLE);
          RETURN_WIDE_CODE(QN_NOT_STARTUP);
          RETURN_WIDE_CODE(QN_EXCEPTION_REMOTE);
          RETURN_WIDE_CODE(QN_EXCEPTION_LOCAL);
          RETURN_WIDE_CODE(QN_STAND_BY);
          RETURN_WIDE_CODE(QN_LOGIN_DENIED);
          RETURN_WIDE_CODE(QN_SYSTEM_CONSTRAINT);

          ///	stl 异常错误码
          RETURN_WIDE_CODE(STL_UNKNOWN);
          RETURN_WIDE_CODE(STL_BAD_ALLOC);
          RETURN_WIDE_CODE(STL_BAD_CAST);
          RETURN_WIDE_CODE(STL_BAD_TYPEID);
          RETURN_WIDE_CODE(STL_DOMAIN_ERROR);
          RETURN_WIDE_CODE(STL_INVALID_ARGUMENT);
          RETURN_WIDE_CODE(STL_LENGTH_ERROR);
          RETURN_WIDE_CODE(STL_OUT_OF_RANGE);
          RETURN_WIDE_CODE(STL_IOS_BASE_FAILURE);
          RETURN_WIDE_CODE(STL_RANGE_ERROR);
          RETURN_WIDE_CODE(STL_OVERFLOW_ERROR);
          RETURN_WIDE_CODE(STL_UNDERFLOW_ERROR);
          RETURN_WIDE_CODE(STL_BAD_EXCEPTION);
      }

      return WSTR("{UNKNOWN ERROR}");
  }
};



/*!@brief
* Class QnRtnCode
* 返回错误码
*/	
class QnRtnCode
{
public:
	/// 构造函数
	QnRtnCode( )
		: m_rtnCode( QN_OK )
	{
	}

	/// 构造函数
	QnRtnCode( QN_RESULT code )
		: m_rtnCode( code )
	{
	}

	/// 是否为成功错误码
	inline QN_BOOL isSuccess() const
	{ 
		return ( m_rtnCode == QN_OK/* || m_rtnCode == QN_S_FALSE*/ ) ; 
	}

	/// 是否为成功错误码
	inline QN_BOOL isFailed() const
	{ 
		return !this->isSuccess(); 
	}

    inline QN_BOOL isRpcError() const 
	{
        return (MAKE_QN_ERROR_RESULT(QN_FACILITY_RPC, 0) == (QN_RESULT)(QN_ERR_FACILITY_MASK & m_rtnCode) ||
            (m_rtnCode == QN_SVC_NOT_AVAILABLE));
    }


	/// 是否为QtPlugin错误码
	inline QN_BOOL isQPluginError() const
	{
        // to add
		return true;
	}

	/// 获得错误码
	inline QN_RESULT getCode() const
	{
		return m_rtnCode;
	}

	/// 设置错误码
	inline void setCode( QN_RESULT code )
	{
		m_rtnCode = code;
	}

	/// 类型转换操作符
	inline operator QN_RESULT() const
	{
		return m_rtnCode;
	}

	/// 赋值运算符
	inline QnRtnCode& operator=( QN_RESULT code )
	{
		m_rtnCode = code;
		return *this;
	}

	/// ==运算符
	inline bool operator==( QN_RESULT code ) const
	{
		return (m_rtnCode == code);
	}

	/// !=运算符
	inline bool operator!=( QN_RESULT code ) const
	{
		return (m_rtnCode != code);
	}

    const QN_WCHAR *toString() const 
    {
        return CQnResultToString::QnResultToString(m_rtnCode);
    }

protected:
	QN_RESULT	m_rtnCode;

};

//==============================================================================
#endif // QUICKNODES_QN_ERRORS_H
//==============================================================================
