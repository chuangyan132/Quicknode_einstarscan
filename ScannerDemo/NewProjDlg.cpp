#include "NewProjDlg.h"
//#include "ScanServiceImp.h"
#include <QFileDialog>
#include <QKeyEvent>
#include <QVariant>

NewProjDlg::NewProjDlg(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	initWidget();
}

NewProjDlg::~NewProjDlg()
{
}

void NewProjDlg::showInit(QString deviceSubType, ScanServiceImp*scanServiceImp)
{
	qDebug() << __FUNCTION__ << __LINE__ << "device sub type:" << deviceSubType;
	m_subType = deviceSubType;
	//m_scanServiceImp = scanServiceImp;
	if (m_subType == C_E3HXDevice)
	{
		m_HXNormal->show();
		m_HXLaser->show();
		m_HNormal->hide();
		m_HBody->hide();
		m_HPortrait->hide();
        // E7的扫描类型
        m_FixedScan->hide();
        m_HandHDScan->hide();
        m_HandRapidScan->hide();
	}
	else if (m_subType == C_E3HDevice)
	{
		m_HXNormal->hide();
		m_HXLaser->hide();
		m_HNormal->show();
		m_HBody->show();
		m_HPortrait->show();
        // E7的扫描类型
        m_FixedScan->hide();
        m_HandHDScan->hide();
        m_HandRapidScan->hide();
	}
    else if (m_subType == C_E7PlusDevice)
    {
        m_HXNormal->hide();
        m_HXLaser->hide();
        m_HNormal->hide();
        m_HBody->hide();
        m_HPortrait->hide();
        // E7的扫描类型
        m_FixedScan->show();
        m_HandHDScan->show();
        m_HandRapidScan->show();
    }
	else if (m_subType == C_E10Device)
	{
		m_HXNormal->hide();
		m_HXLaser->hide();
		m_HNormal->hide();
		m_HBody->hide();
		m_HPortrait->hide();
		// E7的扫描类型
		m_FixedScan->hide();
		m_HandHDScan->hide();
		m_HandRapidScan->hide();
		//m_ObjectScan->show();
		//m_PortraitScan->show();
		m_E10Scan->show();
	}
	ui.stackedWidget->setCurrentIndex(1);
	QSize size = this->size();
	size.setHeight(300);
	this->resize(size);
	this->update();
}

void NewProjDlg::onNewProjGrpClicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	QString sln_path = QFileDialog::getExistingDirectory(this, "Choose Solution Path", "");
	if (sln_path.isEmpty())
	{
		return;
	}
	QString result;
	QStringList slnName = sln_path.split("/");
	sln_path.append("/" + slnName.last());
	m_slnDirPath = sln_path;
	qDebug() << __FUNCTION__ << __LINE__ << "m_slnDirPath:" << m_slnDirPath ;
	ui.stackedWidget->setCurrentIndex(0);
	switch (m_scanType)
	{
	case ST_FIXED:
		break;
	case ST_HD:
		break;
	case ST_RAPID:
		break;
	case ST_E3_HX_NORMAL:
	{
		ui.widget_selectMode->show();
		ui.widget_globalMarkers->show();
		ui.widget_alignType->show();
		ui.widget_pointDist->show();
	}
		break;
	case ST_E3_HX_LASER:
	{
		ui.widget_selectMode->hide();
		ui.widget_globalMarkers->hide();
		ui.widget_alignType->hide();
		ui.widget_pointDist->show();
	}
		break;
	case ST_E3_H_NORMAL:
		ui.widget_selectMode->show();
		ui.widget_globalMarkers->show();
		ui.widget_alignType->show();
		ui.widget_pointDist->show();
		break;
	case ST_E10_NORMAL:
		ui.widget_selectMode->show();
		ui.widget_globalMarkers->show();
		ui.widget_alignType->show();
		ui.widget_pointDist->show();
		ui.widget_chooseScanSize->show();
		break;
	case ST_E3_H_BODY:
		break;
	case ST_E3_H_PORTRAIT:
		break;
	case ST_E3_CALIB:
		break;
	case ST_UE_LASER:
		break;
	default:
		break;
	}
}

