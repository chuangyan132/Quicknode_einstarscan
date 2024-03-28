/*!
* @brief   
* @project qn_exeplugin
* @file    pluginbussiness.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-07 14:58
*/
//==============================================================================
#ifndef QN_EXEPLUGIN_PLUGINBUSSINESS_H
#define QN_EXEPLUGIN_PLUGINBUSSINESS_H
//==============================================================================


#include <qn_plugin.h>
#include <memory>
#include <map>
#include <string>
#include "jsoncpp/json/json.h"


class BaseNode;
class PluginBussiness {
  public:
    PluginBussiness();
    virtual ~PluginBussiness();

    int init(int argc, char *argv[]);
    int fini();

    int sendNodesDesc();

    static void onHandleMessage(const char *data, size_t len, void *pUser);
    static void onHandleLog(int level, const char *log, void *pUser);

  protected:
    void onRestart(const char *data, size_t len);
    void onCreate(const char *data, size_t len);
    void onSetInputs(const char *data, size_t len);
    void onApply(const char *data, size_t len);
    void onGetOutpus(const char *data, size_t len);
    void onSetProperties(const char *data, size_t len);
    void onProperties(const char *data, size_t len);
    void onViewDatas(const char *data, size_t len);

    std::map<std::string, std::shared_ptr<BaseNode>> m_nodes;
};


//==============================================================================
#endif // QN_EXEPLUGIN_PLUGINBUSSINESS_H
//==============================================================================
