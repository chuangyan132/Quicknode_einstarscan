#include "sn3dmainwindow.h"
#include "ui_sn3dmainwindow.h"
#include <QDebug>
#include <QWidgetAction>
#include "MeshDialog.h"
//#include "ScanServiceImp.h"
//#include "zmq.h"
//#include "VideoThread.h"
#include "NewProjDlg.h"
//#include "e7plusfinishscandlg.h"
//#include "e7plusmeshdlg.h"
//#include "e7plusnewprojdlg.h"
//#include "e7plusenterscandlg.h"
#include "opencreateslndlg.h"
//#include "e7plusnewfixprojdlg.h"
//#include "e7plusfixscandlg.h"
#include "sn3dfiledialog.h"
#include "newopenprojdialog.h"
#include "device.h"
#include "devicefactory.h"
#include "ui_opencreateslndlg.h"

#include "Sn3DE10Sdk.h"

#define UI_PAUSE_STR "Pause"
#define UI_PREVIEW_STR "Preview"
#define UI_SCAN_STR "Scan"

Sn3DMainWindow::Sn3DMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sn3DMainWindow),
    m_OpenCreateSlnDlg(new OpenCreateSlnDlg(this))
{
    ui->setupUi(this);
    createMenu();
}

Sn3DMainWindow::~Sn3DMainWindow()
{
    delete ui;
}

void Sn3DMainWindow::createMenu()
{
//     m_ct_stereoMenu = new QMenu(ui->btn_ct_stereo);
//     oneaction = new QAction(m_ct_stereoMenu);
//     oneaction->setText("1111");
//     m_ct_stereoMenu->addAction(oneaction);
//     ui->btn_ct_stereo->setMenu(m_ct_stereoMenu);

    m_brightnessMenu = new QMenu();


	m_brightnessMenu->setStyleSheet("border-color: rgb(170, 0, 0);");

    m_brightnessAction = new menuAction(m_brightnessMenu);
    m_brightnesswidgetaction = new QWidgetAction(m_brightnessMenu);
    m_brightnesswidgetaction->setDefaultWidget(m_brightnessAction);

    m_brightnessMenu->addAction(m_brightnesswidgetaction);
    ui->pushButton_Brightness->setMenu(m_brightnessMenu);
}

void Sn3DMainWindow::initWidget()
{
	ui->stackedWidget_buttons->setCurrentIndex(0);
	m_ct_stereoMenu = new QMenu(ui->btn_ct_stereo);
	m_ct_stereoMenu->setStyleSheet("QMenu::item {"
		"border-width: 1px;"
		"width: 150;"
		"height: 28;"
		"color: #666666;"
		"padding-left:50px;"
		"}"
		"QMenu::item:disabled {"
		"color: #828d91;"
		"}"
		"QMenu {"
		"border-width: 2px solid #44C8FF;"
		"border-radius: 3px;"
		"}"
		"QMenu::item:hover {"
		"background-color: #DFE9F1;"
		"border-width: 2px solid #3AAAD9;"
		"}QMenu::item:selected {background-color: #DFE9F1;"
		"border-width: 2px solid #3AAAD9;}");
	ui->btn_ct_stereo->setMenu(m_ct_stereoMenu);
	connect(m_ct_stereoMenu, SIGNAL(triggered(QAction *)), this, SLOT(onCalibMenuTriggered(QAction *)));

	switchInterfaceState(INIT_STATE);
}

void Sn3DMainWindow::initMember()
{
	m_meshDlg = new MeshDialog;
	m_meshDlg->setWindowFlags(Qt::FramelessWindowHint);
	m_dialogback = new Sn3DDialogBack;
	connect(m_meshDlg, &MeshDialog::sigMeshInfo, this, [this](QVariantMap info) {
		qDebug() << __FUNCTION__ << __LINE__;
	//	if (m_scanServiceImp)
		{
			QString result;
			QJsonObject jo(QJsonObject::fromVariantMap(info));
			QString strInfo = QJsonDocument(jo).toJson(QJsonDocument::Compact);
			QString params = QString("{\"meshInfo\":%1}").arg(strInfo);
			//Sn3DMesh();
		//	m_scanServiceImp->meshMultiProject(params, result);
			qDebug() << __FUNCTION__ << __LINE__ << "result:" << result << "params:" << params;
		}
	});
	m_dialogbackMesh->setWidgetContent(m_meshDlg);
	connect(m_meshDlg, &MeshDialog::sigClose, this, [this]() {
		m_dialogbackMesh->hide();
		ui->stackedWidget_buttons->setEnabled(true);
	});
	connect(m_dialogbackMesh, &Sn3DDialogBack::sigHide, this, &Sn3DMainWindow::onEnableStackWidget);
	connect(m_dialogback, &Sn3DDialogBack::sigHide, this, &Sn3DMainWindow::onEnableStackWidget);
	m_newProjDlg = new NewProjDlg;
	connect(m_newProjDlg, &NewProjDlg::sigPointDistChanged, [this](float pointDist) {
		m_pointDist = pointDist;
		ui->label_pointDist->setText( QString::number(pointDist));
	});
	connect(m_newProjDlg, &NewProjDlg::sigSizeChanged, this, &Sn3DMainWindow::onResizeDialogBack);
	connect(m_newProjDlg, &NewProjDlg::sigTitleChanged, this, &Sn3DMainWindow::onSetDialogBackTitle);

	connect(m_newProjDlg, &NewProjDlg::sigEnterScan, [this](int scanType) {
		m_newProjDlg->setEnabled(false);
		{
			m_scanType = static_cast<ScanType>(scanType);
			QString result;
			if (Sn3DEnterScan() == EC_SUCCESS)
			{
				m_newProjDlg->setEnabled(true);
			}
		
			
		//	m_scanServiceImp->setScanType(scanType, result);
			qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
			Sn3DEnterScan();
		}
	}); 
	connect(m_newProjDlg, &NewProjDlg::sigEscPressed, this, [this]() {
		{
			ui->stackedWidget_buttons->setEnabled(false);
			QString result;
			Sn3DExitScan();
			qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
		}
	});
	connect(m_newProjDlg, &NewProjDlg::sigNewSlnFinished, this, [this]() {
		qDebug() << __FUNCTION__ << __LINE__ << "result:";
		m_dialogback->hide();
		m_iScanStatus = parseScanStatus("SS_PRE_SCAN");
		switchScanUI();

	});

//	m_scanServiceImp = new ScanServiceImp;
// 	connect(m_scanServiceImp, &ScanServiceImp::sigBeginAsyncActionOccurred, this, &Sn3DMainWindow::slot_beginAsyncActionOccurred);
// 	connect(m_scanServiceImp, &ScanServiceImp::sigFinishAsyncActionOccurred, this, &Sn3DMainWindow::slot_finishAsyncActionOccurred);
// 	connect(m_scanServiceImp, &ScanServiceImp::sigPropChanged, this, &Sn3DMainWindow::slot_propChanged);
// 	connect(m_scanServiceImp, &ScanServiceImp::sigDeviceEventOccurred, this, &Sn3DMainWindow::slot_deviceEventOccurred);
// 	connect(m_scanServiceImp, &ScanServiceImp::sigPrintInfo, this, &Sn3DMainWindow::slot_printInfo);
// 	connect(m_scanServiceImp, &ScanServiceImp::sigVideoImageChanged, this, &Sn3DMainWindow::slot_updateVideo);
// 	connect(m_scanServiceImp, &ScanServiceImp::sigCancelScan, [this]() {
// 		ui->pushButton_exitScan->setEnabled(true);
//	});

//     connect(m_brightnessAction, &menuAction::sigImageBrightChanged, [this](int iValue){
//         if (m_pDevice)
//             m_pDevice->setCurrentBrightness(iValue);
//     });
// 
// 	connect(m_brightnessAction, &menuAction::sigLaserChanged, [this](int iValue) {
// 		if (m_pDevice)
// 			m_pDevice->setCurrentLaserBrightness(iValue);
// 	});
// 	connect(m_brightnessAction, &menuAction::sigLEDChanged, [this](int iValue) {
// 		if (m_pDevice)
// 			m_pDevice->setCurrentLEDDutyCycle(iValue);
// 	});
// 	connect(m_brightnessAction, &menuAction::sigLaserQualityChanged, [this](int iValue) {
// 		if (m_pDevice)
// 			m_pDevice->setCurrentLaserQualityStep(iValue);
// 	});
// 	connect(m_brightnessAction, &menuAction::sigLaserObjectType, [this](int iValue) {
// 		if (m_pDevice)
// 			m_pDevice->setLaserScanObject(iValue);
// 	});
// 	m_dialogbackE7NewOpenProj->setTitleText("Project");
// 	m_dialogbackE7NewOpenProj->setWidgetContent(m_OpenCreateSlnDlg);
}

void Sn3DMainWindow::on_tab_scanner_clicked()
{
    ui->stackedWidget_buttons->setCurrentIndex(0);
}

void Sn3DMainWindow::on_tab_calibrate_clicked()
{
    ui->stackedWidget_buttons->setCurrentIndex(1);
}

void Sn3DMainWindow::on_tab_scan_clicked()
{
    ui->stackedWidget_buttons->setCurrentIndex(2);
    enterScanState();
}

void Sn3DMainWindow::on_btn_ct_stereo_clicked()
{
    m_ct_stereoMenu->move(0,0);
    m_ct_stereoMenu->show();	
}

void Sn3DMainWindow::on_progressBar_valueChanged(int value)
{
	ui->label_progressValue->setText(QString::number(value));
}

