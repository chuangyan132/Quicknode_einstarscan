/*!
* @brief   
* @project qn_algorithmexe
* @file    algorithmapplication.cpp
* @author  zhaonanfang
* @email   zhaonanfang@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-zhaonanfang 2023-08-22 15:25
*/
#include "pluginapplication.h"
#include <QDateTime>
#include <QDir>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QTextStream>
#include "pluginbussiness.h"
#include <qn_plugin.h>
#include <QThread>
#include <QtConcurrent>
#include <QtDebug>

static QString saveFilePath;
static QMutex mutex;
// 分配日志文件路径
void logToFile() {
    quint64 t = QDateTime::currentMSecsSinceEpoch();
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    tempPath.append("/Shining3D/" + QCoreApplication::applicationName());
    QDir dir(tempPath);
    dir.mkpath(tempPath);

    tempPath.append(QString("/log_%1_").arg(QCoreApplication::applicationName()));
    tempPath.append(QString::number(t));
    tempPath.append(".txt");
    saveFilePath = tempPath;
}

PluginApplication::PluginApplication(int &argc, char **argv) : QApplication(argc, argv) {
    logToFile();
    //// 重定向C++ 标准输出到QDebug
    //std::streambuf *coutBuffer = std::cout.rdbuf();
    //std::cout.rdbuf(new QDebugStreamBuffer(QtDebugMsg, coutBuffer));
    //
    //// 重定向C++标准错误输出到QDebug
    //std::streambuf *cerrBuffer = std::cerr.rdbuf();
    //std::cerr.rdbuf(new QDebugStreamBuffer(QtWarningMsg, cerrBuffer));
    // 此处为设置日志的格式,开发模式下可以设置日志格式,便于定位到具体哪一行代码输出的Qt日志
    qSetMessagePattern("%{time} : %{file} : %{line} : %{function} : %{category} : %{type} : %{message}");
    qInstallMessageHandler(messageOutput);
    PluginBussiness *handler = new PluginBussiness(this);

    handler->init(argc, argv);
    qn_set_message_callback(PluginBussiness::onHandleMessage, handler);
    qn_set_log_callback(PluginBussiness::onHandleLogs, handler);
    QtConcurrent::run([handler, this]() {
        while (true) {
            std::string str;
            std::cin >> str;
            if (str.compare("exit") == 0) {
                emit toExit();
            }
            else {
                qWarning() << "error command";
            }
        }
    });
    QObject::connect(this, &PluginApplication::toExit, this, [handler]() {
        if (handler != nullptr) {
            handler->deleteLater();
        }
        qApp->quit();
    });
}

PluginApplication::~PluginApplication() {
}

void PluginApplication::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&mutex);
    QString formatMsg = qFormatLogMessage(type, context, msg);
    //formatMsg.append("\r\n");
    fprintf(stdout, formatMsg.append("\r\n").toLocal8Bit());

    QFile file(saveFilePath);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream textStream(&formatMsg);

    QTextStream stream(&file);
    stream << formatMsg;
    //while (!textStream.atEnd()) {
    //    QString line = textStream.readLine();
    //    stream << line << "\r\n";
    //}
    file.flush();
    file.close();
}