/*!
* @brief   
* @project qn_einstarscan
* @file    createprocessnode.cpp
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-24 11:02
*/

#include <QProcess>
#include <QFile>
#include <QThread>
#include <QDialog>
#include <QCoreApplication>
#include <QDir>

#include "../include/plugincorehelper.h"
#include "qn_errors.h"
#include "createprocessnode.h"
#include "mainwindow.h"


REGISTER(createprocess, CreateProcessNode);
REGISTER(Sn3DEnterScan, Sn3DEnterScanNode);
REGISTER(Sn3DNewSolutionByInputs, Sn3DNewSolutionByInputsNode);
REGISTER(Sn3DNewSolutionByProperties, Sn3DNewSolutionByPropertiesNode);
REGISTER(Sn3DOpenSolution, Sn3DOpenSolutionNode);
REGISTER(Sn3DNewProjectByInputs, Sn3DNewProjectByInputsNode)
REGISTER(Sn3DNewProjectByProperties, Sn3DNewProjectByPropertiesNode)
REGISTER(Sn3DOpenProject, Sn3DOpenProjectNode)
REGISTER(Sn3DDeviceConfig, Sn3DDeviceConfigNode)
REGISTER(Sn3DPreviewScan, Sn3DPreviewScanNode)
REGISTER(Sn3DScan, Sn3DScanNode)
REGISTER(Sn3DPauseScan, Sn3DPauseScanNode)
REGISTER(Sn3DEndScan, Sn3DEndScanNode)
REGISTER(Sn3DExitScan, Sn3DExitScanNode)
REGISTER(Sn3DClearScan, Sn3DClearScanNode)
REGISTER(Sn3DSaveDataByProperties, Sn3DSaveDataByPropertiesNode)
REGISTER(Sn3DSaveDataByInputs, Sn3DSaveDataByInputsNode)
REGISTER(Sn3DMesh, Sn3DMeshNode)
REGISTER(Sn3DHandleDeviceEvent, Sn3DHandleDeviceEventNode)


DialogManager *DialogManager::m_instance = new DialogManager(qApp);
DialogManager *DialogManager::instance() {
	//static DialogManager inst;
	if (m_instance)
		return m_instance;
}

DialogManager::DialogManager(QObject *parent) : QObject(parent), m_dialog(nullptr) {
}

DialogManager::~DialogManager() {
	if (m_dialog)
		m_dialog->deleteLater();
}

bool DialogManager::dialogIsClosed() {
	return m_dialogIsClosed;
}

void DialogManager::onOpenDialog(bool isVideo, bool isSaveWholeCloud, QString saveWholeCloudPath) {
	MainWindow w;
	w.setProperties(isVideo, isSaveWholeCloud, saveWholeCloudPath);
	w.show();

	QEventLoop loop;
	connect(&w, &MainWindow::dialogClosed, &loop, [&loop]() { loop.quit(); });
	loop.exec();
}

UiHelper &UiHelper::instance() {
	static UiHelper _helper;
	return _helper;
}

UiHelper::UiHelper(QObject *parent) : QObject(parent) {
	connect(this, &UiHelper::toOpenDialog, DialogManager::instance(), &DialogManager::onOpenDialog,
		Qt::BlockingQueuedConnection);
}

UiHelper::~UiHelper()
{
}

void UiHelper::openDialog(bool isVideo, bool isSaveWholeCloud, QString saveWholeCloudPath)
{
	emit this->toOpenDialog(isVideo, isSaveWholeCloud, saveWholeCloudPath);
}

// ===========================================  CreateProcess Node  ===========================================
CreateProcessNode::CreateProcessNode() {
    m_nodeName = "createprocess";
}

CreateProcessNode::~CreateProcessNode() {
}

QN_RESULT CreateProcessNode::apply() {
    QnRtnCode rc;
    rc = hasInput("appPath");
    if (rc.isSuccess()) {
        std::string data = input("appPath");
        std::string appPath = getValue<std::string>(data);

        if (!QFile::exists(QString(appPath.c_str()))) {
            return QN_NOT_FIND;
        }
        QProcess appProcess;
        appProcess.setProgram(QString(appPath.c_str()));
        appProcess.setWorkingDirectory("D:/Shining3d/EXStar");

        appProcess.start();

        appProcess.waitForStarted();
        appProcess.waitForFinished(-1);

        return QN_OK;
    }
    return QN_FAIL;

}

