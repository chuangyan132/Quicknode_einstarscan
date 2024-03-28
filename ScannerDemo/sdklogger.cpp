#include "sdklogger.h"
#include "qmutex.h"
#include "qstring.h"
#include "qfile.h"
#include "qtextstream.h"
#include "qdatetime.h"
#include "qdir.h"
#include <mutex>

static struct LogWrap {
    LogWrap::LogWrap() { reset(); }
    LogWrap::~LogWrap() { reset(); }
    void reset() {
        isInit = false;
        level = "";
        date = "";
        if (ts) {
            delete ts;
            ts = nullptr;
        }
        if (file) {
            delete file;
            file = nullptr;
        }
    }
    bool isInit;
    QMutex locker;
    QString level;
    QString date;
    QFile *file;
    QTextStream *ts;
} s_logWrap;

#define _TIME_ qPrintable(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
// QStringList s_outputFileNames = QStringList() << "view.cpp" << "store.cpp";
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    /*bool isFilter = true;
    QString file(context.file);
    for (auto iter = s_outputFileNames.begin(); iter != s_outputFileNames.end(); ++iter) {
        if (file.contains(*iter)) {
            isFilter = false;
            break;
        }
    }
    if (isFilter) {
        return;
    }*/

    /*QString formatMsg;
    switch (type) {
    case QtDebugMsg:
        formatMsg = "Debug:" + msg;
        break;
    case QtWarningMsg:
        formatMsg = "Warning:" + msg;
        break;
    case QtCriticalMsg:
        formatMsg = "Critical:" + msg;
        break;
    case QtFatalMsg:
        formatMsg = "Fatal:" + msg;
        break;
    default:
        return;
    }*/
    QString formatMsg = msg;

    QString curDate = QDate::currentDate().toString("yyyyMMdd");
    if (curDate != s_logWrap.date) {
        s_logWrap.reset();
    }

    if (!s_logWrap.isInit) {
        s_logWrap.isInit = true;
        QDir().mkpath("log");
        s_logWrap.level = "debug";
        s_logWrap.date = QDate::currentDate().toString("yyyyMMdd");
        s_logWrap.file = new QFile(QString("log\\log%1.log").arg(s_logWrap.date));
        if (s_logWrap.file->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
            s_logWrap.ts = new QTextStream(s_logWrap.file);
        }
    }

    if (s_logWrap.ts) {
        QString text = _TIME_ + QString("[%1:%2]%3\n").arg(context.file).arg(context.line).arg(formatMsg);
        // OutputDebugString(text.toStdWString().c_str());
        QMutexLocker locker(&s_logWrap.locker);
        (*s_logWrap.ts << text).flush();
    }
}

SDKLogger::SDKLogger()
{
    qInstallMessageHandler(myMessageOutput);
}

static std::once_flag g_flag1;
static SDKLogger* g_instance = nullptr;

SDKLogger* SDKLogger::createInstanc()
{
    std::call_once(g_flag1, [](){g_instance = new SDKLogger();});
    return g_instance;
}
