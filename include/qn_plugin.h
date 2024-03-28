/*!
* @brief   
* @project plugincore
* @file    qn_plugin.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-07-13 11:39
*/
//==============================================================================
#ifndef PLUGINCORE_QN_PLUGIN_H
#define PLUGINCORE_QN_PLUGIN_H
//==============================================================================


#if defined(QUICKNODES_DLLEXPORT)
#define QN_API UNI_PROPER_EXPORT_FLAG
#else
#define QN_API UNI_PROPER_IMPORT_FLAG
#endif

#include "uniexport.h"

/* LIBPLUGINCORE_VERSION_NUMBER looks like 1002001 for e.g. version 1.2.1 */
#define LIBPLUGINCORE_MAJOR 1
#define LIBPLUGINCORE_MINOR 0
#define LIBPLUGINCORE_REVISION 0
#define LIBPLUGINCORE_VERSION_NUMBER \
    (LIBPLUGINCORE_MAJOR * 1000000 + LIBPLUGINCORE_MINOR * 1000 + LIBPLUGINCORE_REVISION)

/* Log types */
#define QN_LOG_NONE          0
#define QN_LOG_INFO          (1 << 0)
#define QN_LOG_NOTICE        (1 << 1)
#define QN_LOG_WARN          (1 << 2)
#define QN_LOG_ERROR         (1 << 3)
#define QN_LOG_DEBUG         (1 << 4)



typedef void (*USER_CALLBACK)(const char* data, size_t len, void *user);




QN_API int qn_init(int argc, char *argv[]);

QN_API int qn_init(const char *host, int port);

QN_API int qn_cleanup();

QN_API int qn_set_plugin_name(const char *name);

QN_API int qn_set_log_path(const char *path);

QN_API int qn_send_command(const char* data, int len);

QN_API int qn_version(int *major, int *minor, int *patch);


/*!
 * @brief 设置指定key对应的数据
 * @param
 * @note
 */
QN_API int qn_set_userdata(const char* key, char* data, int len);

/*!
* @brief 获取指定key对应的数据
* @param 
* @note 
*/
QN_API int qn_get_userdata(const char* key, char** data, int* len);

/*!
* @brief 设置插件运行过程中自定义的统计信息
* @param 
* @note 
*/
QN_API int qn_set_statistics(const char* key, char* data, int len);

/*!
 * @brief 发送指定命令对应消息内容
 * @param
 * @note
 */
QN_API int qn_send_message(const char* cmd, char* data, int len);

/*!
 * @brief 接收消息
 * @param
 * @note data需要用户自己qn_free释放
 *       deprecated
 */
QN_API const char* qn_receive_message(char** data, int *len);

/*!
 * @brief 申请内存
 * @param
 * @note 
 */
QN_API void* qn_malloc(int len);

/*!
 * @brief 释放内存
 * @param
 * @note 
 */
QN_API void qn_free(void* data);

// QN_API int setCallback(USER_CALLBACK callback, void *pUser);

QN_API int qn_set_message_callback(void (*on_message)(const char *data, size_t len, void *user), void *user);

/*
 * Function: qn_set_log_callback
 *
 * Set the logging callback. This should be used if you want event logging
 * information from the client library.
 *
 *  pUser -   the user data provided by client
 *  on_log - a callback function in the following form:
 *           void callback(int level, const char *log, void *pUser)
 *
 * Callback Parameters:
  *	log -   the message string..
 *  pUser -   the user data provided by client
 *  level - the log message level from the values:
 *	        QN_LOG_INFO
 *	        QN_LOG_NOTICE
 *	        QN_LOG_WARN
 *	        QN_LOG_ERROR
 *	        QN_LOG_DEBUG
 */
QN_API int qn_set_log_callback(void (*on_log)(int level, const char *log, void *user), void *user);


//==============================================================================
#endif // PLUGINCORE_QN_PLUGIN_H
//==============================================================================
