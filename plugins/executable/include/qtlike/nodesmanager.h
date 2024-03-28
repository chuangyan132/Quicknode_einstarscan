#pragma once
#include <QObject>
#include "basenode.h"
#include <QJsonObject>

// �ڵ����
class NodesManager : public QObject {
    Q_OBJECT
public:
    static NodesManager *instance();
    void handleMessage(const char *data, size_t len);// ������Ϣ
    void addNodeClass(QString nodeClass, const QMetaObject * metaObject);// ��Ӳ����

private:
    void sendReplyMessage(QString cmdUuid, QString cmd, int result, QVariantMap data); // ���ͻظ���Ϣ
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
    QMap<QString, const QMetaObject *> m_nodesClassList; // �洢�ڵ���Ԫ��Ϣ
    QMap<QString, BaseNode *> m_nodesList;       // �洢�ڵ�ʵ��
    static NodesManager *m_instance;
};