void NewProjDlg::onOpenProjGrpClicked()
{
	//file filter
	QString slnFilter;
	switch (m_scanType)
	{
	case ST_FIXED:
		break;
	case ST_HD:
		break;
	case ST_RAPID:
		break;
	case ST_E3_HX_NORMAL:
		slnFilter = "*.sln_lrap";
		break;
	case ST_E3_HX_LASER:
		slnFilter = "*.sln_laser";
		break;
	case ST_E3_H_NORMAL:
		slnFilter = "*.sln_nrap";
		break;
	case ST_E3_H_BODY:
		slnFilter = "*.sln_brap";
		break;
	case ST_E3_H_PORTRAIT:
		slnFilter = "*.sln_ir";
		break;
	case ST_E3_CALIB:
		break;
	case ST_UE_LASER:
		break;
	case ST_E10_NORMAL:
		slnFilter = "*.sln_E10_ir";
		break;
	default:
		break;
	}
	qDebug() << __FUNCTION__ << __LINE__ << "slnFilter:"<< slnFilter;
	QString sln_path = QFileDialog::getOpenFileName(this, "Choose Solution", "", slnFilter);
	if (sln_path.isEmpty())
	{
		return;
	}
	//if (m_scanServiceImp)
	{
		QString result;
		QString params;
		if (m_subType == C_E3HDevice)
		{
			QJsonObject jo;
			jo.insert("solutionPath", sln_path);
			jo.insert("isCreate", false);
			jo.insert("solutionPointDist", ui.lineEdit_pointDist->text().toFloat());
		//	m_scanServiceImp->openOrCreateSolution(jo, result);
		}
		else if (m_subType == C_E3HXDevice)
		{
			QJsonObject jo;
			jo.insert("sln_path", sln_path);
			jo.insert("isCreate", false);
			jo.insert("pointDis", ui.lineEdit_pointDist->text().toFloat());
			jo.insert("haveTexture", ui.radioButton_texture->isChecked());
			QString json = QJsonDocument(jo).toJson(QJsonDocument::Compact);
			params = QString("{\"info\":%1}").arg(json);
		//	m_scanServiceImp->openOrCreateSolution(params, result);
		}
        else if (m_subType == C_E7PlusDevice)
        {
            QJsonObject jo;
            jo.insert("solutionPath", sln_path);
            jo.insert("isCreate", false);
            // 解决方案点距
            jo.insert("solutionPointDist", ui.lineEdit_pointDist->text().toFloat());
            params = QJsonDocument(jo).toJson(QJsonDocument::Compact);
         //   m_scanServiceImp->openOrCreateSolution(params, result);
        }
		else if (m_subType == C_E10Device)
		{
			m_scanMode = (ScanMode)(ui.radioButton_portraitScan->isChecked() ? 0 : ui.checkBox_scanSize ? 1 : 2);
			m_hasTexture = ui.radioButton_texture->isChecked();
		//	m_scanServiceImp->setIsLedOpen(m_hasTexture, result);
			//QVariantMap info;
			//info.insert("sln_path", sln_path);
			//info.insert("isCreate", false);
			//info.insert("pointDis", ui.lineEdit_pointDist->text().toFloat());
			//info.insert("haveTexture", m_hasTexture);
			//info.insert("scanMode", m_scanMode);

			//QVariantMap info1;
			//info1.insert("info", info);
			//QJsonObject jo;
			//jo = QJsonObject(QJsonDocument::fromJson(QJsonDocument::fromVariant(QVariant(info1)).toJson()).object());
			////jo.insert("sln_path", m_slnDirPath);
			////jo.insert("isCreate", true);
			////jo.insert("pointDis", ui.lineEdit_pointDist->text().toFloat());
			////jo.insert("haveTexture", ui.radioButton_texture->isChecked());
			////jo.insert("scanMode", 1);
			////m_scanServiceImp->openOrCreateSolution(jo, result);
			Sn3DOpenOrCreateSolution(sln_path.toStdString().c_str(), false, ui.lineEdit_pointDist->text().toFloat(), m_hasTexture, m_scanMode);
		}
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result << "params:" << params;
	}
}

void NewProjDlg::on_stackedWidget_currentChanged(int index)
{
	if (index == 0)
	{
		emit sigSizeChanged(QSize(500, 700));
		emit sigTitleChanged("Scan Config");
	}
	else if (index == 1)
	{
		emit sigSizeChanged(QSize(500, 250));
		emit sigTitleChanged("Choose Scan Mode");

	}
	else if (index == 2)
	{
		emit sigSizeChanged(QSize(500, 250));
		emit sigTitleChanged("Project");
	}
}

