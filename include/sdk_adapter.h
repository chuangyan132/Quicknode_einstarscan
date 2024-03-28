#ifndef SN3D_SDK_ADAPTER_H
#define SN3D_SDK_ADAPTER_H

#include "sdk_adapter_global.h"
#include <QScopedPointer>

BEGIN_SDK_ADAPTER_NAMESPACE

class AdapterPrivate;

class SDK_ADAPTER_EXPORTS Adapter : public QObject
{
    Q_OBJECT
public:
    Q_PROPERTY(quint16 pub_port READ pub_port WRITE set_pub_port NOTIFY pub_port_changed)
    Q_PROPERTY(quint16 rep_port READ rep_port WRITE set_rep_port NOTIFY rep_port_changed)
    Q_PROPERTY(bool json_compact READ json_compact WRITE set_json_compact NOTIFY json_compact_changed)
    Q_PROPERTY(bool is_running READ is_running NOTIFY is_running_changed)

    explicit Adapter(QObject* parent = nullptr);
    ~Adapter() override;

    bool start(quint16 pub_port, quint16 rep_port);
    bool start();
    void stop();

    bool is_running() const;

    quint16 pub_port() const;
    void set_pub_port(quint16 new_pub_port);

    quint16 rep_port() const;
    void set_rep_port(quint16 new_rep_port);

    bool json_compact() const;
    void set_json_compact(bool new_json_compact);

    bool enable_remoting(QObject* source, const QString &name = QString());
    void disable_remoting(QObject* source);



    quint32 last_error() const;
signals:
    void started();
    void stopped();

    void error(quint32 error_code);
    void pub_port_changed();
    void rep_port_changed();
    void json_compact_changed();
    void is_running_changed();
private:
    QScopedPointer<AdapterPrivate> d_ptr;
    Q_DECLARE_PRIVATE(Adapter)
};

END_SDK_ADAPTER_NAMESPACE

#endif//SN3D_SDK_ADAPTER_H
