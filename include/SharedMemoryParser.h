#pragma once
#include <QObject>
#include <QApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QImage>
#include <QScopedPointer>
#include <array>

#include "sn3d_smc.h"
#include "sharememstructdef.h"
#include "Sn3DPublic.h"
#include "glog/logging.h"


typedef struct tagSN3D_POINT_DATA
{
	float x;
	float y;
	float z;
}SN3D_POINT_DATA, *LPSN3D_POINT_DATA;

typedef struct tagSN3D_VEC2F
{
	float x;
	float y;
}SN3D_VEC2F, *LPSN3D_VEC2F;

typedef struct tagSN3D_FACE_ID
{
	int x;
	int y;
	int z;
}SN3D_FACE_ID, *LPSN3D_FACE_ID;

struct MeshSharedMemoryMessage
{
	enum DataType
	{
		pointVertex = 1,
		pointNormal,
		pointColor,
		faceID,
		texture,
		textureID,
		textureUV,
		pointNum,
		sendEnd
	};
	bool haveNext;
	int size;
	int begin;
	int end;
	DataType type;
	int variable;
};
struct BodyScanIncrementDataPackage
{
	std::vector<std::array<float, 11>> buf;
	std::vector<int> deletes;           
	std::vector<std::array<float, 16>> rts;
	unsigned long long pointSize = 0;
	int lastId = 0;
	std::array<float, 6> boundingBox;
	std::vector<std::array<float, 7>> currentCloud;
	bool currentChanged;           
	bool isWholeData;
};
class SharedMemoryParser:public QObject
{
	Q_OBJECT
public:
	SharedMemoryParser(QObject*parent = nullptr)
		:QObject(parent)
	{
		sn3d_smc_initialize();
	}
	~SharedMemoryParser() {}

	int getCameraVideo(int camera_id, char**image, int&width, int&height, int&channel);
	int getWholePointData();
	int renderScenData(QString memKey); // hx1.3 E10
	//failed cloud ,not align to whole point cloud
	int getFailedPointCloud();

	//current point cloud
	int getCurPointCloud();

	//mesh 
	int getMeshData();

	//body point cloud
	int getBodyScanPointCloud();

	// whole markers
	int getWholeMarkers();

	//current markers
	int getCurrentMarkers();

	//edit 
	int serialEditSharedMemory(LPSMC_EDIT_DATA edit_data, int begin, int end);

	void setWholePointCloudCallBack(std::function<void(LPSn3dPointCloud)> wholePointCloudCallBackFunc)
	{
		m_WholePointCallBackFunc = wholePointCloudCallBackFunc;
	}

	void setCurrentPointCloudCallBack(std::function<void(LPSn3dPointCloud, void*)> currentPointCloudCallBackFunc, void* owner)
	{
		m_CurrentPointCallBackFunc = currentPointCloudCallBackFunc;
		m_CurrentPoint_owner = owner;
	}

	void setIncreasePointCloudCallBack(std::function<void(LPSn3dIncreasePointCloud, void*)> increasePointCloudCallBackFunc, void* owner)
	{
		m_IncreasePointCallBackFunc = increasePointCloudCallBackFunc;
		m_IncreasePoint_owner = owner;
	}

	//LSC SharedMemory中，定义set函数，设置callback和owner
	void setMeshCallBackEx(std::function<void(LPSn3dMeshData, void*)> meshCallBackFunc, void* owner)
	{
		m_MeshCallBackFuncEx = meshCallBackFunc;
		m_MeshData_owner = owner;
	}

	void setMeshCallBack(std::function<void(LPSn3dPointCloud)> meshCallBackFunc)
	{
		m_MeshCallBackFunc = meshCallBackFunc;
	}

	void setCurrentMarkerCallBack(std::function<void(LPSn3DMakerData, void*)> currentMarkerCallBack, void* owner)
	{
		m_CurrentMarkerCallBackFunc = currentMarkerCallBack;
		m_CurrentMarker_owner = owner;
	}

	void setWholeMarkerCallBack(std::function<void(LPSn3DMakerData, void*)> wholeMarkerCallBack, void* owener)
	{
		m_WholeMarkerCallBackFunc = wholeMarkerCallBack;
		m_WholeMarker_owner = owener;
	}


private:
	int savePointInMemory(LPSMC_POINTS pointData);

	int writePointCloud(QString filename, LPSMC_POINTS pointData);
	void handleSceneDataFromService(const QString& key); //hx1.3激光专用
	int writeMarkers(QString filename, LPSMC_POINTS pointData);
	int parseBodyScanData(LPSMC_POINTS pointData);
	void pointTrans(SN3D_POINT_DATA&point, float rt[16]);
	void normalTrans(SN3D_POINT_DATA&point, float rt[16]);
signals:
	void readDataEnd();
	void requestData();
private:
	//whole point cloud vector
	std::vector<SN3D_POINT_DATA> m_pointVec;
	std::vector<SN3D_POINT_DATA> m_normalVec;
	std::vector<SN3D_POINT_DATA> m_colorVec;
	std::vector<int> m_ids;
	//mesh data 
	std::vector<SN3D_POINT_DATA> m_meshPointVec;
	std::vector<SN3D_POINT_DATA> m_meshNormalVec;
	std::vector<SN3D_FACE_ID>    m_meshTriFaceId;
	std::vector<SN3D_FACE_ID>    m_meshTextureId;
	std::vector<SN3D_VEC2F>      m_textureUV;
	QScopedPointer<QImage>		 m_image;
	QMap<QString, std::string> m_laserRawDataMap;  // hx1.3 激光专用
	std::function<void(LPSn3dPointCloud)> m_WholePointCallBackFunc = nullptr;
	std::function<void(LPSn3dPointCloud, void*)> m_CurrentPointCallBackFunc = nullptr;
	void* m_CurrentPoint_owner = nullptr;
	std::function<void(LPSn3dIncreasePointCloud, void*)> m_IncreasePointCallBackFunc = nullptr;
	void* m_IncreasePoint_owner = nullptr;
	std::function<void(LPSn3dPointCloud)> m_MeshCallBackFunc = nullptr;
	//LSC SharedMemory中声明callback和owner
	std::function<void(LPSn3dMeshData, void*)> m_MeshCallBackFuncEx = nullptr;
	void* m_MeshData_owner = nullptr;

	std::function<void(LPSn3DMakerData, void*)> m_CurrentMarkerCallBackFunc = nullptr;
	void* m_CurrentMarker_owner = nullptr;

	//whole marker
	std::function<void(LPSn3DMakerData, void*)> m_WholeMarkerCallBackFunc = nullptr;
	void* m_WholeMarker_owner = nullptr;


};

