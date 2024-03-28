#include "createuuid.h"
#include <QUuid>
#include <QCoreApplication>
#include <QProcess>
#include <QThread>
#include <QFile>
#include <Windows.h>


CreateUuid::CreateUuid(QObject *parent) : HardwareBaseNode(parent) {
    m_nodeName = "CreateUuid";
    m_description = {
        {},
        {{"string", "value"}},
        {},
        {{"plugin_string"}},
    };
}

CreateUuid::~CreateUuid() {
}

void CreateUuid::apply() {
    
    if (hasInput("appPath")) {
        auto obj = input("appPath");
        QString appPath = obj.toString();

        if (!QFile::exists(appPath)) {
            return ;
        }

        m_appProcess.setProgram(appPath);
        m_appProcess.setWorkingDirectory("D:/Shining3d/EXStar");
#ifndef WIN32
        m_appProcess.setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
            args->flags |= CREATE_NEW_CONSOLE /*| NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP*/;
            args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
            //args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        });
#endif // WIN32
    
        m_appProcess.start();

        m_appProcess.waitForStarted();
        m_appProcess.waitForFinished(-1);
    }

};


#include "../include/plugincorehelper.h"

REGISTER(createprocess, CreateProcessNode);

CreateProcessNode::CreateProcessNode() {
    m_nodeName = "createprocess";
}

CreateProcessNode::~CreateProcessNode() {
}

QN_RESULT CreateProcessNode::apply() {


    if (hasInput("appPath")) {
        std::string appPath = input("appPath");

        if (!QFile::exists(QString(appPath.c_str()))) {
            return QN_NOT_FIND;
        }
        QProcess appProcess;
        appProcess.setProgram(QString(appPath.c_str()));
        appProcess.setWorkingDirectory("D:/Shining3d/EXStar");

        appProcess.start();

        appProcess.waitForStarted();
        appProcess.waitForFinished(-1);
    }

    return QN_OK;
}
