#ifndef __DEVICEFACTORY_H__
#define __DEVICEFACTORY_H__
#include "qstring.h"
#include "qobject.h"

namespace sn3dsdkdemo
{
    class Device;
}
class ScanServiceImp;

class DeviceFactory
{
public:
    DeviceFactory();
    virtual ~DeviceFactory();
    static sn3dsdkdemo::Device* createDevice(QObject* parent, const QString& subType, ScanServiceImp* scanService);
};
#endif