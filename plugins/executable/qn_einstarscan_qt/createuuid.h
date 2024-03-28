#pragma once
#include "hardwarebasenode.h"
#include <QProcess>
#include "../include/basenode.h"

class CreateUuid : public HardwareBaseNode {
    Q_OBJECT
  public:
    Q_INVOKABLE CreateUuid(QObject *parent = nullptr);
    Q_INVOKABLE ~CreateUuid();

    void apply();

    private:
    QProcess m_appProcess;
};

class CreateProcessNode : public /*QObject,*/ BaseNode{
  public:
    CreateProcessNode();
    virtual  ~CreateProcessNode();

    virtual QN_RESULT apply() override;

  private:
    std::string m_nodeName;
};