void Sn3DMainWindow::on_pushButton_launchService_clicked()
{
// 	if (m_scanServiceImp == nullptr)
// 	{
// 		qDebug() << "m_scanServiceImp is null";
// 		return;
// 	}
//	m_scanServiceImp->launchServiceProcess();
	ui->pushButton_connectService->setEnabled(true);
	ui->pushButton_launchService->setEnabled(false);
}
void Sn3DMainWindow::on_pushButton_connectService_clicked()
{
#if 1
	//qDebug() << "enter on_pushButton_connectService_clicked~~~~~";
	m_Sn3DWatchFun = [](int code) {
		if (code == EC_PROCESSERROR)
		{
			// 如果检测到服务端异常退出需要重新
			Sn3DRelease(); // 先释放
            
			// 在线程重新调用 Sn3DInitialize
		}
	std::cout << code << std::endl; 
	//qDebug() << "Sn3DInitialize result-code:" << code;
	};


	m_Sn3DGetWholeCloudPoint = [](LPSn3dPointCloud points) {

	};
	m_Sn3DGetCurrentCloudPoint = [](LPSn3dPointCloud points, void*) {

	};
	m_Sn3DGetIncreaseCloudPoint = [](LPSn3dIncreasePointCloud points, void*) {

	};
	m_Sn3DGetCameraPosition = [](LPSn3dCameraPosition camera, void*) {
	
	};
	m_Sn3DGetTrackLostStatus = [](bool track, void*) {

	};
	m_Sn3DGetScanDist = [](double dist, void*) {

	};
	m_Sn3DGetCurrentMarker = [](LPSn3DMakerData currentMarker, void*) {

	};
	m_Sn3DGetWholeMarker = [](LPSn3DMakerData wholeMarker, void*) {

	};
	m_Sn3DGetTooFlatStatus = [](bool flat, void*) {

	};
	//Sn3DScanServiceWather f = std::bind(&Sn3DMainWindow::getSn3DServerStatus, this, std::placeholders::_1);

	int connectResult = Sn3DInitialize(m_Sn3DWatchFun);
	if (connectResult == EC_SUCCESS)
	{
		Sn3DSetWholePointCloudCallBack(m_Sn3DGetWholeCloudPoint);
		Sn3DSetCurrentPointCloudCallBack(m_Sn3DGetCurrentCloudPoint, nullptr);
		Sn3DSetIncreasePointCloudCallBack(m_Sn3DGetIncreaseCloudPoint, nullptr);
		Sn3DSetCameraPositionCallBack(m_Sn3DGetCameraPosition, nullptr);
		Sn3DSetTrackLostStatusCallBack(m_Sn3DGetTrackLostStatus, nullptr);
		Sn3DSetScanDistCallBack(m_Sn3DGetScanDist, nullptr);
		//Sn3DSetCurrentMarkerCallBack(m_Sn3DGetCurrentMarker, nullptr);


		ui->pushButton_connectService->setEnabled(false);
		ui->pushButton_checkDeviceOnline->setEnabled(true);
		ui->pushButton_sendPing->setEnabled(true);

	}
#else
	if (m_scanServiceImp == nullptr)
	{
		ui->pushButton_connectService->setEnabled(false);
		ui->pushButton_checkDeviceOnline->setEnabled(true);
		ui->pushButton_sendPing->setEnabled(true);
	}
#endif
}

void Sn3DMainWindow::on_pushButton_checkDeviceOnline_clicked()
{
#if 1
	m_isClickedCheckDevice = true;
	int ret = Sn3DConnectDevice();
	qDebug() << __FUNCTION__ << __LINE__;
	if (ret == 0)
	{
		switchInterfaceState(DEVICE_ONLINE_STATE); // 设备连接成功
		qDebug() << "===== Connect Device Success =====" << __LINE__;
	}
#else
	qDebug() << __FUNCTION__ << __LINE__;
	m_isClickedCheckDevice = true;
//	if (m_scanServiceImp)
	{
		QString result;
//		m_scanServiceImp->checkDevice(result);
		qDebug() << __FUNCTION__ << __LINE__ << "checkDevice result:" << result;
	}
#endif
}

void Sn3DMainWindow::on_pushButton_deviceReconnect_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
//	if (m_scanServiceImp)
	{
		QString result;
//		m_scanServiceImp->reconnectDevice(result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
	}
}

void Sn3DMainWindow::on_pushButton_sendPing_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
//	if (m_scanServiceImp)
	{
		QString result;
//		m_scanServiceImp->sendPing(result);
		qDebug() << __FUNCTION__ << __LINE__ << "ping result:" << result;
	}
}

void Sn3DMainWindow::on_pushButton_enterCalib_clicked()
{
	ui->stackedWidget_buttons->setEnabled(false);
	ui->tab_scan->setEnabled(false);
	qDebug() << __FUNCTION__ << __LINE__;
	if (m_pDevice)
	{
		m_pDevice->enterCalibrate();
	}
}

void Sn3DMainWindow::on_pushButton_exitCalib_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	if (m_pDevice)
	{
		m_pDevice->exitCalibrate();
	}
}

void Sn3DMainWindow::on_pushButton_setCaliType_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	if (m_pDevice)
	{
		m_pDevice->setCalibrateType(m_curCalibType);
	}
}

void Sn3DMainWindow::on_pushButton_getCaliType_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	if (m_pDevice)
	{
		QString curCaliType;
		m_pDevice->calibrateType(curCaliType);
		QList<QAction*> actList = m_ct_stereoMenu->actions();
		for (int i = 0;i<actList.size();i++)
		{
			QAction*cur = actList.at(i);
			if (cur->text() == curCaliType)
			{
				setCalibTypeBtnStr(curCaliType);
			}
		}
	}
}

void Sn3DMainWindow::on_pushButton_startSnap_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	if (m_pDevice)
	{
		m_pDevice->startSnap();
		ui->pushButton_startSnap->setEnabled(false);
		ui->pushButton_stopSnap->setEnabled(true);
	}
}

void Sn3DMainWindow::on_pushButton_stopSnap_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	if (m_pDevice)
	{
		m_pDevice->stopSnap();
		ui->pushButton_startSnap->setEnabled(true);
		ui->pushButton_stopSnap->setEnabled(false);
	}
}

void Sn3DMainWindow::on_pushButton_enterScan_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
    ui->stackedWidget_buttons->setEnabled(false);


		if (m_dialogback)
		{
			m_newProjDlg->showInit("DST_E10", nullptr);
			m_dialogback->setWidgetContent(m_newProjDlg);
			m_dialogback->exec();
		}
	
}

void Sn3DMainWindow::on_pushButton_exitScan_clicked()
{
    qDebug() << __FUNCTION__ << __LINE__;
    ui->stackedWidget_buttons->setEnabled(false);
	if (m_pDevice) {
		m_pDevice->stopSnap();
		m_pDevice->exitScan();
		m_canWorkProj = false;
        switchInterfaceState(DEVICE_ONLINE_STATE); // No finish notify for exit scan, update UI here
// 		ui->btnWork->setEnabled(false);
// 		ui->btnProject->setEnabled(false);
		
	}
}

void Sn3DMainWindow::on_pushButton_scanPreview_clicked()
{
#if 1
	ui->stackedWidget_buttons->setEnabled(false);
	
	Sn3DPreviewScan();
	if(Sn3DStartScan() != EC_SUCCESS)
		ui->stackedWidget_buttons->setEnabled(true);

	m_canWorkProj = false;
	ui->pushButton_savePointCloud->setEnabled(false);

#else
	//need to invoke two times,first is preview,second is scan
	qDebug() << __FUNCTION__ << __LINE__;
    if (m_pDevice){
        ui->stackedWidget_buttons->setEnabled(false);
        if (m_pDevice->startScan() != sn3dsdkdemo::OK)
            ui->stackedWidget_buttons->setEnabled(true);
        m_canWorkProj = false;
    }
	ui->pushButton_savePointCloud->setEnabled(false);
#endif
//	if (m_pDevice)
//	{
//		bool isFixScan = m_pDevice->isFixScan();
//
//		if (!isFixScan)
//			changeScanButtonText();
//
//		int ret = m_pDevice->startScan();
//		if (isFixScan) {
////			ui->horizontalSlider_image->setEnabled(false);
//			if (ret == sn3dsdkdemo::OK) {
//				changeScanButtonText();
////				ui->groupBox_brightness->setEnabled(true);
//// 				if (m_pDevice->isHDR())
//// 					ui->horizontalSlider_image->setEnabled(true);
//				ui->pushButton_finishScan->setEnabled(false);
//			}
//// 			else
//// 				ui->horizontalSlider_image->setEnabled(true);
//		}
//		else {
////			ui->groupBox_brightness->setEnabled(true);
//		}
//		m_canWorkProj = false;
// 		ui->btnWork->setEnabled(false);
// 		ui->btnProject->setEnabled(false);
// 	}
}

void Sn3DMainWindow::on_pushButton_finishScan_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	ui->pushButton_finishScan->setEnabled(false);
	m_isClickedFinishScan = true;
// 	if (m_pDevice && m_pDevice->isFixScan())
// 		ui->horizontalSlider_image->setEnabled(true);
//	if (m_scanServiceImp)
	{
        ui->stackedWidget_buttons->setEnabled(false);
		QString result;
		//laser scan only has align type option
		QJsonObject jo;
// 		if (isE7Device()) {
//             if (m_pDevice){
//                 if (m_pDevice->endScan() != sn3dsdkdemo::OK)
//                     ui->stackedWidget_buttons->setEnabled(true);
//             }else
//                 ui->stackedWidget_buttons->setEnabled(true);
// 		}
/*		else */{
			bool globalOptimize = ui->checkBox_opt->isChecked();
			Sn3DEndScan(globalOptimize, m_pointDist, false);
//			m_scanServiceImp->endScan(globalOptimize, m_pointDist, false, result);
			qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
			ui->pushButton_scanPreview->setText("Preview");
			ui->pushButton_exitScan->setEnabled(true);
			if (m_pDevice && m_pDevice->isFixScan())
				ui->pushButton_scanPreview->setText("Scan");
            ui->pushButton_savePointCloud->setEnabled(true);
		}
		m_canWorkProj = true;
// 		ui->btnWork->setEnabled(true);
// 		ui->btnProject->setEnabled(true);
	}
}

	void Sn3DMainWindow::on_pushButton_cancelScan_clicked()
	{
		//ScanService::cancelScan(QVariantList dataNames, bool isCancelCurrentProjectFramerMark)
		qDebug() << __FUNCTION__ << __LINE__;
		if (m_pDevice) {
            ui->stackedWidget_buttons->setEnabled(false);
            m_pDevice->cancelScan();
            m_canWorkProj = false;
// 			if (m_pDevice->isFixScan())
// 				ui->horizontalSlider_image->setEnabled(true);
			/*ui->pushButton_exitScan->setEnabled(true);
			m_pDevice->cancelScan();
			ui->pushButton_scanPreview->setText("Preview");

			m_canWorkProj = false;*/
// 			ui->btnWork->setEnabled(false);
// 			ui->btnProject->setEnabled(false);
		}
}

