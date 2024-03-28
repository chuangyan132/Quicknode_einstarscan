#pragma once

#include <QWidget>
#include <QDebug>
#include "ui_meshDialog.h"
#include "PublicClass.h"

class MeshDialog : public QWidget
{
	Q_OBJECT
public:
	MeshDialog(QWidget *parent = Q_NULLPTR);
	~MeshDialog();
public slots:
	void on_pushButton_apply_clicked();
	void on_pushButton_revert_clicked();
	void on_pushButton_confirm_clicked();
	void on_buttonGroup_meshDepth_buttonToggled(QAbstractButton *button, bool checked);
	void on_radioButton_UnWatertight_clicked(bool checked = false);
	void on_radioButton_HalfWatertight_clicked(bool checked = false);
	void on_radioButton_Watertight_clicked(bool checked = false);
public:
	void setPointDistance(double pointDistance)
	{
		m_pointDistance = pointDistance;
	}
	void setMinSmallPartArea(double minSmallPartArea)
	{
		m_minSmallPartArea = minSmallPartArea;
	}
	void setScanType(int scantype) {
		qDebug() << "scantype:" << scantype;
		m_scanType = scantype;
	}
	void setDeviceSubType(QString subtype) { m_deviceSubType = subtype; }
signals:
	void sigMeshInfo(QVariantMap info);
	void sigClose();
private:
	Ui::meshDialog ui;
	double m_pointDistance;
	double m_minSmallPartArea;
	int m_depth = 1;
	int  m_scanType = 0;
	//device sub type
	QString m_deviceSubType;
};
