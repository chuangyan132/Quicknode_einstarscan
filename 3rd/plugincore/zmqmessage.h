/*!
* @brief   
* @project plugincore
* @file    zmqmessage.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-15 14:56
*/
//==============================================================================
#ifndef PLUGINCORE_ZMQMESSAGE_H
#define PLUGINCORE_ZMQMESSAGE_H
//==============================================================================

#include <zhelpers.h>
#include "qn_errDef.h"


class ZmqMessage {
  public:
    explicit ZmqMessage() {
        zmq_msg_init(&m_msg);
    }

    explicit ZmqMessage(size_t size) {
        zmq_msg_init_size(&m_msg, size);
    }

    ~ZmqMessage() {
        zmq_msg_close(&m_msg);
    }

    ZmqMessage(const ZmqMessage& ohter) {
        zmq_msg_init(&m_msg);
        zmq_msg_copy(&m_msg, &(ohter.m_msg));
    }

    zmq_msg_t *native_handle() {
        return &m_msg;
    }

    void *data() const {
        return zmq_msg_data(&m_msg);
    }

    const char *data() {
        return static_cast<char *>(zmq_msg_data(&m_msg));
    }

    char *chArray() const {
        return static_cast<char *>(zmq_msg_data(&m_msg));
    
    }

    size_t size() const {
        return zmq_msg_size(&m_msg);
    }

  private:
    mutable zmq_msg_t m_msg;
};

//QN_RESULT read(void *socket, ZmqMessage *const msg) {
//    if (!socket || !msg)
//        return QN_INVALIDARG;
//
//    QN_RESULT rc = zmq_msg_recv(msg->native_handle(), socket, 0);
//    if (rc != -1) {
//        return QN_OK;
//    } else {
//        return QN_FAIL;
//    }
//}
//
//QN_RESULT write(void *socket, const ZmqMessage *msg) {
//
//    QN_RESULT rc = zmq_send(socket, msg->data(), msg->size(), 0);
//    if (rc != -1) {
//        return QN_OK;
//    } else {
//        return QN_FAIL;
//    }
//}


//==============================================================================
#endif // PLUGINCORE_ZMQMESSAGE_H
//==============================================================================