void Sn3DMainWindow::on_pushButton_savePointCloud_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	if (m_pDevice) {
        ui->stackedWidget_buttons->setEnabled(false);
        if (m_pDevice->saveCloudPoint(m_currentProjectPath) != sn3dsdkdemo::OK)
            ui->stackedWidget_buttons->setEnabled(true);
	}
}

void Sn3DMainWindow::on_pushButton_GetPostProcessParam_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
//	if (m_scanServiceImp)
	{
		QString result;
//		m_scanServiceImp->getPostProcessParam(result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
	}
}

void Sn3DMainWindow::on_pushButton_Mesh_clicked()
{
    ui->stackedWidget_buttons->setEnabled(false);
	if (m_deviceSubType == C_E3HDevice)
	{
//		if (m_scanServiceImp)
		{
			QString result;
			QJsonObject jo;
			jo.insert("type", 0);
			jo.insert("resolution", m_pointDist);
			jo.insert("dataOptimiaztion", true);
//			m_scanServiceImp->meshMultiProject(jo, result);
			qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
		}
	}
	else if (m_deviceSubType == C_E3HXDevice)
	{
		if (m_dialogbackMesh)
		{
			m_meshDlg->setPointDistance(m_pointDist);
			double multiBoundBox = m_PostProcessParam["multiBoundBox"].toDouble();
			m_meshDlg->setMinSmallPartArea(multiBoundBox);
			m_meshDlg->setScanType(m_scanType);
			m_meshDlg->setDeviceSubType(m_deviceSubType);
			m_dialogbackMesh->setTitleText("Mesh");
			m_dialogbackMesh->exec();
		}

	}
// 	else if (isE7Device())
// 	{
//         if (m_pDevice){
//             if (m_pDevice->mesh() != sn3dsdkdemo::OK)
//                 ui->stackedWidget_buttons->setEnabled(true);
//         }
//         else
//             ui->stackedWidget_buttons->setEnabled(true);
// 	}
/*	else */if (m_deviceSubType == C_E10Device)
	{
		if (m_dialogbackMesh)
		{
			m_meshDlg->setPointDistance(m_pointDist);
			double multiBoundBox = m_PostProcessParam["multiBoundBox"].toDouble();
			m_meshDlg->setMinSmallPartArea(multiBoundBox);
			m_meshDlg->setScanType(m_scanType);
			m_meshDlg->setDeviceSubType(m_deviceSubType);
			m_dialogbackMesh->setTitleText("Mesh");
			m_dialogbackMesh->exec();
		}

	}
	else
	{
		QString log = "Unknown device sub type:" + m_deviceSubType;
		slot_printInfo(log, LogType::ERROR_MESSAGE);
	}
}

void Sn3DMainWindow::on_checkBox_pseudoColor_clicked()
{
	ui->stackedWidget_buttons->setEnabled(false);
	if (m_deviceSubType == C_E10Device)
	{
		QString result;
		m_pseusedoColor= ui->checkBox_pseudoColor->isChecked();
//	    m_scanServiceImp->setEnablePseudoColor(m_pseusedoColor, result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
	}
	ui->stackedWidget_buttons->setEnabled(true);
}

void Sn3DMainWindow::on_pushButton_saveMesh_clicked()
{
	//ScanService::save(QString file, QVariantMap params, QVariantList projList)
	qDebug() << __FUNCTION__ << __LINE__;
    ui->stackedWidget_buttons->setEnabled(false);

	if (m_pDevice) {
		m_pDevice->saveMesh(m_currentProjectPath);
	}
	ui->stackedWidget_buttons->setEnabled(true);
}

void Sn3DMainWindow::on_pushButton_deletePoint_clicked()
{
//	qDebug() << __FUNCTION__ << __LINE__;
////	if (m_scanServiceImp)
//	{
//		int beginIndex = ui->lineEdit_deleteBeginIndex->text().toInt();
//		int endIndex = ui->lineEdit_deleteEndIndex->text().toInt();
//		if (endIndex < beginIndex)
//		{
//			qDebug() << __FUNCTION__ << __LINE__ << "input is illeague.";
//			return;
//		}
//		int ret = SMC_ERR_NO_ERROR;
//		if (isE7Device() && m_pDevice)
//			ret = m_pDevice->deletePoint(beginIndex, endIndex);
//		else {
//			//first need serial shared memory 
//			SMC_EDIT_DATA edit_data;
//			edit_data.name_count = 1;
//			edit_data.data_names = new char*[edit_data.name_count];
//			edit_data.data_buffer = nullptr;
//			QByteArray baCurProj = m_currentProjectPath.toLocal8Bit();
//			edit_data.data_names[0] = new char[baCurProj.size() + 1];
//			memset(edit_data.data_names[0], 0, baCurProj.size() + 1);
//			memcpy(edit_data.data_names[0], baCurProj.data(), baCurProj.size());
//
//			edit_data.data_counts = new int[edit_data.name_count];
//			edit_data.data_counts[0] = endIndex - beginIndex + 1;
//
//// 			ret = m_scanServiceImp->serialEditSharedMemoryData(&edit_data, beginIndex, endIndex);
//// 			if (ret != SMC_ERR_NO_ERROR)
//// 			{
//// 				return;
//// 			}
//			QString result;
//			QJsonObject jo;
//			jo.insert("apply", true);
//			jo.insert("memoryKey", SMC_MK_EDIT_APPLY_DATA);
//			jo.insert("totalProject", edit_data.name_count);
//			jo.insert("currentProjectIndex", 0);
////			m_scanServiceImp->applyEdit(jo, result);
//		}
//
//		qDebug() << __FUNCTION__ << __LINE__;
//	}
}

void Sn3DMainWindow::slot_updateVideo(int camId, const unsigned char* data, int width, int height, int channel)
{
// 	int camId = param["camId"].toInt();
// 	QImage vidoeImage = param["vidoeImage"].value<QImage>();
// 	QString path = param["path"].toString();
	qDebug() << "----- camId: " << camId << "width: " << width << "height: " << height ;

	QImage vidoeImage = QImage(data, width, height, QImage::Format_RGB888);

	QRect rectArea = vidoeImage.rect();

	QPixmap pixmap = QPixmap::fromImage(vidoeImage);
	QSize size = ui->camera_left->size();
	pixmap = pixmap.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
	if (camId == 0)
	{
		ui->camera_left->updateImage(pixmap);
	}
	else if (camId == 1)
	{
		ui->camera_right->updateImage(pixmap);
	}
	else
	{
		ui->camera_color->updateImage(pixmap);
	}
}

void Sn3DMainWindow::slot_deviceEventOccurred(int event)
{
	qDebug() << "event:" << event;
	switch (event)
	{
	case DE_CLICK:
		if (m_curState == ENTER_CALIB_STATE)
		{
			if (ui->pushButton_stopSnap->isEnabled())
				ui->pushButton_stopSnap->clicked();
			else if (ui->pushButton_startSnap->isEnabled())
				ui->pushButton_startSnap->clicked();
		}
		else if (ui->pushButton_scanPreview->isEnabled())
		{
			ui->pushButton_scanPreview->clicked();
		}
	default:
		break;
	}
}

