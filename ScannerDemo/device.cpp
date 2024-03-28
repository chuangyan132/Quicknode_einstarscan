#include "qregexp.h"
#include "device.h"
#include "qdebug.h"
//#include "scanServiceimp.h"

using namespace sn3dsdkdemo;

void sn3dsdkdemo::strJson2Map(QString json, QVariantMap&result)
{
    QJsonParseError error;
    QJsonDocument jd = QJsonDocument::fromJson(json.toLocal8Bit(), &error);
    QString errorStr = error.errorString();
    QJsonObject jo = jd.object();
    result = jo.toVariantMap();
}

Device::Device(QObject* parent, ScanServiceImp* pService)
    : QObject(parent)
    , m_pService(pService)
{
}


Device::~Device()
{
}

int Device::enterCalibrate()
{
    if (m_pService)
    {
        QString result;
       // m_pService->enterProCali(result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
    }
    return OK;
}

int Device::setCalibrateType(int caliType)
{
    if (m_pService)
    {
        QString result;
      //  m_pService->setCalibType(caliType, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result << "caliType:" << caliType;
    }
    return OK;
}

int Device::setCalibrateType(const QString& caliType)
{
    int ret = PARAM_FORMAT_ERROR;
    int iCaliType = 0;
    if (OK == caliTypeToInt(caliType, &iCaliType)){
        ret = setCalibrateType(iCaliType);
    }
    return ret;
}

// @note pCaliType is resvered for future use
int Device::calibrateType(int* pCaliType)
{
    int ret = PARAM_FORMAT_ERROR;
    QString caliTypeStr;
    if (pCaliType && OK == calibrateType(caliTypeStr))
        ret = caliTypeToInt(caliTypeStr, pCaliType);
    return ret;
}

int Device::calibrateType(QString& caliType)
{
    int ret = PARAM_FORMAT_ERROR;
   // if (m_pService)
    {
        QString result;
      //  m_pService->getCalibType(result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;

        QVariantMap map;
        strJson2Map(result, map);
        caliType = map["data"].toString();
        ret = OK;
        // ui.comboBox_caliType->setCurrentIndex(ui.comboBox_caliType->findText(curCaliType));
    }
    return ret;
}

int Device::startSnap()
{
    //if (m_pService)
    {
        QString result;
    //    m_pService->snapEnabled(true, result);
        qDebug() << __FUNCTION__ << __LINE__ << "snapEnabled result:" << result;
    }
    return OK;
}

int Device::stopSnap()
{
  //  if (m_pService)
    {
        QString result;
    //    m_pService->snapEnabled(false, result);
        qDebug() << __FUNCTION__ << __LINE__ << "snapEnabled result:" << result;
    }
    return OK;
}

int Device::exitCalibrate()
{
  //  if (m_pService)
    {
        QString result;
    //    m_pService->exitProCali(result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
    }
    return OK;
}

int Device::enterScan()
{
    return NOT_IMPLEMENTED;
}

int Device::startScan()
{
    return startScan(false, AT_MARKERS, 0, 0);;
}

int Device::preview()
{
    return NOT_IMPLEMENTED;
}

int Device::pauseScan()
{
    int ret = NOT_IMPLEMENTED;
    return ret;
    /*if (m_pService){
        QString result;
        m_pService->pauseScan(result);
        ret = OK;
    }
    return ret;*/
}

int Device::cancelScan()
{
    QJsonObject params;
    QJsonArray projJa;
    params.insert("dataNames", projJa);
	params.insert("isCancelCurrentProjectFramerMark", true);
    return cancelScan(params);
}

int Device::endScan()
{
    return NOT_IMPLEMENTED;
}

int Device::saveCloudPoint(const QString& projectPath)
{
    return NOT_IMPLEMENTED;
}

int Device::mesh()
{
    return NOT_IMPLEMENTED;
}

int Device::saveMesh(const QString& projectPath)
{
    return NOT_IMPLEMENTED;
}

int Device::exitScan()
{
   // if (m_pService){
        QString result;
   //     m_pService->exitScan(result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
  //  }
    return OK;
}

int Device::openProjectGroup()
{
    return NOT_IMPLEMENTED;
}

int Device::openProjectGroup(const QString& groupPath, double pointDist)
{
    return modifyProjectGroup(groupPath, false, pointDist);
}

int Device::newProjectGroup()
{
    return NOT_IMPLEMENTED;
}

int Device::newProjectGroup(const QString& groupPath, double pointDist)
{
    return modifyProjectGroup(groupPath, true, pointDist);
}

int Device::newProject(const QJsonObject& params)
{
    QString result;
 //   m_pService->newProject(params, result);
    qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
    return OK;
}

int Device::openProject()
{
    return NOT_IMPLEMENTED;
}

int Device::newProject()
{
    return NOT_IMPLEMENTED;
}

int Device::deletePoint(int start, int end)
{
    return NOT_IMPLEMENTED;
}

int Device::changePoint(int index, double x, double y, double z)
{
    return NOT_IMPLEMENTED;
}

int Device::calibrateTypeMap(QMap<QString, int>& map)
{
    return NOT_IMPLEMENTED;
}

void Device::setSubType(const QString& subType)
{
    m_subType = subType;
}

QString Device::subType()
{
    return m_subType;
}

int Device::caliTypeToInt(const QString caliTypeStr, int* caliType)
{
    QMap<QString, int> map;
    if (OK == calibrateTypeMap(map)){
        if (map.contains(caliTypeStr) && caliType){
            *caliType = map[caliTypeStr];
            return OK;
        }
    }
    return NOT_IMPLEMENTED;
}

