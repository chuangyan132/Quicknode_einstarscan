/*!
* @brief   
* @project plugincore
* @file    qn_plugin.cpp
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-14 16:00
*/

#include <qn_plugin.h>
#include <thread>
#include <iostream>
#include "rpcmanager.h"


QN_API int qn_init(int argc, char *argv[]) {

    return 0;
}

QN_API int qn_init(const char *host, int port) {
    QnRtnCode rc = QN_RPC_MANAGER().init(host, port);
    return rc;
}

QN_API int qn_cleanup() {
    return QN_RPC_MANAGER().fini();
}

QN_API int qn_set_plugin_name(const char *name){
    return QN_RPC_MANAGER().setPluginName(name);
}

QN_API int qn_set_log_path(const char *path) {
    return QN_RPC_MANAGER().setLogPath(path);
}


QN_API int qn_send_command(const char *data, int len) {
    return QN_RPC_MANAGER().send_command(std::string(data, len));
}

QN_API int qn_version(int *major, int *minor, int *patch) {
    if (major) 
        *major = LIBPLUGINCORE_MAJOR;
    if (minor)
        *minor = LIBPLUGINCORE_MINOR;
    if (patch)
        *patch = LIBPLUGINCORE_REVISION;
    return LIBPLUGINCORE_VERSION_NUMBER;
}

QN_API int qn_set_userdata(const char *key, char *data, int len) {
    return 0;
}


QN_API int qn_get_userdata(const char *key, char **data, int *len) {
    return 0;
}


QN_API int qn_set_statistics(const char *key, char *data, int len) {
    return 0;
}


QN_API int qn_send_message(const char *cmd, char *data, int len) {
    return 0;
}


QN_API const char *qn_receive_message(char **data, int *len) {
    return NULL;
}


QN_API void *qn_malloc(int len) {
    return NULL;
}


QN_API void qn_free(void *data) {
    return ;
}

QN_API int setCallback(USER_CALLBACK callback, void *pUser) {

    QnRtnCode rc;
    return rc;
}

QN_API int qn_set_message_callback(void (*on_message)(const char *data, size_t len, void *user), void *user) {

    QnRtnCode rc = QN_RPC_MANAGER().setMessageCallback(on_message, user);
    if (rc.isFailed()) {
        std::wcout << rc.toString() << std::endl;
    }
    return rc;
}

QN_API int qn_set_log_callback(void (*on_log)(int level, const char *log, void *user), void *user) {

    QnRtnCode rc = QN_RPC_MANAGER().setLogCallback(on_log, user);

    return rc;
}
