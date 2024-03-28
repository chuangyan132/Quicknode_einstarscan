/*!
* @brief   
* @project qn_algorithmexe
* @file    algorithmapplication.h
* @author  zhaonanfang
* @email   zhaonanfang@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-zhaonanfang 2023-08-22 15:24
*/
//==============================================================================
#ifndef QN_HARDWAREEXE_HARDWAREAPPLICATION_H
#define QN_HARDWAREEXE_HARDWAREAPPLICATION_H
//==============================================================================

#include <QApplication>
#include <QDebug>
#include <QMutex>
#include <iostream>

class PluginApplication : public QApplication {
    Q_OBJECT
  public:
    PluginApplication(int &argc, char **argv);
    ~PluginApplication();

  private:
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
signals:
    void toExit();// 发送信号退出程序
};

class QDebugStreamBuffer : public std::basic_streambuf<char> {
  public:
    QDebugStreamBuffer(QtMsgType type, std::streambuf *oldStreamBuf) : m_type(type), m_oldStreamBuf(oldStreamBuf) {
        setp(m_buffer, m_buffer + BUFFER_SIZE);
    }

    ~QDebugStreamBuffer() {
        sync();
    }

  protected:
    int_type overflow(int_type c) override {
        if (c != EOF) {
            *pptr() = c;
            pbump(1);
        }
        return sync() ? c : EOF;
    }

    int sync() override {
        if (pbase() != pptr()) {
            QString msg = QString::fromUtf8(pbase(), pptr() - pbase());
            if (m_type == QtDebugMsg) {
                qDebug().noquote() << msg;
            } else if (m_type == QtWarningMsg) {
                qWarning().noquote() << msg;
            }
            setp(m_buffer, m_buffer + BUFFER_SIZE);
        }
        return m_oldStreamBuf ? m_oldStreamBuf->pubsync() : 0;
    }

  private:
    static const int BUFFER_SIZE = 1024;
    char m_buffer[BUFFER_SIZE];
    QtMsgType m_type;
    std::streambuf *m_oldStreamBuf;
};

//==============================================================================
#endif // QN_HARDWAREEXE_HARDWAREAPPLICATION_H
//==============================================================================
