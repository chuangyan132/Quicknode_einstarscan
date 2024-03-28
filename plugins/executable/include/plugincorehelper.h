/*!
* @brief   
* @project qn_exeplugin
* @file    plugincorehelper.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-07 11:22
*/
//==============================================================================
#ifndef QN_EXEPLUGIN_PLUGINCOREHELPER_H
#define QN_EXEPLUGIN_PLUGINCOREHELPER_H
//==============================================================================

#include <memory>
#include <map>
#include <string>
#include "jsoncpp/json/json.h"

typedef void *(*CreateObject)(void);

class ClassFactory
{
  public:
    static ClassFactory &instance() {
        static ClassFactory _instance;
        return _instance;
    }

    void *createObjectByName(std::string className) {
        auto itr = m_classMap.find(className);
        if (itr != m_classMap.end())
            return itr->second();
        else
            return nullptr;
    }

    void registClass(std::string cmdName, std::string className, CreateObject method) {
        m_classMap.emplace(className, method);
        m_nodeClass.emplace(cmdName, className);
    }
    
    std::string className(std::string cmdName) {
		auto itr = m_nodeClass.find(cmdName);
		if (itr != m_nodeClass.end())
            return itr->second;
        return std::string();
    }

  private:
    std::map<std::string, CreateObject> m_classMap;
    std::map<std::string, std::string> m_nodeClass;
};

class RegisterAction
{
  public:
    RegisterAction(std::string cmdName, std::string className, CreateObject createFunc) {
        ClassFactory::instance().registClass(cmdName, className, createFunc);
    }
};

#define REGISTER(cmdName, className)                                   \
    className *objectCreator##className() {                   \
        return new className();                               \
    }                                                         \
    RegisterAction g_creatorRegister##className(              \
        #cmdName, #className, (CreateObject)objectCreator##className);



class BaseCommand
{
  public:
    BaseCommand(const char *data, size_t len) {
        m_jsonReader.parse(std::string(data, len), m_root, true);
        if (!m_root.empty())
            if (m_root.isMember("data"))
                if (m_root["data"].isObject())
                    m_jsonData = m_root["data"];

        m_cmd = queryCommandElement("cmd");
        m_cmdUuid = queryCommandElement("cmdUuid");
        m_nodeId = queryCommandElement("nodeId");
    }

    std::string command() const {
        return m_cmd;
    }

    std::string commandUuid() const {
        return m_cmdUuid;
    }

    std::string nodeId() const {
        return m_nodeId;
    }

    std::string dataStyledString() {
        
        return m_root["data"].isObject() ? m_root["data"].toStyledString() : std::string();
    }

    std::string generateReply(int result = 0, const std::string &why = "") {
        Json::Value root;
        Json::Value jsonData;

        root["cmdUuid"] = m_cmdUuid;
        root["cmd"] = m_cmd + "Reply";
        root["result"] = result;
        root["why"] = why;

        jsonData["nodeId"] = m_nodeId;

        root["data"] = jsonData;

        Json::StreamWriterBuilder jswBuilder;
        jswBuilder["emitUTF8"] = true;
        std::unique_ptr<Json::StreamWriter> jsWriter(jswBuilder.newStreamWriter());
        std::ostringstream os;
        jsWriter->write(root, &os);

        return os.str().c_str();
    }

  protected:
    bool isActive() { return !m_root.empty(); }

    std::string queryCommandElement(std::string element) {
        if (m_root.empty())
            return std::string();

        if (m_root[element].isString())
            return m_root[element].asString();

        if (m_root["data"].isObject())
            if (m_root["data"][element].isString())
                return m_root["data"][element].asString();

        return std::string();
    }

    Json::Reader m_jsonReader;
    Json::Value m_root;
    Json::Value m_jsonData;

    std::string m_cmdUuid;
    std::string m_cmd;
    std::string m_nodeId;
};

class RestartCommandHelp : public BaseCommand {
  public:
    RestartCommandHelp(const char *data, size_t len) 
        : BaseCommand(data, len) {}
};

class CreateCommandHelp : public BaseCommand
{
  public:
    CreateCommandHelp(const char *data, size_t len) 
        : BaseCommand(data, len) {
        m_nodeClass = queryCommandElement("nodeClass");
    }

