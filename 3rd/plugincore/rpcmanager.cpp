/*!
* @brief   
* @project plugincore
* @file    UniRpcEnv.cpp
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-20 17:11
*/
#include "rpcmanager.h"
#include <iostream>
#include <thread>
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/json-forwards.h>

#include "qn_defines.h"
#include "logging.h"

QnRpcManager::QnRpcManager() : m_context(nullptr) {
}

QnRpcManager::~QnRpcManager() {
    fini();
}

QnRpcManager& QnRpcManager::instance() 
{
    static QnRpcManager instance;
    return instance;
}

int QnRpcManager::setLogPath(const char *path) {
#ifdef WIN32
    std::string _path = path;
    std::string film1("\\");
    std::string film2(".");
    size_t pos = _path.find_last_of(film1);
    pos = _path.find_last_of(film2);
    m_logName = _path.substr(_path.find_last_of(film1) + film1.length(),
                             _path.find_last_of(film2) - (_path.find_last_of(film1) + film2.length()));

    m_pluginName = m_logName;
#elif __linux__

#endif
    return 0;
}

QN_RESULT QnRpcManager::printLog(int priority, const char *fmt, ...) {
    va_list va;
    char *s;
    int len;

    std::unique_lock<std::mutex> lock(m_mtx_log);
    if (m_logEvent)
    {
        len = strlen(fmt) + 512;
        s = new (std::nothrow) char[len * sizeof(char)];
        if (!s)
            return QN_FAIL;

        va_start(va, fmt);
        vsnprintf(s, len, fmt, va);
        va_end(va);
        s[len - 1] = '\0';
        m_logEvent(priority, s, m_pUser_log);
    }

    return 0;
}

QN_RESULT QnRpcManager::init(const char *repAddress, const char *pubAddress) {

    return QN_OK;
}

QN_RESULT QnRpcManager::init(const char *host, int port) {

    if (!host || port < 0) {
        printLog(QN_LOG_ERROR, "INVALIDARG");
        return QN_INVALIDARG;
    }

    if (isInit())
        return -1;
    
    // logInit(("D:\\logs\\" + m_logName).c_str());

    QnRtnCode rc;
    if((rc = mosquittoInit(host, port)) != QN_OK) return rc;
    
    m_popUniMsg_thread = std::thread(&QnRpcManager::handlePlayloadLoop, this);
    
    m_initCount++;

    return QN_OK;
}

QN_RESULT QnRpcManager::fini() {
    if (isInit())
        m_initCount--;
    else 
        return QN_OK;

    m_fifo.close();

    if (m_popUniMsg_thread.joinable())
        m_popUniMsg_thread.join();

    if (m_handleEvent_thread.joinable())
        m_handleEvent_thread.join();


    int rc = mosquitto_disconnect(m_mosq);
    if (rc != MOSQ_ERR_SUCCESS) {
        // PC_INFO("mosquitto_disconnect failed");
        return QN_FAIL;
    }

    rc = mosquitto_loop_stop(m_mosq, true);
    if (rc != MOSQ_ERR_SUCCESS) {
        // PC_INFO("mosquitto_loop_stop failed");
        return QN_FAIL;
    }
    mosquitto_destroy(m_mosq);
    m_mosq = nullptr;

    // // PC_INFO("========== Glog End ==========");
    // google::ShutdownGoogleLogging();

    return 0;
}

QN_RESULT QnRpcManager::setPluginName(const char *pluginName) {
    m_pluginName = pluginName;
    return QN_OK;
}

QN_RESULT QnRpcManager::send_command(const std::string &data) {
    std::cout << "send command: " << std::endl << data << std::endl;
    // // PC_INFO("send command: \n %s", data.c_str());

    int rc = mosquitto_publish(m_mosq, nullptr, m_topic_reply.c_str(), int(data.size()), data.c_str(),
                               DEFAULT_QOS, false);
    if (rc != MOSQ_ERR_SUCCESS){
        // // PC_INFO("mosquitto_publish failed");
        return QN_FAIL;
    }
}