// ===========================================  Sn3DEnterScan Node  ===========================================
Sn3DEnterScanNode::Sn3DEnterScanNode()
{
	m_nodeName = "Sn3DEnterScan";

	m_Sn3DWatchFun = [](int code) {
		if (code == EC_PROCESSERROR) {
			Sn3DRelease(); // 先释放
		}
	};
	if (!isInit()) {
		QnRtnCode rc;
		rc = Sn3DInitialize(m_Sn3DWatchFun);	// 不可重入
		if (rc.isSuccess()) {
			m_initCount++;
		}
	}

	QnRtnCode rc = Sn3DConnectDevice();	// 可重入

}

Sn3DEnterScanNode::~Sn3DEnterScanNode()
{
	QnRtnCode rc = Sn3DRelease();
}

QN_RESULT Sn3DEnterScanNode::apply()
{
	QnRtnCode rc;
	
	//QThread::sleep(10);

	//m_Sn3DWatchFun = [](int code) {
	//	if (code == EC_PROCESSERROR) {
	//		Sn3DRelease(); // 先释放
	//	}
	//};
	//if (!isInit()) {
	//	rc = Sn3DInitialize(m_Sn3DWatchFun);	// 不可重入
	//	if (rc.isSuccess()) {
	//		m_initCount++;
	//	}
	//}
	//else
	//	return rc;

	// rc = Sn3DConnectDevice();	// 可重入

	rc = Sn3DEnterScan();	// 不可重入，会返回EC_PROCESSERROR(0x10000000   // 流程调用错误)

	return rc;
}

// ===========================================  Sn3DNewSolutionByInputs Node  ===========================================
Sn3DNewSolutionByInputsNode::Sn3DNewSolutionByInputsNode()
{
	m_nodeName = "Sn3DNewSolutionByInputs";
}

Sn3DNewSolutionByInputsNode::~Sn3DNewSolutionByInputsNode()
{
}

QN_RESULT Sn3DNewSolutionByInputsNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> inputs{ "Solution Path" , "Scan Mode", "Texture Scan", "Point Distance" };
	if ((rc = hasInputs(inputs)).isFailed()){
		return QN_FAIL;
	};

	std::string data = input("Solution Path");
	std::string slnPath = getValue<std::string>(data);
	//QFileInfo fileinfo(slnPath.c_str());
	//if (!fileinfo.isDir())
	//	return QN_FAIL;

	QDir dir(slnPath.c_str());
	if (!dir.exists()) {
		if (!dir.mkpath(slnPath.c_str()))
			return QN_FAIL;
	}

	QString dirName = dir.dirName();
	slnPath += ("/" + dirName.toStdString());
	
	bool isCreate = true;

	data = input("Scan Mode");
	int scanMode = getValue<int>(data);

	data = input("Texture Scan");
	bool hasTexture = getValue<bool>(data);

	data = input("Point Distance");
	float pointDis = getValue<float>(data);

	rc = Sn3DOpenOrCreateSolution(slnPath.c_str(), isCreate, scanMode, hasTexture, pointDis);

	if (rc.isSuccess())
	{
		Json::Value output;
		output["name"] = "Solution Path";
		output["isSharedMemory"] = false;
		output["value"] = slnPath;

		setOutput("Solution Path", output.toStyledString());
	}

	return rc;
}

// ===========================================  Sn3DNewSolutionByProperties Node  ===========================================
Sn3DNewSolutionByPropertiesNode::Sn3DNewSolutionByPropertiesNode()
{
	m_nodeName = "Sn3DNewSolutionByProperties";
}

Sn3DNewSolutionByPropertiesNode::~Sn3DNewSolutionByPropertiesNode()
{
}

QN_RESULT Sn3DNewSolutionByPropertiesNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Solution Path" , "Scan Mode", "Texture Scan", "Point Distance" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Solution Path");
	std::string slnPath = getValue<std::string>(data);
	//QFileInfo fileinfo(slnPath.c_str());
	//if (!fileinfo.isDir())
	//	return QN_FAIL;
	QDir dir(slnPath.c_str());
	if (!dir.exists()) {
		if (!dir.mkpath(slnPath.c_str()))
			return QN_FAIL;
	}

	bool isCreate = true;

	data = property("Scan Mode");
	int nScanMode = 0;
	std::string scanMode = getValue<std::string>(data);
	if (scanMode.compare("Portrait") == 0)
		nScanMode = 0;
	else if (scanMode.compare("Object") == 0)
		nScanMode = 1;
	else if (scanMode.compare("Smallobject") == 0)
		nScanMode = 2;
	else {}

	data = property("Texture Scan");
	bool hasTexture = getValue<bool>(data);

	data = property("Point Distance");
	float pointDis = getValue<float>(data);

	rc = Sn3DOpenOrCreateSolution(slnPath.c_str(), isCreate, nScanMode, hasTexture, pointDis);

	return rc;
}


