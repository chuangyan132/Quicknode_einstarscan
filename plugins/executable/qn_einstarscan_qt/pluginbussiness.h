#pragma once

#include "createuuid.h"
#include <QMap>
#include <QObject>

class PluginBussiness : public QObject {
    Q_OBJECT
  public:
    PluginBussiness(QObject *parent = nullptr);
    ~PluginBussiness();

    int init(int argc, char *argv[]);
    int fini();
    static void reset();

    static void onHandleMessage(const char *data, size_t len, void *pUser);                   // 处理消息
    static void sendReplyMessage(QString cmdUuid, QString cmd, int result, QVariantMap data); // 发送回复消息
};
