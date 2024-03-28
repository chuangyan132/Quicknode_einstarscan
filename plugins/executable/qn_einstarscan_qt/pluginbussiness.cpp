#include "pluginbussiness.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QThread>
#include <qn_plugin.h>

QMap<QString, const QMetaObject *> m_nodesClassList; // 存储节点类元信息
QMap<QString, HardwareBaseNode *> m_nodesList;       // 存储节点实例
PluginBussiness::PluginBussiness(QObject *parent) : QObject(parent) {
    m_nodesClassList.insert("CreateUuid2", &CreateUuid::staticMetaObject);
    m_nodesClassList.insert("createprocess", &CreateUuid::staticMetaObject);
}

PluginBussiness::~PluginBussiness() {
    fini();
    reset();
}

int PluginBussiness::init(int argc, char *argv[]) {
    std::string host, pluginName;
    int port = 0;
    if (argc >= 2) {
        host = argv[1];
        host = std::stoi(argv[2]);
    } else {
        host = "127.0.0.1";
        port = 10086;
    }
    ::qn_set_log_path(argv[0]);
    int rc = ::qn_init(host.c_str(), port);
    int major, minor, patch = 0;
    qn_version(&major, &minor, &patch);
    return rc;
}

int PluginBussiness::fini() {
    return ::qn_cleanup();
}

void PluginBussiness::reset() {
    for (auto iter = m_nodesList.constBegin(); iter != m_nodesList.constEnd(); iter++) {
        delete iter.value();
    }
    //m_nodesList.clear();
}

void PluginBussiness::onHandleMessage(const char *data, size_t len, void *pUser) {
    QString str = QString::fromLocal8Bit(data);
    qDebug().noquote() << "recv:" << str;
    //QThread::sleep(5);
    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(str.toLocal8Bit(), &error);
    if (error.error != QJsonParseError::NoError) {
        qDebug() << "not json:";
        qDebug() << error.errorString();
        return;
    }
    auto object = doc.object();
    // 处理协议
    if (object.contains("cmd")) {
        if (object.contains("data")) {
            auto data = object.value("data").toObject();
            auto cmd = object.value("cmd").toString();
            auto cmdUuid = object.value("cmdUuid").toString();
            if ("restart" == cmd) {
                qDebug() << "start cmd:" << cmd;
                sendReplyMessage(cmdUuid, cmd, 0, QVariantMap());
            } else if ("create" == cmd) { // 创建节点实例
                auto nodeClass = data.value("nodeClass").toString();
                //auto nodeClass = data.value("nodeId").toString().split("-").last();
                if (!nodeClass.isEmpty() && m_nodesClassList.contains(nodeClass)) {
                    auto nodeId = data.value("nodeId").toString();
                    if (!nodeId.isEmpty()) {
                        auto ptr = qobject_cast<HardwareBaseNode *>(m_nodesClassList.value(nodeClass)->newInstance());
                        m_nodesList.insert(nodeId, ptr);
                        QVariantMap map;
                        map.insert("nodeId", nodeId);
                        sendReplyMessage(cmdUuid, cmd, 0, map);
                    }
                }
            } else if ("getOutputs" == cmd) { // 读取节点输出
                auto nodeId = data.value("nodeId").toString();
                if (!nodeId.isEmpty() && m_nodesList.contains(nodeId)) {
                    QVariantList list;
                    auto outputNames = data.value("outputs").toArray();
                    if (outputNames.size() >= 1) {
                        for (int i = 0; i < outputNames.size(); i++) {
                            QVariantMap map;
                            auto outputName = outputNames.at(i).toString();
                            map.insert("name", outputName);
                            map.insert("isSharedMemory", false);
                            map.insert("value", m_nodesList.value(nodeId)->output(outputName));
                            list.append(map);
                        }
                    }
                    QVariantMap map;
                    map.insert("nodeId", nodeId);
                    map.insert("outputs", list);
                    sendReplyMessage(cmdUuid, cmd, 0, map);
                }
            } else if ("apply" == cmd) { // 执行节点
                auto nodeId = data.value("nodeId").toString();
                if (!nodeId.isEmpty() && m_nodesList.contains(nodeId)) {
                    m_nodesList.value(nodeId)->apply();
                    QVariantMap map;
                    map.insert("nodeId", nodeId);
                    sendReplyMessage(cmdUuid, cmd, 0, map);
                }
            } else if ("setInputs" == cmd) { // 设置节点输入
                auto nodeId = data.value("nodeId").toString();
                if (!nodeId.isEmpty() && m_nodesList.contains(nodeId)) {
                    auto list = data.value("inputs").toArray().toVariantList();
                    if (list.size() > 0) {
                        QVariantMap values;
                        for (int i = 0; i < list.size(); i++) {
                            auto map = list.at(i).toMap();
                            auto name = map.value("name").toString();
                            if (!name.isEmpty()) {
                                values.insert(name, map.value("value"));
                            }
                        }
                        m_nodesList.value(nodeId)->setInput(values);
                    }
                    QVariantMap map;
                    map.insert("nodeId", nodeId);
                    sendReplyMessage(cmdUuid, cmd, 0, map);
                }
            } else if ("setProperties" == cmd) { // 设置节点属性
                auto nodeId = data.value("nodeId").toString();
                if (!nodeId.isEmpty() && m_nodesList.contains(nodeId)) {
                    auto list = data.value("properties").toArray().toVariantList();
                    if (list.size() > 0) {
                        QVariantMap values;
                        for (int i = 0; i < list.size(); i++) {
                            auto map = list.at(i).toMap();
                            values.insert(map.value("name").toString(), map.value("value"));
                        }
                        m_nodesList.value(nodeId)->setProperty(values);
                    }
                    QVariantMap map;
                    map.insert("nodeId", nodeId);
                    sendReplyMessage(cmdUuid, cmd, 0, map);
                }
            }
        }
    }
}

void PluginBussiness::sendReplyMessage(QString cmdUuid, QString cmd, int result, QVariantMap data) {
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("cmdUuid", cmdUuid);
    obj.insert("cmd", cmd + "Reply");
    obj.insert("result", result);
    obj.insert("data", QJsonObject::fromVariantMap(data));
    doc.setObject(obj);
    auto sendData = doc.toJson();
    qn_send_command(sendData.toStdString().c_str(), sendData.size());
    qDebug().noquote() << sendData;
}
