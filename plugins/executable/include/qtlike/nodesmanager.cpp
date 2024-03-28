#include "nodesmanager.h"
#include <QDebug>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <qn_plugin.h>

NodesManager *NodesManager::m_instance = nullptr;
NodesManager *NodesManager::instance() {
    //static NodesManager inst;
    if (!m_instance)
        m_instance = new NodesManager(qApp);
    return m_instance;
}

NodesManager::NodesManager(QObject *parent) : QObject(parent),m_nodesList(),m_nodesClassList() {
}

NodesManager::~NodesManager() {

}

void NodesManager::handleMessage(const char * data, size_t len)
{
    QString str = QString::fromLocal8Bit(data);
    // qDebug().noquote() << "recv:" << str;
    // QThread::sleep(5);
    QJsonParseError error;
    auto doc = QJsonDocument::fromJson(str.toLocal8Bit(), &error);
    if (error.error != QJsonParseError::NoError) {
        qWarning() << "not json:";
        qWarning() << error.errorString();
        return;
    }
    auto object = doc.object();
    // ����Э��
    if (object.contains("cmd")) {
        if (object.contains("data")) {
            auto data = object.value("data").toObject();
            auto cmd = object.value("cmd").toString();
            auto cmdUuid = object.value("cmdUuid").toString();
            if ("restart" == cmd) { // ���ִ��,����Ѵ�ڵ���Դ
                onRestart(cmd, cmdUuid, data);
            }
            else if ("create" == cmd) { // �����ڵ�ʵ��
                onCreate(cmd, cmdUuid, data);
            }
            else if ("setProperties" == cmd) { // ���ýڵ�����
                onSetProperties(cmd, cmdUuid, data);
            }
            else if ("setInputs" == cmd) { // ���ýڵ�����
                onSetInputs(cmd, cmdUuid, data);
            }
            else if ("apply" == cmd) { // ִ�нڵ�
                onApply(cmd, cmdUuid, data);
            }
            else if ("getOutputs" == cmd) { // ��ȡ�ڵ����
                onGetOutputs(cmd, cmdUuid, data);
            }
        }
    }
}

void NodesManager::addNodeClass(QString nodeClass, const QMetaObject * metaObject)
{
    m_nodesClassList.insert(nodeClass, metaObject);
}

void NodesManager::onRestart(QString cmd, QString cmdUuid, QJsonObject data)
{
    for (auto iter = m_nodesList.constBegin(); iter != m_nodesList.constEnd(); iter++) {
        delete iter.value();
    }
    //m_nodesList.clear();
    sendReplyMessage(cmdUuid, cmd, 0, QVariantMap());
}

void NodesManager::onCreate(QString cmd, QString cmdUuid, QJsonObject data)
{
    auto nodeClass = data.value("nodeClass").toString();
    //auto nodeClass = data.value("nodeId").toString().split("-").last();
    if (!nodeClass.isEmpty() && m_nodesClassList.contains(nodeClass)) {
        auto nodeId = data.value("nodeId").toString();
        if (!nodeId.isEmpty()) {
            if (!m_nodesList.contains(nodeId)) { // �ж��Ƿ���ʵ������node,�������е�ʵ��
                auto metaObj = m_nodesClassList.value(nodeClass);
                if (metaObj == nullptr) {
                    qWarning() << QString("%1 metaObject is nullptr").arg(nodeClass);
                    return;
                }
                auto ptr = qobject_cast<BaseNode *>(metaObj->newInstance());
                m_nodesList.insert(nodeId, ptr);
            }
            QVariantMap map;
            map.insert("nodeId", nodeId);
            sendReplyMessage(cmdUuid, cmd, 0, map);
        }
    }
}

void NodesManager::onSetInputs(QString cmd, QString cmdUuid, QJsonObject data)
{
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
}

void NodesManager::onApply(QString cmd, QString cmdUuid, QJsonObject data)
{
    auto nodeId = data.value("nodeId").toString();
    if (!nodeId.isEmpty() && m_nodesList.contains(nodeId)) {
        m_nodesList.value(nodeId)->apply();
        QVariantMap map;
        map.insert("nodeId", nodeId);
        sendReplyMessage(cmdUuid, cmd, 0, map);
    }
}

void NodesManager::onGetOutputs(QString cmd, QString cmdUuid, QJsonObject data)
{
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
}

void NodesManager::onSetProperties(QString cmd, QString cmdUuid, QJsonObject data)
{
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

void NodesManager::onProperties(QString cmd, QString cmdUuid, QJsonObject data)
{
}

void NodesManager::onViewDatas(QString cmd, QString cmdUuid, QJsonObject data)
{
}

void NodesManager::sendReplyMessage(QString cmdUuid, QString cmd, int result, QVariantMap data)
{
    QJsonDocument doc;
    QJsonObject obj;
    obj.insert("cmdUuid", cmdUuid);
    obj.insert("cmd", cmd + "Reply");
    obj.insert("result", result);
    obj.insert("data", QJsonObject::fromVariantMap(data));
    doc.setObject(obj);
    auto sendData = doc.toJson();
    qn_send_command(sendData.toStdString().c_str(), sendData.size());
    // qDebug().noquote() << sendData;
}
