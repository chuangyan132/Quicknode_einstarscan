
#include <iostream>
#include <fstream>
#include <thread>
#include <qn_plugin.h>
#include <jsoncpp/json/json-forwards.h>
#include <jsoncpp/json/json.h>

#include "pluginbussiness.h"
#include "basenode.h"
#include "qn_errors.h"
#include "plugincorehelper.h"


PluginBussiness::PluginBussiness() {
}

PluginBussiness::~PluginBussiness() {
}

int PluginBussiness::init(int argc, char *argv[]) {
    std::string host, pluginName;
    int port = 0;
    if (argc >= 2) {
        host = argv[1];
		port = std::stoi(argv[2]);
    } else {
        host = "127.0.0.1";
        port = 10086;
    }
    std::string appName;
#ifdef WIN32
    std::string _path = argv[0];
    std::string film1("\\");
    std::string film2(".");
    size_t pos = _path.find_last_of(film1);
    pos = _path.find_last_of(film2);
    appName = _path.substr(_path.find_last_of(film1) + film1.length(),
                             _path.find_last_of(film2) - (_path.find_last_of(film1) + film2.length()));
#elif __linux__

#endif
    ::qn_set_plugin_name(appName.c_str());
    int rc = ::qn_init(host.c_str(), port);
    int major, minor, patch = 0;
    qn_version(&major, &minor, &patch);
    std::cout << "Plugin Core Version: " << major << "." << minor << "." << patch << std::endl;
    return rc;
}

int PluginBussiness::fini() {
    return ::qn_cleanup();
}

int PluginBussiness::sendNodesDesc() {
    return 0;
}

void PluginBussiness::onHandleMessage(const char *data, size_t len, void *pUser) {
    Json::Reader jsonReader;
    Json::Value root;
    if (!jsonReader.parse(data, root, true)){
        std::cout << "!!! Json Error !!!" << std::endl;
        return;
    } 
    else {
        std::string cmdUuid, cmd;
        if (!root["cmdUuid"].isString())
            return;
        if (!root["cmd"].isString())
            return;
        cmdUuid = root["cmdUuid"].asString();
        cmd = root["cmd"].asString();
        if (root["data"].isObject())
        {
            PluginBussiness *pluginBusiness = (PluginBussiness *)pUser;

            Json::Value jsonData = root["data"];

            if (cmd.compare("restart") == 0) {
                pluginBusiness->onRestart(data, len);
            }    
            else if (cmd.compare("create") == 0){
                pluginBusiness->onCreate(data, len);
            } 
            else if (cmd.compare("setInputs") == 0) {
                pluginBusiness->onSetInputs(data, len);
            } 
            else if (cmd.compare("apply") == 0) {
                pluginBusiness->onApply(data, len);
            } 
            else if (cmd.compare("getOutputs") == 0) {
                pluginBusiness->onGetOutpus(data, len);
            } 
            else if (cmd.compare("setProperties") == 0) {
                pluginBusiness->onSetProperties(data, len);
            } 
            else if (cmd.compare("getProperties") == 0) {
                pluginBusiness->onProperties(data, len);
            } 
            else if (cmd.compare("viewDatas") == 0) {
                pluginBusiness->onViewDatas(data, len);
            } 
            else {
            }
        }

    }
    
}

void PluginBussiness::onHandleLog(int level, const char *log, void *pUser) {
}

void PluginBussiness::onRestart(const char *data, size_t len) {
}

void PluginBussiness::onCreate(const char *data, size_t len) {

    CreateCommandHelp helper(data, len);

    std::string cmdUUid = helper.commandUuid();
    std::string nodeClass = helper.nodeClass();
    std::string nodeId = helper.nodeId();

	auto itr = m_nodes.find(nodeId);
    if ( itr == m_nodes.end())
    {
        auto className = ClassFactory::instance().className(nodeClass);
        if (!className.empty())
        {
            auto nodeObj = ClassFactory::instance().createObjectByName(className);
            m_nodes.emplace(nodeId, (BaseNode *)nodeObj);
        }
    }
    

    std::string reply = helper.generateReply();
    qn_send_command(reply.c_str(), (int)reply.length());
}

void PluginBussiness::onSetInputs(const char *data, size_t len) {

    SetInputsCommandHelp helper(data, len);
    std::string nodeId = helper.nodeId();

	auto itr = m_nodes.find(nodeId);
    if ( itr != m_nodes.end()) {
        auto nodeObj = itr->second;

        QnRtnCode rc = nodeObj->setInput(helper.dataStyledString());

        std::string reply = helper.generateReply(rc);
        qn_send_command(reply.c_str(), (int)reply.length());
    }
}

void PluginBussiness::onApply(const char *data, size_t len) {

    ApplyCommandHelper helper(data, len);
    std::string nodeId = helper.nodeId();

	auto itr = m_nodes.find(nodeId);
    if ( itr != m_nodes.end()) {
        auto nodeObj = itr->second;

        QnRtnCode rc = nodeObj->apply();
        
        std::string reply = helper.generateReply(rc);
        qn_send_command(reply.c_str(), (int)reply.length());
    }
}

void PluginBussiness::onGetOutpus(const char *data, size_t len) {

    GetOutpusCommandHelper helper(data, len);
    std::string nodeId = helper.nodeId();

	auto itr = m_nodes.find(nodeId);
	if ( itr != m_nodes.end()) {
        auto nodeObj = itr->second;
        
        std::vector<std::string> datas;
        auto outputsName = helper.outputsName();
        for (auto &name : outputsName)
        {
            auto items = nodeObj->output(name);
            datas.push_back(items);
        }

        std::string reply = helper.generateReply(datas);
        qn_send_command(reply.c_str(), (int)reply.length());
    }
}

void PluginBussiness::onSetProperties(const char *data, size_t len) {
    SetPropertiesCommandHelp helper(data, len);
    std::string nodeId = helper.nodeId();

	auto itr = m_nodes.find(nodeId);
    if (itr != m_nodes.end()) {
        auto nodeObj = itr->second;

        QnRtnCode rc = nodeObj->setProperty(helper.dataStyledString());

        std::string reply = helper.generateReply(rc);
        qn_send_command(reply.c_str(), (int)reply.length());
    }
}

void PluginBussiness::onProperties(const char *data, size_t len) {
}

void PluginBussiness::onViewDatas(const char *data, size_t len) {
}