int Device::caliTypeToStr(int caliType, QString& caliTypeStr)
{
    QMap<QString, int> map;
    if (OK == calibrateTypeMap(map)){
        for (QMap<QString, int>::iterator it = map.begin(); it != map.end(); it++){
            if (it.value() == caliType){
                caliTypeStr = it.key();
                return OK;
            }
        }
    }
    return NOT_IMPLEMENTED;
}

int Device::setScanType(int iScanType)
{
 //   if (m_pService){
        QString result;
  //      m_pService->setScanType(iScanType, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
 //   }
    return OK;
}

int Device::scanType()
{
    return NOT_IMPLEMENTED;
}

int Device::enterScan(int scanType)
{
  //  if (m_pService){
        QString result;
 //       m_pService->enterScan(scanType, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
 //   }
    return OK;
}

int Device::modifyProjectGroup(const QString& groupPath, bool isCreate, double pointDist)
{
    QString result;
    QString paramsStr;
    QJsonObject jo;
    m_isCreateGroup = isCreate;
    jo.insert("solutionPath", groupPath);
    jo.insert("isCreate", isCreate);
    // point distance 
    jo.insert("solutionPointDist", pointDist);
    paramsStr = QJsonDocument(jo).toJson(QJsonDocument::Compact);
  //  m_pService->openOrCreateSolution(paramsStr, result);
    return OK;
}

QString Device::groupPath()
{
    return m_groupPath;
}

void Device::setGroupPath(const QString& groupPath)
{
    m_groupPath = groupPath;
}

int Device::openProject(const QString& projFileName, int type)
{
 //   if (m_pService){
        QString result;
 //       m_pService->openProject(projFileName, type, result);
        return OK;
 //   }
    return NOT_IMPLEMENTED;
}

bool Device::isFixScan()
{
    return false;
}

int Device::saveCloudPoint(const QJsonObject& params)
{
 //   if (m_pService){
        QString result;
 //       m_pService->save(params, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
 //   }
    return OK;
}

int Device::cancelScan(const QJsonObject& params)
{
//    if (m_pService){
        QString result;
 //       m_pService->cancelScan(params, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
 //   }
    return OK;
}

int Device::startScan(bool enableHDR, int alignType, int scanSubType, int turntableTimes)
{
    m_isHDR = enableHDR;
 //   if (m_pService){
        QString result;
        QJsonObject params;
        params.insert("_enableHDR", enableHDR);
        params.insert("_alignType", alignType);
        params.insert("_scanSubType", scanSubType);       // According to app, use 1 or 0
        params.insert("_turntableTimes", turntableTimes);
//        m_pService->startScan(params, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
 //   }
    return OK;
}

bool Device::isHDR()
{
    return m_isHDR;
}

void Device::setIsHDR(bool isHDR)
{
    m_isHDR = isHDR;
}

int Device::endScan(bool globalOptimize, double pointDist, bool reBuildData)
{
//    if (m_pService){
        QString result;
 //       m_pService->endScan(globalOptimize, pointDist, reBuildData, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
 //   }
    return OK;
}

double Device::pointDist()
{
    return m_pointDist;
}

void Device::setPointDist(double pointDist)
{
    m_pointDist = pointDist;
}

int Device::mesh(const QJsonObject& params)
{
  //  if (m_pService){
        QString result;
 //       m_pService->meshMultiProject(params, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
   // }
    return OK;
}

int Device::saveMesh(const QJsonObject& params)
{
 //   if (m_pService){
        QString result;
//         m_pService->save(params, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
 //   }
    return OK;
}

//int Device::serialEditSharedMemoryData(LPSMC_EDIT_DATA edit_data, int begin, int end)
//{
//    int ret = NOT_IMPLEMENTED;
//  //  if (m_pService)
// //       ret = m_pService->serialEditSharedMemoryData(edit_data, begin, end);
//    return ret;
//}

int Device::applyEdit(const QJsonObject& params)
{
    int ret = NOT_IMPLEMENTED;
//    if (m_pService){
        QString result;
//        m_pService->applyEdit(params, result);
        ret = OK;
//    }
    return ret;
}

QString Device::projectName()
{
    return m_projectName;
}

void Device::setProjectName(const QString& projectName)
{
    m_projectName = projectName;
}

bool Device::isTurnTable()
{
    return m_isTurnTable;
}

void Device::setIsTurnTable(bool isTurnTable)
{
    m_isTurnTable = isTurnTable;
}

int Device::scanStatus()
{
    return m_iScanStatue;
}

void Device::setScanStatus(int iScanStatus)
{
    m_iScanStatue = iScanStatus;
}

int Device::setCurrentBrightness(int data)
{
 //   if (m_pService)
    {
        QString result;
//        m_pService->setCurrentBrightness(data, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
    }
    return OK;
}

bool Device::isCreateGroup()
{
    return m_isCreateGroup;
}

void sn3dsdkdemo::Device::setCurrentLaserBrightness(int data)
{
//	if (m_pService)
	{
		QString result;
//		m_pService->setCurrentLaserBrightness(data, result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
	}
}

void sn3dsdkdemo::Device::setCurrentLEDDutyCycle(int data)
{
// 	if (m_pService)
	{
		QString result;
//		m_pService->setCurrentLEDDutyCycle(data, result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
	}
}

void sn3dsdkdemo::Device::setCurrentLaserQualityStep(int data)
{
	//if (m_pService)
	{
		QString result;
//		m_pService->setCurrentLaserQualityStep(data, result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
	}
}

void sn3dsdkdemo::Device::setLaserScanObject(int type)
{
//	if (m_pService)
	{
		QString result;
//		m_pService->setLaserScanObject(type, result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
	}
}
