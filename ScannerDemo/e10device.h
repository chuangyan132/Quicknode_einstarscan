#ifndef __E10DEVICE_H__
#define __E10DEVICE_H__
#include "device.h"
class E10Device :
	public sn3dsdkdemo::Device
{
public:
	E10Device(QObject* parent, ScanServiceImp* pService);
	virtual int saveCloudPoint(const QString& projectPath) override;
	virtual int calibrateTypeMap(QMap<QString, int>& map) override;
	virtual int saveMesh(const QString& projectPath) override;
	virtual ~E10Device() = default;

	//virtual int saveCloudPoint(const QString& projectPath);
};
#endif

