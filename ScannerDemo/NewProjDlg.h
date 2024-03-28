#pragma once

#include <QWidget>
#include "ui_NewProjDlg.h"
#include "PublicClass.h"
#include <QDebug>
#include "Sn3DE10Sdk.h"
class ScanServiceImp;
class NewProjDlg : public QWidget
{
	Q_OBJECT
public:
	NewProjDlg(QWidget *parent = Q_NULLPTR);
	~NewProjDlg();
	void showInit(QString deviceSubType, ScanServiceImp*scanServiceImp);
	int createNewProject();
    // E7 device
    int createNewProject(E7PlusDevice::ProjectType projectType);
    bool isE7Plus();
    void setIsE7Plus(bool isE7Plus);
public slots:
	void onNewProjGrpClicked();
	void onOpenProjGrpClicked();
	void on_stackedWidget_currentChanged(int index);

	void on_pushButton_confirm_clicked();
	void on_pushButton_scanGlobalMarkersPath_clicked();
	void on_pushButton_cancel_clicked();
	void on_buttonGroup_scanAlignType_buttonToggled(QAbstractButton *button, bool checked);
protected: 
	virtual void keyPressEvent(QKeyEvent *event);
private slots:
signals:
	void sigEnterScan(int scantype);
	void sigPointDistChanged(float pointDist);
	void sigSizeChanged(QSize size);
	void sigTitleChanged(QString title); 
	void sigEscPressed();
	void sigNewSlnFinished();
private:
	void initWidget();
    // E7 device project type
    E7PlusDevice::ProjectType e7ProjectType();
private:
	Ui::NewProjDlg ui;
	//ScanServiceImp* m_scanServiceImp = nullptr;
	//device sub type
	QString m_subType;
	QString m_slnDirPath;
	//scanning align type
	AlignType m_alignType = AT_FEATURES;
	ScanMode m_scanMode = AT_SMALL_OBJECT;
	bool m_hasTexture = false;
	//scan type
	ScanType m_scanType;
	//page chooseScanMode
	QPushButton*m_HXNormal	= new QPushButton("Rapid Scan");
	QPushButton*m_HXLaser	= new QPushButton("Laser Scan");
	QPushButton*m_HNormal	= new QPushButton("Normal Scan");
	QPushButton*m_HBody		= new QPushButton("Body Scan");
	QPushButton*m_HPortrait = new QPushButton("Portrait Scan");
    QPushButton* m_FixedScan = new QPushButton("Fixed Scan");               // Fix scan
    QPushButton* m_HandHDScan = new QPushButton("Handheld HD Scan");        // Handheld HD Scan
    QPushButton* m_HandRapidScan = new QPushButton("Handheld Rapid Scan");  // Handheld Rapid Scan
	//QPushButton* m_PortraitScan = new QPushButton("Portrait Scan");			// E10  
	//QPushButton* m_ObjectScan = new QPushButton("Object Scan"); //
	QPushButton* m_E10Scan = new QPushButton("Scan");
	//page solution
	QPushButton*m_newProjGrp = new QPushButton("New Project Group");
	QPushButton*m_openProjGrp = new QPushButton("Open Project Group");
    bool m_isE7Plus = false;       // is it a E7 plus device
};
