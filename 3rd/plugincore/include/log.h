/*!
* @brief   
* @project plugincore
* @file    log.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-25 14:09
*/
//==============================================================================
#ifndef PLUGINCORE_LOG_H
#define PLUGINCORE_LOG_H
//==============================================================================

#include <stdarg.h>
#include <string>

#include <glog/logging.h>
#include <glog/log_severity.h>

#include <qn_errors.h>
#include <stdio.h>

class CGlog {
  public:

    static CGlog &instance() {
        static CGlog instance;
        return instance;
    }
    
    ~CGlog() = default;

    QN_RESULT init() {
        QnRtnCode rc;
    
        return rc;
    }

    void glogMsg(QN_UINT32 severity, const char *format, ...) {
        va_list arglist;
        va_start(arglist, format);
        vsprintf_s(m_logBuf, sizeof(m_logBuf), format, arglist);
        switch (severity) {
        case 0: LOG(INFO) << m_logBuf;break;
        case 1: LOG(WARNING) << m_logBuf; break;
        case 2: LOG(ERROR) << m_logBuf; break;
        case 3: LOG(FATAL) << m_logBuf; break;

        default: break;
        }
        va_end(arglist);

        memset(m_logBuf, 0, sizeof(m_logBuf));
    }


  private:
    CGlog() {
        memset(m_logBuf, 0, sizeof(m_logBuf));
    }

  private:
    char m_logBuf[2048];
};

#define PC_INFO(...) CGlog::instance().glogMsg(0, __VA_ARGS__);
#define PC_WARN(...) CGlog::instance().glogMsg(1, __VA_ARGS__);
#define PC_ERROR(...) CGlog::instance().glogMsg(2, __VA_ARGS__);
#define PC_FATAL(...) CGlog::instance().glogMsg(3, __VA_ARGS__);

//==============================================================================
#endif // PLUGINCORE_LOG_H
//==============================================================================