void Sn3DMainWindow::slot_propChanged(QString topicName, QString payloadBytes, QVariantMap payload)
{
	
	if (topicName == "deviceStatus")
	{
		QString status = payload["data"].toString();
//		if (m_scanServiceImp)
		{
			QString result;
//			m_scanServiceImp->getDeviceSubType(result);
			qDebug() << __FUNCTION__ << __LINE__ << "getDeviceSubType result:" << result;
			QVariantMap map;
			sn3dsdkdemo::strJson2Map(result, map);
			m_deviceSubType = map["data"].toString();
//			m_scanServiceImp->setIsE7Device(isE7Device());
		}
		if (status == "DS_ONLINE" && m_isClickedCheckDevice)
		{
			if (m_pDevice)
				delete m_pDevice;
		//	m_pDevice = DeviceFactory::createDevice(this, m_deviceSubType, m_scanServiceImp);
			initCalibType();
			m_isClickedCheckDevice = false;
			switchInterfaceState(DEVICE_ONLINE_STATE);
		}
		if (status == "DS_OFFLINE")
		{
			switchInterfaceState(DEVICE_OFFLINE_STATE);
		}
		ui->label_devicestatus->setText(status);
		ui->label_devicesubtype->setText(m_deviceSubType);
	}
	else if (topicName == "framerate")
	{
		ui->label_frameRate->setText(payload["data"].toString());
	}
	else if (topicName == "deviceSeries")
	{
		ui->label_deviceseries->setText(payload["data"].toString());
	}
	else if (topicName == "isTextureAlignEnable")
	{
		ui->label_texturealignenable->setText(payload["data"].toString());
	}
	else if (topicName == "currentCaliDist")
	{
		if (payload["data"].toString() == "99" && ui->pushButton_stopSnap->isEnabled())
		{
			ui->pushButton_stopSnap->clicked();
		}
		ui->label_currentcalibdist_num->setText( payload["data"].toString());
	}
	else if (topicName == "currentCaliGroup")
	{
		// double camera calib group
		ui->label_curentcalibgroup_step->setText(payload["data"].toString());
	}
	else if (topicName == "updateFirm")
	{
		//need update firmware
		ui->label_updatefirm->setText( payload["data"].toString());
	}
	else if (topicName == "authSurplusDay")
	{
		//surplus day,device is or nou activation by this 
		//surplusDay = -1  out of time,= -2 serial number is not match, = -3 register create failed.
		ui->label_authsurplusday->setText( payload["data"].toString()+"Day");
	}
	else if (topicName == "hasColor")
	{
		//has texture camera
		ui->label_hascolor->setText(payload["data"].toString());
	}
	else if (topicName == "deviceEdition")
	{
		ui->label_deviceedition->setText( payload["data"].toString());
	}
	else if (topicName == "currentPle")
	{
		//current use ple file.
		QString pleFile = payload["data"].toString();
		//ui->label_currentPle->setText("CurrentPle:" + pleFile);
	}
	else if (topicName == "progress")
	{
		ui->progressBar->setValue(payload["data"].toInt());
	}
	else if (topicName == "pointCount")
	{
		int pointCount = payload["data"].toInt();
		ui->label_pointCount->setText( QString::number(pointCount));
	}
	else if (topicName == "postProcessParamChanged")
	{
		m_PostProcessParam = payload["data"].toMap();
	}
	else if (topicName == "handleScanAlignType") {
		m_handleScanAlignType = payload["data"].toInt();
	}
	else if (topicName == "rapidSave") {
		m_rapidSave = payload["data"].toBool();
	}
    else if ("scanStatus" == topicName){
        QString scanStatus = payload["data"].toString();
        m_iScanStatus = parseScanStatus(scanStatus);
        switchScanUI();
    }
	
	else {
		slot_printInfo("handlePropChangedMsg \ntopicName:" + topicName + "\t" + payloadBytes, LogType::RECEIVE_MESSAGE);
	}
}

void Sn3DMainWindow::slot_beginAsyncActionOccurred(QString payload_bytes, QVariantMap payload)
{

}

void Sn3DMainWindow::slot_finishAsyncActionOccurred(QString payload_bytes, QVariantMap payload)
{
	ui->stackedWidget_buttons->setEnabled(true);
	this->setEnabled(true);
	ui->progressBar->setValue(100);
	if (m_newProjDlg)
	{
		m_newProjDlg->setEnabled(true);
		m_newProjDlg->setFocus();
	}
	QVariantMap data = payload["data"].toMap();
	int type = data["type"].toInt();

	QString vResult = data["result"].toString();
	QJsonParseError error;
	QJsonDocument jd(QJsonDocument::fromJson(vResult.toLocal8Bit(), &error));
	QJsonObject jo = jd.object();
	QVariantMap result = jo.toVariantMap();
	bool success = result["_success"].toBool();
	int errCode = result["_error"].toInt();

	QVariantMap props = data["props"].toMap();
	switch (type)
	{
	case AAT_OPEN_PROJECT:
	{
		m_handleScanAlignType = props["alignMode"].toInt();
	}
	case AAT_MULTI_PROJECT_LOAD_MAIN_PROJECT:
	{
		//todo  main project opened.
		qDebug() << __FUNCTION__ << __LINE__ << "main project opened..";
	}
	case AAT_NEW_PROJECT:
	{
		if (m_dialogback->isVisible())
		{
			m_dialogback->hide();
		}
		m_currentProjectPath = props["currentProjectPath"].toString();
		if (m_pDevice)
			m_pDevice->setProjectName(m_currentProjectPath);
		if (success)
		{
			switchInterfaceState(NEW_PROJECT);
		}
	}
	break;
	case AAT_LOAING_SOLUTION:
	{
		m_slnPath = props["filePath"].toString();
		if (isE7Device()) {
            if (m_pDevice){
                if(m_pDevice->isCreateGroup())
                    m_pDevice->newProject();
            }
		}
		else {
			m_slnPath = props["filePath"].toString();
			if (props["createSolution"].toBool())
			{
				if (m_newProjDlg)
				{
					m_newProjDlg->setIsE7Plus(isE7Device());
					int code = m_newProjDlg->createNewProject();
					if (EC_SUCCESS == code)
					{
						auto getImageCallback = [](int camId, const unsigned char* data, int  width, int height, int channel, void* owner) {
							((Sn3DMainWindow*)owner)->slot_updateVideo(camId, data, width, height, channel);

						};
					//	Sn3DIntallGetImagasCallBack(getImageCallback, this); // 注册回调
					}
                    
				}
			}
			else
			{

			}
		}
		m_canWorkProj = true;
// 		ui->btnWork->setEnabled(true);
// 		ui->btnProject->setEnabled(true);
	}
	break;
	case AAT_ENTER_CALI:
	{
		if (success)
		{
			switchInterfaceState(ENTER_CALIB_STATE);
		}
		else
		{
			if (errCode == SN3D_RET_DEVICE_LICENSE_ERROR)
			{
				QMessageBox::warning(this, "Warning!", "License error!!!");
			}
		}
	}
	break;
	case AAT_EXIT_CALI:
	{
		if (success)
		{
			switchInterfaceState(DEVICE_ONLINE_STATE);
		}
	}
	break;
	case AAT_COMPUTE_CALI:
	{
		if (ui->pushButton_stopSnap->isEnabled())
		{
			ui->pushButton_stopSnap->clicked();
		}
	}
	break;
	case AAT_EXIT_SCAN:
	{
		if (success)
		{
			switchInterfaceState(DEVICE_ONLINE_STATE);
		}
	}
	break;
	case AAT_ENTER_SCAN:
	{
		if (success)
		{
			switchInterfaceState(ENTER_SCAN_STATE);
		}
		else
		{
			if (errCode == SN3D_RET_DEVICE_LICENSE_ERROR)
			{
				if (m_dialogback)
				{
					m_dialogback->hide();
					QMessageBox::warning(this, "Warning!", "License error!!!");
				}
			}
		}
		if (isE7Device()&& m_pDevice) {
			if (QDialog::Accepted == m_dialogbackE7NewOpenProj->exec()) {
				if (m_pDevice->isFixScan())
				{
					m_isE7FirstHandleScan = false;
				}
				else
					m_isE7FirstHandleScan = true;
				if (m_OpenCreateSlnDlg->isCreate())
					m_pDevice->newProjectGroup();
				else
					m_pDevice->openProjectGroup();
			}
		}
	}
	break;
	case AAT_END_SCAN:
	{
        ui->scan_page->setEnabled(true);
		if (success && m_isClickedFinishScan)
		{
			m_isClickedFinishScan = false;
			switchInterfaceState(FINISH_SCAN);
		}
	}
	break;
	case AAT_MULTI_PROJECT_MESHING:
	{
        ui->scan_page->setEnabled(true);
		switchInterfaceState(FINISH_MESH);
	}
	break;
	case AAT_FIX_SCAN:
	{
        ui->scan_page->setEnabled(true);
		if (ST_FIXED == m_scanType)
			// FinishAsyncAction is the end of fix scan
			switchInterfaceState(FINISH_SCAN);
	}
	break;
	case AAT_MESH:
	{
        ui->scan_page->setEnabled(true);
		if (ST_FIXED == m_scanType)
			// FinishAsyncAction is the end of fix scan
			switchInterfaceState(FINISH_MESH);
	}
	break;
	case AAT_MULTI_PROJECT_LOAD_SUB_PROJECT_DATA:
	{
        ui->scan_page->setEnabled(true);
//		m_scanServiceImp->getWholePointData();
	}
	break;
    case AAT_CANCEL_SCAN:
    {
        ui->scan_page->setEnabled(true);
        if (m_pDevice){
            bool isFixScan = m_pDevice->isFixScan();
            if (isFixScan)
                m_brightnessAction->setEnabled(true);
            ui->pushButton_exitScan->setEnabled(true);
            // ui.pushButton_scanPreview->setText(m_pDevice->isFixScan() ? tr(UI_SCAN_STR) : tr(UI_PREVIEW_STR));
            m_canWorkProj = false;
            // ui->btnWork->setEnabled(false);
            // ui->btnProject->setEnabled(false);
        }
    }
    break;
    case AAT_SCAN:
    {
        ui->scan_page->setEnabled(true);
        if (m_pDevice){
            bool isFixScan = m_pDevice->isFixScan();
            if (isFixScan){
                m_brightnessAction->setEnabled(true);
                if (m_pDevice->isHDR())
                    m_brightnessAction->setEnabled(false);
                ui->pushButton_finishScan->setEnabled(false);
            }
            else{
                m_brightnessAction->setEnabled(true);
            }
            m_canWorkProj = false;
            // ui.btnWork->setEnabled(false);
            // ui.btnProject->setEnabled(false);
        }
        ui->pushButton_scanPreview->setEnabled(true);
    }
    break;
    case AAT_SAVE:
    {
        ui->scan_page->setEnabled(true);
        ui->pushButton_savePointCloud->setEnabled(true);
        ui->pushButton_Mesh->setEnabled(true);
        //ui->pushButton_saveMesh->setEnabled(true);
        ui->pushButton_exitScan->setEnabled(true);
    }
    break;
    case AAT_MULTI_PROJECT_SAVE:
    {
        ui->scan_page->setEnabled(true);
        // @note same as AAT_SAVE, change this process if need
        ui->pushButton_savePointCloud->setEnabled(true);
        ui->pushButton_Mesh->setEnabled(true);
        //ui->pushButton_saveMesh->setEnabled(true);
        ui->pushButton_exitScan->setEnabled(true);
    }
    break;
	default:
		break;
	}
}

