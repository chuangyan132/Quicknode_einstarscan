#pragma once
#include <QObject>
#include "basenode.h"
#include <QJsonObject>

// 节点管理
class NodesManager : public QObject {
    Q_OBJECT
public:
    static NodesManager *instance();
    void handleMessage(const char *data, size_t len);// 处理消息
    void addNodeClass(QString nodeClass, const QMetaObject * metaObject);// 添加插件类

private:
    void sendReplyMessage(QString cmdUuid, QString cmd, int result, QVariantMap data); // 发送回复消息
    void onRestart(QString cmd, QString cmdUuid, QJsonObject data);
    void onCreate(QString cmd, QString cmdUuid, QJsonObject data);
    void onSetInputs(QString cmd, QString cmdUuid, QJsonObject data);
    void onApply(QString cmd, QString cmdUuid, QJsonObject data);
    void onGetOutputs(QString cmd, QString cmdUuid, QJsonObject data);
    void onSetProperties(QString cmd, QString cmdUuid, QJsonObject data);
    void onProperties(QString cmd, QString cmdUuid, QJsonObject data);
    void onViewDatas(QString cmd, QString cmdUuid, QJsonObject data);
private:
    NodesManager(QObject *parent = nullptr);
    ~NodesManager();
    QMap<QString, const QMetaObject *> m_nodesClassList; // 存储节点类元信息
    QMap<QString, BaseNode *> m_nodesList;       // 存储节点实例
    static NodesManager *m_instance;
};