void NewProjDlg::on_pushButton_confirm_clicked()
{
	qDebug() << __FUNCTION__ << __LINE__;
	//first create solution,after created, create new project 
	emit sigPointDistChanged(ui.lineEdit_pointDist->text().toFloat());
	//if (m_scanServiceImp)
	{
		QString result;
		QString params;
		if (m_subType == C_E3HDevice)
		{
			QJsonObject jo;
			jo.insert("solutionPath", m_slnDirPath);
			jo.insert("isCreate", true);
			jo.insert("solutionPointDist", ui.lineEdit_pointDist->text().toFloat());
			//m_scanServiceImp->openOrCreateSolution(jo, result);
		}
		else if (m_subType == C_E3HXDevice)
		{
			QJsonObject jo;
			jo.insert("sln_path", m_slnDirPath);
			jo.insert("isCreate", true);
			jo.insert("pointDis", ui.lineEdit_pointDist->text().toFloat());
			jo.insert("haveTexture", ui.radioButton_texture->isChecked());
			QString json = QJsonDocument(jo).toJson(QJsonDocument::Compact);
			params = QString("{\"info\":%1}").arg(json);
			//m_scanServiceImp->openOrCreateSolution(params, result);
		}
        else if (m_subType == C_E7PlusDevice)
        {
            QJsonObject jo;
            jo.insert("solutionPath", m_slnDirPath);
            jo.insert("isCreate", true);
            // 解决方案点距
            jo.insert("solutionPointDist", ui.lineEdit_pointDist->text().toFloat());
           // m_scanServiceImp->openOrCreateSolution(jo, result);
        }
		else if (m_subType == C_E10Device)
		{
			m_scanMode = (ScanMode)(ui.radioButton_portraitScan->isChecked() ? 0 : ui.checkBox_scanSize ? 1 : 2);
			m_hasTexture = ui.radioButton_texture->isChecked();
#if 1

			int ret = Sn3DOpenOrCreateSolution(
				m_slnDirPath.toStdString().c_str(), 
				true, 
				m_scanMode, 
				m_hasTexture, 
				ui.lineEdit_pointDist->text().toFloat());

			if (ret == EC_SUCCESS)
			{
				ret = Sn3DNewProject(m_slnDirPath.toStdString().c_str(),
					m_scanMode,
					m_hasTexture,
					ui.lineEdit_pointDist->text().toFloat(),
					(int)m_alignType,
					ui.lineEdit_globalMarkerPath->text().toStdString().c_str());

				if (ret == EC_SUCCESS)
				{
					emit sigNewSlnFinished();

				}
			}

#else
			//m_scanServiceImp->setIsLedOpen(m_hasTexture, result);
			QVariantMap info;
			info.insert("sln_path", m_slnDirPath);
			info.insert("isCreate", true);
			info.insert("pointDis", ui.lineEdit_pointDist->text().toFloat());
			info.insert("haveTexture", m_hasTexture);
			info.insert("scanMode", m_scanMode);

			QVariantMap info1;
			info1.insert("info", info);
			QJsonObject jo;
			jo = QJsonObject(QJsonDocument::fromJson(QJsonDocument::fromVariant(QVariant(info1)).toJson()).object());
			//jo.insert("sln_path", m_slnDirPath);
			//jo.insert("isCreate", true);
			//jo.insert("pointDis", ui.lineEdit_pointDist->text().toFloat());
			//jo.insert("haveTexture", ui.radioButton_texture->isChecked());
			//jo.insert("scanMode", 1);
			m_scanServiceImp->openOrCreateSolution(jo, result);
#endif
		}
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result << "params:" << params;
	}
}

void NewProjDlg::on_pushButton_cancel_clicked()
{
	ui.stackedWidget->setCurrentIndex(1);
}

void NewProjDlg::on_buttonGroup_scanAlignType_buttonToggled(QAbstractButton *button, bool checked)
{
	if (checked)
	{
		if (button->text() == "GlobalMarkers")
		{
			m_alignType = AT_GLOBAL_POINT;	
		}
		else if (button->text() == "Feature")
		{
			m_alignType = AT_FEATURES;
		}
		else if (button->text() == "Hybird")
		{
			m_alignType = AT_HYBRID;
		}
		else if (button->text() == "Texture")
		{
			m_alignType = AT_TEXTURE;
		}
		else
		{
			QString log = "unknow button,buttonText:" + button->text();
			qDebug() << log;
		}
	}
}

void NewProjDlg::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Escape && ui.stackedWidget->currentIndex() == 2)
	{
		emit sigEscPressed();
	}
	QWidget::keyPressEvent(event);
}