// ===========================================  Sn3DOpenSolution Node  ===========================================
Sn3DOpenSolutionNode::Sn3DOpenSolutionNode()
{
	m_nodeName = "Sn3DOpenSolutionNode";
}

Sn3DOpenSolutionNode::~Sn3DOpenSolutionNode()
{
}

QN_RESULT Sn3DOpenSolutionNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Solution Path" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Solution Path");
	std::string slnPath = getValue<std::string>(data);

	bool isCreate = false;

	int scanMode = 1;

	bool hasTexture = true;

	float pointDis = 1.f;

	rc = Sn3DOpenOrCreateSolution(
		slnPath.c_str(),
		isCreate,
		scanMode,
		hasTexture,
		pointDis);

	return rc;
}

// ===========================================  Sn3DNewProjectByInputs Node  ===========================================
Sn3DNewProjectByInputsNode::Sn3DNewProjectByInputsNode()
{
	m_nodeName = "Sn3DNewProjectByInputs";
}

Sn3DNewProjectByInputsNode::~Sn3DNewProjectByInputsNode()
{
}

QN_RESULT Sn3DNewProjectByInputsNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> inputs{ "Solution Path" , "Scan Mode", "Texture Scan", "Point Distance"
		, "Alignment Mode", "Global Markers" };
	if ((rc = hasInputs(inputs)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = input("Solution Path");
	std::string slnPath = getValue<std::string>(data);

	data = input("Scan Mode");
	int scanMode = getValue<int>(data);

	data = input("Texture Scan");
	bool hasTexture = getValue<bool>(data);

	data = input("Point Distance");
	float pointDis = getValue<float>(data);

	data = input("Alignment Mode");
	int alignType = getValue<int>(data);

	data = input("Global Markers");
	std::string globalMarkerPath = getValue<std::string>(data);

	rc = Sn3DNewProject(
		slnPath.c_str(),
		scanMode,
		hasTexture,
		pointDis,
		alignType,
		globalMarkerPath.c_str());

	if (rc.isSuccess())
	{
		size_t pos = slnPath.rfind("/");
		std::string prjPath = slnPath.substr(0, slnPath.rfind("/") + 1);
		prjPath += "Project1.ir_E10_prj";
		Json::Value output;
		output["name"] = "Project Path";
		output["isSharedMemory"] = false;
		output["value"] = prjPath;

		setOutput("Project Path", output.toStyledString());
	}

	return rc;
}

// ===========================================  Sn3DNewProjectByProperties Node  ===========================================
Sn3DNewProjectByPropertiesNode::Sn3DNewProjectByPropertiesNode()
{
	m_nodeName = "Sn3DNewProjectByProperties";
}

Sn3DNewProjectByPropertiesNode::~Sn3DNewProjectByPropertiesNode()
{
}

QN_RESULT Sn3DNewProjectByPropertiesNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Solution Path" , "Scan Mode", "Texture Scan", "Point Distance"
		, "Alignment Mode", "Global Markers" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Solution Path");
	std::string slnPath = getValue<std::string>(data);

	data = property("Scan Mode");
	int nScanMode = 0;
	std::string scanMode = getValue<std::string>(data);
	if (scanMode.compare("Portrait") == 0)
		nScanMode = 0;
	else if (scanMode.compare("Object") == 0)
		nScanMode = 1;
	else if (scanMode.compare("Smallobject") == 0)
		nScanMode = 2;
	else {}

	data = property("Texture Scan");
	bool hasTexture = getValue<bool>(data);

	data = property("Point Distance");
	float pointDis = getValue<float>(data);

	data = property("Alignment Mode");
	int nAlignType = 0;
	std::string alignType = getValue<std::string>(data);
	if (alignType.compare("Features") == 0)
		nAlignType = 0;
	else if (alignType.compare("Texture") == 0)
		nAlignType = 8;
	else if (alignType.compare("Hybrid") == 0)
		nAlignType = 2;
	else {}

	data = property("Global Markers");
	std::string globalMarkerPath = getValue<std::string>(data);

	rc = Sn3DNewProject(
		slnPath.c_str(),
		nScanMode,
		hasTexture,
		pointDis,
		nAlignType,
		globalMarkerPath.c_str());

	Json::Value output;
	output["name"] = "result";
	output["isSharedMemory"] = false;
	output["value"] = true;

	setOutput("result", output.toStyledString());

	return rc;
}