    std::string nodeClass() const {
        return m_nodeClass;
    }

  private:
    std::string m_nodeClass;
};

class SetInputsCommandHelp : public BaseCommand {
  public:
    SetInputsCommandHelp(const char *data, size_t len) : BaseCommand(data, len) {
        
    }

};

class ApplyCommandHelper : public BaseCommand
{
  public:
    ApplyCommandHelper(const char *data, size_t len) 
        : BaseCommand(data, len) {
    }
    ~ApplyCommandHelper() {
    }

  private:
};

class GetOutpusCommandHelper : public BaseCommand {
  public:
    GetOutpusCommandHelper(const char *data, size_t len) : BaseCommand(data, len) {
    }
    ~GetOutpusCommandHelper() {
    }

    std::string generateReply(std::vector<std::string> outputsData, int result = 0, const std::string &why = "") {
        Json::Value root;
        Json::Value jsonData;

        root["cmdUuid"] = m_cmdUuid;
        root["cmd"] = m_cmd + "Reply";
        root["result"] = result;
        root["why"] = why;

        jsonData["nodeId"] = m_nodeId;
        for (auto &item : outputsData) {
            Json::Value arrayElement;
            m_jsonReader.parse(item, arrayElement, true);
            jsonData["outputs"].append(arrayElement);
        }
        root["data"] = jsonData;

        Json::StreamWriterBuilder jswBuilder;
        jswBuilder["emitUTF8"] = true;
        std::unique_ptr<Json::StreamWriter> jsWriter(jswBuilder.newStreamWriter());
        std::ostringstream os;
        jsWriter->write(root, &os);

        return os.str().c_str();

    }

    std::vector<std::string> outputsName() {
        std::vector<std::string> vec;
        if (!m_jsonData.empty())
            if (m_jsonData.isMember("outputs"))
                if (m_jsonData["outputs"].isArray()){
                    Json::Value value = m_jsonData["outputs"];
                    Json::ArrayIndex count = value.size();
                    for (Json::ArrayIndex i = 0; i < count; i++) {
                        Json::Value arrayElement = value[i];
                        if (arrayElement.isString())
                            vec.push_back(arrayElement.asString());
                    }
                }
        return vec;
    }

  private:
};

class SetPropertiesCommandHelp : public BaseCommand {
  public:
    SetPropertiesCommandHelp(const char *data, size_t len) : BaseCommand(data, len) {
    }
};

class GetPropertiesCommandHelper : public BaseCommand {
  public:
    GetPropertiesCommandHelper(const char *data, size_t len) : BaseCommand(data, len) {
    }
    ~GetPropertiesCommandHelper() {
    }

    std::string generateReply(std::vector<std::string> outputsData, int result = 0, const std::string &why = "") {
        Json::Value root;
        Json::Value jsonData;

        root["cmdUuid"] = m_cmdUuid;
        root["cmd"] = m_cmd + "Reply";
        root["result"] = result;
        root["why"] = why;

        jsonData["nodeId"] = m_nodeId;
        for (auto &item : outputsData) {
            Json::Value arrayElement;
            m_jsonReader.parse(item, arrayElement, true);
            jsonData["properties"].append(arrayElement);
        }
        root["data"] = jsonData;

        Json::StreamWriterBuilder jswBuilder;
        jswBuilder["emitUTF8"] = true;
        std::unique_ptr<Json::StreamWriter> jsWriter(jswBuilder.newStreamWriter());
        std::ostringstream os;
        jsWriter->write(root, &os);

        return os.str().c_str();
    }

    std::vector<std::string> propertiesName() {
        std::vector<std::string> vec;
        if (!m_jsonData.empty())
            if (m_jsonData.isMember("properties"))
                if (m_jsonData["properties"].isArray()) {
                    Json::Value value = m_jsonData["properties"];
                    Json::ArrayIndex count = value.size();
                    for (Json::ArrayIndex i = 0; i < count; i++) {
                        Json::Value arrayElement = value[i];
                        if (arrayElement.isString())
                            vec.push_back(arrayElement.asString());
                    }
                }
        return vec;
    }

  private:
};



//==============================================================================
#endif // QN_EXEPLUGIN_PLUGINCOREHELPER_H
//==============================================================================
