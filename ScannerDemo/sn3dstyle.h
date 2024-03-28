#ifndef __SN3DSTYLE_H__
#define __SN3DSTYLE_H__
#include "qobject.h"
class Sn3DStyle :
    public QObject
{
    Q_OBJECT
public:
    Sn3DStyle(QObject* parent = nullptr);
    virtual ~Sn3DStyle();
    virtual void setStyleSheet();
};
#endif

