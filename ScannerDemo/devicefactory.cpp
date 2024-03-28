#include "devicefactory.h"
//#include "e7device.h"
#include "PublicClass.h"
//#include "e3deviceh.h"
//#include "e3devicehx.h"
#include "e10device.h"

using namespace sn3dsdkdemo;

Device* DeviceFactory::createDevice(QObject* parent, const QString& subType, ScanServiceImp* scanService)
{
//     Device* ret = nullptr;
//     if (subType == C_E3HDevice){
//         return new E3DeviceH(parent, scanService);
//     }
//     else if (subType == C_E3HXDevice){
//         return new E3DeviceHX(parent, scanService);
//     }
//     else if (subType == C_E7PlusDevice || subType == C_E7ProDevice){
//         ret = new E7Device(parent, scanService);
//         ret->setSubType(subType);
//         return ret;
//     }
// 	else if (subType == C_E10Device)
// 	{
// 		return new E10Device(parent, scanService);
// 	}

	return nullptr;
}