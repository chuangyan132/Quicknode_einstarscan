#include "createuuid.h"
#include <QUuid>
#include <QCoreApplication>
#include <QProcess>
#include <QThread>

CreateUuid::CreateUuid(QObject *parent) : BaseNode(parent) {
    m_nodeName = "CreateUuid";
}

CreateUuid::~CreateUuid() {
}

void CreateUuid::apply() {
    QString uuid = QUuid::createUuid().toString();
    uuid = uuid.mid(1, uuid.size() - 2);
    for (auto iter = m_inputs.constBegin(); iter != m_inputs.constEnd(); iter++) {
        uuid += iter.value().toString();
    }
    for (auto iter = m_properties.constBegin(); iter != m_properties.constEnd(); iter++) {
        uuid += iter.value().toString();
    }
    setOutput("value", "<" + uuid + ">");
};
