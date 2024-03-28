#include "MeshDialog.h"

MeshDialog::MeshDialog(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	on_radioButton_UnWatertight_clicked(true);
}

MeshDialog::~MeshDialog()
{
	qDebug();
}

void MeshDialog::on_pushButton_apply_clicked()
{
	int index = 0;
	if (ui.radioButton_UnWatertight->isChecked())
		index = 0;
	else if (ui.radioButton_Watertight->isChecked())
		index = 1;
	else if (ui.radioButton_HalfWatertight->isChecked())
		index = 2;
	QVariantMap info;
	//meshType has two type,one is watertight,the other is unwatertight,
	//unwatertight value is 0,watertight value is 1.
	//interface index,0 is unwatertight,1 is watertight
	info.insert("meshType", index);
	/*depth value:
	* under unwatertight ,depth value is -1£¬under watertight,this can choose three value,
	* high value is 0,mid value is 1,low value is 2
	*/
	int depth = (index == 0) ? -1 : m_depth;
	info.insert("depth", depth);
	//isNeedOpt: is or not optimize in meshing,always is true£¬type is bool
	info.insert("isNeedOpt", true);
	// point distance 
	info.insert("pointDistance", m_pointDistance);
	//delete smallest connected area£¬the value get by post process parameters£¬type is double
	double minSmallPartArea = m_minSmallPartArea * (ui.spinBox_removeSmallParts->value()) / 100.0;
	info.insert("minSmallPartArea", minSmallPartArea);
	//type is double
	info.insert("markerRadius", 7.0);
	//type double£¬under unwatertight laser scan set 10,speckle scan set 50,under watertight the value is 5000
	bool isMarkHole = ui.checkBox_markerHoleFilling->isChecked();
	int HXLaser = 4;
	double maxFillParam = (index == 1) ? (5000) : (isMarkHole ? (m_scanType == HXLaser ? 10 : 50) : 0);
	info.insert("maxFillParam", maxFillParam);
	//optimize value£¬type is int
	info.insert("optPower", ui.spinBox_optimize->value());
	//type is double
	info.insert("optSmooth", ui.spinBox_smooth->value());
	//type is double
	info.insert("simpRate", double(ui.spinBox_simpli->value()));
	//type is double
	double spikeSensitivity = ui.checkBox_removeSpike->isChecked() ? 0.4 : 0;
	info.insert("spikeSensitivity", spikeSensitivity);
	qDebug() << "mesh info:" << info;
	emit sigMeshInfo(info);
}

void MeshDialog::on_pushButton_revert_clicked()
{
	qDebug()<<__FUNCTION__;
	//revert no need invoke any method,client need to recover.
}

void MeshDialog::on_pushButton_confirm_clicked()
{
	qDebug();
	//confirm no need invoke any method.
	emit sigClose();
}

void MeshDialog::on_buttonGroup_meshDepth_buttonToggled(QAbstractButton *button, bool checked)
{
	qDebug();
	if (checked)
	{
		if (button->text() == "High")
		{
			m_depth = 0;
		}
		else if (button->text() == "Mid")
		{
			m_depth = 1;
		}
		else if (button->text() == "Low")
		{
			m_depth = 2;
		}
		else
		{
			m_depth = -1;
		}
	}
}

void MeshDialog::on_radioButton_UnWatertight_clicked(bool checked /*= false*/)
{
	if (checked)
	{
		//ui.labelModelQuality->setEnabled(false);
		ui.modelquality_radioButtons->setEnabled(false);
	}
}


void MeshDialog::on_radioButton_HalfWatertight_clicked(bool checked /*= false*/)
{
	if (checked)
	{
		//ui.labelModelQuality->setEnabled(false);
		ui.modelquality_radioButtons->setEnabled(false);
	}
}

void MeshDialog::on_radioButton_Watertight_clicked(bool checked /*= false*/)
{
	if (checked)
	{
		//ui.labelModelQuality->setEnabled(true);
		ui.modelquality_radioButtons->setEnabled(true);
	}
}
