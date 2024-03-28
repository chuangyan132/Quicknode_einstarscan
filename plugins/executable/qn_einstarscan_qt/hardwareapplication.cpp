#include "hardwareapplication.h"
#include <QDateTime>
#include <QDir>
#include <QMutexLocker>
#include <QStandardPaths>
#include <QTextStream>

static QString saveFilePath;
static QMutex mutex;
// 分配日志文件路径
void logToFile() {
    quint64 t = QDateTime::currentMSecsSinceEpoch();
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    tempPath.append("/Shining3D/HardwarePlugin");
    QDir dir(tempPath);
    dir.mkpath(tempPath);

    tempPath.append("/log_HardwareExePlugin_");
    tempPath.append(QString::number(t));
    tempPath.append(".txt");
    saveFilePath = tempPath;
}

EinstarScanApplication::EinstarScanApplication(int &argc, char **argv) : QCoreApplication(argc, argv) {
    logToFile();
    // 重定向C++ 标准输出到QDebug
    std::streambuf *coutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(new QDebugStreamBuffer(QtDebugMsg, coutBuffer));

    // 重定向C++标准错误输出到QDebug
    std::streambuf *cerrBuffer = std::cerr.rdbuf();
    std::cerr.rdbuf(new QDebugStreamBuffer(QtWarningMsg, cerrBuffer));
    // 此处为设置日志的格式,开发模式下可以设置日志格式,便于定位到具体哪一行代码输出的Qt日志
    qSetMessagePattern("%{time} : %{file} : %{line} : %{function} : %{category} : %{message}");
    qInstallMessageHandler(messageOutput);
}

EinstarScanApplication::~EinstarScanApplication() {
}

void EinstarScanApplication::messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
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