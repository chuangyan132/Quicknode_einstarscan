/*!
* @brief
* @file    pluginbussiness.cpp
* @author  zhaonanfang
* @email   zhaonanfang@shining3d.com
* @license
*     Copyright(C) shining3d. All rights reserved.
* @versions
*     V0.1-zhaonanfang 2023-08-30 16:11
*/

#include "pluginbussiness.h"
#include <QDebug>
#include <qn_plugin.h>
#include "nodesmanager.h"

Q_LOGGING_CATEGORY(qn_plugincore, "sn3d.quicknodes.plugincore")
PluginBussiness::PluginBussiness(QObject *parent) : QObject(parent) {

}

PluginBussiness::~PluginBussiness() {
    fini();
}

int PluginBussiness::init(int argc, char *argv[]) {
    std::string host, pluginName;
    int port = 0;
    if (argc >= 2) {
        host = argv[1];
        host = std::stoi(argv[2]);
    }
    else {
        host = "127.0.0.1";
        port = 10086;
    }
    ::qn_set_log_path(argv[0]);
    int res = ::qn_init(host.c_str(), port);
    int major, minor, patch = 0;
    qn_version(&major, &minor, &patch);
    qDebug() << "Plugin Version: " << major << "." << minor << "." << patch;
    return res;
}

int PluginBussiness::fini() {
    return ::qn_cleanup();
}

void PluginBussiness::onHandleMessage(const char *data, size_t len, void *pUser) {
    NodesManager::instance()->handleMessage(data, len);
}

void PluginBussiness::onHandleLogs(int level, const char *log, void *pUser) {
    switch (level) {
    case QN_LOG_INFO: qCInfo(qn_plugincore).noquote() << log; break;
    case QN_LOG_NOTICE: qCInfo(qn_plugincore).noquote() << log; break;
    case QN_LOG_WARN: qCWarning(qn_plugincore).noquote() << log; break;
    case QN_LOG_ERROR: qCCritical(qn_plugincore).noquote() << log; break;
    case QN_LOG_DEBUG: qCDebug(qn_plugincore).noquote() << log; break;
    default: qCDebug(qn_plugincore).noquote() << log; break;
    }
}