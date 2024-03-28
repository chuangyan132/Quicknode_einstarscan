#ifndef SN3DMAINWINDOW_H
#define SN3DMAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QAction>
#include <QWidgetAction>
#include <QMessageBox>

#include "./subContrl/menuaction.h"
#include "./subContrl/sn3ddialogback.h"
#include "./subContrl/widgetcontent.h"
#include "Sn3DE10Sdk.h"
//#include "transmem.h"
#include "PublicClass.h"
#include <iostream>

namespace Ui {
class Sn3DMainWindow;
}

class VideoThread;
class MeshDialog;
class NewProjDlg;
//class ScanServiceImp;
class E7PlusMeshDlg;
class E7PlusNewProjDlg;
class E7PlusEnterScanDlg;
class OpenCreateSlnDlg;
class E7PlusNewFixProjDlg;
class E7PlusFixScanDlg;
class Sn3DDialogBack;

namespace sn3dsdkdemo
{
	class Device;
}

class Sn3DMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sn3DMainWindow(QWidget *parent = 0);
    ~Sn3DMainWindow();

    void createMenu();
	void initWidget();
	void initMember();

	Sn3DScanServiceWatcherCallBack m_Sn3DWatchFun;
	Sn3DWholePointCloudCallBack   m_Sn3DGetWholeCloudPoint;
	Sn3DCurrentPointCloudCallBack   m_Sn3DGetCurrentCloudPoint;
	Sn3DIncreasePointCloudCallBack m_Sn3DGetIncreaseCloudPoint;
	Sn3DCameraPositionCallBack	m_Sn3DGetCameraPosition;
	Sn3DTrackLostStatusCallBack m_Sn3DGetTrackLostStatus;
	Sn3DScanDistCallBack m_Sn3DGetScanDist;
	Sn3DCurrentMarkerCallBack m_Sn3DGetCurrentMarker;
	Sn3DWholeMarkerCallBack m_Sn3DGetWholeMarker;
	Sn3DTooFlatStatusCallBack m_Sn3DGetTooFlatStatus;
	//widget state control
	enum StateControl
	{
		INIT_STATE,
		ENTER_CALIB_STATE,		//enter calib state，widget in scan is disable.
		ENTER_SCAN_STATE,		//enter scan state，widget in calib is disable.
		DEVICE_ONLINE_STATE,	//check device online state,exit scan and exit calib is also this state
		DEVICE_OFFLINE_STATE,	//device offline state.
		NEW_PROJECT,			//new project finish
		FINISH_SCAN,			//scan finish
		FINISH_MESH				//mesh finish
	};

public slots:
    void on_tab_scanner_clicked();
    void on_tab_calibrate_clicked();
    void on_tab_scan_clicked();
	void on_btn_ct_stereo_clicked();
	void on_progressBar_valueChanged(int value);

	//device
	void on_pushButton_launchService_clicked();
	void on_pushButton_connectService_clicked();
	void on_pushButton_checkDeviceOnline_clicked();
	void on_pushButton_deviceReconnect_clicked();
	void on_pushButton_sendPing_clicked();
	//calib
	void on_pushButton_enterCalib_clicked();
	void on_pushButton_exitCalib_clicked();
	void on_pushButton_setCaliType_clicked();
	void on_pushButton_getCaliType_clicked();
	void on_pushButton_startSnap_clicked();
	void on_pushButton_stopSnap_clicked();
	//scan
	void on_pushButton_enterScan_clicked();
	void on_pushButton_exitScan_clicked();
	void on_pushButton_scanPreview_clicked();
	void on_pushButton_finishScan_clicked();
	void on_pushButton_cancelScan_clicked();
	void on_pushButton_savePointCloud_clicked();
	void on_pushButton_GetPostProcessParam_clicked();
	void on_pushButton_Mesh_clicked();
	void on_checkBox_pseudoColor_clicked();
	void on_pushButton_saveMesh_clicked();
	void on_pushButton_deletePoint_clicked();
    void on_pushButton_Brightness_clicked();
    void on_pushButton_Process_clicked();
	void on_pushButton_End_clicked();

	// set brightness 
// 	void on_horizontalSlider_image_valueChanged(int value);
// 	void on_horizontalSlider_laser_valueChanged(int value);
// 	void on_horizontalSlider_LED_valueChanged(int value);
// 	void on_horizontalSlider_laserQuality_valueChanged(int value);
// 	void on_buttonGroup_scanObjectType_buttonToggled(QAbstractButton *button, bool checked);
// 	void on_checkBox_bodyPortraitSwitch_clicked(bool checked);
// 	void on_checkBox_plainCheck_clicked(bool checked);
// 	void on_pushButton_setWorkRange_clicked();

	//update video
	void slot_updateVideo(int camId, const unsigned char* data, int width, int height, int channel);
	void slot_deviceEventOccurred(int event);
	void slot_propChanged(QString topicName, QString payloadBytes, QVariantMap payload);
	void slot_beginAsyncActionOccurred(QString payload_bytes, QVariantMap payload);
	void slot_finishAsyncActionOccurred(QString payload_bytes, QVariantMap payload);

	void slot_printInfo(QString info, LogType type = LogType::INTERAL_LOG);
