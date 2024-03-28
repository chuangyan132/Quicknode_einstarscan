/*!
* @brief   
* @project qn_algorithm
* @file    main.cpp
* @author  zhaonanfang
* @email   zhaonanfang@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-zhaonanfang 2023-08-30 09:58
*/
#include "../include/qtlike/pluginapplication.h"
#include "../include/qtlike/nodesmanager.h"
#include "createuuid.h"
#include <QThread>

int main(int argc, char *argv[]) {
    QThread::sleep(5);
    PluginApplication a(argc, argv);
    NodesManager::instance()->addNodeClass("CreateUuid3", &CreateUuid::staticMetaObject);
    return a.exec();
}
