#include "hardwareapplication.h"
#include "pluginbussiness.h"
#include <QCoreApplication>
#include <QProcess>
#include <QScopedPointer>
#include <QThread>
#include <QtDebug>
#include <qn_plugin.h>
#include <Windows.h>


int main(int argc, char *argv[]) {
    EinstarScanApplication app(argc, argv);
    //QThread::sleep(10);
#if 0
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    auto lm_launch_result = CreateProcess("D:/Shining3d/EXStar/ScannerDemo.exe", NULL, NULL, NULL, FALSE,
                                              CREATE_NEW_CONSOLE | NORMAL_PRIORITY_CLASS | CREATE_NEW_PROCESS_GROUP,
                                              NULL, "D:/Shining3d/EXStar/", &si, &pi);
    DWORD dwExitCode;
    WaitForSingleObject(pi.hProcess, -1);
    GetExitCodeProcess(pi.hProcess, &dwExitCode);
#endif

    QScopedPointer<PluginBussiness> handler(new PluginBussiness());

    std::string repAddr, pubAddr;

    // 初始化
    handler->init(argc, argv);
    
    qn_set_message_callback(PluginBussiness::onHandleMessage, handler.get());
    return app.exec();
}

bool checkProcessByQProcess(const QString &strExe) {
    bool bResult = false;
    // 判断进程是否存在
    QProcess tasklist;
    tasklist.start("tasklist", QStringList() << "/NH"
                                             << "/FO"
                                             << "CSV"
                                             << "/FI" << QString("IMAGENAME eq %1").arg(strExe));
    tasklist.waitForFinished();
    QString strOutput = tasklist.readAllStandardOutput();
    //如果进程存在,则结束进程
    if (strOutput.startsWith(QString("\"%1").arg(strExe))) {
        qInfo() << "check process";
        bResult = true;
    }
    return bResult;
}
