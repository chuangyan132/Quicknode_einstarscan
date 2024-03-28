/*!
* @brief
* @file    pluginbussiness.h
* @author  zhaonanfang
* @email   zhaonanfang@shining3d.com
* @license
*     Copyright(C) shining3d. All rights reserved.
* @versions
*     V0.1-zhaonanfang 2023-08-30 16:11
*/
//==============================================================================
#ifndef QN_ALGORITHM_PLUGINBUSSINESS_H
#define QN_ALGORITHM_PLUGINBUSSINESS_H
//==============================================================================

#include <QObject>
#include <QLoggingCategory>

class PluginBussiness : public QObject {
    Q_OBJECT
public:
    PluginBussiness(QObject *parent = nullptr);
    ~PluginBussiness();

    int init(int argc, char *argv[]);
    int fini();

    static void onHandleMessage(const char *data, size_t len, void *pUser);                   // 处理消息
    static void onHandleLogs(int level, const char *log, void *pUser);                        // plugincore日志回调
};

Q_DECLARE_LOGGING_CATEGORY(qn_plugincore)

//==============================================================================
#endif // QN_ALGORITHM_PLUGINBUSSINESS_H
//==============================================================================