QN_RESULT QnRpcManager::setMessageCallback(void (*on_message)(const char *, size_t, void *), void *pUser) {
    std::unique_lock<std::mutex> lock(m_mtx_msg);
    m_msgEvent = on_message;
    m_pUser = pUser;

    std::string endAddr("127.0.0.1");
    int port = 10086;
    int rc = mosquitto_connect_async(m_mosq, endAddr.c_str(), port, DEFAULT_KEEPALIVE);
    if (rc != MOSQ_ERR_SUCCESS) {
        // // PC_INFO("mosquitto_connect_async failed[%d, %s]", rc, mosquitto_strerror(rc));
        return QN_FAIL;
    }

    return QN_OK;
}

QN_RESULT QnRpcManager::setLogCallback(void (*on_log)(int, const char *, void *), void *pUser) {

    std::unique_lock<std::mutex> lock(m_mtx_log);
    m_logEvent = on_log;
    m_pUser_log = pUser;

    return QN_OK;
}

void QnRpcManager::handlePlayloadLoop() {
    std::thread::id this_id = std::this_thread::get_id(); // = GetCurrentThreadId()
    // // PC_INFO("..... Run handlePlayloadLoop: [0x%.08x] .....", this_id);
    printLog(QN_LOG_DEBUG, "..... Run handlePlayloadLoop: [0x%.08x] .....", this_id);

    std::string req;
    while (m_fifo.pop(req)) {
        // PC_INFO("pop req from inner-fifo: \n %s", req.c_str());
        auto reqAck = generateReplyMessage(req);
        send_command(reqAck);
        std::unique_lock<std::mutex> lock(m_mtx_msg);
        if (m_msgEvent)
            m_msgEvent(req.c_str(), req.size(), m_pUser);
    }

    // PC_INFO("..... Terminate handlePlayloadLoop: [0x%.08x] .....", this_id);
    printLog(QN_LOG_DEBUG, "..... Terminate handlePlayloadLoop: [0x%.08x] .....", this_id);
}

std::string QnRpcManager::generateWillPayload() {
    std::string cmdUuid = "will-uuid-123456";
    std::string cmd = "will";

    Json::Value root;
    Json::Value jsonData;

    root["cmdUuid"] = cmdUuid;
    root["cmd"] = cmd;
    root["result"] = 0;
    root["why"] = "";

    jsonData["online"] = false;
    root["data"] = jsonData;

    Json::StreamWriterBuilder jswBuilder;
    jswBuilder["emitUTF8"] = true;
    std::unique_ptr<Json::StreamWriter> jsWriter(jswBuilder.newStreamWriter());
    std::ostringstream os;
    jsWriter->write(root, &os);

    return os.str();
}

QN_RESULT QnRpcManager::logInit(const char *filename) {

    //google::InitGoogleLogging("test-123456");
    //google::SetLogDestination(0, filename);
    //google::SetLogFilenameExtension(".log");
    ////FLAGS_stderrthreshold = google::GLOG_INFO;
    //FLAGS_colorlogtostderr = true;
    //FLAGS_logbufsecs = 0;

    // PC_INFO("========== Glog Start ==========");

    return QN_OK;
}

QN_RESULT QnRpcManager::mosquittoInit(const char *host, int port) {
    if (!host || port < 0) {
        printLog(QN_LOG_ERROR, "INVALIDARG");
        return QN_INVALIDARG;      
    }

    m_mosq_host = host;
    m_mosq_port = port;

    int major, minor, revision = 0;
    int rc = mosquitto_lib_version(&major, &minor, &revision);
    // PC_INFO("Mosquitto Version: %d.%d.%d", major, minor, revision);
    printLog(QN_LOG_INFO , "Mosquitto Version: %d.%d.%d", major, minor, revision);

    // 1. init mosquitto client lib
    rc = mosquitto_lib_init();
    if (rc != MOSQ_ERR_SUCCESS) {
        // PC_INFO("mosquitto_lib_init failed");
        printLog(QN_LOG_ERROR, "mosquitto_lib_init failed");
        return QN_FAIL;
    }

    // 2. create a new mosquitto client instance, id = NULL
    m_mosq = mosquitto_new(nullptr, true, this);
    if (!m_mosq) {
        // PC_INFO("mosquitto_new failed");
        printLog(QN_LOG_ERROR, "mosquitto_new failed");
        return QN_FAIL;
    }

    // 3. set callbacks
    mosquitto_connect_callback_set(m_mosq, this->onConnect);
    mosquitto_disconnect_callback_set(m_mosq, this->onDisconnect);
    mosquitto_message_callback_set(m_mosq, this->onMessage);

    // 4. configure will information for m_mosq instance
    std::string topicWill(TOPIC_WILL);
    std::string willPayload = generateWillPayload();

    auto header = topicWill.substr(0, topicWill.find("*"));
    auto temp = topicWill.substr(topicWill.find("*") + 1, topicWill.length() - topicWill.find('*'));
    topicWill = header + m_pluginName + temp;

    rc = mosquitto_will_set(m_mosq, topicWill.c_str(), int(willPayload.size()), willPayload.c_str(), DEFAULT_QOS, true);
    if (rc != MOSQ_ERR_SUCCESS) {
        // PC_INFO("mosquitto_will_set failed");
        printLog(QN_LOG_ERROR, "mosquitto_will_set failed");
        return QN_FAIL;
    }

    // 5. start a new thread to process network traffic
    rc = mosquitto_loop_start(m_mosq);
    if (rc != MOSQ_ERR_SUCCESS) {
        // PC_INFO("mosquitto_loop_start failed");
        printLog(QN_LOG_ERROR, "mosquitto_loop_start failed");
        return QN_FAIL;
    }

    return QN_RESULT();
}