void Sn3DMainWindow::slot_printInfo(QString info, LogType type)
{
	QColor color = Qt::white;
	switch (type)
	{
	case LogType::SEND_MESSAGE:
		if (!ui->cb_sendmsg->isChecked())
		{
			return;
		}
		color = QColor(62, 162, 255);//蓝色
		break;
	case LogType::RECEIVE_MESSAGE:
		if (!ui->cb_receivemsg->isChecked())
		{
			return;
		}
		color = QColor(73,170,25);//绿色
		break;
	case LogType::ERROR_MESSAGE:
		if (!ui->cb_errormsg->isChecked())
		{
			return;
		}
		color = QColor(166,29,36);//红色
		break;
	case LogType::INTERAL_LOG:
		if (!ui->cb_interallog->isChecked())
		{
			return;
		}
		color = Qt::white;
		break;
	default:
		break;
	}
	ui->textBrowser->setTextColor(color);
	ui->textBrowser->append(info);
}

void Sn3DMainWindow::onCalibMenuTriggered(QAction*action)
{
	QString text = action->text();
	setCalibTypeBtnStr(text);
	m_curCalibType = action->data().toInt();
}

void Sn3DMainWindow::switchInterfaceState(StateControl state)
{
	//return; @note debug
	m_curState = state;
	switch (state)
	{
	case Sn3DMainWindow::INIT_STATE:
		iniState();
		break;
	case Sn3DMainWindow::ENTER_CALIB_STATE:
		enterCalibState();
		break;
	case Sn3DMainWindow::ENTER_SCAN_STATE:
		enterScanState();
		break;
	case DEVICE_ONLINE_STATE:
		deviceOnlineState();
		break;
	case DEVICE_OFFLINE_STATE:
		deviceOfflineState();
		break;
	case NEW_PROJECT:
		newProjectState();
		break;
	case FINISH_SCAN:
	{
        ui->pushButton_scanPreview->setEnabled(true);
        ui->pushButton_cancelScan->setEnabled(true);
        ui->pushButton_savePointCloud->setEnabled(true);
        ui->pushButton_finishScan->setEnabled(!m_pDevice->isFixScan());
		ui->pushButton_exitScan->setEnabled(true);
		ui->pushButton_Mesh->setEnabled(true);
        setDelPointEnabled(true);
	}
	break;
	case FINISH_MESH:
	{
        ui->pushButton_scanPreview->setEnabled(true);
        ui->pushButton_cancelScan->setEnabled(false);
        ui->pushButton_savePointCloud->setEnabled(true);
        ui->pushButton_finishScan->setEnabled(!m_pDevice->isFixScan());
		ui->pushButton_exitScan->setEnabled(true);
		ui->pushButton_savePointCloud->setEnabled(true);
		ui->pushButton_Mesh->setEnabled(true);
		ui->pushButton_saveMesh->setEnabled(true);
        setDelPointEnabled(true);
	}
	break;
	default:
		break;
	}
}

void Sn3DMainWindow::iniState()
{
	ui->tab_scan->setEnabled(false);
#if USEOLDDEMO
	ui->tab_calibrate->setEnabled(false);
    ui->pushButton_deviceReconnect->setEnabled(false);
    ui->pushButton_sendPing->setEnabled(false);
#else
	//原本是false
    ui->tab_calibrate->setVisible(false);
    ui->pushButton_deviceReconnect->setVisible(false);
    ui->pushButton_sendPing->setVisible(false);
#endif
	ui->pushButton_launchService->setVisible(false);
	ui->pushButton_connectService->setEnabled(true);
	ui->pushButton_checkDeviceOnline->setEnabled(false);
	ui->label_logo->setVisible(false);	// sd
	ui->textBrowser->setVisible(false);	// sd
	ui->cb_sendmsg->setVisible(false);	// sd
	ui->cb_receivemsg->setVisible(false);	// sd
	ui->cb_errormsg->setVisible(false);	// sd
	ui->cb_interallog->setVisible(false);	// sd
	ui->progressBar->setVisible(false);	// sd
	ui->label_29->setVisible(false);	// sd
	ui->label_31->setVisible(false);	// sd
	ui->label_progressValue->setVisible(false);	// sd
	ui->widget_9->setVisible(false);	// sd
	ui->debugwindow->setVisible(false);	// sd
}

void Sn3DMainWindow::enterCalibState()
{
	ui->tab_scan->setEnabled(false); // false

	ui->pushButton_enterCalib->setEnabled(false); //false
	ui->pushButton_exitCalib->setEnabled(true);
	ui->btn_ct_stereo->setEnabled(true);
	ui->pushButton_setCaliType->setEnabled(true);
	ui->pushButton_getCaliType->setEnabled(true);

	ui->pushButton_startSnap->setEnabled(true);
	ui->pushButton_stopSnap->setEnabled(false); 
}

void Sn3DMainWindow::enterScanState()
{
	ui->tab_calibrate->setEnabled(false);
	ui->tab_scan->setEnabled(true);
	//scan
#if USEOLDDEMO
    ui->pushButton_enterScan->setEnabled(false);
    ui->pushButton_newProject->setEnabled(false);
    ui->pushButton_exitScan->setEnabled(true);
    ui->pushButton_Brightness->setEnabled(true);
    ui->pushButton_scanPreview->setEnabled(true);
    ui->pushButton_cancelScan->setEnabled(true);
    ui->pushButton_finishScan->setEnabled(true);
    ui->pushButton_savePointCloud->setEnabled(false);
    ui->pushButton_Mesh->setEnabled(false);
    ui->pushButton_saveMesh->setEnabled(false);
    ui->lineEdit_deleteBeginIndex->setEnabled(true);
    ui->lineEdit_deleteEndIndex->setEnabled(true);
    ui->pushButton_deletePoint->setEnabled(true);
#else
    ui->pushButton_enterScan->setVisible(false);
    ui->pushButton_newProject->setVisible(false);
    ui->pushButton_exitScan->setVisible(false);
    ui->pushButton_Brightness->setVisible(false);
    ui->pushButton_scanPreview->setVisible(false);
    ui->pushButton_cancelScan->setVisible(false);
    ui->pushButton_finishScan->setVisible(false);
    ui->pushButton_savePointCloud->setVisible(false);
    ui->pushButton_Mesh->setVisible(false);
    ui->pushButton_saveMesh->setVisible(false);
    ui->lineEdit_deleteBeginIndex->setVisible(false);
    ui->lineEdit_deleteEndIndex->setVisible(false);
    ui->pushButton_deletePoint->setVisible(false);
    ui->checkBox_opt->setVisible(false);
    ui->checkBox_pseudoColor->setVisible(false);
    ui->line->setVisible(false);
    ui->line_2->setVisible(false);
    ui->label_4->setVisible(false);
    ui->label_5->setVisible(false);
#endif
    
}

void Sn3DMainWindow::deviceOnlineState()
{
	ui->tab_calibrate->setEnabled(true);
	ui->tab_scan->setEnabled(true);
	//calib
	ui->pushButton_enterCalib->setEnabled(true);
	ui->pushButton_exitCalib->setEnabled(false);
	ui->btn_ct_stereo->setEnabled(false);
	ui->pushButton_setCaliType->setEnabled(false);
	ui->pushButton_getCaliType->setEnabled(false);
	ui->pushButton_startSnap->setEnabled(false);
	ui->pushButton_stopSnap->setEnabled(false);
	//scan
	ui->pushButton_enterScan->setEnabled(true);
	ui->pushButton_newProject->setEnabled(false);
	ui->pushButton_exitScan->setEnabled(false);
	ui->pushButton_Brightness->setEnabled(false);
	ui->pushButton_scanPreview->setEnabled(false);
	ui->pushButton_cancelScan->setEnabled(false);
	ui->pushButton_finishScan->setEnabled(false);
	ui->pushButton_savePointCloud->setEnabled(false);
	ui->pushButton_Mesh->setEnabled(false);
	ui->pushButton_saveMesh->setEnabled(false);
	ui->lineEdit_deleteBeginIndex->setEnabled(false);
	ui->lineEdit_deleteEndIndex->setEnabled(false);
	ui->pushButton_deletePoint->setEnabled(false);

	//scanner
	ui->pushButton_deviceReconnect->setEnabled(false);
	ui->pushButton_checkDeviceOnline->setEnabled(true);

	//
	ui->pushButton_Process->setEnabled(true);
	ui->pushButton_End->setEnabled(false);
	ui->pushButton_enterScan->setEnabled(false);

}

void Sn3DMainWindow::deviceOfflineState()
{
	ui->pushButton_deviceReconnect->setEnabled(true);
	ui->pushButton_checkDeviceOnline->setEnabled(false);
}

void Sn3DMainWindow::newProjectState()
{

}

void Sn3DMainWindow::exitScan()
{

}

void Sn3DMainWindow::e7PlusEnterScan()
{
    if (m_pDevice){
        if (m_pDevice->enterScan() != sn3dsdkdemo::OK)
            ui->stackedWidget_buttons->setEnabled(true);
    }else
        ui->stackedWidget_buttons->setEnabled(true);
}

void Sn3DMainWindow::changeScanButtonText()
{
	QPushButton* btn = qobject_cast<QPushButton*>(sender());
	if (btn->text() == "Preview")
	{
		btn->setText("Scan");
		if (m_pDevice)
			m_pDevice->startSnap();
		ui->pushButton_exitScan->setEnabled(false);
		ui->pushButton_savePointCloud->setEnabled(false);
	}
	else if (btn->text() == "Scan")
	{
		// E7 device fixed scan don't need to change this
		if (m_pDevice && m_pDevice->isFixScan()) {
			btn->setText("Pause");
		}
		if (m_pDevice)
			m_pDevice->startSnap();
		ui->pushButton_exitScan->setEnabled(false);
	}
	else if (btn->text() == "Pause")
	{
		btn->setText("Preview");
		if (m_pDevice)
			m_pDevice->stopSnap();
		ui->pushButton_exitScan->setEnabled(true);
	}
}

bool Sn3DMainWindow::isE7Device()
{
	return m_deviceSubType == C_E7PlusDevice || m_deviceSubType == C_E7ProDevice;
}

