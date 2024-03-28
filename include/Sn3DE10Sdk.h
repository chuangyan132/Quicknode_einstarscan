#ifndef SN3DSDKE10_H
#define SN3DSDKE10_H

#include "Sn3DErrorCode.h"
#include "Sn3DPublic.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32

#ifdef SN3DSDKEXPORT
#define SN3DSDKE10API __declspec(dllexport)
#else
#define SN3DSDKE10API  __declspec(dllimport)
#endif

#define CALLMETHOD __stdcall
#define CALLBACK __stdcall

#else

#define SN3DSDKE10API 
#define CALLMETHOD
#define CALLBACK

#endif
//Initialize the device setup callback
typedef  void (CALLBACK *Sn3DScanServiceWatcherCallBack)(int);

//LSC新增回调函数  Add callback function
//Get whole point cloud callback
typedef void(CALLBACK *Sn3DWholePointCloudCallBack)(LPSn3dPointCloud wholePointCloud);
//Get current point cloud callback
typedef void(CALLBACK *Sn3DCurrentPointCloudCallBack)(LPSn3dPointCloud currentPointCloud, void* owner);
//Get increase point cloud callback funtion
typedef void(CALLBACK *Sn3DIncreasePointCloudCallBack)(LPSn3dIncreasePointCloud increasePointCloud, void* owner);
//Get Camera Position callback function
typedef void(CALLBACK *Sn3DCameraPositionCallBack)(LPSn3dCameraPosition cameraPosition, void* owner);
//Get track lost status callback funtion
typedef void(CALLBACK *Sn3DTrackLostStatusCallBack)(bool trackLostStatus, void* owner);
//Get scan distance gear callback funtion
typedef void(CALLBACK *Sn3DScanDistCallBack)(double scanDist, void* owner);
//Get mesh data callback function
typedef void(CALLBACK *Sn3DMeshDataCallBack)(LPSn3dMeshData meshData, void* owner);
//Get device event callback function
typedef void(CALLBACK *Sn3DDeviceEventCallBack)(DeviceEvent event, void* owner);
//Get currnt marker infomation
typedef void(CALLBACK *Sn3DCurrentMarkerCallBack)(LPSn3DMakerData currentMarker, void* owner);
//Get Whole marker information
typedef void(CALLBACK *Sn3DWholeMarkerCallBack)(LPSn3DMakerData wholeMarker, void* ownner);
//Get too flat callback function
typedef void(CALLBACK *Sn3DTooFlatStatusCallBack)(bool tooFlatStatus, void* owner);
/*!
* @brief 定义获图片的回调      
* param camId 相机索引					Index of Camera
* param data 图像数据					Data of image
* param width 图像宽度					Width of image
* param height 图像高度					height of image
* param channel 通道数					num of channel
* param owner 回调的函数的拥有者指针	Pointer to the owner of the callback function
* @return 0初始化成功，失败参考错误		Initialization success.
*/
typedef void(CALLBACK *Sn3DGetImagesCallBack)(int camId, const unsigned char* data, int  width, int height, int channel, void* owner);

/*!
* @brief sdk初始化，在使用sdk前必须初始化	You must initialize before using the SDK Just initialize SDK once.
*        只能初始化一次						
* param wather 主要监控服务端是否正常运行，必须注册，且需要一直有效	Registration service monitors the callback and receives notice when the server ended.
* @return 0初始化成功，失败参考错误
*/
SN3DSDKE10API int Sn3DInitialize(Sn3DScanServiceWatcherCallBack wacther);

/*!

* @brief 初始化日志库，sdk提供了日志记录功能(glog)，如果需要的可以调用该函数启动日志功能	Initialize the log library. The SDK provides logging functions. 
															If necessary, you can call this function to start the logging function.
*如果exe程序也是用glog或者不想开启日志可以不调用该函数		If the exe program also uses glog or does not want to start logging, you can not call this function
* @return 0初始化成功，失败参考错误
*/
SN3DSDKE10API int Sn3DInitialLog(char* exeName, char* path);

/*!

* @brief sdk释放,不在调用sdk，可以调用该函数		The function for release the SDK resource.
													if you do not call the SDK, you can Call this function.
* @return 0成功，失败参考错误
*/
SN3DSDKE10API int Sn3DRelease();



/*!
* @brief 检测设备并连接设备			Check and connect device
* @return 0成功，失败参考错误码
*/
[[deprecated]] SN3DSDKE10API int  Sn3DCheckDevice();