std::string QnRpcManager::generateReplyMessage(const std::string &msg) {

    Json::Reader reader;
    Json::Value root;

    printLog(QN_LOG_DEBUG, "Request-Cotent ---> %s", msg.c_str());

    std::string nodeId;
    if (reader.parse(msg, root)) {
        if (root.isMember("data")) {
            if (root["data"].isMember("nodeId"))
                if (root["data"]["nodeId"].isString())
                    nodeId = root["data"]["nodeId"].asString();
            root.removeMember("data");
            root["nodeId"] = nodeId;
        }

        root["status"] = "OK";

        Json::StreamWriterBuilder jswBuilder;
        jswBuilder["emitUTF8"] = true;
        std::unique_ptr<Json::StreamWriter> jsWriter(jswBuilder.newStreamWriter());
        std::ostringstream os;
        jsWriter->write(root, &os);

        printLog(QN_LOG_DEBUG, "Reply-Cotent ---> %s", os.str().c_str());

        return os.str();
    };

    return std::string();
}

void QnRpcManager::onConnect(mosquitto *mosq, void *obj, int result) {
    QnRpcManager* handler = static_cast<QnRpcManager *>(obj);

    // PC_INFO("connect success");
    handler->printLog(QN_LOG_INFO, "Connect mqtt-server success");

    auto dumpTopic = [](const std::string &data, const std::string &plugigName) -> std::string {
        auto header = data.substr(0, data.find("*"));
        auto temp = data.substr(data.find("*") + 1, data.length() - data.find('*'));
        return header + plugigName + temp;
    };
    handler->m_topic_request = dumpTopic(TOPIC_REQUEST, handler->m_pluginName);
    handler->m_topic_reply = dumpTopic(TOPIC_REPLY, handler->m_pluginName);

    int rc = mosquitto_subscribe(mosq, nullptr, handler->m_topic_request.c_str(), DEFAULT_QOS);
    if (rc != MOSQ_ERR_SUCCESS) {
        // PC_INFO("mosquitto_subscribe failed[%s]", handler->m_topic_request.c_str());
        handler->printLog(QN_LOG_ERROR, "mosquitto_subscribe failed[%s]", handler->m_topic_request.c_str());
        return;
    }
}

void QnRpcManager::onDisconnect(mosquitto *mosq, void *obj, int result) {
    QnRpcManager *handler = static_cast<QnRpcManager *>(obj);

    // PC_INFO("Disconnect");
    handler->printLog(QN_LOG_ERROR, "Disconnect to mqtt-server ");
}

void QnRpcManager::onMessage(mosquitto *mosq, void *obj, const mosquitto_message *message) {
    QnRpcManager *handler = static_cast<QnRpcManager *>(obj);
    std::string invalidTopic = handler->m_topic_request;

    std::string topic(message->topic);

    if (0 == topic.compare(invalidTopic))
    {
        std::string payload(static_cast<char *>(message->payload), message->payloadlen);
        handler->m_fifo.push(payload);
        handler->printLog(QN_LOG_DEBUG, "Receive mqtt message:[topic = %s][payload = %s]", topic.c_str(),
                          payload.c_str());
    }
    
}

