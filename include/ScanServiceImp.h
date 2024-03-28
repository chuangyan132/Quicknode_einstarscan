#pragma once
#include <QBoxLayout>
#include <QScopedPointer>
#include <QMessageBox>
#include <QSettings>
#include <QLoggingCategory>
#include <QThread>
#include <QProcess>
#include "qjsonobject.h"

#include "SharedMemoryParser.h"
#include "transmem.h"
#include "PublicClass.h"
#include "VideoThread.h"
#include "Sn3DE10Sdk.h"
#include <QSemaphore>

//#include "e7plussharememory.h"

#define SDK_MAX_MSG_FRAME_SIZE 1048576 //1MB
#define DATA_READY 10

//typedef void(CALLBACK *Sn3DGetImagesCallBack)(int camId, unsigned char* data, int  width, int height, int channel);

class ScanServiceImp;
//ZMQ pub/sub mode，receive event and propChanged message.
class Subscriber : public QThread
{
	Q_OBJECT
public:
	Subscriber(void* zmqContext, quint16 port, QObject* parent = nullptr)
		: QThread(parent),
		m_zmqContext(zmqContext), m_port(port)
	{}

	~Subscriber() {};
	void run() override;
	bool receiveMessage(QByteArrayList &envelopeTokens, QByteArrayList &payloadFrames);
	void setImp(ScanServiceImp* impl) { m_impl = impl; }
signals:
	void error(QString msg);
	void received(QByteArray type, QByteArray objectName, QByteArray topicName, QByteArray payloadBytes, QVariantMap payload);
private:
	void* m_zmqContext;
	void* m_socket;
	quint16 m_port;
	char m_msgBuf[SDK_MAX_MSG_FRAME_SIZE];
	ScanServiceImp* m_impl = nullptr;

};


class ScanServiceImp :public QObject
{
	Q_OBJECT
	
public:
	static ScanServiceImp& getInstance();
	ScanServiceImp(QObject*parent = nullptr);
	~ScanServiceImp();


	bool launchServiceProcess(std::function<void(int code)> func);
	bool connectToService();
	//device
	void checkDevice(QString&result);
	void reconnectDevice(QString&result);
	void sendPing(QString&result);
	void getDeviceSubType(QString&result);
	bool hasCorrectPle();
	bool isDeviceOnLine();
	bool isOpenOrCreateSlnSuccess();
	bool isCreateNewProjectSuccess();
	bool isEnterScanSuccess();

	void installGetImageCallBack(Sn3DGetImagesCallBack, void* owner);
	

	bool isSuccess(int keyWord, int timeout = 0);
	//calib
	void enterProCali(QString&result);
	void exitProCali(QString&result);
	void setCalibType(int calibType, QString&result);
	void getCalibType(QString&result);
	void snapEnabled(bool enabled,QString&result);
	//void showVideo(bool enabled, QString&result);
	//scan
    void enterScan(int scanType, QString&result);
	void exitScan(QString&result);
    void openOrCreateSolution(const QJsonObject& params, QString&result);
    void openOrCreateSolution(QString payload, QString&result);
	void newProject(const QJsonObject& params, QString&result);
    void setScanType(int scanType, QString&result);
	void getScanType(QString payload, QString&result);
	void setEnablePseudoColor(bool enabled, QString&result);
	void getEnablePseudoColor(QString payload, QString&result);
	void setIsLedOpen(bool enabled, QString&result);
	void getIsLedOpen(QString payload, QString&result);
	void startScan(const QJsonObject& params, QString&result);
    void pauseScan(QString&result);
    void endScan(bool globalOptimize, double pointDist, bool reBuildData, QString&result);
	void cancelScan(const QJsonObject& params, QString&result);
    void save(const QJsonObject& params, QString&result);
	void getPostProcessParam(QString&result);
	void meshMultiProject(QString payload, QString&result);
    void meshMultiProject(const QJsonObject& params, QString&result);
    void applyEdit(const QJsonObject& params, QString&result);
    void applyEditE7(const QJsonObject& params, QString&result);

	//brightness
	void setCurrentBrightness(int data, QString&result);
	void setCurrentLaserBrightness(int data, QString&result); 
	void setCurrentLEDDutyCycle(int data, QString&result);
	void setCurrentLaserQualityStep(int data, QString&result);
	void setLaserScanObject(int data, QString&result); 
	void setIsBigWorkRange(bool data, QString&result);
	void setIsPlaneDetectOpen(bool data, QString&result);
	void setWorkRange(const QJsonObject& params);
	void getBrightnessRange(int& min, int&max);
	void getCurrentBrightness(int& currentBrightness);
	//sharedMemory
	void requestData(QString&result);
	void readDataEnd(QString&result);
	int serialEditSharedMemoryData(LPSMC_EDIT_DATA edit_data, int begin, int end);
    int serialEditSharedMemoryDataE7(LPSMC_EDIT_DATA edit_data, int begin, int end, bool isFixedScan);
    void setIsE7Device(bool isE7);
    void getWholePointData();
    void openProject(QString payload, QString&result);
    void openProject(QString projFileName, int type, QString&result);
	void setWholePointCloudCallBack(std::function<void(LPSn3dPointCloud)> func);
	void setCurrentPointCloudCallBack(std::function<void(LPSn3dPointCloud, void*)> func, void* owner);
	void setIncreasePointCloudCallBack(std::function<void(LPSn3dIncreasePointCloud, void*)> func, void* owner);
	void setCameraPositionCallBack(std::function<void(LPSn3dCameraPosition, void*)> func, void* owner);
	void setTrackLostStatusCallBack(std::function<void(bool, void*)> func, void* owner);
	void setScanDistCallBack(std::function<void(double, void*)> func, void* owner);
	void setDeviceEventCallBack(std::function<void(DeviceEvent, void*)> func, void* owner);