// ===========================================  Sn3DOpenProject Node  ===========================================
Sn3DOpenProjectNode::Sn3DOpenProjectNode()
{
	m_nodeName = "Sn3DOpenProject";
}

Sn3DOpenProjectNode::~Sn3DOpenProjectNode()
{
}

QN_RESULT Sn3DOpenProjectNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Project Name" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Project Name");
	std::string projectName = getValue<std::string>(data);

	rc = Sn3DOpenProject(projectName.c_str());

	Json::Value output;
	output["name"] = "result";
	output["isSharedMemory"] = false;
	output["value"] = true;

	setOutput("result", output.toStyledString());

	return rc;
}

// ===========================================  Sn3DDeviceConfig Node  ===========================================
Sn3DDeviceConfigNode::Sn3DDeviceConfigNode()
{
	m_nodeName = "Sn3DDeviceConfig";
}

Sn3DDeviceConfigNode::~Sn3DDeviceConfigNode()
{
}

QN_RESULT Sn3DDeviceConfigNode::apply()
{
	QnRtnCode rc;

	int BrightnessLevel = 8;
	int BrightnessMax;
	int BrightnessMin;
	Sn3DGetBrightnessRange(BrightnessMin, BrightnessMax);
	BrightnessLevel = BrightnessMax;

	rc = Sn3DSetBrightness(BrightnessLevel / 2);

	Sn3DSetWorkRange(200, 600);

	// Sn3DSetEnablePseudoColor();

	return rc;
}

// ===========================================  Sn3DPreviewScan Node  ===========================================
Sn3DPreviewScanNode::Sn3DPreviewScanNode()
{
	m_nodeName = "Sn3DPreviewScan";
}

Sn3DPreviewScanNode::~Sn3DPreviewScanNode()
{
}

QN_RESULT Sn3DPreviewScanNode::apply()
{
	QnRtnCode rc;

	rc = Sn3DPreviewScan();

	return rc;
}

// ===========================================  Sn3DExitScan Node  ===========================================
Sn3DExitScanNode::Sn3DExitScanNode()
{
	m_nodeName = "Sn3DRelease";
}

Sn3DExitScanNode::~Sn3DExitScanNode()
{
}

QN_RESULT Sn3DExitScanNode::apply()
{
	QnRtnCode rc;

	rc = Sn3DExitScan();
	
	return rc;
}

// ===========================================  Sn3DScan Node  ===========================================
Sn3DScanNode::Sn3DScanNode()
{
	m_nodeName = "Sn3DScan";
	
}

Sn3DScanNode::~Sn3DScanNode()
{
}

QN_RESULT Sn3DScanNode::apply()
{
	QnRtnCode rc;

	//QThread::sleep(10);

	std::vector<std::string> properties{ "video" , "saveWholeCloud" , "saveWholeCloudPath" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("video");
	bool isVideo = getValue<bool>(data);

	data = property("saveWholeCloud");
	bool isSaveWholeCloud = getValue<bool>(data);

	data = property("saveWholeCloudPath");
	std::string saveWholeCloudPath = getValue<std::string>(data);

	UiHelper::instance().openDialog(isVideo, isSaveWholeCloud, QString(saveWholeCloudPath.c_str()));

	return rc;
}

// ===========================================  Sn3DPauseScan Node  ===========================================
Sn3DPauseScanNode::Sn3DPauseScanNode()
{
	m_nodeName = "Sn3DPauseScan";
}

Sn3DPauseScanNode::~Sn3DPauseScanNode()
{
}

QN_RESULT Sn3DPauseScanNode::apply()
{
	QnRtnCode rc;

	rc = Sn3DPauseScan();

	return rc;
}

// ===========================================  Sn3DClearScan Node  ===========================================
Sn3DClearScanNode::Sn3DClearScanNode()
{
	m_nodeName = "Sn3DClearScan";
}

Sn3DClearScanNode::~Sn3DClearScanNode()
{
}

QN_RESULT Sn3DClearScanNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Cancle Current Project FramerMark" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Cancle Current Project FramerMark");
	bool cancleCurrentProjectFramerMark = getValue<bool>(data);

	rc = Sn3DClearScan(cancleCurrentProjectFramerMark);

	return rc;
}

