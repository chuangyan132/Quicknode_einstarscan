/*!
* @brief   
* @project plugincore
* @file    CMQSocket.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-17 15:18
*/
//==============================================================================
#ifndef PLUGINCORE_UNIMQSOCKET_H
#define PLUGINCORE_UNIMQSOCKET_H
//==============================================================================

#include <zhelpers.h>
#include <memory>
#include <atomic>
#include <cassert>
#include <string>
#include <qn_errDef.h>
#include <qn_errors.h>


class MQErrorHandler {
  public:
    
    static QnRtnCode convertZMQCodeToQnResult(const std::string &func, int code, const std::string &info) {
        switch (code) {
        case EAGAIN: return QN_TIMEOUT;
        default: {
            printf("ZeroMQ return error, func[%s], code[%d], info[%s, %s]", func.c_str(), code,
                      mq_strerror(code), info.c_str());
            return QN_FAIL;
        }
        }
    }

    static int mq_errno() {
#ifdef WIN32
        return zmq_errno();
#elif defined __linux__
        return errno;
#endif // WIN32
    }

    static const char *mq_strerror(int code) {
#ifdef WIN32
        return zmq_strerror(code);
#elif defined __linux__
        return strerror(code);
#endif // WIN32
    }
};

#define QnResultFromZMQCodeWith(code, info) MQErrorHandler::convertZMQCodeToQnResult(__FUNCTION__, code, info)

#define QnResultFromZMQCode(code) QnResultFromZMQCodeWith(code, "")



class CMQSocket {
  public:
    enum Status
    {
        LISTENING,
        CONNECTED,
        CLOSED,
        DISCONNECTED
    };

    explicit CMQSocket(void *context, int type) 
        : m_state(CLOSED), m_context(context), m_socket(zmq_socket(m_context, type)) {
    }

    CMQSocket(const CMQSocket &socket) = delete;
    CMQSocket &operator=(const CMQSocket &socket) = delete;

    virtual ~CMQSocket() {
    
    }

    void *native_handle() {
        return m_socket;
    }

    template <typename T> 
    int setSockopt(int option_name, const T *option_value, size_t option_len) {

        assert(m_socket != nullptr);
        int rc = zmq_setsockopt(m_socket, option_name, static_cast<const void *>(option_value), option_len);
        return rc == 0 ? UNI_OK : UniResultFromZMQCodeWith(rc, "option_name: " + option_name);
    }

    Status state() const {
        return m_state.load();
    }

    bool isAvailable() const {
        return m_state.load() == CONNECTED || m_state.load() == LISTENING;
    }

    std::string endpoint() const {
        return m_endpoint;
    }


  protected:
    void setState(Status state) {
        m_state = state;
    }

    std::string m_endpoint;

  private:
    std::atomic<Status> m_state;

    void *m_context = nullptr;
    void *m_socket = nullptr;
};

class CMQRepSocket : public CMQSocket {};

class CMQPubSocket : public CMQSocket
{

};

//==============================================================================
#endif // PLUGINCORE_UNIMQSOCKET_H
//==============================================================================