void Sn3DMainWindow::initCalibType()
{
	if (m_pDevice)
	{
		m_ct_stereoMenu->clear();
		QMap<QString,int> calibTypeMap;
		m_pDevice->calibrateTypeMap(calibTypeMap);
		for (auto it = calibTypeMap.begin();it!= calibTypeMap.end();it++)
		{
			QAction* a = new QAction(it.key(), m_ct_stereoMenu);
			a->setData(it.value());
			m_ct_stereoMenu->addAction(a);
		}		
	}
}

void Sn3DMainWindow::initScanType()
{

}

void Sn3DMainWindow::setCalibTypeBtnStr(QString text)
{
	QFontMetrics fm(ui->btn_ct_stereo->font());
	int arrowWid = 40;
	int wid = fm.width(text) + arrowWid;
	ui->btn_ct_stereo->setFixedWidth(wid);
	ui->btn_ct_stereo->setText(text);
}

void Sn3DMainWindow::onResizeDialogBack(QSize size)
{
	if (m_dialogback)
	{
		m_dialogback->resize(size);
	}
}

void Sn3DMainWindow::onSetDialogBackTitle(QString title)
{
	if (m_dialogback)
	{
		m_dialogback->setTitleText(title);
	}
}

void Sn3DMainWindow::onEnableStackWidget()
{
	ui->stackedWidget_buttons->setEnabled(true);
}

void Sn3DMainWindow::switchScanUI()
{
 /*   if (m_pDevice){*/
        ui->scan_page->setEnabled(true);
       // m_pDevice->setScanStatus(m_iScanStatus);
        if (SS_PAUSED == m_iScanStatus){
            ui->pushButton_scanPreview->setText(tr(UI_SCAN_STR));
            ui->pushButton_scanPreview->setEnabled(true);
            ui->pushButton_cancelScan->setEnabled(true);
            ui->pushButton_finishScan->setEnabled(!m_pDevice->isFixScan());
            ui->pushButton_exitScan->setEnabled(!m_pDevice->isFixScan());
            ui->pushButton_Mesh->setEnabled(false);
            ui->pushButton_saveMesh->setEnabled(false);
           // m_pDevice->stopSnap();
        }
        else if (SS_PRE_SCAN == m_iScanStatus){
           // if (m_pDevice && m_pDevice->isFixScan())
             //   ui->pushButton_scanPreview->setText(tr(UI_SCAN_STR));
            //else
                ui->pushButton_scanPreview->setText(tr(UI_PREVIEW_STR));
            ui->pushButton_scanPreview->setEnabled(true);
            ui->pushButton_cancelScan->setEnabled(false);
           // m_pDevice->startSnap();
        }
        else if (SS_PRE_SCANNING == m_iScanStatus){
            ui->pushButton_scanPreview->setText(tr(UI_SCAN_STR));
            ui->pushButton_scanPreview->setEnabled(true);
            ui->pushButton_cancelScan->setEnabled(false);
            ui->pushButton_finishScan->setEnabled(false);
            ui->pushButton_exitScan->setEnabled(false);
            m_brightnessAction->setEnabled(true);
            ui->pushButton_Mesh->setEnabled(false);
            ui->pushButton_saveMesh->setEnabled(false);
           // if (!m_pDevice->isFixScan())
             //   m_pDevice->startSnap();
        }
        else if (SS_SCAN == m_iScanStatus){
//             if (m_pDevice->isFixScan()){
//                 if (m_pDevice->isTurnTable()){
//                     ui->pushButton_scanPreview->setText(tr(UI_PAUSE_STR));
//                     ui->pushButton_scanPreview->setEnabled(true);
//                     ui->pushButton_cancelScan->setEnabled(true);
//                     m_pDevice->startSnap();
//                 }
//                 else{
//                     ui->pushButton_scanPreview->setText(tr(UI_SCAN_STR));
//                     ui->pushButton_scanPreview->setEnabled(false);
//                     ui->pushButton_cancelScan->setEnabled(false);
//                 }
//             }
          /*  else*/{
                ui->pushButton_scanPreview->setText(tr(UI_PAUSE_STR));
                ui->pushButton_scanPreview->setEnabled(true);
                ui->pushButton_cancelScan->setEnabled(false);
               // m_pDevice->startSnap();
            }
            ui->pushButton_Mesh->setEnabled(false);
            ui->pushButton_saveMesh->setEnabled(false);
            ui->pushButton_finishScan->setEnabled(false);
        }
        else if (SS_SCAN_STOPED == m_iScanStatus){
			//if (m_pDevice && m_pDevice->isFixScan())
				ui->pushButton_scanPreview->setText(tr(UI_SCAN_STR));
// 			else if (isE7Device()&&!m_isE7FirstHandleScan)
// 			{
// 				ui->pushButton_scanPreview->setText(tr(UI_SCAN_STR));
// 			}
// 			else
			{
				ui->pushButton_scanPreview->setText(tr(UI_PREVIEW_STR));
				m_isE7FirstHandleScan = false;
			}
            ui->pushButton_scanPreview->setEnabled(true);
            ui->pushButton_cancelScan->setEnabled(false);
            ui->pushButton_Mesh->setEnabled(false);
            ui->pushButton_saveMesh->setEnabled(false);
            ui->pushButton_finishScan->setEnabled(false);
            ui->pushButton_exitScan->setEnabled(true);
            m_pDevice->stopSnap();
        }
//    }
    // ui->pushButton_exitScan->setEnabled(true);
}

int Sn3DMainWindow::parseScanStatus(const QString& scanStatus)
{
    int ret = SS_SCAN_STOPED;
    if ("SS_PRE_SCAN" == scanStatus)
        ret = SS_PRE_SCAN;
    else if ("SS_PRE_SCANNING" == scanStatus)
        ret = SS_PRE_SCANNING;
    else if ("SS_SCAN" == scanStatus)
        ret = SS_SCAN;
    else if ("SS_PAUSED" == scanStatus)
        ret = SS_PAUSED;
    return ret;
}

void Sn3DMainWindow::on_pushButton_Brightness_clicked()
{

}

void Sn3DMainWindow::setDelPointEnabled(bool isEnabled)
{
    ui->lineEdit_deleteBeginIndex->setEnabled(isEnabled);
    ui->lineEdit_deleteEndIndex->setEnabled(isEnabled);
    ui->pushButton_deletePoint->setEnabled(isEnabled);
}

void Sn3DMainWindow::getSn3DServiceRunStat(int code)
{
	if (code == EC_PROCESSERROR)
	{
		// 如果检测到服务端异常退出需要重新
		Sn3DRelease(); // 先释放

		// 在外部重新调用 Sn3DInitialize
	}
}

void watcherCallBack(int ret)
{

}
#include <fstream>
#include <ctime>
void getWholeCloudCallBack(LPSn3dPointCloud wholePointCloud)
{
	qDebug() << "-------------SAVE WHOLE CLOUD!";
	std::ofstream ofs;
	std::string callBackPath("./demo/Whole/");
	callBackPath.append(std::to_string(std::time(0)));
	callBackPath.append(".asc");
	ofs.open(callBackPath);


	for (int index = 0; index < wholePointCloud->norma_count; index++)
	{
		const auto& vertex = wholePointCloud->vertex_data;
		const auto& normal = wholePointCloud->norma_data;

		ofs << vertex[index].x << " " \
			<< vertex[index].y << " " \
			<< vertex[index].z << " " \
			<< normal[index].x << " " \
			<< normal[index].y << " " \
			<< normal[index].z << std::endl;
	}
	ofs.close();
	return;
}

QVector<LPSn3dPointCloud> CurrentPointCloudDatas;
void Sn3DMainWindow::getCurrentCloud(LPSn3dPointCloud currentPointCloud)
{
	LPSn3dPointCloud pointCloud = new Sn3DPointCloud;
	pointCloud->vertex_count = currentPointCloud->vertex_count;
	pointCloud->norma_count = currentPointCloud->norma_count;
	pointCloud->vertex_color_count = currentPointCloud->vertex_color_count;

	pointCloud->vertex_data = new Sn3dPointData[pointCloud->vertex_count];
	pointCloud->norma_data = new Sn3dPointData[pointCloud->norma_count];
	pointCloud->vertex_color_data = new Sn3dPointData[pointCloud->vertex_color_count];


	memcpy(pointCloud->vertex_data, currentPointCloud->vertex_data, sizeof(float) * 3 * pointCloud->vertex_count);
	memcpy(pointCloud->norma_data, currentPointCloud->norma_data, sizeof(float) * 3 * pointCloud->norma_count);
	memcpy(pointCloud->vertex_color_data, currentPointCloud->vertex_color_data, sizeof(float) * 3 * pointCloud->vertex_color_count);

	CurrentPointCloudDatas.push_back(pointCloud);
	return;
}

void SaveCurrentPointCloud()
{
	std::ofstream ofs;
	
	qDebug() << "************* print getCurrentCloudCallBack ***************";

	for (auto& pointCloud : CurrentPointCloudDatas)
	{
		std::string callBackPath("./demo/Current/");
		callBackPath.append(std::to_string(std::time(0)));
		callBackPath.append(".asc");
		ofs.open(callBackPath);

		auto& vertex = pointCloud->vertex_data;
		auto& norma = pointCloud->norma_data;
		auto& color = pointCloud->vertex_color_data;

		for (int index = 0; index < pointCloud->norma_count; index++)
		{
			ofs << vertex[index].x << " " \
				<< vertex[index].y << " " \
				<< vertex[index].z << " " \
				<< norma[index].x << " " \
				<< norma[index].y << " " \
				<< norma[index].z << std::endl;
		}
		delete[] vertex;
		delete[] norma;
		delete[] color;
		delete pointCloud;

		ofs.close();
	}
	QVector<LPSn3dPointCloud>().swap(CurrentPointCloudDatas);
}


QVector<LPSn3dIncreasePointCloud> IncreasePointCloudDatas;