/*!
* @brief 连接设备功能		Connect device
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int  Sn3DConnectDevice();

/*!
* @brief 设置设备亮度		Set device brightness
* @param level 档位，不能超过SN3DGetBrightnessRange获取到的最高和最低	The level can't exceed the highest and lowest values of the device
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int  Sn3DSetBrightness(int level);

/*!
* @brief 设置设备亮度范围				Set device brightness range
* @param min 最低档位					Reserved, the minimum brightness level is set to 0 (default). By default, this function is disable.
* @param max 最高档位					Maximum level of brightness.
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int Sn3DGetBrightnessRange(int& min, int& max);

/*!
* @brief 设置设备亮度					Set device brightness
* @param currentBrightness 当前亮度		current brightness			
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int Sn3DGetCurrentBrightness(int& currentBrightness);
/*!
* @brief 开启伪彩色						Turn on pseudo color
* @param enable true开启，false关闭		“true”-open, “false”-off
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int Sn3DSetEnablePseudoColor(bool enable);

/*
* @brief 设置扫描距离范围     set work range
* @param min 最小值				set the minimum working distance
* @param max 最大值				set the Maximum working distance
*/
SN3DSDKE10API int Sn3DSetWorkRange(float min, float max);

/*!
* @brief 保存数据		Save Data
* @param absolutePrjName 工程文件的绝对路径					Complete path of project files.
* @param savePath 保存文件的绝对路径不能带后缀				Path and name of the file required to be saved (without a suffix). 
* @param saveType 保存格式,目前只有 asc ply stl obj 这四种	The format of the file required to be saved (asc ply stl obj).
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int Sn3DSaveData(char* absolutePrjName, char*savePath, char*saveType);


/*!
* @brief 获取视频流						Get video stream
* @param imageCallback 相机采图回调		Set the callback function.
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int Sn3DIntallGetImagasCallBack(Sn3DGetImagesCallBack imageCallback, void* owner);


/*!
* @brief 新建或打开解决方案			New or open solutions
* @param slnDirPath 解决方案路径	Solution path
* @param isCreate true新建, false打开	“true”- new, “false”-open
* @param scanMode 扫描模式, 0 portrait, 1 object, 2 smallobject. Reference: ScanMode  
							Scan mode, 0 portrait, 1 object, 2 smallobject. Reference: ScanMode   
* @param hasTexture true纹理扫描, false不带纹理		“true”- texture, “false”-no texture
* @param pointDis 点距		Point distance
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int Sn3DOpenOrCreateSolution(
	const char* slnDirPath,
	bool isCreate,
	int scanMode,
	bool hasTexture,
	float pointDis);

/*!
* @brief 新建工程				New project
* @param slnDirPath 解决方案路径	Solution Path
* @param scanMode 扫描模式, 0 portrait, 1 object, 2 smallobject. Reference: ScanMode	
					Scan mode, 0 portrait, 1 object, 2 smallobject. Reference: ScanMode
* @param hasTexture true纹理扫描, false不带纹理		“true”- texture, “false”-no texture	
* @param pointDis 点距									Point distance
* @param alignType 拼接模式. Reference: AlignType		Alignment type. Reference: AlignType  See appendix
* @param globalMarkerPath 框架点路径				Global markers path，no global markers path：set scan mode to empty.
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DNewProject(
	const char* slnDirPath,
	int scanMode,
	bool hasTexture,
	float pointDis,
	int alignType,
	const char* globalMarkerPath);

/*!
* @brief 打开工程			Open project
* @param projFileName 工程路径		Project path
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DOpenProject(
	const char* projFileName);

/*!
* @brief 扫描, 需要在预扫描之后进行		scan can been started after preview scan
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DStartScan();

/*!
* @brief 预扫描		Preview	Scan
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DPreviewScan();

/*!
* @brief 暂停扫描		Pause Scan
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DPauseScan();

/*!
* @brief 结束扫描		End scan
* @param globalOptimize true 优化, false 不优化		“true”-optimize , “false” -not optimized
* @param pointDist 点距设置		setting point distance.
* @param callback 设置回调, 可以拿结束扫描后的点云数据		Set up the callback to get the point cloud data after finishing the scan.
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DEndScan(
	bool globalOptimize,
	double pointDist,
	Sn3DWholePointCloudCallBack wholeCallback = nullptr);

/*!
* @brief 取消扫描		Cancel Scan
* @param isCancelCurrentProjectFramerMark true 清空框架点信息			“true”- Empty the global markers information.
* @return 0成功，失败参考错误码
*/
[[deprecated]] SN3DSDKE10API int  Sn3DCancelScan(
	bool isCancelCurrentProjectFramerMark);