int NewProjDlg::createNewProject()
{
	//if (m_scanServiceImp)
	{
		QString result;

		m_scanMode = (ScanMode)(ui.radioButton_portraitScan->isChecked() ? 0 : ui.checkBox_scanSize ? 1 : 2);
		m_hasTexture = ui.radioButton_texture->isChecked();

#if 1
		int code = Sn3DNewProject(
			m_slnDirPath.toStdString().c_str(),
			m_scanMode,
			m_hasTexture,
			ui.lineEdit_pointDist->text().toFloat(),
			(int)m_alignType,
			ui.lineEdit_globalMarkerPath->text().toStdString().c_str());


		

#else
		//m_scanServiceImp->setIsLedOpen(m_hasTexture, result);

		QJsonObject joparams;
		joparams.insert("_globalMarkerFilePath", ui.lineEdit_globalMarkerPath->text());
		joparams.insert("_textureEnabled", m_hasTexture);
		joparams.insert("_pointDist", ui.lineEdit_pointDist->text().toFloat());
		joparams.insert("_alignType", m_alignType);
		joparams.insert("_rapidMode",false);
		joparams.insert("_fastSave", false);
		joparams.insert("_flagAfterSolution", false);
		joparams.insert("_scanMode", m_scanMode);
		joparams.insert("_optimization", false);

		QString json = QJsonDocument(joparams).toJson(QJsonDocument::Compact);

		QJsonObject jo;
		jo.insert("path", m_slnDirPath);
        if (m_isE7Plus)
            jo.insert("type", e7ProjectType());
		jo.insert("params", json);
//		m_scanServiceImp->newProject(jo, result);
		qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
#endif
		return code;
	}
}

// E7 device
int NewProjDlg::createNewProject(E7PlusDevice::ProjectType projectType)
{
   // if (m_scanServiceImp)
    {
        QJsonObject joparams;
        joparams.insert("_globalMarkerFilePath", ui.lineEdit_globalMarkerPath->text());
        joparams.insert("_textureEnabled", ui.radioButton_texture->isChecked());
        joparams.insert("_pointDist", ui.lineEdit_pointDist->text().toFloat());
        joparams.insert("_alignType", m_alignType);
        joparams.insert("_rapidMode", false);
        joparams.insert("_fastSave", false);
        joparams.insert("_flagAfterSolution", false);
        QString json = QJsonDocument(joparams).toJson(QJsonDocument::Compact);

        QJsonObject jo;
        jo.insert("path", m_slnDirPath);
        jo.insert("type", projectType);
        jo.insert("params", json);
        QString result;
 //       m_scanServiceImp->newProject(jo, result);
        qDebug() << __FUNCTION__ << __LINE__ << "result:" << result;
    }
	return 0;
}