void Sn3DMainWindow::getIncreaseCloud(LPSn3dIncreasePointCloud increasePointCloud)
{
	qDebug() << "************* print getIncreaseCloudCallBack size ***************" << IncreasePointCloudDatas.size();
	LPSn3dIncreasePointCloud pointCloud = new Sn3DIncreasePointCloud;
	pointCloud->vertex_count = increasePointCloud->vertex_count;
	pointCloud->norma_count = increasePointCloud->norma_count;
	pointCloud->vertex_color_count = increasePointCloud->vertex_color_count;
	pointCloud->index_count = increasePointCloud->index_count;

	pointCloud->vertex_data = new Sn3dPointData[pointCloud->vertex_count];
	pointCloud->norma_data = new Sn3dPointData[pointCloud->norma_count];
	pointCloud->vertex_color_data = new Sn3dPointData[pointCloud->vertex_color_count];
	pointCloud->index = new unsigned int[pointCloud->index_count];

	memcpy(pointCloud->vertex_data, increasePointCloud->vertex_data, sizeof(float) * 3 * pointCloud->vertex_count);
	memcpy(pointCloud->norma_data, increasePointCloud->norma_data, sizeof(float) * 3 * pointCloud->norma_count);
	memcpy(pointCloud->vertex_color_data, increasePointCloud->vertex_color_data, sizeof(float) * 3 * pointCloud->vertex_color_count);
	memcpy(pointCloud->index, increasePointCloud->index, sizeof(unsigned int) * pointCloud->index_count);

	IncreasePointCloudDatas.push_back(pointCloud);
}

void SaveIncreasePointCloud()
{
	std::ofstream ofs;

	qDebug() << "************* print getIncreaseCloudCallBack ***************";

	for (auto& pointCloud : IncreasePointCloudDatas)
	{
		std::string callBackPath("./demo/Increase/");
		callBackPath.append(std::to_string(std::time(0)));
		callBackPath.append(".asc");
		ofs.open(callBackPath);

		auto& vertex = pointCloud->vertex_data;
		auto& norma = pointCloud->norma_data;
		auto& color = pointCloud->vertex_color_data;
		auto& index = pointCloud->index;

		for (int i = 0; i < pointCloud->norma_count; i++)
		{
			ofs << vertex[i].x << " " \
				<< vertex[i].y << " " \
				<< vertex[i].z << " " \
				<< norma[i].x << " " \
				<< norma[i].y << " " \
				<< norma[i].z << " " \
				<< index[i] << std::endl;
		}
		delete[] vertex;
		delete[] norma;
		delete[] color;
		delete[] index;
		delete pointCloud;

		ofs.close();
	}
	QVector<LPSn3dPointCloud>().swap(CurrentPointCloudDatas);
}

void Sn3DMainWindow::getCameraPosition(LPSn3dCameraPosition cameraPosition)
{
	QVector3D tmp_position = cameraPosition->position;
	QVector3D tmp_center = cameraPosition->center;
	QVector3D tmp_up = cameraPosition->up;
	qDebug() << "************* camera position ***************";
	qDebug() << "position: " << tmp_position[0] << " " << tmp_position[1] << " " << tmp_position[2]\
		<< "center: " << tmp_center[0] << " " << tmp_center[1] << " " << tmp_center[2] \
		<< "up: " << tmp_up[0] << " " << tmp_up[1] << " " << tmp_up[2];
}

void Sn3DMainWindow::getTrackLostStatus(bool status)
{
	if (status)
	{
		slot_printInfo(QString("track lost!"), LogType::ERROR_MESSAGE);
	}
	qDebug() << "************* track lost status **************" << status;
}

void Sn3DMainWindow::getTooFlatStatus(bool status)
{
	if (status)
	{
		slot_printInfo(QString("please use marker to scan plane"), LogType::ERROR_MESSAGE);
	}
	qDebug() << "************* too flat status **************" << status;

}

void Sn3DMainWindow::getScanDist(double dist)
{
	qDebug() << "************* Scan Dist **************" << dist;
}

void Sn3DMainWindow::getMeshData(LPSn3dMeshData meshData)
{
	std::ofstream ofs;
	std::string callBackPath("./demo/MeshData/");
	//
	//QString str = "<font color=\"#FF0000\">" + QString::fromStdString(std::to_string(meshData->meshpoint_count) + " meshpoint_count") + "</font>";
	//ui->textBrowser->append(str);
	if (meshData->meshpoint_count)
	{
		std::string name("meshpoint.asc");
		ofs.open(callBackPath + name);
		LPSn3dPointData meshpointdata = new Sn3dPointData[meshData->meshpoint_count];
		memcpy(meshpointdata, meshData->meshpoint, sizeof(float) * 3 * meshData->meshpoint_count);
		for (int i = 0; i < meshData->meshpoint_count; ++i)
		{
			ofs << meshpointdata[i].x << " " << meshpointdata[i].y << " " << meshpointdata[i].z << std::endl;
		}
		delete[] meshpointdata;
		ofs.close();
	}

	if (meshData->meshnormal_count)
	{
		std::string name("meshnormal.asc");
		ofs.open(callBackPath + name);
		LPSn3dPointData meshnormal = new Sn3dPointData[meshData->meshnormal_count];
		memcpy(meshnormal, meshData->meshnormal, sizeof(float) * 3 * meshData->meshnormal_count);
		for (int i = 0; i < meshData->meshnormal_count; ++i)
		{
			ofs << meshnormal[i].x << " " << meshnormal[i].y << " " << meshnormal[i].z << std::endl;
		}
		delete[] meshnormal;
		ofs.close();
	}

	if (meshData->meshtrifaceid_count)
	{
		std::string name("meshtrifaceid.asc");
		ofs.open(callBackPath + name);
		LPSn3dFaceId meshtrifaceid = new Sn3DFaceId[meshData->meshtrifaceid_count];
		memcpy(meshtrifaceid, meshData->meshtrifaceid, sizeof(int) * 3 * meshData->meshtrifaceid_count);
		for (int i = 0; i < meshData->meshtrifaceid_count; ++i)
		{
			ofs << meshtrifaceid[i].x << " " << meshtrifaceid[i].y << " " << meshtrifaceid[i].z << std::endl;
		}
		delete[] meshtrifaceid;
		ofs.close();
	}

	if (meshData->meshtextureid_count)
	{
		std::string name("meshtextureid.asc");
		ofs.open(callBackPath + name);
		LPSn3dFaceId meshtextureid = new Sn3DFaceId[meshData->meshtextureid_count];
		memcpy(meshtextureid, meshData->meshtextureid, sizeof(int) * 3 * meshData->meshtextureid_count);
		for (int i = 0; i < meshData->meshtextureid_count; ++i)
		{
			ofs << meshtextureid[i].x << " " << meshtextureid[i].y << " " << meshtextureid[i].z << std::endl;
		}
		delete[] meshtextureid;
		ofs.close();
	}

	if (meshData->textureUV_count)
	{
		std::string name("textureUV.asc");
		ofs.open(callBackPath + name);
		LPSn3dVec2F textureUV = new Sn3DVec2F[meshData->textureUV_count];
		memcpy(textureUV, meshData->textureUV, sizeof(float) * 2 * meshData->textureUV_count);
		for (int i = 0; i < meshData->textureUV_count; ++i)
		{
			ofs << textureUV[i].x << " " << textureUV[i].y << std::endl;
		}
		delete[] textureUV;
		ofs.close();
	}

	Sn3DImage image = meshData->image;
	QImage img(image.data, image.width, image.height, image.width * image.channel, QImage::Format_RGB888);
	img.save("./demo/MeshData/image.jpg");
}

void Sn3DMainWindow::getDeviceEvent(DeviceEvent event)
{
	qDebug() << "Current Event: " << event;
	if (event == DeviceEvent::DE_DOUBLECLICK) DoubleClickModel ^= 1;
	if (DoubleClickModel){
		if (event == DeviceEvent::DE_PLUS)
		{
			BrightnessLevel = std::min(++BrightnessLevel, BrightnessMax);
			Sn3DSetBrightness(BrightnessLevel);
		}
		if (event == DeviceEvent::DE_SUB)
		{
			BrightnessLevel = std::max(--BrightnessLevel, BrightnessMin);
			Sn3DSetBrightness(BrightnessLevel);
		}
		int currentBrightness;
		Sn3DGetCurrentBrightness(currentBrightness);
		qDebug() << "Current Brightness Level: " << BrightnessLevel;
	}
	else {
		if (event == DeviceEvent::DE_PLUS)
		{
			Sn3DSetWorkRange(300, 500);
		}
		if (event == DeviceEvent::DE_SUB)
		{
			Sn3DSetWorkRange(200, 600);
		}
	}
}