/*!
* @brief 清空扫描数据		Clear Scan Data
* @param isCancelCurrentProjectFramerMark true 清空框架点信息			"true" - Cancel current project framermark data
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int  Sn3DClearScan(
    bool isCancelCurrentProjectFramerMark);

/*!
* @brief 网格化数据			Generate mesh data
MeshType
* @param meshType.  Reference: MeshType
* @param filterLevel.  0-3档, 不同的滤波力度				0-3 level, different levels of filtering degree.
* @param SmoothLevel.  1-3档, 在滤波基础上设置平滑力度		1-3 level, Setting up smooth level in filter.
* @param PointDis.  点距									point distance.
* @param fillSmallHole.  true填充小洞						“true”-Fill the small hole
* @param smellHolePerimeter.  填充小洞周长					Fill the circumference of the small hole
* @param fillMarkerHole.  true填充标志点					“true”-filling markers.
* @param spikeSensitivity.									Remove the spikes
* @param faceLimit. 最大面片限制							Maximum number of patch.
* @param callback. 设置回调, 取网格化数据后的点云数据		setting up callback for getting point cloud data of meshed.
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DMesh(
	int meshType,
	int filterLevel,
	int SmoothLevel,
	float PointDis,
	bool fillSmallHole,
	double smellHolePerimeter,
	bool fillMarkerHole,
	double spikeSensitivity,
	int faceLimit,
	Sn3DWholePointCloudCallBack callback = nullptr);


//LSC新增MeshEx接口
/*!
* @brief 网格化数据		Generate mesh data
* @param meshType.  Reference: MeshType
* @param filterLevel.  0-3档, 不同的滤波力度				0-3 level, different levels of filtering degree.
* @param SmoothLevel.  1-3档, 在滤波基础上设置平滑力度		1-3 level, Setting up smooth level in filter.
* @param PointDis.  点距									point distance.
* @param fillSmallHole.  true填充小洞						“true”-Fill the small hole
* @param smellHolePerimeter.  填充小洞周长					Fill the circumference of the small hole
* @param fillMarkerHole.  true填充标志点					“true”-filling markers.
* @param spikeSensitivity.									Remove the spikes
* @param faceLimit. 最大面片限制							Maximum number of patch.
* @param callback. 设置回调, 取网格化数据后的网格数据		setting up callback for getting point cloud data of meshed.
* @param owner. 提供调用对象指针							owner of callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DMeshEx(
	int meshType,
	int filterLevel,
	int SmoothLevel,
	float PointDis,
	bool fillSmallHole,
	double smellHolePerimeter,
	bool fillMarkerHole,
	double spikeSensitivity,
	int faceLimit,
	void* owner,
	Sn3DMeshDataCallBack callback = nullptr);


/*!
* @brief 进入扫描			Enter scan
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DEnterScan();
/*!
* @brief 退出扫描		Quit scan mode
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DExitScan();

/*!
* @brief 注册回调, 只要整体点云数据刷新就触发		Register whole point cloud callback
* @param callBackFunc 回调函数						Whole point cloud callback 
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetWholePointCloudCallBack(Sn3DWholePointCloudCallBack callBackFunc);

/*!
* @brief 注册回调, 当前帧点云数据刷新触发		Register current point cloud callback
* @param callBackFunc 回调函数					current point cloud callback
* @return 0成功，失败参考错误码
*/

//LSC 新增接口
SN3DSDKE10API int CALLMETHOD Sn3DSetCurrentPointCloudCallBack(Sn3DCurrentPointCloudCallBack callBackFunc, void* owner);

/*!
* @brief 注册回调, 增量点云数据刷新触发			Register increase point cloud callback
* @param callBackFunc 回调函数					increase point cloud callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetIncreasePointCloudCallBack(Sn3DIncreasePointCloudCallBack callBackFunc, void* owner);

/*!
* @brief 注册回调, 相机视角数据刷新触发		Register Camera	Position callback
* @param callBackFunc 回调函数				Camera	Position callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetCameraPositionCallBack(Sn3DCameraPositionCallBack callBackFunc, void* owner);

/*!
* @brief 注册回调, 跟踪丢失状态数据刷新触发		Register track loss status callback
* @param callBackFunc 回调函数					track loss status callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetTrackLostStatusCallBack(Sn3DTrackLostStatusCallBack callBackFunc, void* owner);

/*!
* @brief 注册回调, 扫描距离挡位数据刷新触发		Register scan distance gear callback
* @param callBackFunc 回调函数					scan distance gear callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetScanDistCallBack(Sn3DScanDistCallBack callBackFunc, void* owner);

/*!
* @brief 注册回调, 设置设备操作摁键触发		Register device event callback
* @param callBackFunc 回调函数				device event callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetDeviceEventCallBack(Sn3DDeviceEventCallBack callBackFunc, void* owner);

/*
* @brief 注册回调, 当前标志点刷新触发			Register current marker callback
* @param callBackFunc 回调函数				device event callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetCurrentMarkerCallBack(Sn3DCurrentMarkerCallBack callBackFunc, void* owner);

/*
* @brief 注册回调, 全量标志点刷新触发		Register whole marker callback
* @param callBackFunc 回调函数				device event callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetWholeMarkerCallBack(Sn3DWholeMarkerCallBack callBackFunc, void* owner);

/*!
* @brief 注册回调, 平面提示状态数据刷新触发		Register too flat status callback
* @param callBackFunc 回调函数					track loss status callback
* @return 0成功，失败参考错误码
*/
SN3DSDKE10API int CALLMETHOD Sn3DSetTooFlatStatusCallBack(Sn3DTooFlatStatusCallBack callBackFunc, void* owner);

#ifdef __cplusplus
}
#endif
#endif // SN3DSDKE10_H
