/*!
* @brief   
* @project plugincore
* @file    rpcmanager.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-01 18:29
*/
//==============================================================================
#ifndef PLUGINCORE_RPCMANAGER_H
#define PLUGINCORE_RPCMANAGER_H
//==============================================================================

#include <memory>
#include <atomic>
#include <thread>
#include <functional>
#include <qn_errors.h>
#include <mosquitto.h>

#include "qn_plugin.h"
#include "../../include/fifo.h"


class QnRpcManager final {
  public:
    ~QnRpcManager();

    static QnRpcManager& instance();

    int setLogPath(const char *path);
    QN_RESULT printLog(int priority, const char *fmt, ...);


    QN_RESULT init(const char *repAddress, const char *pubAddress);
    QN_RESULT init(const char *host, int port);
    QN_RESULT fini();

    QN_RESULT setPluginName(const char *pluginName);

    QN_RESULT send_command(const std::string &data);

    QN_RESULT setMessageCallback(void (*on_message)(const char *, size_t , void *), void *pUser);
    QN_RESULT setLogCallback(void (*on_log)(int, const char *, void *), void *pUser);


  private:
    QnRpcManager();
    QnRpcManager(const QnRpcManager &) = delete;
    QnRpcManager(QnRpcManager &&) = delete;
    QnRpcManager &operator=(QnRpcManager const &) = delete;
    QnRpcManager &operator=(QnRpcManager &&) = delete;

  private:
    inline bool isInit() const { return (m_initCount.load() > 0); }
    inline bool isRun() const { return isInit(); }

    QN_RESULT logInit(const char *filename);
    QN_RESULT mosquittoInit(const char *host, int port);

    void handlePlayloadLoop();

    std::string generateWillPayload();

    std::string generateReplyMessage(const std::string &msg);

    std::function<void(const char *data, size_t len, void *pUser)> m_handleEvent;

    std::mutex m_mtx_msg;
    std::mutex m_mtx_log;
    std::function<void(const char *data, size_t len, void *pUser)> m_msgEvent = nullptr;
    std::function<void(int level, const char *log, void *pUser)> m_logEvent = nullptr;


  private:
      static void onConnect(struct mosquitto *mosq, void *obj, int result);
      static void onDisconnect(struct mosquitto *mosq, void *obj, int result);
      static void onMessage(mosquitto *mosq, void *obj, const struct mosquitto_message *message);
      
  private:
    std::atomic_int m_initCount;
    
    void *m_context;
    void *m_publisher = nullptr;
    void *m_responder = nullptr;
    void *m_subcriber = nullptr;
    void *m_requester = nullptr;

    mosquitto *m_mosq = nullptr;
    std::string m_mosq_host;
    int m_mosq_port = 0;

    std::string m_pluginName;

    FIFO<std::string> m_fifo;
    std::string m_topic_request;
    std::string m_topic_reply;

    std::thread m_handleEvent_thread;
    std::thread m_popUniMsg_thread;

    void *m_pUser = nullptr;
    void *m_pUser_log = nullptr;

    std::string m_logPath;
    std::string m_logName;
};
#define QN_RPC_MANAGER() QnRpcManager::instance()

//==============================================================================
#endif // PLUGINCORE_RPCMANAGER_H
//==============================================================================