void NewProjDlg::initWidget()
{
	//page chooseScanMode
	QWidget* chooseScanModePage = ui.stackedWidget->widget(1);
	QVBoxLayout* scanLayout = new QVBoxLayout;
	scanLayout->addWidget(m_HXNormal);
	scanLayout->addWidget(m_HXLaser);
	scanLayout->addWidget(m_HNormal);
	scanLayout->addWidget(m_HBody);
	scanLayout->addWidget(m_HPortrait);
    scanLayout->addWidget(m_FixedScan);
    scanLayout->addWidget(m_HandHDScan);
    scanLayout->addWidget(m_HandRapidScan);
	//scanLayout->addWidget(m_PortraitScan);
	//scanLayout->addWidget(m_ObjectScan);
	scanLayout->addWidget(m_E10Scan);
	scanLayout->setAlignment(m_HXNormal, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_HXLaser, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_HNormal, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_HBody, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_HPortrait, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_FixedScan, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_HandHDScan, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_HandRapidScan, Qt::AlignVCenter | Qt::AlignHCenter);
	//scanLayout->setAlignment(m_PortraitScan, Qt::AlignVCenter | Qt::AlignHCenter);
	//scanLayout->setAlignment(m_ObjectScan, Qt::AlignVCenter | Qt::AlignHCenter);
	scanLayout->setAlignment(m_E10Scan, Qt::AlignVCenter | Qt::AlignHCenter);

	chooseScanModePage->setLayout(scanLayout);

    // E7设备按钮宽度特殊处理，因为文字比较长，显示不下
    m_FixedScan->setFixedHeight(100);
    m_FixedScan->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_HandHDScan->setFixedHeight(100);
    m_HandHDScan->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_HandRapidScan->setFixedHeight(100);
    m_HandRapidScan->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

	connect(m_HXNormal, &QPushButton::clicked, [this]() {
		m_scanType = ST_E3_HX_NORMAL;
		emit sigEnterScan(m_scanType);
		ui.stackedWidget->setCurrentIndex(2);
	});	
	connect(m_HXLaser, &QPushButton::clicked, [this]() {
		m_scanType = ST_E3_HX_LASER;
		emit sigEnterScan(m_scanType);
		ui.stackedWidget->setCurrentIndex(2);
	});	
	connect(m_HNormal, &QPushButton::clicked, [this]() {
		m_scanType = ST_E3_H_NORMAL;
		emit sigEnterScan(m_scanType);
		ui.stackedWidget->setCurrentIndex(2);
	});	
	connect(m_HBody, &QPushButton::clicked, [this]() {
		m_scanType = ST_E3_H_BODY;
		emit sigEnterScan(m_scanType);
		ui.stackedWidget->setCurrentIndex(2);
	});	
	connect(m_HPortrait, &QPushButton::clicked, [this]() {
		m_scanType = ST_E3_H_PORTRAIT;
		emit sigEnterScan(m_scanType);
		ui.stackedWidget->setCurrentIndex(2);
	});
    // E7 设备
    connect(m_FixedScan, &QPushButton::clicked, [this]() {
        m_scanType = ST_FIXED;
        emit sigEnterScan(m_scanType);
        ui.stackedWidget->setCurrentIndex(2);
    });
    connect(m_HandHDScan, &QPushButton::clicked, [this]() {
        m_scanType = ST_HD;
        emit sigEnterScan(m_scanType);
        ui.stackedWidget->setCurrentIndex(2);
    });
    connect(m_HandRapidScan, &QPushButton::clicked, [this]() {
        m_scanType = ST_RAPID;
        emit sigEnterScan(m_scanType);
        ui.stackedWidget->setCurrentIndex(2);
    });
	//// E10 设备
	//connect(m_PortraitScan, &QPushButton::clicked, [this]() {
	//	m_scanType = ST_E10_NORMAL;
	//	emit sigEnterScan(m_scanType);
	//	ui.stackedWidget->setCurrentIndex(2);
	//});
	//connect(m_ObjectScan, &QPushButton::clicked, [this]() {
	//	m_scanType = ST_E10_NORMAL;
	//	emit sigEnterScan(m_scanType);
	//	ui.stackedWidget->setCurrentIndex(2);
	//});	
	connect(m_E10Scan, &QPushButton::clicked, [this]() {
		m_scanType = ST_E10_NORMAL;
		emit sigEnterScan(m_scanType);
		ui.stackedWidget->setCurrentIndex(2);
	});

	//page solution
	QWidget* solutionPage = ui.stackedWidget->widget(2);
	QVBoxLayout* solutionLayout = new QVBoxLayout;
	solutionLayout->addWidget(m_newProjGrp);
	solutionLayout->addWidget(m_openProjGrp);
	solutionLayout->setAlignment(m_newProjGrp, Qt::AlignVCenter | Qt::AlignHCenter);
	solutionLayout->setAlignment(m_openProjGrp, Qt::AlignVCenter | Qt::AlignHCenter);
	solutionPage->setLayout(solutionLayout);
	connect(m_newProjGrp, SIGNAL(clicked()), this, SLOT(onNewProjGrpClicked()));
	connect(m_openProjGrp, SIGNAL(clicked()), this, SLOT(onOpenProjGrpClicked()));
}

bool NewProjDlg::isE7Plus()
{
    return m_isE7Plus;
}

void NewProjDlg::setIsE7Plus(bool isE7Plus)
{
    m_isE7Plus = isE7Plus;
}

void NewProjDlg::on_pushButton_scanGlobalMarkersPath_clicked()
{
	QString globalMarkerPath = QFileDialog::getOpenFileName(this, tr("Choose global marker file"), "", tr("*.marker"));
	ui.lineEdit_globalMarkerPath->setText(globalMarkerPath);
}

// E7 device project type
E7PlusDevice::ProjectType NewProjDlg::e7ProjectType()
{
    E7PlusDevice::ProjectType ret = E7PlusDevice::PT_FIX;
    if (ST_HD == m_scanType){
        ret = E7PlusDevice::PT_HANDLE_HD;
    }
    else if (ST_RAPID == m_scanType){
        ret = E7PlusDevice::PT_HANDLE_RAPID;
    }
    return ret;
}