void Sn3DMainWindow::on_pushButton_End_clicked()
{
	
	SN3DERROR t_ret = EC_SUCCESS;
	char* t_sln_path = "./demo/demo";
	char* t_prj_path = "./demo/Project1.ir_E10_prj";
	char* t_save_path = "./demo/data";
	ScanMode t_scan_mode = AT_OBJECT;
	MeshType t_mesh_type = AT_UNWATERTIGHT;
	bool t_hasTexture = false;
	bool t_isCreate = true;
	float t_point_dis = 1.f;
	bool t_globalOpt = true;
	char* t_globalMaker = "";

	//t_ret = Sn3DIntallGetImagasCallBack(nullptr, nullptr);
	DEBUG_TRACE(Sn3DPauseScan, t_ret, "");	// sd

	if (t_ret == EC_SUCCESS)
	{
		t_ret = Sn3DPauseScan();
		DEBUG_TRACE(Sn3DPauseScan, t_ret, "");	// sd

	}
	//end
	if (t_ret == EC_SUCCESS)
	{
		SaveCurrentPointCloud();
		SaveIncreasePointCloud();
		t_ret = Sn3DEndScan(
			t_globalOpt, 
			t_point_dis, 
			getWholeCloudCallBack);		
	}
	//saveasc
	if (t_ret == EC_SUCCESS)
	{
		t_ret = Sn3DSaveData(t_prj_path,t_save_path,"asc");
	}

	//mesh
	if (t_ret == EC_SUCCESS)
	{
		auto getMestDataCallback = [](LPSn3dMeshData MeshData, void* owner) {
			((Sn3DMainWindow*)owner)->getMeshData(MeshData);
		};
		//t_ret = Sn3DMeshEx(
		//	t_mesh_type,
		//	1,
		//	1,
		//	t_point_dis,
		//	true,
		//	10,
		//	true,
		//	.4,
		//	SN3D_MAX_FACELIMIT,
		//	this,
		//	getMestDataCallback);
		std::string brightness_min = "get mesh data result " + std::to_string(t_ret);
		std::string str = "<font color = \"#FF0000\">" + brightness_min + "</font>";
		ui->textBrowser->append(QString::fromStdString(str));
	}
	//saveply
	if (t_ret == EC_SUCCESS)
	{
		//t_ret = Sn3DSaveData(t_prj_path,t_save_path,"ply");
	}
	//exit
	if (t_ret == EC_SUCCESS)
	{
		//t_ret = Sn3DExitScan();
	}

	if (t_ret == EC_SUCCESS)
	{
		ui->pushButton_Process->setEnabled(true);
		ui->pushButton_End->setEnabled(false);
	}
}

void Sn3DMainWindow::getCurrentMarker(LPSn3DMakerData currentMarker)
{
	std::string printinfo = "<font color = \"#FF0000\">" + std::to_string(currentMarker->marker_count) +  "current marker size" + "<\font>";
	slot_printInfo(QString::fromStdString(printinfo), RECEIVE_MESSAGE);
	qDebug() << "--------current marker information\n";
	for (size_t i = 0; i < currentMarker->marker_count; i++)
	{
		qDebug() << "id " << currentMarker->markerList[i].codeId << "diameter " << currentMarker->markerList[i].diameter << " " << currentMarker->markerList[i].coord.x << " " 
			<< currentMarker->markerList[i].coord.y << " " << currentMarker->markerList[i].coord.z;
	}
}

void Sn3DMainWindow::getWholeMarker(LPSn3DMakerData wholeMarker)
{
	std::string printinfo = "whole maker size" + std::to_string(wholeMarker->marker_count);
	slot_printInfo(QString::fromStdString(printinfo), RECEIVE_MESSAGE);
	qDebug() << "--------whole marker information\n";
	for (size_t i = 0; i < wholeMarker->marker_count; i++)
	{
		qDebug() << "id " << wholeMarker->markerList[i].codeId << "diameter " << wholeMarker->markerList[i].diameter << " " << wholeMarker->markerList[i].coord.x << " "
			<< wholeMarker->markerList[i].coord.y << " " << wholeMarker->markerList[i].coord.z;
	}
}
void Sn3DMainWindow::on_pushButton_Process_clicked()
{	
	
	ui->pushButton_Process->setEnabled(false);
	ui->pushButton_End->setEnabled(true);

	SN3DERROR t_ret = EC_SUCCESS;
	//init
	char* t_sln_path = "./demo/demo";
	char* t_prj_path = "./demo/Project1.ir_E10_prj";
	char* t_save_path = "./demo/data";
	ScanMode t_scan_mode = AT_OBJECT;
	MeshType t_mesh_type = AT_UNWATERTIGHT;
	AlignType t_align_type = AT_HYBRID;
    bool t_hasTexture = true;	//LSC 修改，不带纹理
	bool t_isCreate = true;
	float t_point_dis = 1.f;
	bool t_globalOpt = true;
	char* t_globalMaker = "";
	char* logPath = "./demo/log/";
	char* exePath = "ScannerDemo.exe";
	// Sn3DInitialLog(exePath, logPath);
	//if(t_ret == EC_SUCCESS)
	//{
	//	t_ret = Sn3DInitialize(watcherCallBack);
	//}
	//enter
	/*if (t_ret == EC_SUCCESS)
	{
		t_ret = Sn3DEnterScan();
		DEBUG_TRACE(Sn3DEnterScan, t_ret, "");	// sd

		//LSC 获取设备相机亮度范围最大值与最小值，并设置相机亮度为其中间值
		Sn3DGetBrightnessRange(BrightnessMin, BrightnessMax);
		BrightnessLevel = BrightnessMax;
		Sn3DSetBrightness(BrightnessLevel);
 		qDebug() << "Sn3DGetBrightnessRange BrightnessMin" << BrightnessMin;
		qDebug() << "Sn3DGetBrightnessRange BrightnessMax" << BrightnessMax;
	}
*/
	//new
	if (t_ret == EC_SUCCESS)
	{
		t_ret = Sn3DOpenOrCreateSolution(
			t_sln_path,
			t_isCreate,
			t_scan_mode,
			t_hasTexture,
			t_point_dis);
		DEBUG_TRACE(Sn3DOpenOrCreateSolution, t_ret, "");	// sd

		t_ret = Sn3DNewProject(
			t_sln_path, 
			t_scan_mode, 
			t_hasTexture, 
			t_point_dis, 
			t_align_type,
			t_globalMaker);
		DEBUG_TRACE(Sn3DNewProject, t_ret, "");	// sd
	}

	//priview
	if (t_ret == EC_SUCCESS)
	{
		
		auto getImageCallback = [](int camId, const unsigned char* data, int  width, int height, int channel, void* owner) {
			((Sn3DMainWindow*)owner)->slot_updateVideo(camId, data, width, height, channel);

		};
		
		qDebug() << "===== start get video =====";
		
		Sn3DIntallGetImagasCallBack(getImageCallback, this); // 注册接受视频流的回调,注意这个函数是非必须的，需要用图片做分析或者其他用途的才调用,
		
		// 如果想终止直接Sn3DIntallGetImagasCallBack(nullptr, nullptr);
		t_ret = Sn3DPreviewScan();
		DEBUG_TRACE(Sn3DPreviewScan, t_ret, "");	// sd 预扫描

		t_ret = Sn3DStartScan();
		DEBUG_TRACE(Sn3DStartScan, t_ret, "");	// sd	开始扫描
	}
	//scan
	/*if (t_ret == EC_SUCCESS)
	{
		auto getCurrentPointCloudCallback = [](LPSn3dPointCloud currentPointCloud, void* owner) {
			((Sn3DMainWindow*)owner)->getCurrentCloud(currentPointCloud);
		};
		Sn3DSetCurrentPointCloudCallBack(getCurrentPointCloudCallback, this); // 注册接受当前帧的回调
																	// 如果想终止直接Sn3DSetCurrentPointCloudCallBack(nullptr, nullptr);

		auto getCurrentMarkerCallBack = [](LPSn3DMakerData currentMarker, void* owner) {
			((Sn3DMainWindow*)owner)->getCurrentMarker(currentMarker);
		};
		Sn3DSetCurrentMarkerCallBack(getCurrentMarkerCallBack, this); // 注册当前帧标志点信息的回调


		auto getWholeMarkerCallBack = [](LPSn3DMakerData wholeMarker, void* owner) {
			((Sn3DMainWindow*)owner)->getWholeMarker(wholeMarker);
		};
		Sn3DSetWholeMarkerCallBack(getWholeMarkerCallBack, this); // 注册全量标志点信息的回调

		auto getIncreasePointCloudCallback = [](LPSn3dIncreasePointCloud increasePointCloud, void* owner) {
			((Sn3DMainWindow*)owner)->getIncreaseCloud(increasePointCloud);
		};
		Sn3DSetIncreasePointCloudCallBack(getIncreasePointCloudCallback, this); // 注册接受增量点云回调
																	// 如果想终止直接Sn3DSetIncreasePointCloudCallBack(nullptr, nullptr);

		auto getCameraPositionCallback = [](LPSn3dCameraPosition cameraPosition, void* owner) {
			((Sn3DMainWindow*)owner)->getCameraPosition(cameraPosition);
		};
		Sn3DSetCameraPositionCallBack(getCameraPositionCallback, this); // 注册接受相机视角回调
																	// 如果想终止直接Sn3DSetCameraPositionCallBack(nullptr, nullptr);

		auto getTrackLostStatusCallback = [](bool status, void* owner) {
			((Sn3DMainWindow*)owner)->getTrackLostStatus(status);
		};
		Sn3DSetTrackLostStatusCallBack(getTrackLostStatusCallback, this); // 注册接受跟踪丢失回调
																	// 如果想终止直接Sn3DSetTrackLostStatusCallBack(nullptr, nullptr);
		
		auto getScanDIstCallback = [](double dist, void* owner) {
			((Sn3DMainWindow*)owner)->getScanDist(dist);
		};
		Sn3DSetScanDistCallBack(getScanDIstCallback, this); // 注册接受跟踪距离挡位回调
																	// 如果想终止直接Sn3DSetScanDistCallBack(nullptr, nullptr);


		auto getTooFlatStatusCallback = [](bool status, void* owner) {
			((Sn3DMainWindow*)owner)->getTooFlatStatus(status);
		};
		Sn3DSetTooFlatStatusCallBack(getTooFlatStatusCallback, this); // 注册接受平面提示回调
																	// 如果想终止直接Sn3DSetTooFlatStatusCallBack(nullptr, nullptr);


		auto getDeviceEventCallBack = [](DeviceEvent event, void* owner){
			((Sn3DMainWindow*)owner)->getDeviceEvent(event);
		};
		Sn3DSetDeviceEventCallBack(getDeviceEventCallBack, this);
		t_ret = Sn3DStartScan();
		DEBUG_TRACE(Sn3DStartScan, t_ret, "");	// sd
	}
	*/

	if (t_ret != EC_SUCCESS)
	{
		ui->pushButton_Process->setEnabled(true);
		ui->pushButton_End->setEnabled(false);
	}
}