	void setMeshCallBack(std::function<void(LPSn3dPointCloud)> func);
	void setCurrentMarkerCallBack(std::function<void(LPSn3DMakerData, void*)>, void* owner);
	void setWholeMarkerCallBack(std::function<void(LPSn3DMakerData, void*)>, void* owener);
	//LSC IMP中声明网格回调函数Ex
	void setMeshCallBackEx(std::function<void(LPSn3dMeshData, void*)> func, void* owner);
	//void waitForCheckDeviceProcess();
	void setTooFlatStatusCallBack(std::function<void(bool, void*)> func, void* owner);


public slots:
	void onSubscriberError(QString msg);
	void onSubscriberReceived(QByteArray type, QByteArray objectName, QByteArray topicName, QByteArray payloadBytes, QVariantMap payload);
	void slot_requestData();
	void slot_readDataEnd();
	void slot_propChanged(QString topicName, QString payloadBytes, QVariantMap payload);
	void showVideo(QVariantMap imageMap);
signals:
	void sigDeviceEventOccurred(int event);
	void sigPropChanged(QString topicName, QString payloadBytes, QVariantMap payload);
	void sigBeginAsyncActionOccurred(QString payloadBytes, QVariantMap payload);
	void sigFinishAsyncActionOccurred(QString payloadBytes, QVariantMap payload);
	void sigPrintInfo(QString info,LogType type);
	void sigVideoImageChanged(QVariantMap imageMap);
    void sigCancelScan();
private:
	//handle service message.
	void handleEventMsg(QString topicName, QString payloadBytes, QVariantMap payload);
	void handlePropChangedMsg(QString topicName, QString payload_bytes, QVariantMap payload);
	void handleBeginAsyncAction(QString payloadBytes, QVariantMap payload);
	void handleFinishAsyncAction(QString payloadBytes, QVariantMap payload);
	void handleDataReady(QString memoryKey, int memoryType);
	void handlePointCloud(QString memoryKey);

    void sendMessageToService(QString topicName, QString payload, QString& result);
	//test code
	void savePointCloud();
	void saveMarkers();
    void endScan(QString payload, QString&result);
private:
	//service process,first launch.
	QProcess *m_ServiceProcess = nullptr;
	QProcess *m_HubProcess = nullptr;
	QProcess *m_SnSyncServiceProcess = nullptr;

	QScopedPointer<Subscriber, QScopedPointerDeleteLater> m_subscriber;
	QScopedPointer<QSettings> m_settings;

	/*QSemaphore m_checkDeviceSem;*/

	QMap<QString, QSemaphore*> m_processSem; // 动作对应的信号
	//message buffer
	char *m_msgBuf;
	void* m_owner = nullptr;


	//ZMQ
	void* m_zmqContext = nullptr;
	void* m_subSocket = nullptr;
	void* m_reqSocket = nullptr;
	const QString C_zmqVersion = "v3.0";
	const QString C_zmqObjectName = "scanservice";
	const QString C_msgTypeInvoke = "invoke";
	const QString C_msgTypePropGet = "propGet";
	const QString C_msgTypePropSet = "propSet";
	const QString C_msgTypePropChanged = "propChanged";
	const QString C_msgTypeEvent = "event";
	//get video thread 
	VideoThread*m_videoThread = nullptr;
	//parse shared memory
	SharedMemoryParser* m_sharedMemotyParser = nullptr;
	//shared memory Map
	QMap<QString, TransMem<PointCloudMemoryHeader>* > mapShareMemory;
    //E7PlusShareMemory m_e7shareMem;
    bool m_isE7 = false;
	bool m_hasCorrectPle = false;
	bool m_isDeviceOnLine = false;
	bool  m_openOrCreatSlnSuccess = false;
	bool  m_createNewProjectSuccess = false;
	bool  m_enterScanSuccess = false;

	QMap<int,bool> m_processSuccess;
	int m_dataReadyRet = 0;
	std::function<void(int)> m_watcherCallBackFunc = nullptr;
	std::function<void(int camId, const unsigned char* data, int  width, int height, int channel, void* owner)> m_getImagesCallBackFunc = nullptr;	
	std::function<void(Async* props)> finishAsyncCallBackFunc = nullptr;
	std::function<void(LPSn3dCameraPosition, void* owner)> m_CameraPositionCallBack = nullptr;
	void* m_CameraPosition_owner = nullptr;
	std::function<void(bool, void* owner)> m_TrackLostStatusCallBack = nullptr;
	void* m_TrackLostStatus_owner = nullptr;
	std::function<void(double, void*)> m_ScanDistCallBack = nullptr;
	void* m_ScanDist_owner = nullptr;
	std::function<void(DeviceEvent, void*)> m_DeviceEventCallBack = nullptr;
	void* m_DeviceEvent_owner = nullptr;
	std::function<void(bool, void* owner)> m_TooFlatCallBack = nullptr;
	void* m_TooFlatStatus_owner = nullptr;

	QMutex m_callbackMutex;
};