// 	void on_btnWork_clicked(bool checked = false);
// 	void on_btnProject_clicked(bool checked = false);
	void onCalibMenuTriggered(QAction*action);
	void onResizeDialogBack(QSize size);
	void onSetDialogBackTitle(QString title);
	void onEnableStackWidget();
private:
	//interface state
	void switchInterfaceState(StateControl state);
	void iniState();
	void enterCalibState();
	void enterScanState();
	void deviceOnlineState();
	void deviceOfflineState();
	void newProjectState();
	void exitScan();
	// E7 plus device HD and Rapid end scan
	void e7PlusHREndScan(bool globalOptimize, double pointDist, bool reBuildData);
	void e7PlusEnterScan();
	void changeScanButtonText();
	void showE7PlusBrightness();
	bool isE7Device();
	void initCalibType();
	void initScanType();
	void setCalibTypeBtnStr(QString text);
	// bool isE7FixedScan();
	QString projExt(); // project file's ext name
    void switchScanUI();
    int parseScanStatus(const QString& scanStatus);
    void setDelPointEnabled(bool isEnabled);
	void getSn3DServiceRunStat(int code);

	//LSC Demo中，获取网格数据函数
	void getMeshData(LPSn3dMeshData meshData);
	void getCurrentCloud(LPSn3dPointCloud currentPointCloud);
	void getIncreaseCloud(LPSn3dIncreasePointCloud increasePointCloud);
	void getCameraPosition(LPSn3dCameraPosition cameraPosition);
	void getTrackLostStatus(bool status);
	void getScanDist(double dist);
	void getDeviceEvent(DeviceEvent event);
	//获取当前标志点信息
	void getCurrentMarker(LPSn3DMakerData currentMarker);
	void getWholeMarker(LPSn3DMakerData wholeMarker);
	void getTooFlatStatus(bool status);

private:
	Ui::Sn3DMainWindow *ui;
	//calib type
	QMenu* m_ct_stereoMenu = nullptr;
	int    m_curCalibType = 1;

	QString m_deviceSubType;                     
//	ScanServiceImp* m_scanServiceImp = nullptr;
	//scanning align type
	AlignType m_alignType = AT_FEATURES;
	//current scanning project name
	QString m_currentProjectPath;
	QString m_slnPath;
	QString m_slnDirPath;
	//post process parameters
	QVariantMap m_PostProcessParam;
	//meshDialog
	MeshDialog* m_meshDlg = nullptr;
	NewProjDlg* m_newProjDlg = nullptr;
	// mesh parameter
	double m_pointDist;
	ScanType m_scanType;
	//current state
	StateControl m_curState;
	//state control,filter needless event
	bool m_isClickedCheckDevice = false;
	bool m_isClickedFinishScan = false;
	bool m_isClickedExit = false;
	bool m_pseusedoColor = true;
    E7PlusMeshDlg* m_e7plusMeshDlg;
    E7PlusNewProjDlg* m_E7PlusNewProjDlg;
    E7PlusEnterScanDlg* m_E7PlusEnterScanDlg;
    OpenCreateSlnDlg* m_OpenCreateSlnDlg;
    bool m_isHRRefine = false; // Handheld repid scan with refine operation mode
    int m_handleScanAlignType = AT_FEATURES;
    bool m_rapidSave = false;
    E7PlusNewFixProjDlg* m_e7PlusNewFixProjDlg;
    E7PlusFixScanDlg* m_e7PlusFixScanDlg;
    bool m_canWorkProj = false; // can create/open work/proj
    sn3dsdkdemo::Device* m_pDevice = nullptr;
    int m_iScanStatus = SS_SCAN_STOPED;
    QAction* oneaction;
    QMenu* m_brightnessMenu;
    menuAction* m_brightnessAction;
    QWidgetAction* m_brightnesswidgetaction;
	Sn3DDialogBack* m_dialogback = nullptr;
	Sn3DDialogBack* m_dialogbackE7NewOpenProj = new Sn3DDialogBack;
	Sn3DDialogBack* m_dialogbackMesh = new Sn3DDialogBack;
	//first handle scan flag
	bool m_isE7FirstHandleScan = false;
	Sn3DGetImagesCallBack m_getImagCallback = nullptr;
	//LSC 相机亮度等级，初始值为最大值与最小值中间
	int BrightnessLevel = 8;
	int BrightnessMax;
	int BrightnessMin;
	//LSC 切换模式 双击OK键后，切换(服务端默认相机视角)WorkRange和Brightness
	bool DoubleClickModel = false;
};

#define DEBUG_TRACE(FUNCTION, RC, INFO)\
do \
{ \
	 if (RC == EC_SUCCESS) \
		 qDebug() << "==== "#FUNCTION << "SUCCESS [RC=" << RC << "]"; \
	 else \
		 qDebug() << "==== "#FUNCTION << "FAILED [RC=" << RC << "]"; \
} while (0);


#endif // SN3DMAINWINDOW_H
