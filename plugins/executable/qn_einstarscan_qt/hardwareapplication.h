#pragma once
#include <QCoreApplication>
#include <QDebug>
#include <QMutex>
#include <iostream>

class EinstarScanApplication : public QCoreApplication {
    Q_OBJECT
  public:
    EinstarScanApplication(int &argc, char **argv);
    ~EinstarScanApplication();

  private:
    static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
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
