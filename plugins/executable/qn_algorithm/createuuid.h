#pragma once
#include "../include/qtlike/basenode.h"

class CreateUuid : public BaseNode {
    Q_OBJECT
  public:
    Q_INVOKABLE CreateUuid(QObject *parent = nullptr);
    Q_INVOKABLE ~CreateUuid();

    void apply();
};
