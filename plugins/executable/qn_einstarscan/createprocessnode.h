/*!
* @brief   
* @project qn_einstarscan
* @file    createprocessnode.h
* @author  sunduo
* @email   sunduo@shining3d.com
* @license 
*     Copyright(C) shining3d. All rights reserved.
* @versions 
*     V0.1-sunduo 2023-08-24 11:02
*/
//==============================================================================
#ifndef QN_EINSTARSCAN_CREATEPROCESSNODE_H
#define QN_EINSTARSCAN_CREATEPROCESSNODE_H
//==============================================================================


#include <QProcess>
#include <atomic>

#include "../include/basenode.h"
#include "Sn3DE10Sdk.h"

class CreateProcessNode : public /*QObject,*/ BaseNode{
  public:
    CreateProcessNode();
    virtual  ~CreateProcessNode();

    virtual QN_RESULT apply() override;

  private:
    std::string m_nodeName;
};

class Sn3DEnterScanNode : public BaseNode {
public:
	Sn3DEnterScanNode();
	virtual  ~Sn3DEnterScanNode();

	virtual QN_RESULT apply() override;

private:
	inline bool isInit() const { return (m_initCount.load() > 0); }

private:
	std::string m_nodeName;
	std::atomic_int m_initCount{0};

	Sn3DScanServiceWatcherCallBack m_Sn3DWatchFun;

};

class Sn3DNewSolutionByInputsNode : public BaseNode {
public:
	Sn3DNewSolutionByInputsNode();
	virtual  ~Sn3DNewSolutionByInputsNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DNewSolutionByPropertiesNode : public BaseNode {
public:
	Sn3DNewSolutionByPropertiesNode();
	virtual  ~Sn3DNewSolutionByPropertiesNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DOpenSolutionNode : public BaseNode {
public:
	Sn3DOpenSolutionNode();
	virtual  ~Sn3DOpenSolutionNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DNewProjectByInputsNode : public BaseNode {
public:
	Sn3DNewProjectByInputsNode();
	virtual  ~Sn3DNewProjectByInputsNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DNewProjectByPropertiesNode : public BaseNode {
public:
	Sn3DNewProjectByPropertiesNode();
	virtual  ~Sn3DNewProjectByPropertiesNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DOpenProjectNode : public BaseNode {
public:
	Sn3DOpenProjectNode();
	virtual  ~Sn3DOpenProjectNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DDeviceConfigNode : public BaseNode {
public:
	Sn3DDeviceConfigNode();
	virtual  ~Sn3DDeviceConfigNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DPreviewScanNode : public BaseNode {
public:
	Sn3DPreviewScanNode();
	virtual  ~Sn3DPreviewScanNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DExitScanNode : public BaseNode {
public:
	Sn3DExitScanNode();
	virtual  ~Sn3DExitScanNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DScanNode : public BaseNode{
public:
	Sn3DScanNode();
	virtual  ~Sn3DScanNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DPauseScanNode : public BaseNode {
public:
	Sn3DPauseScanNode();
	virtual  ~Sn3DPauseScanNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DClearScanNode : public BaseNode {
public:
	Sn3DClearScanNode();
	virtual  ~Sn3DClearScanNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DEndScanNode : public BaseNode {
public:
	Sn3DEndScanNode();
	virtual  ~Sn3DEndScanNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DSaveDataByPropertiesNode : public BaseNode {
public:
	Sn3DSaveDataByPropertiesNode();
	virtual  ~Sn3DSaveDataByPropertiesNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DSaveDataByInputsNode : public BaseNode {
public:
	Sn3DSaveDataByInputsNode();
	virtual  ~Sn3DSaveDataByInputsNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DMeshNode : public BaseNode {
public:
	Sn3DMeshNode();
	virtual  ~Sn3DMeshNode();

	virtual QN_RESULT apply() override;

private:
	std::string m_nodeName;
};

class Sn3DHandleDeviceEventNode : public BaseNode {
public:
	Sn3DHandleDeviceEventNode();
	virtual  ~Sn3DHandleDeviceEventNode();

	virtual QN_RESULT apply() override;

private:
	void getDeviceEvent(DeviceEvent event);
	bool DoubleClickModel = false;
	int BrightnessLevel = 8;
	int BrightnessMax;
	int BrightnessMin;

	std::string m_nodeName;
};

// 对话框管理
class QDialog;
class DialogManager : public QObject {
	Q_OBJECT
public:
	static DialogManager *instance();
	bool dialogIsClosed();
public slots:
	void onOpenDialog(bool isVideo, bool isSaveWholeCloud, QString SaveWholeCloud);
signals:
	void dialogClosed();

private:
	DialogManager(QObject *parent = nullptr);
	~DialogManager();
	bool m_dialogIsClosed = false;
	static DialogManager *m_instance;
	QDialog *m_dialog;
};

class UiHelper : public QObject {
	Q_OBJECT
public:
	static UiHelper &instance();
	UiHelper(QObject *parent = nullptr);
	~UiHelper();

	void openDialog(bool isVideo, bool isSaveWholeCloud, QString SaveWholeCloud);

signals:
	void toOpenDialog(bool isVideo, bool isSaveWholeCloud, QString SaveWholeCloud); // 弹出对话框
};


//==============================================================================
#endif // QN_EINSTARSCAN_CREATEPROCESSNODE_H
//==============================================================================
