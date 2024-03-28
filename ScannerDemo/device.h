#ifndef __DEVICE_H__
#define __DEVICE_H__
#include "qmap.h"
#include "qobject.h"
//#include "sn3d_smc.h"
#include "PublicClass.h"

class ScanServiceImp;

namespace sn3dsdkdemo{
    enum errorCode{
        OK,
        NOT_IMPLEMENTED,
        PARAM_FORMAT_ERROR,
        USER_CANCEL
    };
	void strJson2Map(QString json, QVariantMap&result);
    class Device :
        public QObject
    {
        Q_OBJECT
    public:
        Device(QObject* parent, ScanServiceImp* pService);
        virtual ~Device();
        virtual int enterCalibrate();
        virtual int setCalibrateType(int caliType);
        virtual int setCalibrateType(const QString& caliType);
        virtual int calibrateType(int* pCaliType);
        virtual int calibrateType(QString& caliType);
        virtual int startSnap();
        virtual int stopSnap();
        virtual int exitCalibrate();
        virtual int enterScan();
        virtual int startScan();
        virtual int preview();
        virtual int pauseScan();
        virtual int cancelScan();
        virtual int endScan();
        virtual int saveCloudPoint(const QString& projectPath);
        virtual int mesh();
        virtual int saveMesh(const QString& projectPath);
        virtual int exitScan();
        // @note Work, solution and project group're the same.
        virtual int openProjectGroup();
        virtual int newProjectGroup();
        virtual int openProject();
        virtual int newProject();
        virtual int deletePoint(int start, int end);
        virtual int changePoint(int index, double x, double y, double z);
        virtual int calibrateTypeMap(QMap<QString, int>& map);
        virtual void setSubType(const QString& subType);
        virtual QString subType();
        virtual int setScanType(int iScanType);
        virtual int scanType();
        virtual bool isFixScan();
        virtual bool isHDR();
        virtual void setIsHDR(bool isHDR);
        virtual void setProjectName(const QString& projectName);
        virtual bool isTurnTable();
        virtual int scanStatus();
        virtual void setScanStatus(int iScanStatus);
        virtual int setCurrentBrightness(int data);
		virtual void setCurrentLaserBrightness(int data);
		virtual void setCurrentLEDDutyCycle(int data);
		virtual void setCurrentLaserQualityStep(int data);
		virtual void setLaserScanObject(int type);
        virtual bool isCreateGroup();
    protected:
        virtual int caliTypeToInt(const QString caliTypeStr, int* caliType);
        virtual int caliTypeToStr(int caliType, QString& caliTypeStr);
        virtual int enterScan(int scanType);
        virtual int openProjectGroup(const QString& groupPath, double pointDist);
        // virtual int newProjectGroup(const QString& groupPath, bool textureEnabled);
        virtual int newProjectGroup(const QString& groupPath, double pointDist);
        virtual int newProject(const QJsonObject& params);
        virtual QString groupPath();
        virtual void setGroupPath(const QString& groupPath);
        virtual QString projectName();
        virtual int openProject(const QString& projFileName, int type);
        virtual int saveCloudPoint(const QJsonObject& params);
        virtual int cancelScan(const QJsonObject& params);
        virtual int startScan(bool enableHDR, int alignType, int scanSubType, int turntableTimes);
        virtual int endScan(bool globalOptimize, double pointDist, bool reBuildData);
        virtual double pointDist();
        virtual void setPointDist(double pointDist);
        virtual int mesh(const QJsonObject& params);
        virtual int saveMesh(const QJsonObject& params);
        //virtual int serialEditSharedMemoryData(LPSMC_EDIT_DATA edit_data, int begin, int end);
        virtual int applyEdit(const QJsonObject& params);
        virtual void setIsTurnTable(bool isTurnTable);
    private:
        virtual int modifyProjectGroup(const QString& groupPath, bool isCreate, double pointDist);
    private:
        ScanServiceImp* m_pService;
        QString m_subType;
        QString m_groupPath; // Project group name
        bool m_isHDR = false;
        double m_pointDist = .0;
        QString m_projectName; // Project name
        bool m_isTurnTable = false;
        int m_iScanStatue = SS_SCAN_STOPED;
        bool m_isCreateGroup = false;
    };
}
#endif