// ===========================================  Sn3DEndScan Node  ===========================================
Sn3DEndScanNode::Sn3DEndScanNode()
{
	m_nodeName = "Sn3DEndScan";
}

Sn3DEndScanNode::~Sn3DEndScanNode()
{
}

QN_RESULT Sn3DEndScanNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Optimize" , "Point Distance", "Save Whole Point Cloud", "Save Path"};
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Cancle Current Project FramerMark");
	bool isOptimize = getValue<bool>(data);

	data = property("Point Distance");
	float pointDis = /*1.f*/getValue<float>(data);
	data = property("Save Whole Point Cloud");
	bool isSave = getValue<bool>(data);

	data = property("Save Path");
	std::string savePath = getValue<std::string>(data);

	// 1. 目前没有能支持回调的节点
	// 2. 对于需要保存数据的场景，在Sn3DEndScan后，可以通过信号等方式等待回调执行完成
	rc = Sn3DEndScan(isOptimize, pointDis, nullptr);

	return rc;
}

// ===========================================  Sn3DSaveDataByProperties Node  ===========================================
Sn3DSaveDataByPropertiesNode::Sn3DSaveDataByPropertiesNode()
{
	m_nodeName = "Sn3DSaveData";
}

Sn3DSaveDataByPropertiesNode::~Sn3DSaveDataByPropertiesNode()
{
}

QN_RESULT Sn3DSaveDataByPropertiesNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Project Path", "Save Path", "Save Type" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Project Path");
	std::string absolutePrjName = getValue<std::string>(data);

	data = property("Save Path");
	std::string savePath = getValue<std::string>(data);

	data = property("Save Type");
	std::string temp = getValue<std::string>(data);
	std::vector<std::string> typeVec;
	Json::Reader reader;
	Json::Value root;

	if (reader.parse(temp, root, true))
		if (!root.empty())
			if (root.isObject())
			{
				if (root.isMember(".asc"))
					if (root[".asc"].isBool())
						if(root[".asc"].asBool())
							typeVec.push_back("asc");
				if (root.isMember(".ply"))
					if (root[".ply"].isBool())
						if (root[".ply"].asBool())
							typeVec.push_back("ply");
				if (root.isMember(".stl"))
					if (root[".stl"].isBool())
						if (root[".stl"].asBool())
							typeVec.push_back("stl");
				if (root.isMember(".obj"))
					if (root[".obj"].isBool())
						if (root[".obj"].asBool())
							typeVec.push_back("obj");
			}
				

	for (auto item : typeVec)
	{

		std::string type = item;
		rc = Sn3DSaveData(
			const_cast<char*>(absolutePrjName.c_str()),
			const_cast<char*>(savePath.c_str()),
			const_cast<char*>(type.c_str()));

		if (rc.isFailed())
			return rc;
	}

	return rc;
}

// ===========================================  Sn3DSaveDataByInputs Node  ===========================================
Sn3DSaveDataByInputsNode::Sn3DSaveDataByInputsNode()
{
	m_nodeName = "Sn3DSaveData";
}

Sn3DSaveDataByInputsNode::~Sn3DSaveDataByInputsNode()
{
}

QN_RESULT Sn3DSaveDataByInputsNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> inputs{ "Project Path", "Save Path" };
	if ((rc = hasInputs(inputs)).isFailed()) {
		return QN_FAIL;
	};

	if ((rc = hasProperty("Save Type")).isFailed()) {
		return QN_FAIL;
	};

	std::string data = input("Project Path");
	std::string absolutePrjName = getValue<std::string>(data);

	data = input("Save Path");
	std::string savePath = getValue<std::string>(data);

	data = property("Save Type");
	std::string temp = getValue<std::string>(data);
	std::vector<std::string> typeVec;
	Json::Reader reader;
	Json::Value root;

	if (reader.parse(temp, root, true))
		if (!root.empty())
			if (root.isObject())
			{
				if (root.isMember(".asc"))
					if (root[".asc"].isBool())
						if (root[".asc"].asBool())
							typeVec.push_back("asc");
				if (root.isMember(".ply"))
					if (root[".ply"].isBool())
						if (root[".ply"].asBool())
							typeVec.push_back("ply");
				if (root.isMember(".stl"))
					if (root[".stl"].isBool())
						if (root[".stl"].asBool())
							typeVec.push_back("stl");
				if (root.isMember(".obj"))
					if (root[".obj"].isBool())
						if (root[".obj"].asBool())
							typeVec.push_back("obj");
			}


	for (auto item : typeVec)
	{

		std::string type = item;
		rc = Sn3DSaveData(
			const_cast<char*>(absolutePrjName.c_str()),
			const_cast<char*>(savePath.c_str()),
			const_cast<char*>(type.c_str()));

		if (rc.isFailed())
			return rc;
	}

	return rc;
}

// ===========================================  Sn3DMesh Node  ===========================================
Sn3DMeshNode::Sn3DMeshNode()
{
	m_nodeName = "Sn3DMesh";
}

Sn3DMeshNode::~Sn3DMeshNode()
{
}

QN_RESULT Sn3DMeshNode::apply()
{
	QnRtnCode rc;

	std::vector<std::string> properties{ "Mesh" , "Filter", "Smooth", "Fill Small Hole", "Small Hole Perimeter(mm)", 
										"Marker Hole Filling", "Max Triangles" };
	if ((rc = hasProperties(properties)).isFailed()) {
		return QN_FAIL;
	};

	std::string data = property("Mesh");
	std::string meshType = getValue<std::string>(data);
	int nMeshType = 0;
	if (meshType.compare("Unwatertight") == 0)
		nMeshType = 0;
	else if (meshType.compare("Semi-watertight") == 0)
		nMeshType = 1;
	else if (meshType.compare("Watertight") == 0)
		nMeshType = 2;
	else{}

	data = property("Filter");
	std::string filter = getValue<std::string>(data);
	int nFilter = 0;
	if (filter.compare("None") == 0)
		nFilter = 0;
	else if (filter.compare("Low") == 0)
		nFilter = 1;
	else if (filter.compare("Med") == 0)
		nFilter = 2;
	else if (filter.compare("High") == 0)
		nFilter = 3;
	else {}

	data = property("Smooth");
	std::string smooth = getValue<std::string>(data);
	int nSmooth = 0;
	if (smooth.compare("Low") == 0)
		nSmooth = 1;
	else if (smooth.compare("Med") == 0)
		nSmooth = 2;
	else if (smooth.compare("High") == 0)
		nSmooth = 3;
	else {}

	float t_point_dis = 1.f;

	data = property("Fill Small Hole");
	bool fillSmallHole = getValue<bool>(data);

	data = property("Small Hole Perimeter(mm)");
	float smellHolePerimeter = getValue<float>(data);
		
	data = property("Marker Hole Filling");
	bool fillMarkerHole = getValue<bool>(data);

	double spikeSensitivity = .4;

	data = property("Max Triangles");
	int faceLimit = getValue<int>(data);

	rc = Sn3DMeshEx(
		nMeshType,
		nFilter,
		nSmooth,
		t_point_dis,
		fillSmallHole,
		smellHolePerimeter,
		fillMarkerHole,
		spikeSensitivity,
		faceLimit,
		this,
		nullptr);

	return rc;
}

// ===========================================  Sn3DHandleDeviceEvent Node  ===========================================
Sn3DHandleDeviceEventNode::Sn3DHandleDeviceEventNode()
{
	m_nodeName = "Sn3DHandleDeviceEvent";
}

Sn3DHandleDeviceEventNode::~Sn3DHandleDeviceEventNode()
{
}

QN_RESULT Sn3DHandleDeviceEventNode::apply()
{
	QnRtnCode rc;

	Sn3DGetBrightnessRange(BrightnessMin, BrightnessMax);
	BrightnessLevel = BrightnessMax;
	Sn3DSetBrightness(BrightnessLevel);

	auto getDeviceEventCallBack = [](DeviceEvent event, void* owner) {
		((Sn3DHandleDeviceEventNode*)owner)->getDeviceEvent(event);
	};
	rc = Sn3DSetDeviceEventCallBack(getDeviceEventCallBack, this);

	return rc;
}

void Sn3DHandleDeviceEventNode::getDeviceEvent(DeviceEvent event)
{
	if (event == DeviceEvent::DE_DOUBLECLICK) DoubleClickModel ^= 1;
	if (DoubleClickModel) {
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
